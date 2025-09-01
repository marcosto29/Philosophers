/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:41:25 by matoledo          #+#    #+#             */
/*   Updated: 2025/09/01 20:19:48 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//function to make a philo think
int	philo_think(t_table *table, int id)
{
	if (check_death(table, id, "is thinking") == 1)
		return (1);
	return (0);
}

//function to feed a philo
int	philo_eat(t_table *table, t_philosopher *philo, int time_to_eat)
{
	long	wait_time;

	wait_time = get_time_in_ms() + time_to_eat;
	if (check_death(table, philo->id, "is eating") == 1)
		return (1);
	while (get_time_in_ms() < wait_time)
	{
		pthread_mutex_lock(&philo->eat_mutex);
		philo->eat = get_time_in_ms();
		pthread_mutex_unlock(&philo->eat_mutex);
		if (check_death(table, philo->id, NULL) == 1)
			return (1);
		usleep(100);
	}
	if (--philo->own_required_eat == 0)
	{
		pthread_mutex_lock(&table->finished_mutex);
		table->finished++;
		pthread_mutex_unlock(&table->finished_mutex);
		return (1);
	}
	return (0);
}

//function to put a philo to sleep
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

//function to kill the philo
void	philo_die(t_table *table, int id)
{
	pthread_mutex_lock(&table->deat_flag_mutex);
	if (table->death_flag == 1)
	{
		pthread_mutex_unlock(&table->deat_flag_mutex);
		return ;
	}
	table->death_flag = 1;
	show_event(get_time_in_ms(), id, "died");
	pthread_mutex_unlock(&table->deat_flag_mutex);
}
