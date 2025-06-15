/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:05:04 by gdosch            #+#    #+#             */
/*   Updated: 2025/05/26 15:24:26 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_write_state_debug(t_ps state, t_philo *philo, long elapsed_time)
{
	int	meal_ct;

	meal_ct = ft_get_long(&philo->mutex, &philo->meal_ct);
	if (state == TAKING_FIRST_FORK)
		printf("%6ld | Philo No.%d (ID %d) has taken the 1st fork (ID %d)\n",
			elapsed_time, philo->id + 1, philo->id, philo->first_fork->id);
	else if (state == TAKING_SECOND_FORK)
		printf("%6ld | Philo No.%d (ID %d) has taken the 2nd fork (ID %d)\n",
			elapsed_time, philo->id + 1, philo->id, philo->second_fork->id);
	else if (state == EATING)
		printf("%6ld | Philo No.%d (ID %d) is eating meal No.%d\n",
			elapsed_time, philo->id + 1, philo->id, meal_ct + 1);
	else if (state == SLEEPING)
		printf("%6ld | Philo No.%d (ID %d) is sleeping\n", elapsed_time,
			philo->id + 1, philo->id);
	else if (state == THINKING)
		printf("%6ld | Philo No.%d (ID %d) is thinking\n", elapsed_time,
			philo->id + 1, philo->id);
	else if (state == DIED)
		printf("%6ld | Philo No.%d (ID %d) died\n", elapsed_time, philo->id + 1,
			philo->id);
}

void	ft_write_state(t_ps state, t_philo *philo)
{
	long	elapsed_time;
	long	current_time;

	pthread_mutex_lock(&philo->data->write_mutex);
	current_time = ft_get_time(MILLISECOND, philo->data);
	if (current_time >= 0 && (!ft_sim_is_over(philo->data) || state == DIED))
	{
		elapsed_time = current_time - philo->data->start_time;
		if (DEBUG_MODE)
			ft_write_state_debug(state, philo, elapsed_time);
		else if (state == TAKING_FIRST_FORK || state == TAKING_SECOND_FORK)
			printf("%ld %d has taken a fork\n", elapsed_time, philo->id + 1);
		else if (state == EATING)
			printf("%ld %d is eating\n", elapsed_time, philo->id + 1);
		else if (state == SLEEPING)
			printf("%ld %d is sleeping\n", elapsed_time, philo->id + 1);
		else if (state == THINKING)
			printf("%ld %d is thinking\n", elapsed_time, philo->id + 1);
		else if (state == DIED)
			printf("%ld %d died\n", elapsed_time, philo->id + 1);
	}
	pthread_mutex_unlock(&philo->data->write_mutex);
}

static int	ft_fed_or_dead(t_data *d, t_philo *philo, long time_since_last_meal)
{
	bool	all_full;
	int		i;

	if (time_since_last_meal >= d->time_to_die / 1e3
		&& !ft_get_bool(&philo->mutex, &philo->is_full))
	{
		ft_set_bool(&d->state_mutex, &d->end_sim, true);
		ft_write_state(DIED, philo);
		return (1);
	}
	if (d->max_meals > 0)
	{
		all_full = true;
		i = -1;
		while (++i < d->philo_nbr)
			if (!ft_get_bool(&d->philo[i].mutex, &d->philo[i].is_full))
				all_full = false;
		if (all_full)
		{
			ft_set_bool(&d->state_mutex, &d->end_sim, true);
			return (1);
		}
	}
	return (0);
}

void	*ft_monitor(void *data)
{
	t_data	*d;
	long	current_time;
	long	last_meal_time;
	int		i;

	d = (t_data *)data;
	while (!ft_get_bool(&d->state_mutex, &d->all_threads_ready))
		usleep(500);
	while (!ft_sim_is_over(d))
	{
		i = -1;
		while (++i < d->philo_nbr)
		{
			current_time = ft_get_time(MILLISECOND, d);
			if (ft_sim_is_over(d))
				return (NULL);
			last_meal_time = ft_get_long(&d->philo[i].mutex,
					&d->philo[i].last_meal_time);
			if (ft_fed_or_dead(d, &d->philo[i], current_time - last_meal_time))
				return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
