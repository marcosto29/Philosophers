/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matoledo <matoledo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:04:45 by marcos            #+#    #+#             */
/*   Updated: 2025/09/10 17:23:28 by matoledo         ###   ########.fr       */
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
	pthread_mutex_t		eat_mutex;
	long				eat;
	int					own_required_eat;
}	t_philosopher;

typedef struct table
{
	pthread_mutex_t	*forks;
	int				*forks_state;
	int				*last_fork_philo;
	pthread_mutex_t	deat_flag_mutex;
	int				death_flag;
	pthread_mutex_t	finished_mutex;
	int				finished;
	t_philosopher	**philos;
}	t_table;

typedef struct philo_context
{
	t_table			*table;
	t_philosopher	*philo;
	int				*config;
}	t_philo_context;

typedef struct monitor_context
{
	t_table	*table;
	int		*config;
}	t_monitor_context;

int					parse_input(int *arguments, int argc, char **argv);
int					ft_size(int *array);
t_table				*create_table(int *config, t_philosopher **philosophers);
void				create_monitor(t_table *table, int *config);
void				monitor_check(t_monitor_context *ctx);
int					initialize_philosophers(int *config,
						t_philosopher **philosophers, int argc);
void				free_philosopher(t_philosopher *philo);
void				free_philosophers(t_philosopher **philosophers, int size);
void				free_table(t_table *table, int size);
void				free_phi_tab_con(t_philo_context **phi_tab_con,
						int size);
void				show_event(long actual_time, int philo_id, char *message);
long				get_time_in_ms(void);
int					check_death(t_table *table, int id, char *msg);
void				monitor_check(t_monitor_context *ctx);
int					philo_think(t_table *table, int id);
int					assign_forks(t_table *table, int id, int philos);
void				drop_fork(t_table *table, int id, int philo_id);
void				drop_side_fork(t_table *table, int id, int philos);
int					philo_eat(t_table *table, t_philosopher *philo,
						int time_to_eat);
int					philo_sleep(t_table *table, int id,
						int time_to_sleep);
void				philo_die(t_table *table, int id);
#endif