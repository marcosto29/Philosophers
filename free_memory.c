/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcos <marcos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:28:57 by marcos            #+#    #+#             */
/*   Updated: 2025/07/11 17:35:33 by marcos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_info(t_philosophers_info *info)
{
	if (info->forks)
		free(info->forks);
	free(info);
}

void	free_philosopher(t_philosopher *philo)
{
	if (philo->thread)
		free(philo->thread);
	free(philo);	
}

void	free_philosophers(int size, t_philosopher **philosophers)
{
	int	counter;

	counter = 0;
	while (counter < size)
	{
		free_philosopher(philosophers[counter]);
		counter++;
	}
	free(philosophers);
}