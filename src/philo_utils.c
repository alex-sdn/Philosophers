/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:16:30 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/15 14:16:31 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eating(t_start_data *data, t_philo_data *philo)
{
	pthread_mutex_lock(&data->forks[philo->my_forks[0]]);
	write_msg(data, philo->philo_id, MSG_FORK);
	if (data->philo_amt % 2 != 0 && philo->philo_id % 2 != 0)
		my_sleep(1);
	if (data->philo_amt == 1)
	{
		pthread_mutex_unlock(&data->forks[philo->my_forks[0]]);
		my_sleep(data->time_to_die);
		return ;
	}
	pthread_mutex_lock(&data->forks[philo->my_forks[1]]);
	write_msg(data, philo->philo_id, MSG_FORK);
	pthread_mutex_lock(&data->var);
	philo->last_eat = get_timestamp(data->time_0);
	philo->eat_count -= 1;
	pthread_mutex_unlock(&data->var);
	if (write_msg(data, philo->philo_id, MSG_EAT) == 0)
		my_sleep(data->time_to_eat);
	pthread_mutex_unlock(&data->forks[philo->my_forks[1]]);
	pthread_mutex_unlock(&data->forks[philo->my_forks[0]]);
}

int	write_msg(t_start_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->writing);
	pthread_mutex_lock(&data->var);
	if (data->over == 1 && ft_strncmp(msg, MSG_DEAD, 4) != 0)
		return (pthread_mutex_unlock(&data->var),
			pthread_mutex_unlock(&data->writing), 1);
	pthread_mutex_unlock(&data->var);
	printf("%ldms %d %s\n", get_timestamp(data->time_0), id, msg);
	pthread_mutex_unlock(&data->writing);
	return (0);
}

void	check_over(t_start_data **start_data, t_philo_data *philo_data)
{
	int	i;
	int	eat_qty;

	eat_qty = (*start_data)->eat_qty;
	while (1)
	{
		i = 0;
		while (i < (*start_data)->philo_amt)
		{
			if (check_death(start_data, philo_data[i]) == 1)
				return ;
			i++;
		}
		if (eat_qty > 0 && check_eat(start_data, philo_data) == 1)
			return ;
	}
}

int	check_death(t_start_data **start_data, t_philo_data philo_data)
{
	int	time_diff;

	pthread_mutex_lock(&(*start_data)->var);
	time_diff = get_timestamp((*start_data)->time_0) - philo_data.last_eat;
	if (time_diff >= (*start_data)->time_to_die)
	{
		(*start_data)->over = 1;
		pthread_mutex_unlock(&(*start_data)->var);
		write_msg(*start_data, philo_data.philo_id, MSG_DEAD);
		return (1);
	}
	pthread_mutex_unlock(&(*start_data)->var);
	return (0);
}

int	check_eat(t_start_data **start_data, t_philo_data *philo_data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(*start_data)->var);
	while (i < (*start_data)->philo_amt)
	{
		if (philo_data[i].eat_count > 0)
			break ;
		i++;
	}
	if (i == (*start_data)->philo_amt)
	{
		(*start_data)->over = 1;
		pthread_mutex_unlock(&(*start_data)->var);
		return (1);
	}
	pthread_mutex_unlock(&(*start_data)->var);
	return (0);
}
