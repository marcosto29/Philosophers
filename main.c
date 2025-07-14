/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcos <marcos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:02:41 by marcos            #+#    #+#             */
/*   Updated: 2025/07/14 10:38:26 by marcos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	module(int a, int b)
{
	return (b + (a % b)) % b;
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
void	*round_table(t_philosopher *philo)
{
	printf("Soy %d, Bloqueando... %d\n", philo->id, module(philo->id - 1, philo->info->philo_num));
	pthread_mutex_lock(&philo->info->forks[module(philo->id - 1, philo->info->philo_num)]);
	printf("Soy %d, Bloqueado %d\n", philo->id, module(philo->id - 1, philo->info->philo_num));
	printf("Soy %d, Bloqueando... %d\n", philo->id, module(philo->id + 1, philo->info->philo_num));
	pthread_mutex_lock(&philo->info->forks[module(philo->id + 1, philo->info->philo_num)]);
	printf("Soy %d, Bloqueado %d\n", philo->id, module(philo->id + 1, philo->info->philo_num));
	printf("Soy %d, using their fork\n", philo->id);
	printf("Soy %d, %d\n", philo->id, module(philo->id + 1, philo->info->philo_num));
	printf("Soy %d, %d\n", philo->id, module(philo->id - 1, philo->info->philo_num));
	usleep(1000000);
	pthread_mutex_unlock(&philo->info->forks[module(philo->id - 1, philo->info->philo_num)]);
	printf("Soy %d, Desbloqueado %d\n", philo->id, module(philo->id - 1, philo->info->philo_num));
	pthread_mutex_unlock(&philo->info->forks[module(philo->id + 1, philo->info->philo_num)]);
	printf("Soy %d, Desbloqueado %d\n", philo->id, module(philo->id + 1, philo->info->philo_num));
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_philosophers_info	*info;
	t_philosopher		**philosophers;
	int			counter;

	if (argc < 5 || argc > 6)
	{
		printf("incorrect number of arguments\n");
		exit(1);
	}
	info = configurate_philo(argc, argv);
	if (!info)
		exit(1);
	philosophers = malloc(sizeof(t_philosopher *) * info->philo_num);
	if (!philosophers)
		exit(1);
	counter = 0;
	while (counter < info->philo_num)
	{
		philosophers[counter] = create_philosopher(counter, info);
		if (!philosophers[counter])
		{
			free_philosophers(counter, philosophers);
			exit(1);
		}
		pthread_create(philosophers[counter]->thread, NULL, (void *)round_table, philosophers[counter]);
		pthread_mutex_init(&info->forks[counter], NULL);
		counter++;
	}
	counter = 0;
	while (counter < info->philo_num)
	{
		pthread_join(*(philosophers[counter]->thread), NULL);
		pthread_mutex_destroy(&info->forks[counter]);
		counter++;
	}
	free_philosophers(info->philo_num, philosophers);
	free_info(info);
}
