/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:25:17 by gdosch            #+#    #+#             */
/*   Updated: 2025/05/26 15:22:36 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_usleep(long sleep_time, t_data *data)
{
	const long	start_time = ft_get_time(MICROSECOND, data);
	const long	end_time = start_time + sleep_time;
	long		current_time;
	long		remaining_time;

	while (1)
	{
		current_time = ft_get_time(MICROSECOND, data);
		remaining_time = end_time - current_time;
		if (ft_sim_is_over(data) || remaining_time <= 0)
			break ;
		else if (remaining_time > 350)
			usleep(remaining_time - 350);
	}
}

static void	ft_eat(t_philo *philo)
{
	long	last_meal_time;

	pthread_mutex_lock(&philo->first_fork->mutex);
	ft_write_state(TAKING_FIRST_FORK, philo);
	if (philo->data->philo_nbr == 1)
		while (!ft_sim_is_over(philo->data))
			usleep(100);
	else
	{
		ft_write_state(TAKING_SECOND_FORK, philo);
		pthread_mutex_lock(&philo->second_fork->mutex);
		ft_write_state(EATING, philo);
		pthread_mutex_lock(&philo->mutex);
		last_meal_time = ft_get_time(MILLISECOND, philo->data);
		if (last_meal_time >= 0)
			philo->last_meal_time = last_meal_time;
		if (!ft_sim_is_over(philo->data) && ++philo->meal_ct
			== philo->data->max_meals && philo->data->max_meals > 0)
			philo->is_full = true;
		pthread_mutex_unlock(&philo->mutex);
		if (!ft_sim_is_over(philo->data))
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
		if (ft_get_bool(&philo->mutex, &philo->is_full) || ft_sim_is_over(d))
			break ;
		ft_write_state(SLEEPING, philo);
		ft_usleep(d->time_to_sleep, d);
		if (ft_sim_is_over(d))
			break ;
		ft_write_state(THINKING, philo);
		if (d->think_time > 0)
			ft_usleep(d->think_time, d);
	}
	return (NULL);
}

static int	ft_start_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr && !ft_sim_is_over(data))
	{
		if (pthread_create(&data->philo[i].thread, NULL, ft_dinner,
				&data->philo[i]))
		{
			ft_set_bool(&data->state_mutex, &data->end_sim, true);
			perror("philo: pthread_create failed");
			return (1);
		}
	}
	if (pthread_create(&data->monitor, NULL, ft_monitor, data))
	{
		ft_set_bool(&data->state_mutex, &data->end_sim, true);
		perror("philo: pthread_create failed");
		return (1);
	}
	return (0);
}

int	ft_sim(t_data *data)
{
	int		i;
	int		status;

	status = ft_start_threads(data);
	if (!status)
		data->start_time = ft_get_time(MILLISECOND, data);
	if (data->start_time < 0)
		status = 1;
	i = -1;
	while (++i < data->philo_nbr && !status)
		ft_set_long(&data->philo[i].mutex, &data->philo[i].last_meal_time,
			data->start_time);
	ft_set_bool(&data->state_mutex, &data->all_threads_ready, true);
	if (data->monitor)
		pthread_join(data->monitor, NULL);
	i = -1;
	while (++i < data->philo_nbr)
		if (data->philo[i].thread)
			pthread_join(data->philo[i].thread, NULL);
	return (status);
}
