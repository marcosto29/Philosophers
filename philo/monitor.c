/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 21:34:51 by matoledo          #+#    #+#             */
/*   Updated: 2025/09/02 21:35:19 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//check the state of a philosopher
int	check_philo_state(t_table *table, t_philosopher *philo,
	int die_time, int philos)
{
	pthread_mutex_lock(&philo->eat_mutex);
	if (get_time_in_ms() >= philo->eat + die_time)
	{
		pthread_mutex_unlock(&philo->eat_mutex);
		pthread_mutex_lock(&table->finished_mutex);
		if (table->finished == philos
			|| philo->own_required_eat == 0)
		{
			pthread_mutex_unlock(&table->finished_mutex);
			return (1);
		}
		philo_die(table, philo->id);
		pthread_mutex_unlock(&table->finished_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->eat_mutex);
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
			if (check_philo_state(ctx->table, ctx->table->philos[counter],
					ctx->config[1], ctx->config[0]) == 1)
				return ;
			counter++;
		}
		usleep(100);
	}
}
