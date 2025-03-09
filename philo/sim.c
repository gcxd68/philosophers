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

static void ft_wait_all_threads(t_data *data)
{
	while (!ft_get_bool(&data->mutex, &data->all_threads_ready))
		;//SPINLOCK, preferer usleep(100) ??
}

static void	ft_think(t_philo *philo)
{
	ft_write_status(THINKING, philo, DEBUG_MODE);
}

static void ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	ft_write_status(TAKING_LEFT_FORK, philo, DEBUG_MODE);
	pthread_mutex_lock(&philo->right_fork->mutex);
	ft_write_status(TAKING_RIGHT_FORK, philo, DEBUG_MODE);

	//EAT
	ft_set_long(&philo->mutex, &philo->last_meal_time, ft_get_time(MILLISECOND));
	philo->meal_ct++;
	ft_write_status(EATING, philo, DEBUG_MODE);
	ft_usleep(philo->data->time_to_eat, philo->data);
	if (philo->data->max_meals > 0
		&& philo->meal_ct == philo->data->max_meals)
		ft_set_bool(&philo->mutex, &philo->is_full, true);

	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	*ft_dinner(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;

	//SPINLOCK
	ft_wait_all_threads(philo->data);

	//SET LAST MEAL TIME
	while(!ft_sim_is_over(philo->data))
	{
		if (philo->is_full) // TODO THREAD SAFE ?
			break ;
		ft_eat(philo);
		
		//sleep -> write status + precise usleep
		ft_write_status(SLEEPING, philo, DEBUG_MODE);
		ft_usleep(philo->data->time_to_sleep, philo->data);

		ft_think(philo);//TODO
	}



	return (NULL);
}

void	ft_sim(t_data *data)
{
	int	i;


	if (!data->max_meals)
		return ;
/*	else if (data->philo_nbr == 1)
		//TODO
*/	else
	{
		i = -1;
		while (++i < data->philo_nbr)
		{
			pthread_create(&data->philo[i].thread, NULL,
				ft_dinner, &data->philo[i]);
		}
	}
	
	//SIM START
	data->start_time = ft_get_time(MILLISECOND);


	//ALL_THREADS_READY
	ft_set_bool(&data->mutex, &data->all_threads_ready, true);


	//WAIT FOR EVERYONE
	i = -1;
	while(++i < data->philo_nbr)
	{
		pthread_join(data->philo[i].thread, NULL);
	}

	//if we manage to reach this line, all philos are full


}
