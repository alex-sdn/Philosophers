/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:16:33 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/15 14:16:34 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	get_timestamp(struct timeval tv1)
{
	struct timeval	tv2;

	gettimeofday(&tv2, NULL);
	return ((tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec
			/ 1000 - tv1.tv_usec / 1000));
}

void	my_sleep(long int time)
{
	struct timeval	tv1;

	gettimeofday(&tv1, NULL);
	while (get_timestamp(tv1) < time)
		usleep(5);
}

int	fill_forks(int id, int philo_amt, int fork_nb)
{
	if (id % 2 != 0)
	{
		if (fork_nb == 0)
			return (id - 1);
		else if (id == 1)
			return (philo_amt - 1);
		else
			return (id - 2);
	}
	else if (fork_nb == 0)
		return (id - 2);
	else
		return (id - 1);
}

int	check_format(int ac, char **av)
{
	int		i;
	int		j;
	char	*nbr;

	i = 0;
	j = 0;
	while (++i < ac)
	{
		while (av[i][j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (0);
			j++;
		}
		if (j == 0 || (j == 1 && av[i][0] == '0') || j > 10)
			return (0);
		nbr = ft_itoa(ft_atoi(av[i]));
		if (ft_strncmp(nbr, av[i], j) != 0)
			return (free(nbr), 0);
		free(nbr);
		j = 0;
	}
	return (1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i] && i < n)
		i++;
	if (s1[i] != s2[i] && i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
