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

void	ft_cleanup_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		pthread_mutex_destroy(&data->philo[i].mutex);
		pthread_mutex_destroy(&data->fork[i].mutex);
	}
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	free(data->fork);
	free(data->philo);
}

void	ft_error_exit(const char *err_msg, t_om output_mode)
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
			ft_error_exit("philo: write failed", PERROR);
	}
	exit(EXIT_FAILURE);
}

static void	ft_write_state_debug(t_ps state, t_philo *philo, long elapsed_time)
{
	int	meal_ct;

	meal_ct = ft_get_long(&philo->mutex, &philo->meal_ct);
	if (state == TAKING_FIRST_FORK && !ft_sim_is_over(philo->data))
		printf("%6ld | Philo No.%d (ID %d) has taken the 1st fork (ID %d)\n",
			elapsed_time, philo->id + 1, philo->id, philo->first_fork->id);
	else if (state == TAKING_SECOND_FORK
		&& !ft_sim_is_over(philo->data))
		printf("%6ld | Philo No.%d (ID %d) has taken the 2nd fork (ID %d)\n",
			elapsed_time, philo->id + 1, philo->id, philo->second_fork->id);
	else if (state == EATING && !ft_sim_is_over(philo->data))
		printf("%6ld | Philo No.%d (ID %d) is eating meal No.%d\n",
			elapsed_time, philo->id + 1, philo->id, meal_ct + 1);
	else if (state == SLEEPING && !ft_sim_is_over(philo->data))
		printf("%6ld | Philo No.%d (ID %d) is sleeping\n", elapsed_time,
			philo->id + 1, philo->id);
	else if (state == THINKING && !ft_sim_is_over(philo->data))
		printf("%6ld | Philo No.%d (ID %d) is thinking\n", elapsed_time,
			philo->id + 1, philo->id);
	else if (state == DIED)
		printf("%6ld | Philo No.%d (ID %d) died\n", elapsed_time, philo->id + 1,
			philo->id);
}

void	ft_write_state(t_ps state, t_philo *philo, bool debug)
{
	long	elapsed_time;

	pthread_mutex_lock(&philo->data->write_mutex);
	elapsed_time
		= ft_get_time(MILLISECOND, philo->data) - philo->data->start_time;
	if (debug)
		ft_write_state_debug(state, philo, elapsed_time);
	else
	{
		if ((state == TAKING_FIRST_FORK || state == TAKING_SECOND_FORK)
			&& !ft_sim_is_over(philo->data))
			printf("%ld %d has taken a fork\n", elapsed_time, philo->id + 1);
		else if (state == EATING && !ft_sim_is_over(philo->data))
			printf("%ld %d is eating\n", elapsed_time, philo->id + 1);
		else if (state == SLEEPING && !ft_sim_is_over(philo->data))
			printf("%ld %d is sleeping\n", elapsed_time, philo->id + 1);
		else if (state == THINKING && !ft_sim_is_over(philo->data))
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

	if (time_since_last_meal > d->time_to_die / 1e3
		&& !ft_get_bool(&philo->mutex, &philo->is_full))
	{
		ft_set_bool(&d->state_mutex, &d->end_sim, true);
		ft_write_state(DIED, philo, DEBUG_MODE);
		return (-1);
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
			return (-1);
		}
	}
	return (0);
}

void	*ft_monitor(void *data)
{
	t_data	*d;
	long	current_time;
	long	last_meal;
	int		i;

	d = (t_data *)data;
	while (!ft_get_bool(&d->state_mutex, &d->all_threads_ready))
		usleep(100);
	while (!ft_sim_is_over(d))
	{
		i = -1;
		while (++i < d->philo_nbr)
		{
			current_time = ft_get_time(MILLISECOND, data);
			last_meal = ft_get_long(&d->philo[i].mutex,
					&d->philo[i].last_meal_time);
			if (ft_fed_or_dead(d, &d->philo[i], current_time - last_meal) < 0)
				return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
