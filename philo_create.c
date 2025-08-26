/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:43 by marcos            #+#    #+#             */
/*   Updated: 2025/08/26 18:26:54 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_table	*create_table(int *config, t_philosopher **philosophers)
{
	t_table	*table;
	int		counter;

	counter = 0;
	table = malloc(sizeof(t_table));
	table->forks = malloc(sizeof(pthread_mutex_t *) * config[0]);
	table->forks_state = malloc(sizeof(int) * config[0]);
	table->state_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->state_mutex, NULL);
	table->state = 0;
	while (counter < config[0])
	{
		table->forks[counter] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(table->forks[counter], NULL);
		table->forks_state[counter] = 0;
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
	philosopher->eat_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philosopher->eat_mutex, NULL);
	if (ft_size(config) == 5)
		philosopher->own_required_eat = config[4];
	else
		philosopher->own_required_eat = -1;
	philosopher->last_eat_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philosopher->last_eat_mutex, NULL);
	return (philosopher);
}

void	create_all_philosophers(int *config,
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

int	end_philosophers(int *config,
	t_philosopher **philosophers, t_table *table,
	t_context **phi_tab_con)
{
	int		counter;
	int		size;
	int		state;

	state = table->state;
	size = config[0];
	counter = 0;
	while (counter < size)
	{
		pthread_join(*(philosophers[counter]->thread), NULL);
		pthread_mutex_destroy(table->forks[counter]);
		pthread_mutex_destroy(table->state_mutex);
		pthread_mutex_destroy(philosophers[counter]->last_eat_mutex);
		counter++;
	}
	free_philosophers(philosophers, size);
	free_table(table, size);
	free(config);
	free_phi_tab_con(phi_tab_con, size);
	if (state == -1)
		return (1);
	return (0);
}

int	start_table(int *config, t_philosopher **philosophers)
{
	t_table		*table;
	t_context	**ctx;
	int			counter;

	ctx = malloc(sizeof(t_context *) * config[0]);
	counter = 0;
	table = create_table(config, philosophers);
	while (counter < config[0])
	{
		ctx[counter] = malloc(sizeof(t_context));
		ctx[counter]->philo = philosophers[counter];
		ctx[counter]->table = table;
		ctx[counter]->config = config;
		pthread_create(philosophers[counter]->thread, NULL,
			(void *)round_table, ctx[counter]);
		counter++;
	}
	return (end_philosophers(config, philosophers, table, ctx));
}
