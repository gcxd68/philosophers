/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:45 by gdosch            #+#    #+#             */
/*   Updated: 2025/05/21 17:25:07 by gdosch           ###   ########.fr       */
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
	data->philo_nbr = ft_atol_p(argv[1]);
	data->time_to_die = ft_atol_p(argv[2]) * 1e3;
	data->time_to_eat = ft_atol_p(argv[3]) * 1e3;
	data->time_to_sleep = ft_atol_p(argv[4]) * 1e3;
	if (argv[5])
		data->max_meals = ft_atol_p(argv[5]);
	if (data->philo_nbr < 0 || data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0 || data->max_meals < 0)
		return (ft_error("philo: "
				"arguments must be positive integers or zero\n", WRITE, 2));
	if (!argv[5])
		data->max_meals = -1;
	if (!data->philo_nbr || !data->max_meals)
		return (ft_error("philo: simulation cannot proceed with zero "
				"philosophers or a maximum meal count of zero\n", WRITE, 3));
	return (0);
}

static void	ft_assign_forks(t_philo *philo, t_fork *fork)
{
	if (philo->id % 2)
	{
		philo->first_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
		philo->second_fork = &fork[philo->id];
	}
	else
	{
		philo->first_fork = &fork[philo->id];
		philo->second_fork = &fork[(philo->id + 1) % philo->data->philo_nbr];
	}
}

static int	ft_init_data(t_data *data)
{
	int	i;

	data->fork = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->fork)
		return (ft_error("philo: malloc failed", PERROR, 1));
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philo)
		return (ft_error("philo: malloc failed", PERROR, 1));
	if (pthread_mutex_init(&data->state_mutex, NULL)
		|| pthread_mutex_init(&data->write_mutex, NULL))
		return (ft_error("philo: mutex init failed", PERROR, 1));
	data->mutex_init = true;
	i = -1;
	while (++i < data->philo_nbr)
	{
		data->fork[i].id = i;
		data->philo[i] = (t_philo){.id = i, .data = data};
		if (pthread_mutex_init(&data->fork[i].mutex, NULL)
			|| pthread_mutex_init(&data->philo[i].mutex, NULL))
			return (ft_error("philo: mutex init failed", PERROR, 1));
		ft_assign_forks(&data->philo[i], data->fork);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		exit_code;
	int		ret;

	data = (t_data){0};
	exit_code = EXIT_SUCCESS;
	if (argc < 5 || argc > 6)
		return (ft_error("Usage : ./philo number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n", WRITE, 2));
	ret = ft_parse_input(&data, argv);
	if (ret == 2)
		return (ft_cleanup(&data, 2));
	else if (ret)
		return (ft_cleanup(&data, EXIT_SUCCESS));
	if (ft_init_data(&data))
		return (ft_cleanup(&data, EXIT_FAILURE));
	if (ft_sim(&data))
		exit_code = EXIT_FAILURE;
	return (ft_cleanup(&data, exit_code));
}
