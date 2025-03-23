/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:25:17 by gdosch            #+#    #+#             */
/*   Updated: 2025/03/09 11:25:19 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_get_time(t_tc time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		ft_error_exit("philo: gettimeofday failed", PERROR);
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + tv.tv_usec / 1e3);
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		ft_error_exit("philo: wrong input to gettime", WRITE);
	return (0);
}

static void	ft_usleep(long sleep_time, t_data *data)
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

static void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	ft_write_state(TAKING_LEFT_FORK, philo, DEBUG_MODE);
	if (philo->data->philo_nbr == 1)
		while (!ft_sim_is_over(philo->data))
			usleep(100);
	else
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		ft_write_state(TAKING_RIGHT_FORK, philo, DEBUG_MODE);
		ft_set_long(&philo->mutex, &philo->last_meal_time,
			ft_get_time(MILLISECOND));
		philo->meal_ct++;
		ft_write_state(EATING, philo, DEBUG_MODE);
		ft_usleep(philo->data->time_to_eat, philo->data);
		if (philo->data->max_meals > 0
			&& philo->meal_ct == philo->data->max_meals)
			ft_set_bool(&philo->mutex, &philo->is_full, true);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

static void	*ft_dinner(void *data)
{
	t_philo	*philo;
	t_data	*d;

	philo = (t_philo *)data;
	d = philo->data;
	while (!ft_get_bool(&d->state_mutex, &d->all_threads_ready))
		usleep(100);
	ft_set_long(&philo->mutex, &philo->last_meal_time, d->start_time);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!ft_sim_is_over(d))
	{
		if (philo->is_full)
			break ;
		ft_eat(philo);
		ft_write_state(SLEEPING, philo, DEBUG_MODE);
		ft_usleep(d->time_to_sleep, d);
		ft_write_state(THINKING, philo, DEBUG_MODE);
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
		data->start_time = ft_get_time(MILLISECOND);
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
