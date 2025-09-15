/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:17:36 by matoledo          #+#    #+#             */
/*   Updated: 2025/09/15 23:29:49 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//size of an array
int	ft_size(int *array)
{
	int	size;

	size = 0;
	while (*array)
	{
		size++;
		array++;
	}
	return (size);
}

//get time of the day in millisecons
long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

//function to show the event with the time
void	show_event(long actual_time, int philo_id, char *message)
{
	static long		start_time;

	if (!start_time)
		start_time = get_time_in_ms();
	printf("%ld %d %s\n", actual_time - start_time, philo_id, message);
}

//function to check wether a philos has died
//it also prints an action if necessary
//death_flag_mutex used also to print correctly
int	check_death(t_table *table, int id, char *msg)
{
	pthread_mutex_lock(&table->death_flag_mutex);
	if (table->death_flag == 1)
	{
		pthread_mutex_unlock(&table->death_flag_mutex);
		return (1);
	}
	if (msg != NULL)
		show_event(get_time_in_ms(), id, msg);
	pthread_mutex_unlock(&table->death_flag_mutex);
	return (0);
}
