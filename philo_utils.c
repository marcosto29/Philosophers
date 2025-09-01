/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:17:36 by matoledo          #+#    #+#             */
/*   Updated: 2025/09/01 20:19:01 by matoledo         ###   ########.fr       */
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
	printf("%ld ms %d %s\n", actual_time - start_time, philo_id, message);
}

//function to check wether a philos has died
//it also prints an action if necessary
//death_flag_mutex used also to print correctly
int	check_death(t_table *table, int id, char *msg)
{
	pthread_mutex_lock(&table->deat_flag_mutex);
	if (table->death_flag == 1)
	{
		pthread_mutex_unlock(&table->deat_flag_mutex);
		return (1);
	}
	if (msg != NULL)
		show_event(get_time_in_ms(), id, msg);
	pthread_mutex_unlock(&table->deat_flag_mutex);
	return (0);
}

//thread to check when a philo dies or when everynoe finished their food
void	monitor_check(t_monitor_context *ctx)
{
	int	counter;

	while (1)
	{
		counter = 0;
		while (counter < ctx->config[0])
		{
			pthread_mutex_lock(&ctx->table->philos[counter]->eat_mutex);
			if (get_time_in_ms() >= ctx->table->philos[counter]->eat
				+ ctx->config[1])
			{
				pthread_mutex_unlock(&ctx->table->philos[counter]->eat_mutex);
				pthread_mutex_lock(&ctx->table->finished_mutex);
				if (ctx->table->finished == ctx->config[0]
					|| ctx->table->philos[counter]->own_required_eat == 0)
				{
					pthread_mutex_unlock(&ctx->table->finished_mutex);
					return ;
				}
				philo_die(ctx->table, counter);
				pthread_mutex_unlock(&ctx->table->finished_mutex);
				return ;
			}
			pthread_mutex_unlock(&ctx->table->philos[counter]->eat_mutex);
			counter++;
		}
	}
}
