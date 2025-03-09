/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:48:14 by gdosch            #+#    #+#             */
/*   Updated: 2025/03/09 12:48:15 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_get_time(t_time_code time_code) // MODIFIER SI ON GET TJRS EN MILLISECONDES
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		ft_error_exit("gettimeofday failed");
	if (SECOND == time_code)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (MILLISECOND == time_code)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (MICROSECOND == time_code)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		ft_error_exit("Wrong input to gettime");
	return(0);
}

void	ft_usleep(long sleep_time, t_data *data)
{
	long	start_time;
	long	remaining_time;
	long	current_time;

	start_time = ft_get_time(MICROSECOND);
	current_time = start_time;
	while (current_time - start_time < sleep_time)
	{
		if (ft_sim_is_over(data))
			break ;
		remaining_time = start_time + sleep_time - current_time;
		if (remaining_time > 1e3)
			usleep (remaining_time / 2);
		else
			while (current_time - start_time < sleep_time)
				current_time = ft_get_time(MICROSECOND);
		current_time = ft_get_time(MICROSECOND);
	}
}

void	ft_error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}
