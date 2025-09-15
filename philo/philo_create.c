/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:43 by marcos            #+#    #+#             */
/*   Updated: 2025/09/15 23:29:49 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//create the table
t_table	*create_table(int *config, t_philosopher **philosophers)
{
	t_table	*table;
	int		counter;

	counter = 0;
	table = malloc(sizeof(t_table));
	table->forks = malloc(sizeof(pthread_mutex_t) * config[0]);
	table->forks_state = malloc(sizeof(int) * config[0]);
	table->last_fork_philo = malloc(sizeof(int) * config[0]);
	pthread_mutex_init(&table->death_flag_mutex, NULL);
	table->death_flag = 0;
	pthread_mutex_init(&table->finished_mutex, NULL);
	table->finished = 0;
	while (counter < config[0])
	{
		pthread_mutex_init(&table->forks[counter], NULL);
		table->forks_state[counter] = 0;
		table->last_fork_philo[counter] = -1;
		counter++;
	}
	table->philos = philosophers;
	return (table);
}

//create a single philosopher
t_philosopher	*create_philosopher(int counter, int *config, int argc)
{
	t_philosopher	*philosopher;

	philosopher = malloc(sizeof(t_philosopher));
	if (!philosopher)
		return (NULL);
	philosopher->id = counter;
	philosopher->thread = malloc(sizeof(pthread_t *));
	if (!philosopher->thread)
	{
		free_philosopher(philosopher);
		return (NULL);
	}
	if (argc == 6)
		philosopher->own_required_eat = config[4];
	else
		philosopher->own_required_eat = -1;
	pthread_mutex_init(&philosopher->eat_mutex, NULL);
	return (philosopher);
}

//create the monitor to check end condition or dead philosopher
void	create_monitor(t_table *table, int *config)
{
	pthread_t			monitor;
	t_monitor_context	*m_ctx;

	m_ctx = malloc(sizeof(t_monitor_context));
	m_ctx->table = table;
	m_ctx->config = config;
	pthread_create(&monitor, NULL, (void *)monitor_check, m_ctx);
	pthread_join(monitor, NULL);
	free(m_ctx);
}

//initialize a single phlosopher
int	initialize_philosophers(int *config,
	t_philosopher **philosophers, int argc)
{
	int	counter;

	counter = 0;
	while (counter < config[0])
	{
		philosophers[counter] = create_philosopher(counter, config, argc);
		if (!philosophers[counter])
		{
			free_philosophers(philosophers, counter);
			return (1);
		}
		counter++;
	}
	return (0);
}
