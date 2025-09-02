/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:43 by marcos            #+#    #+#             */
/*   Updated: 2025/09/02 20:18:30 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_table	*create_table(int *config, t_philosopher **philosophers)
{
	t_table	*table;
	int		counter;

	counter = 0;
	table = malloc(sizeof(t_table));
	table->forks = malloc(sizeof(pthread_mutex_t) * config[0]);
	table->forks_state = malloc(sizeof(int) * config[0]);
	table->last_ate = malloc(sizeof(int) * config[0]);
	pthread_mutex_init(&table->deat_flag_mutex, NULL);
	table->death_flag = 0;
	pthread_mutex_init(&table->finished_mutex, NULL);
	table->finished = 0;
	while (counter < config[0])
	{
		pthread_mutex_init(&table->forks[counter], NULL);
		table->forks_state[counter] = 0;
		table->last_ate[counter] = -1;
		counter++;
	}
	table->philos = philosophers;
	return (table);
}

t_philosopher	*create_philosopher(int counter, int *config)
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
	if (ft_size(config) == 5)
		philosopher->own_required_eat = config[4];
	else
		philosopher->own_required_eat = -1;
	pthread_mutex_init(&philosopher->eat_mutex, NULL);
	return (philosopher);
}

void	create_monitor(t_table *table, int *config)
{
	pthread_t			monitor;
	t_monitor_context	*m_ctx;

	m_ctx = malloc(sizeof(t_monitor_context));
	m_ctx->table = table;
	m_ctx->config = config;
	pthread_create(&monitor, NULL, (void *)monitor_check, m_ctx);
	pthread_join(monitor, NULL);
}

void	initialize_philosophers(int *config,
	t_philosopher **philosophers)
{
	int	counter;

	counter = 0;
	while (counter < config[0])
	{
		philosophers[counter] = create_philosopher(counter, config);
		if (!philosophers[counter])
		{
			free_philosophers(philosophers, counter);
			exit(1);
		}
		counter++;
	}
}

void	end_simulation(int *config,
	t_philosopher **philosophers, t_table *table)
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
	free(config);
	free_philosophers(philosophers, size);
	free_table(table, size);
}

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
	end_simulation(config, philosophers, table);
	return (status);
}
