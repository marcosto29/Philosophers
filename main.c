/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:02:41 by marcos            #+#    #+#             */
/*   Updated: 2025/08/18 12:24:47 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_sleep(t_philosopher *philo)
{
	long	wait_time;

	wait_time = get_time_in_ms() + philo->info->time_to_sleep;
	show_event(get_time_in_ms(), philo->id, "is sleeping");
	while (get_time_in_ms() < wait_time)
		usleep(100);
}

void	philo_eat(t_philosopher *philo)
{
	long	wait_time;

	pthread_mutex_lock(philo->last_eat_mutex);
	philo->last_eat = get_time_in_ms();
	wait_time = philo->last_eat + (philo->info->time_to_eat);
	show_event(philo->last_eat, philo->id, "is eating");
	while (get_time_in_ms() < wait_time)
		usleep(100);
	pthread_mutex_unlock(philo->last_eat_mutex);
}

void take_right_fork(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->info->forks[(philo->id + 1) % philo->info->philo_num]);
}

void take_left_fork(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->info->forks[philo->id]);
}

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		take_left_fork(philo);
		take_right_fork(philo);
	}
	else
	{
		take_left_fork(philo);
		take_right_fork(philo);
	}
}

void	drop_fork(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->info->forks[philo->id]);
	pthread_mutex_unlock(&philo->info->forks[(philo->id + 1) % philo->info->philo_num]);
}

//filósofos en mesa redonda
//iterativamente verán si necesitan comer, pensar, o dormir.
//Mientras están comiendo, no pueden pensar ni dormir;
//Mientras están pensando, no pueden dormir ni comer;
//Mientras están durmiendo, no pueden comer ni pensar.

//para comer habrá tantos tenedores como filosofos
//pero un filosofo necesita 2 tenedores para comer
//el de la derecha y el de la izquierda 
//cada tenedor va a estar representado con un mutex

//para gestionar la inanición, hacer un thread con detach
//el thread comprobará periodicamente hace cuanto no come
//si lleva mucho tiempo si comer muere

//detached thread to check starve levels
void	starve_timer(t_philosopher *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->last_eat_mutex);
		if (get_time_in_ms() >= philo->last_eat + philo->info->time_to_die)
		{
			pthread_mutex_unlock(philo->last_eat_mutex);
			break ;
		}
		pthread_mutex_unlock(philo->last_eat_mutex);
	}
	if (philo->eat == 0)
		return ;
	show_event(get_time_in_ms(), philo->id, "died");
	pthread_mutex_lock(philo->info->state_mutex);
	philo->info->state = -1;
	pthread_mutex_unlock(philo->info->state_mutex);
}

void	iterative_actions(t_philosopher *philo)
{
	while (1 && philo->eat != 0)
	{
		show_event(get_time_in_ms(), philo->id, "is thinking");
		take_forks(philo);
		show_event(get_time_in_ms(), philo->id, "has taken a fork");
		philo_eat(philo);
		drop_fork(philo);
		philo->eat--;
		philo_sleep(philo);
	}
	pthread_mutex_lock(philo->info->state_mutex);
	philo->info->state++;
	pthread_mutex_unlock(philo->info->state_mutex);
}

void	round_table(t_philosopher *philo)
{
	pthread_t		death_check;
	pthread_t		routine;

	philo->last_eat = get_time_in_ms();
	pthread_create(&routine, NULL, (void *)iterative_actions, philo);
	pthread_detach(routine);
	pthread_create(&death_check, NULL, (void *)starve_timer, philo);
	pthread_detach(death_check);
}

int	main(int argc, char *argv[])
{
	t_philo_info	*info;
	t_philosopher	**philosophers;
	int				*arguments;
	int				status;

	arguments = malloc(sizeof(int) * argc - 1);
	if (parse_input(arguments, argc, argv) == 1)
		return (1);
	info = configurate_philo(argc, arguments);
	free(arguments);
	if (!info)
		return (1);
	philosophers = malloc(sizeof(t_philosopher *) * info->philo_num);
	if (!philosophers)
		exit(1);
	create_all_philosophers(info, philosophers);
	//hacer que join devuelva un numero que si se mnuere sea un numero para en el main return y acabar 
	status = end_threads(info, philosophers);
	free_philosophers(info->philo_num, philosophers);
	free_info(info);
	return (status);
}
