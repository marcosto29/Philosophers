/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:43 by marcos            #+#    #+#             */
/*   Updated: 2025/08/18 12:33:36 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo_info	*configurate_philo(int argc, int *arguments)
{
	t_philo_info	*info;

	info = malloc(sizeof(t_philo_info));
	if (!info)
		return (NULL);
	info->philo_num = arguments[0];
	info->time_to_die = arguments[1];
	info->time_to_eat = arguments[2];
	info->time_to_sleep = arguments[3];
	if (argc == 6)
		info->required_eat = arguments[4];
	info->forks = malloc(sizeof(pthread_mutex_t) * info->philo_num);
	if (!info->forks)
	{
		free_info(info);
		return (NULL);
	}
	info->state = 0;
	info->state_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(info->state_mutex, NULL);
	return (info);
}

t_philosopher	*create_philosopher(int counter, t_philo_info *info)
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
	if (info->required_eat)
		philosopher->eat = info->required_eat;
	else
		philosopher->eat = -1;
	philosopher->last_eat_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philosopher->last_eat_mutex, NULL);
	philosopher->info = info;
	return (philosopher);
}

void	create_all_philosophers(t_philo_info *info,
	t_philosopher **philosophers)
{
	int	counter;

	counter = 0;
	while (counter < info->philo_num)
	{
		philosophers[counter] = create_philosopher(counter, info);
		if (!philosophers[counter])
		{
			free_philosophers(counter, philosophers);
			exit(1);
		}
		pthread_create(philosophers[counter]->thread, NULL,
			(void *)round_table, philosophers[counter]);
		pthread_mutex_init(&info->forks[counter], NULL);
		counter++;
	}
}

int	end_threads(t_philo_info *info,
	t_philosopher **philosophers)
{
	int		counter;

	counter = 0;
	while (1)
	{
		pthread_mutex_lock(info->state_mutex);
		if (info->state == -1 || info->state == info->philo_num)
			break ;
		pthread_mutex_unlock(info->state_mutex);
	}
	while (counter < info->philo_num)
	{
		pthread_join(*(philosophers[counter]->thread), NULL);
		pthread_mutex_destroy(&info->forks[counter]);
		pthread_mutex_destroy(philosophers[counter]->last_eat_mutex);
		counter++;
	}
	if (info->state == -1)
		return (1);
	return (0);
}
