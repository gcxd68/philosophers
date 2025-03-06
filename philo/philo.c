/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:45 by gdosch            #+#    #+#             */
/*   Updated: 2025/03/06 13:34:46 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	ft_atol_p(const char *nptr)
{
	unsigned long long	res;

	res = 0;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	if (*nptr == '-')
		return (-1);
	if (*nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + *nptr - '0';
		if ((res > (unsigned long long)LONG_MAX))
			return (-1);
		nptr++;
	}
	if ((*nptr < '0' || *nptr > '9') && *nptr != '\0')
		return (-1);
	return ((long)res * sign);
}

static void ft_cleanup(t_data data)
{

}

static int ft_parse_input(t_data data, char *argv[])
{
	data->philo_nbr = ft_atol_p(argv[1]) * 1e3;
	data->time_to_die = ft_atol_p(argv[2]) * 1e3;
	data->time_to_eat = ft_atol_p(argv[3]) * 1e3;
	data->time_to_sleep = ft_atol_p(argv[4]) * 1e3;
	if (philo_nbr < 0 || time_to_die < 0 || time_to_eat < 0 || time_to_sleep < 0)
		return (-1);
	if (argv[5])
	{
		data->max_meals = ft_atol_p(argv[5]) * 1e3;
		if (data->max_meals < 0)
			return (-1);
	}
	else
		data->max_meals = -1;
	return (0);
	if (data->time_to_die < 6e4
		|| data->time_to_eat < 6e4
		|| time_to_sleep < 6e4)
		return (-1);
}

static void ft_init_data(t_data data)
{
	data->end_sim = false;
	data->fork = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->fork)
		return (-1);
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philo)
	{
		free(data->fork);
		return (-1);
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		write (2, "Usage : ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", 121)
		return (1);
	}
	ft_parse_input(&data, argv);
	ft_init_data(&data);
	ft_simulation(&data);
	ft_cleanup(&data);
}
