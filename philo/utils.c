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

void	ft_destroy_mutexes(int philo_nbr, t_data *data)
{
	int	i;

	i = -1;
	while (++i < philo_nbr)
	{
		pthread_mutex_destroy(&data->philo[i].mutex);
		pthread_mutex_destroy(&data->fork[i].mutex);
	}
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

int	ft_cleanup(t_data *data, int error_code)
{
	if (!data)
		return (0);
	if (data->mutex_init)
		ft_destroy_mutexes(data->philo_nbr, data);
	if (data->fork)
		free(data->fork);
	if (data->philo)
		free(data->philo);
	return (error_code);
}

int	ft_error(const char *err_msg, t_om output_mode, int error_code)
{
	size_t	err_msg_len;

	if (output_mode == PERROR)
		perror(err_msg);
	else if (output_mode == WRITE)
	{
		err_msg_len = 0;
		while (err_msg[err_msg_len])
			err_msg_len++;
		if (write (2, err_msg, err_msg_len) < 0)
			perror("philo: write failed");
	}
	return (error_code);
}

long	ft_get_time(t_tc time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		return (ft_error("philo: gettimeofday failed", PERROR, -1));
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
