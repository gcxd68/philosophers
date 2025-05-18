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
	return ((long)res);
}

static int	ft_parse_input(t_data *data, char *argv[])
{
	*data = (t_data){
		.philo_nbr = ft_atol_p(argv[1]),
		.time_to_die = ft_atol_p(argv[2]) * 1e3,
		.time_to_eat = ft_atol_p(argv[3]) * 1e3,
		.time_to_sleep = ft_atol_p(argv[4]) * 1e3
	};
	if (data->philo_nbr < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (-1);
	if (argv[5])
	{
		data->max_meals = ft_atol_p(argv[5]);
		if (data->max_meals < 0)
			return (-1);
	}
	else
		data->max_meals = -1;
	if (data->time_to_die < 6e4
		|| data->time_to_eat < 6e4
		|| data->time_to_sleep < 6e4)
		return (-1);
	return (0);
}

static void	ft_assign_forks(t_philo *philo, t_fork *fork)
{
	if (philo->id % 2)
	{
		philo->left_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
		philo->right_fork = &fork[philo->id];
	}
	else
	{
		philo->right_fork = &fork[philo->id];
		philo->left_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
	}
}

static void	ft_init_data(t_data *data)
{
	int	i;

	data->fork = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->fork)
		ft_error_exit("philo: malloc failed", PERROR);
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philo)
	{
		free(data->fork);
		ft_error_exit("philo: malloc failed", PERROR);
	}
	pthread_mutex_init(&data->state_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	i = -1;
	while (++i < data->philo_nbr)
	{
		pthread_mutex_init(&data->fork[i].mutex, NULL);
		data->fork[i].id = i;
		data->philo[i].id = i;
		data->philo[i].data = data;
		pthread_mutex_init(&data->philo[i].mutex, NULL);
		ft_assign_forks(&data->philo[i], data->fork);
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		if (write(2, "Usage : ./philo number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n", 121) < 0)
		{
			ft_error_exit("philo: write failed", PERROR);
			return (EXIT_FAILURE);
		}
		return (2);
	}
	if (ft_parse_input(&data, argv) < 0)
	{
		if (write(2, "philo: arguments must be positive integers\n", 43) < 0)
		{
			ft_error_exit("philo: write failed", PERROR);
			return (EXIT_FAILURE);
		}
		return (2);
	}
	ft_init_data(&data);
	ft_sim(&data);
	return (EXIT_SUCCESS);
}
