/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:28:57 by marcos            #+#    #+#             */
/*   Updated: 2025/08/27 12:51:20 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_phi_tab_con(t_context **phi_tab_con, int size)
{
	int	counter;

	counter = 0;
	while (counter < size)
	{
		free(phi_tab_con[counter]);
		counter++;
	}
	free(phi_tab_con);
}

void	free_table(t_table *table, int size)
{
	int	counter;

	counter = 0;
	while (counter < size)
	{
		pthread_mutex_destroy(&table->forks[counter]);
		counter++;
	}
	free(table->forks);
	free(table->forks_state);
	pthread_mutex_destroy(&table->state_mutex);
	free(table);
}

void	free_philosopher(t_philosopher *philo)
{
	free(philo->thread);
	free(philo);
}

void	free_philosophers(t_philosopher **philosophers, int size)
{
	int	counter;

	counter = 0;
	while (counter < size)
	{
		pthread_mutex_destroy(&philosophers[counter]->last_eat_mutex);
		free_philosopher(philosophers[counter]);
		counter++;
	}
	free(philosophers);
}
