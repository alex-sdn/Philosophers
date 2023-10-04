/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:16:25 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/15 14:16:26 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_thread(void *philo_data)
{
	t_philo_data	*my_data;
	t_start_data	*start_data;
	long int		time;
	struct timeval	t_0;

	my_data = (t_philo_data *)philo_data;
	start_data = my_data->start_data;
	t_0 = start_data->time_0;
	if (my_data->philo_id % 2 == 0)
		my_sleep(50);
	pthread_mutex_lock(&start_data->var);
	while (start_data->over == 0)
	{
		pthread_mutex_unlock(&start_data->var);
		eating(start_data, my_data);
		time = get_timestamp(t_0);
		if (write_msg(start_data, my_data->philo_id, MSG_SLEEP) == 0)
			my_sleep(start_data->time_to_sleep - (get_timestamp(t_0) - time));
		write_msg(start_data, my_data->philo_id, MSG_THINK);
		pthread_mutex_lock(&start_data->var);
	}
	pthread_mutex_unlock(&start_data->var);
	return (NULL);
}

t_start_data	*fill_start_data(char **av, int eat_qty, int i)
{
	t_start_data	*start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	var;

	start = malloc(sizeof(t_start_data));
	if (!start)
		return (NULL);
	start->philo_amt = ft_atoi(av[1]);
	start->time_to_die = ft_atoi(av[2]);
	start->time_to_eat = ft_atoi(av[3]);
	start->time_to_sleep = ft_atoi(av[4]);
	start->eat_qty = eat_qty;
	start->over = 0;
	forks = malloc(sizeof(pthread_mutex_t) * start->philo_amt);
	if (!forks)
		return (free(start), NULL);
	while (i < start->philo_amt)
		pthread_mutex_init(&forks[i++], NULL);
	start->forks = forks;
	pthread_mutex_init(&writing, NULL);
	start->writing = writing;
	pthread_mutex_init(&var, NULL);
	start->var = var;
	return (start);
}

void	fill_philo_data(t_philo_data *me, t_start_data *start, int i)
{
	me->philo_id = i + 1;
	me->my_forks[0] = fill_forks(i + 1, start->philo_amt, 0);
	me->my_forks[1] = fill_forks(i + 1, start->philo_amt, 1);
	me->last_eat = 0;
	me->eat_count = start->eat_qty;
	me->start_data = start;
}

int	start_threads(t_start_data *start_data)
{
	pthread_t		*thread_id;
	t_philo_data	*philo_data;
	int				i;

	thread_id = malloc(sizeof(pthread_t) * start_data->philo_amt);
	if (!thread_id)
		return (1);
	philo_data = malloc(sizeof(t_philo_data) * start_data->philo_amt);
	if (!philo_data)
		return (free(thread_id), 1);
	i = -1;
	while (++i < start_data->philo_amt)
		fill_philo_data(&philo_data[i], start_data, i);
	i = -1;
	gettimeofday(&start_data->time_0, NULL);
	while (++i < start_data->philo_amt)
		if (pthread_create(&thread_id[i], NULL, philo_thread, &philo_data[i]))
			return (free(thread_id), free(philo_data), 1);
	check_over(&start_data, philo_data);
	i = -1;
	while (++i < start_data->philo_amt)
		pthread_join(thread_id[i], NULL);
	free(thread_id);
	free(philo_data);
	return (0);
}

int	main(int ac, char **av)
{
	t_start_data	*start_data;
	struct timeval	time_0;
	int				i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (printf("Wrong arg amount.\n"), 1);
	if (check_format(ac, av) == 0)
		return (printf("Wrong arg format.\n"), 1);
	if (ac == 5)
		start_data = fill_start_data(av, -1, 0);
	else
		start_data = fill_start_data(av, ft_atoi(av[5]), 0);
	if (!start_data)
		return (1);
	gettimeofday(&time_0, NULL);
	start_data->time_0 = time_0;
	start_threads(start_data);
	while (i < start_data->philo_amt)
		pthread_mutex_destroy(&start_data->forks[i++]);
	pthread_mutex_destroy(&start_data->writing);
	pthread_mutex_destroy(&start_data->var);
	free(start_data->forks);
	free(start_data);
	return (0);
}
