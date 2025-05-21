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

static void	ft_parse_input(t_data *data, char *argv[])
{
	*data = (t_data){
		.philo_nbr = ft_atol_p(argv[1]),
		.time_to_die = ft_atol_p(argv[2]) * 1e3,
		.time_to_eat = ft_atol_p(argv[3]) * 1e3,
		.time_to_sleep = ft_atol_p(argv[4]) * 1e3
	};
	if (argv[5])
		data->max_meals = ft_atol_p(argv[5]);
	if (data->philo_nbr < 0 || data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0 || data->max_meals < 0)
		ft_error_exit("philo: arguments cannot be negative integers\n", 2, WRITE, data);
	if (!argv[5])
		data->max_meals = -1;
//	if (data->time_to_die < 6e4
//		|| data->time_to_eat < 6e4
//		|| data->time_to_sleep < 6e4)
//		ft_error_exit(???);
	if (data->philo_nbr == 0 || data->max_meals == 0)
		ft_error_exit(NULL, 0, NO_PRINT, data);
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

static void	ft_init_data(t_data *data)
{
	int	i;

	data->fork = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->fork)
		ft_error_exit("philo: malloc failed", 1, PERROR, data);
	data->philo = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philo)
		ft_error_exit("philo: malloc failed", 1, PERROR, data);
	pthread_mutex_init(&data->state_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	i = -1;
	while (++i < data->philo_nbr)
	{
		pthread_mutex_init(&data->fork[i].mutex, NULL);
		data->fork[i].id = i;
		data->philo[i] = (t_philo){.id = i, .data = data};
		pthread_mutex_init(&data->philo[i].mutex, NULL);
		ft_assign_forks(&data->philo[i], data->fork);
	}
	data->mutex_init = true;
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
		ft_error_exit("Usage : ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", 2, WRITE, &data);
	ft_parse_input(&data, argv);
	ft_init_data(&data);
	ft_sim(&data);
	ft_cleanup_data(&data);
	return (EXIT_SUCCESS);
}
