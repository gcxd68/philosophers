/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:25:17 by gdosch            #+#    #+#             */
/*   Updated: 2025/05/20 19:15:20 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_get_time(t_tc time_code, t_data *data)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		ft_error_exit("philo: gettimeofday failed", 1, PERROR, data);
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (0);
}

static void	ft_usleep(long sleep_time, t_data *data)
{
	const long	start_time = ft_get_time(MICROSECOND, data);
	const long	end_time = start_time + sleep_time;
	long		current_time;

	while (!ft_sim_is_over(data))
	{
		current_time = ft_get_time(MICROSECOND, data);
		if (current_time >= end_time)
			break ;
		if (end_time - current_time > 1000)
			usleep((end_time - current_time) * 3 / 4);
	}
}

static void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->mutex);
	ft_write_state(TAKING_FIRST_FORK, philo);
	if (philo->data->philo_nbr == 1)
		while (!ft_sim_is_over(philo->data))
			usleep(100);
	else
	{
		pthread_mutex_lock(&philo->second_fork->mutex);
		ft_write_state(TAKING_SECOND_FORK, philo);
		ft_write_state(EATING, philo);
		pthread_mutex_lock(&philo->mutex);
		philo->last_meal_time = ft_get_time(MILLISECOND, philo->data);
		philo->meal_ct++;
		if (philo->data->max_meals > 0
			&& philo->meal_ct == philo->data->max_meals)
			philo->is_full = true;
		pthread_mutex_unlock(&philo->mutex);
		ft_usleep(philo->data->time_to_eat, philo->data);
		pthread_mutex_unlock(&philo->second_fork->mutex);
	}
	pthread_mutex_unlock(&philo->first_fork->mutex);
}

static void	*ft_dinner(void *data)
{
	t_philo	*philo;
	t_data	*d;

	philo = (t_philo *)data;
	d = philo->data;
	while (!ft_get_bool(&d->state_mutex, &d->all_threads_ready))
		usleep(100);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!ft_sim_is_over(d))
	{
		ft_eat(philo);
		if (ft_get_bool(&philo->mutex, &philo->is_full))
			break ;
		ft_write_state(SLEEPING, philo);
		ft_usleep(d->time_to_sleep, d);
		ft_write_state(THINKING, philo);
	}
	return (NULL);
}

void	ft_sim(t_data *data)
{
	pthread_t	monitor;
	int			i;

	if (!data->max_meals)
		return ;
	else
	{
		data->start_time = ft_get_time(MILLISECOND, data);
		i = -1;
		while (++i < data->philo_nbr)
			ft_set_long(&data->philo[i].mutex, &data->philo[i].last_meal_time,
				data->start_time);
		i = -1;
		while (++i < data->philo_nbr)
			pthread_create(&data->philo[i].thread, NULL, ft_dinner,
				&data->philo[i]);
		pthread_create(&monitor, NULL, ft_monitor, data);
		ft_set_bool(&data->state_mutex, &data->all_threads_ready, true);
		pthread_join(monitor, NULL);
		i = -1;
		while (++i < data->philo_nbr)
			pthread_join(data->philo[i].thread, NULL);
	}
}
