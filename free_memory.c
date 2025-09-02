/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:28:57 by marcos            #+#    #+#             */
/*   Updated: 2025/09/02 21:34:01 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// free the context used on the main loop
void	free_phi_tab_con(t_philo_context **phi_tab_con, int size)
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

// free the table
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
	pthread_mutex_destroy(&table->deat_flag_mutex);
	pthread_mutex_destroy(&table->finished_mutex);
	free(table);
}

//free  single philosopher
void	free_philosopher(t_philosopher *philo)
{
	free(philo->thread);
	free(philo);
}

//free all philosophers
void	free_philosophers(t_philosopher **philosophers, int size)
{
	int	counter;

	counter = 0;
	while (counter < size)
	{
		pthread_mutex_destroy(&philosophers[counter]->eat_mutex);
		free_philosopher(philosophers[counter]);
		counter++;
	}
	free(philosophers);
}
