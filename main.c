/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:02:41 by marcos            #+#    #+#             */
/*   Updated: 2025/09/01 22:02:35 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//drop the fork
void	drop_fork(t_table *table, int id)
{
	pthread_mutex_lock(&table->forks[id]);
	table->forks_state[id] = 0;
	pthread_mutex_unlock(&table->forks[id]);
}

//take the fork
int	take_fork(t_table *table, int id, int philo_id)
{
	if (check_death(table, philo_id, NULL) == 1)
		return (2);
	pthread_mutex_lock(&table->forks[id]);
	if (table->forks_state[id] == 0)
	{
		table->forks_state[id] = 1;
		pthread_mutex_unlock(&table->forks[id]);
		if (check_death(table, philo_id, "has taken the fork") == 1)
			return (2);
		return (0);
	}
	pthread_mutex_unlock(&table->forks[id]);
	return (1);
}

//take both forks
int	take_forks(t_table *table, int id, int first, int second)
{
	int	fork_return;

	while (1)
	{
		fork_return = take_fork(table, first, id);
		if (fork_return == 0)
			break ;
		if (fork_return == 2)
			return (1);
		usleep(100);
	}
	while (1)
	{
		fork_return = take_fork(table, second, id);
		if (fork_return == 0)
			break ;
		if (fork_return == 2)
			return (1);
		usleep(100);
	}
	return (0);
}

//layer to filter odds and even and have a rule to make deadlocks rarer
//check if the philo ate with the fork already
int	assign_forks(t_table *table, int id, int philos)
{
	if (id % 2 == 0)
		return (take_forks(table, id, id, (id + 1) % philos));
	else
		return (take_forks(table, id, (id + 1) % philos, id));
}

//main fnuction, iterative philo actions and detach the starving thread
void	round_table(t_philo_context *ctx)
{
	while (1)
	{
		if (philo_think(ctx->table, ctx->philo->id) == 1)
			break ;
		if (assign_forks(ctx->table, ctx->philo->id, ctx->config[0]) == 1)
			break ;
		if (philo_eat(ctx->table, ctx->philo, ctx->config[2]) == 1)
		{
			drop_fork(ctx->table, ctx->philo->id);
			drop_fork(ctx->table, (ctx->philo->id + 1) % ctx->config[0]);
			break ;
		}
		drop_fork(ctx->table, ctx->philo->id);
		drop_fork(ctx->table, (ctx->philo->id + 1) % ctx->config[0]);
		if (philo_sleep(ctx->table, ctx->philo->id, ctx->config[3]) == 1)
			break ;
		usleep(100);
	}
}

int	main(int argc, char *argv[])
{
	t_philosopher	**philosophers;
	t_table			*table;
	int				*config;
	int				status;

	config = malloc(sizeof(int) * argc - 1);
	if (parse_input(config, argc, argv) == 1)
		return (1);
	philosophers = malloc(sizeof(t_philosopher *) * config[0]);
	if (!philosophers)
		exit(1);
	initialize_philosophers(config, philosophers);
	table = create_table(config, philosophers);
	status = start_simulation(config, philosophers, table);
	return (status);
}
