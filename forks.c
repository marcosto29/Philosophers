/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 21:10:37 by matoledo          #+#    #+#             */
/*   Updated: 2025/09/02 21:27:18 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//drop the fork
void	drop_fork(t_table *table, int id, int philo_id)
{
	pthread_mutex_lock(&table->forks[id]);
	table->forks_state[id] = 0;
	table->last_ate[id] = philo_id;
	pthread_mutex_unlock(&table->forks[id]);
}

//drop the side fork, different depending on the number of philos
void	drop_side_fork(t_table *table, int id, int philos)
{
	if (philos % 2 != 0)
		drop_fork(table, (id + 1) % philos, id);
	else
	{
		if (id % 2 == 0)
			drop_fork(table, (id + 1) % philos, id);
		else
			drop_fork(table, (id - 1 + philos) % philos, id);
	}
}

//take the fork
int	take_fork(t_table *table, int id, int philo_id)
{
	if (check_death(table, philo_id, NULL) == 1)
		return (2);
	pthread_mutex_lock(&table->forks[id]);
	if (table->last_ate[id] == philo_id)
	{
		pthread_mutex_unlock(&table->forks[id]);
		return (1);
	}
	if (table->forks_state[id] == 0)
	{
		table->forks_state[id] = 1;
		pthread_mutex_unlock(&table->forks[id]);
		if (check_death(table, philo_id, "has taken the fork") == 1)
			return (2);
		return (0);
	}
	pthread_mutex_unlock(&table->forks[id]);
	return (1);
}

//take both forks
//check to see who has used the last the fork
int	take_forks(t_table *table, int id, int first, int second)
{
	int	fork_return;

	while (1)
	{
		fork_return = take_fork(table, first, id);
		if (fork_return == 0)
			break ;
		if (fork_return == 2)
			return (1);
		usleep(100);
	}
	while (1)
	{
		fork_return = take_fork(table, second, id);
		if (fork_return == 0)
			break ;
		if (fork_return == 2)
			return (1);
		usleep(100);
	}
	return (0);
}

//layer to filter odds and even and have a rule to make deadlocks rarer
//check if the philo ate with the fork already
int	assign_forks(t_table *table, int id, int philos)
{
	if (philos % 2 != 0)
	{
		if (id % 2 == 0)
			return (take_forks(table, id, id, (id + 1) % philos));
		else
			return (take_forks(table, id, (id + 1) % philos, id));
	}
	if (id % 2 == 0)
		return (take_forks(table, id, id, (id + 1) % philos));
	else
		return (take_forks(table, id, (id - 1 + philos) % philos, id));
}
