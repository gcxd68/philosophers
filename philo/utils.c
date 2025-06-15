/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:48:14 by gdosch            #+#    #+#             */
/*   Updated: 2025/05/26 15:53:21 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_destroy_mutexes(int n, t_data *data)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		pthread_mutex_destroy(&data->philo[i].mutex);
		pthread_mutex_destroy(&data->fork[i].mutex);
	}
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

void	ft_cleanup(t_data *data)
{
	if (data->mutex_init)
		ft_destroy_mutexes(data->philo_nbr, data);
	if (data->fork)
		free(data->fork);
	if (data->philo)
		free(data->philo);
}

void	ft_error(const char *err_msg, t_om output_mode)
{
	size_t	len;

	if (output_mode == PERROR)
		perror(err_msg);
	else if (output_mode == WRITE)
	{
		len = 0;
		while (err_msg[len])
			len++;
		if (write(2, err_msg, len) < 0)
			perror("philo: write failed");
	}
}

long	ft_get_time(t_tc time_code, t_data *data)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		ft_set_bool(&data->state_mutex, &data->end_sim, true);
		ft_error("philo: gettimeofday failed", PERROR);
		return (-1);
	}
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (0);
}

long	ft_atol_p(const char *nptr)
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
