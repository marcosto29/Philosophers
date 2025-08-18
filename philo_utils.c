/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:17:36 by matoledo          #+#    #+#             */
/*   Updated: 2025/08/15 12:56:44 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	printf("%ld ms %d %s\n", actual_time - start_time, philo_id, message);
}

