/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:41:25 by matoledo          #+#    #+#             */
/*   Updated: 2025/08/26 18:26:28 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//function to makethe philo think
int	philo_think(t_table *table, int id)
{
	if (check_death(table, id, "is thinking") == 1)
		return (1);
	return (0);
}

//function to make the philo eat
int	philo_eat(t_table *table, t_philosopher *philo, int time_to_eat)
{
	long	wait_time;

	wait_time = get_time_in_ms() + time_to_eat;
	if (check_death(table, philo->id, "is eating") == 1)
		return (1);
	while (get_time_in_ms() < wait_time)
	{
		if (check_death(table, philo->id, NULL) == 1)
			return (1);
		usleep(100);
	}
	pthread_mutex_lock(philo->last_eat_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(philo->last_eat_mutex);
	pthread_mutex_lock(philo->eat_mutex);
	if (--philo->own_required_eat == 0)
	{
		pthread_mutex_unlock(philo->eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->eat_mutex);
	return (0);
}

//function to make the philo sleep
int	philo_sleep(t_table *table, int id, int time_to_sleep)
{
	long	wait_time;

	wait_time = get_time_in_ms() + time_to_sleep;
	if (check_death(table, id, "is sleeping") == 1)
		return (1);
	while (get_time_in_ms() < wait_time)
	{
		if (check_death(table, id, NULL) == 1)
			return (1);
		usleep(100);
	}
	return (0);
}

void	philo_die(t_table *table, int id)
{
	pthread_mutex_lock(table->state_mutex);
	if (table->state == -1)
	{
		pthread_mutex_unlock(table->state_mutex);
		return ;
	}
	table->state = -1;
	show_event(get_time_in_ms(), id, "died");
	pthread_mutex_unlock(table->state_mutex);
}
