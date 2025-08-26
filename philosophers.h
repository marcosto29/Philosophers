/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:04:45 by marcos            #+#    #+#             */
/*   Updated: 2025/08/26 18:24:58 by matoledo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct philosopher
{
	int					id;
	pthread_t			*thread;
	pthread_mutex_t		*last_eat_mutex;
	long				last_eat;
	pthread_mutex_t		*eat_mutex;
	int					own_required_eat;
}	t_philosopher;

typedef struct table
{
	pthread_mutex_t	**forks;
	int				*forks_state;
	pthread_mutex_t	*state_mutex;
	int				state;
	t_philosopher	**philos;
}	t_table;

typedef struct philo_table_config
{
	t_table			*table;
	t_philosopher	*philo;
	int				*config;
}	t_context;

int					parse_input(int *arguments, int argc, char **argv);
int					ft_size(int *array);
t_philosopher		*create_philosopher(int counter, int *config);
void				create_all_philosophers(int *config,
						t_philosopher **philosophers);
int					start_table(int *config, t_philosopher **philosophers);
void				round_table(t_context*philo);
void				free_philosopher(t_philosopher *philo);
void				free_philosophers(t_philosopher **philosophers, int size);
void				free_table(t_table *table, int size);
void				free_phi_tab_con(t_context **phi_tab_con,
						int size);
void				show_event(long actual_time, int philo_id, char *message);
long				get_time_in_ms(void);
int					check_death(t_table *table, int id, char *msg);
int					philo_think(t_table *table, int id);
int					philo_eat(t_table *table, t_philosopher *philo,
						int time_to_eat);
int					philo_sleep(t_table *table, int id,
						int time_to_sleep);
void				philo_die(t_table *table, int id);
#endif