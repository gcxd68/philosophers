/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 18:23:11 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:49:17 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*ft_sem_name(int id)
{
	static char	name[16] = "/philo_lock_000";

	name[12] = '0' + id / 100;
	name[13] = '0' + (id / 10) % 10;
	name[14] = '0' + id % 10;
	return (name);
}

void	ft_remove_sem(sem_t *sem, const char *sem_name)
{
	if (sem && sem != SEM_FAILED)
		sem_close(sem);
	sem_unlink(sem_name);
}

void	ft_remove_all_sems(int n, t_data *data)
{
	int	i;

	i = -1;
	while (++i < n)
		ft_remove_sem(data->philo[i].lock_sem, ft_sem_name(i));
	ft_remove_sem(data->forks_sem, "/philo_forks");
	ft_remove_sem(data->diners_sem, "/philo_diners");
	ft_remove_sem(data->write_sem, "/philo_write");
	ft_remove_sem(data->done_sem, "/philo_done");
	ft_remove_sem(data->stop_sem, "/philo_stop");
}

void	ft_sem_set(sem_t *sem, long *dest, long value)
{
	sem_wait(sem);
	*dest = value;
	sem_post(sem);
}

long	ft_sem_get(sem_t *sem, long *value)
{
	long	ret;

	sem_wait(sem);
	ret = *value;
	sem_post(sem);
	return (ret);
}
