/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:02:41 by marcos            #+#    #+#             */
/*   Updated: 2025/09/10 17:23:35 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//main fnuction, iterative philo actions
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
			drop_fork(ctx->table, ctx->philo->id, ctx->philo->id);
			drop_side_fork(ctx->table, ctx->philo->id, ctx->config[0]);
			break ;
		}
		drop_fork(ctx->table, ctx->philo->id, ctx->philo->id);
		drop_side_fork(ctx->table, ctx->philo->id, ctx->config[0]);
		if (philo_sleep(ctx->table, ctx->philo->id, ctx->config[3]) == 1)
			break ;
		usleep(100);
	}
}

//end all threads and free memory
void	end_simulation(int *config,
	t_philosopher **philosophers, t_table *table, t_philo_context **p_ctx)
{
	int		counter;
	int		size;

	size = config[0];
	counter = 0;
	while (counter < size)
	{
		pthread_join(*(philosophers[counter]->thread), NULL);
		counter++;
	}
	free_phi_tab_con(p_ctx, config[0]);
	free(config);
	free_philosophers(philosophers, size);
	free_table(table, size);
}

//prepare and start all threads
int	start_simulation(int *config, t_philosopher **philosophers, t_table *table)
{
	t_philo_context		**p_ctx;
	int					counter;
	int					status;

	p_ctx = malloc(sizeof(t_philo_context *) * config[0]);
	counter = 0;
	while (counter < config[0])
	{
		p_ctx[counter] = malloc(sizeof(t_philo_context));
		p_ctx[counter]->philo = philosophers[counter];
		p_ctx[counter]->table = table;
		p_ctx[counter]->config = config;
		p_ctx[counter]->philo->eat = get_time_in_ms();
		pthread_create(philosophers[counter]->thread, NULL,
			(void *)round_table, p_ctx[counter]);
		counter++;
	}
	create_monitor(table, config);
	status = table->death_flag;
	end_simulation(config, philosophers, table, p_ctx);
	return (status);
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
		return (1);
	if (initialize_philosophers(config, philosophers, argc) == 1)
		return (1);
	table = create_table(config, philosophers);
	status = start_simulation(config, philosophers, table);
	return (status);
}
