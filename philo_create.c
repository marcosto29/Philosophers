/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcos <marcos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:43 by marcos            #+#    #+#             */
/*   Updated: 2025/07/13 17:07:50 by marcos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philosophers_info	*configurate_philo(int argc, char *argv[])
{
	t_philosophers_info	*info;

	info = malloc(sizeof(t_philosophers_info));
	if (!info)
		return (NULL);
	info->philo_num = parse_to_number(argv[1]);
	if (info->philo_num < 3)
		return (NULL);
	info->time_to_die = parse_to_number(argv[2]);
	info->time_to_eat = parse_to_number(argv[3]);
	info->time_to_sleep = parse_to_number(argv[4]);
	if (argc == 6)
		info->required_eat = parse_to_number(argv[5]);
	info->forks = malloc(sizeof(pthread_mutex_t) * info->philo_num);
	if (!info->forks)
		return (NULL);
	return (info);
}

t_philosopher	*create_philosopher(int counter, t_philosophers_info *info)
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
	philosopher->info = info;
	return (philosopher);
}