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

static void ft_parse_input(t_data data, char *argv[])
{
	data->philo_nbr = ft_atol_p(argv[1]);
	data->time_to_die = ft_atol_p(argv[2]);
	data->time_to_eat = ft_atol_p(argv[3]);
	data->time_to_sleep = ft_atol_p(argv[4]);
	if (philo_nbr < 0 || time_to_die < 0 || time_to_eat < 0 || time_to_sleep < 0)
		return (-1);
	if (argv[5])
	{
		data->max_meals = ft_atol_p(argv[5]);
		if (data->max_meals < 0)
			return (-1);
	}
	else
		data->max_meals = -1;










	if (argv[5])
		data->max_meals = ft_atol_p(argv[5]);
	else
		data->max_meals = -1;


	if (philo_nbr < 0 || philo_nbr > (long long)UINT_MAX
		|| time_to_die < 0 || time_to_die > (long long)LONG_MAX
		|| time_to_eat < 0 || time_to_eat > (long long)LONG_MAX
		|| time_to_sleep < 0 || time_to_sleep > (long long)LONG_MAX
		|| max_meals < -1 || max_meals > (long long)INT_MAX)
		return (-1);

	data->philo_nbr = (long)philo_nbr;
	data->time_to_die = (long)time_to_die;
	data->time_to_eat = (long)time_to_eat;
	data->time_to_sleep = (long)time_to_sleep;
	data->max_meals = (long)max_meals;
}

static void ft_init_data(t_data data)
{

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
