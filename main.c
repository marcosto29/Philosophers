/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:02:41 by marcos            #+#    #+#             */
/*   Updated: 2025/08/26 21:08:59 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//drop the fork
void	drop_fork(t_table *table, int id)
{
	pthread_mutex_lock(table->forks[id]);
	table->forks_state[id] = 0;
	pthread_mutex_unlock(table->forks[id]);
}

void	drop_forks(t_table *table, int id, int philos)
{
	drop_fork(table, id);
	drop_fork(table, (id + 1) % philos);
}

//take the fork
int	take_fork(t_table *table, int id)
{
	pthread_mutex_lock(table->forks[id]);
	if (table->forks_state[id] == 0)
	{
		table->forks_state[id] = 1;
		pthread_mutex_unlock(table->forks[id]);
		return (0);
	}
	pthread_mutex_unlock(table->forks[id]);
	return (1);
}

//check to see if the left and right philos needs to eat before
int	check_side_philo(t_table *table, t_philosopher *philo, int id)
{
	pthread_mutex_lock(table->philos[id]->last_eat_mutex);
	if (table->philos[id]->last_eat < philo->last_eat)
	{
		pthread_mutex_unlock(table->philos[id]->last_eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(table->philos[id]->last_eat_mutex);
	return (0);
}

//take the forks after checking if any philo needs to eat before
int	take_forks(t_table *table, t_philosopher *philo, int philos)
{
	while (1)
	{
		if (check_death(table, philo->id, NULL) == 1)
			return (1);
		if (check_side_philo(table, philo, (philo->id + 1) % philos) == 0
			&& check_side_philo(table, philo,
				(philo->id + philos - 1) % philos) == 0)
			break ;
	}
	while (1)
	{
		if (check_death(table, philo->id, NULL) == 1)
			return (1);
		if (take_fork(table, philo->id) == 0)
		{
			if (take_fork(table, (philo->id + 1) % philos) == 0)
			{
				if (check_death(table, philo->id, "has taken a fork") == 1)
					return (1);
				return (0);
			}
			drop_fork(table, philo->id);
		}
	}
}

//detached thread to check starve levels
void	starve_timer(t_context *ctx)
{
	while (1)
	{
		pthread_mutex_lock(ctx->philo->last_eat_mutex);
		if (get_time_in_ms() >= ctx->philo->last_eat + ctx->config[1])
		{
			pthread_mutex_unlock(ctx->philo->last_eat_mutex);
			break ;
		}
		pthread_mutex_unlock(ctx->philo->last_eat_mutex);
	}
	philo_die(ctx->table, ctx->philo->id);
}

//main fnuction, iterative philo actions and detach the starving thread
void	round_table(t_context *ctx)
{
	pthread_t		death_check;

	if (ctx->philo->id % 2 != 0)
		usleep(1000);
	ctx->philo->last_eat = get_time_in_ms();
	pthread_create(&death_check, NULL, (void *)starve_timer, ctx);
	pthread_detach(death_check);
	while (1)
	{
		if (philo_think(ctx->table, ctx->philo->id) == 1)
			break ;
		if (take_forks(ctx->table, ctx->philo, ctx->config[0]) == 1)
			break;
		if (philo_eat(ctx->table, ctx->philo, ctx->config[2]) == 1)
		{
			drop_forks(ctx->table, ctx->philo->id, ctx->config[0]);
			break ;
		}
		drop_forks(ctx->table, ctx->philo->id, ctx->config[0]);
		if (philo_sleep(ctx->table, ctx->philo->id, ctx->config[3]) == 1)
			break ;
	}
}

int	main(int argc, char *argv[])
{
	t_philosopher	**philosophers;
	int				*config;
	int				status;

	config = malloc(sizeof(int) * argc - 1);
	if (parse_input(config, argc, argv) == 1)
		return (1);
	philosophers = malloc(sizeof(t_philosopher *) * config[0]);
	if (!philosophers)
		exit(1);
	create_all_philosophers(config, philosophers);
	status = start_table(config, philosophers);
	return (status);
}
