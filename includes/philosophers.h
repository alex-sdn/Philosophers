/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:16:42 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/15 14:16:43 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DEAD "died"

typedef struct s_start_data {
	int				philo_amt;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_qty;
	int				over;
	struct timeval	time_0;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	var;
}					t_start_data;

typedef struct s_philo_data {
	int				philo_id;
	int				my_forks[2];
	long int		last_eat;
	int				eat_count;
	t_start_data	*start_data;
}					t_philo_data;

/**  main  **/
void			*philo_thread(void *philo_data);
t_start_data	*fill_start_data(char **av, int eat_qty, int i);
void			fill_philo_data(t_philo_data *me, t_start_data *start, int i);
int				start_threads(t_start_data *start_data);

/**  philo_utils  **/
void			eating(t_start_data *data, t_philo_data *philo);
int				write_msg(t_start_data *data, int id, char *msg);
void			check_over(t_start_data **start_data, t_philo_data *philo_data);
int				check_death(t_start_data **start_data, t_philo_data philo_data);
int				check_eat(t_start_data **start_data, t_philo_data *philo_data);

/**  utils  **/
long int		get_timestamp(struct timeval tv1);
void			my_sleep(long int time);
int				fill_forks(int id, int philo_amt, int fork_nb);
int				check_format(int ac, char **av);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *nptr);
char			*ft_itoa(int n);

#endif
