/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcos <marcos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:04:45 by marcos            #+#    #+#             */
/*   Updated: 2025/07/13 18:23:02 by marcos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct philosophers_information
{
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				required_eat;
	pthread_mutex_t	*forks;
}	t_philosophers_info;

typedef struct philosopher
{
	t_philosophers_info	*info;
	pthread_t			*thread;
	int					id;
}	t_philosopher;

int					parse_to_number(char *argument);
t_philosophers_info	*configurate_philo(int argc, char *argv[]);
t_philosopher		*create_philosopher(int counter, t_philosophers_info *info);
void				free_info(t_philosophers_info *config);
void				free_philosopher(t_philosopher *philo);
void				free_philosophers(int size, t_philosopher **philosophers);
#endif