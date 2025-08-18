/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:04:45 by marcos            #+#    #+#             */
/*   Updated: 2025/08/18 11:20:20 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct philosophers_information
{
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				required_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t *state_mutex;
	int				state;
}	t_philo_info;

typedef struct philosopher
{
	t_philo_info		*info;
	pthread_t			*thread;
	int					id;
	long				last_eat;
	int					eat;
	pthread_mutex_t		*last_eat_mutex;
}	t_philosopher;

int					parse_input(int *arguments, int argc, char **argv);
int					parse_to_number(char *argument);
t_philo_info		*configurate_philo(int argc, int *arguments);
t_philosopher		*create_philosopher(int counter, t_philo_info *info);
void				create_all_philosophers(t_philo_info *info,
						t_philosopher **philosophers);
int				end_threads(t_philo_info *info,
						t_philosopher **philosophers);
void				free_info(t_philo_info *config);
void				free_philosopher(t_philosopher *philo);
void				free_philosophers(int size, t_philosopher **philosophers);
void				round_table(t_philosopher *philo);
void				show_event(long actual_time, int philo_id, char *message);
long				get_time_in_ms(void);
#endif