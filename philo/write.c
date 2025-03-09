/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 14:31:50 by gdosch            #+#    #+#             */
/*   Updated: 2025/03/09 14:31:52 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void ft_write_status_debug(t_philo_status status, t_philo *philo, long elapsed_time)
{
	if (status == TAKING_LEFT_FORK && !ft_sim_is_over(philo->data))
		printf("%6ld %d has taken the left fork (ID %d)\n", elapsed_time, philo->id, philo->left_fork->id);
	else if (status == TAKING_RIGHT_FORK && !ft_sim_is_over(philo->data))
		printf("%6ld %d has taken the right fork (ID %d)\n", elapsed_time, philo->id, philo->right_fork->id);
	else if (status == EATING && !ft_sim_is_over(philo->data))
		printf("%6ld %d is eating meal No.%d\n", elapsed_time, philo->id, philo->meal_ct);
	else if (status == SLEEPING && !ft_sim_is_over(philo->data))
		printf("%6ld %d is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING && !ft_sim_is_over(philo->data))
		printf("%6ld %d is thinking\n", elapsed_time, philo->id);
	else if (status == DIED)
		printf("%6ld %d died\n", elapsed_time, philo->id);
}

void	ft_write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed_time;

	elapsed_time = ft_get_time(MILLISECOND) - philo->data->start_time;
	if (philo->is_full)
		return ;
	pthread_mutex_lock(&philo->data->wr_mutex);
	if (debug)
		ft_write_status_debug(status, philo, elapsed_time);
	else
	{
		if ((status == TAKING_LEFT_FORK || status == TAKING_RIGHT_FORK)
			&& !ft_sim_is_over(philo->data))
			printf("%6ld %d has taken a fork\n", elapsed_time, philo->id);
		else if (status == EATING && !ft_sim_is_over(philo->data))
			printf("%6ld %d is eating\n", elapsed_time, philo->id);
		else if (status == SLEEPING && !ft_sim_is_over(philo->data))
			printf("%6ld %d is sleeping\n", elapsed_time, philo->id);
		else if (status == THINKING && !ft_sim_is_over(philo->data))
			printf("%6ld %d is thinking\n", elapsed_time, philo->id);
		else if (status == DIED)
			printf("%6ld %d died\n", elapsed_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->wr_mutex);
}
