/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 18:23:11 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/14 21:00:22 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	ft_sem_remove(sem_t *sem, const char *sem_name)
{
	if (!sem || sem == SEM_FAILED)
		return ;
	sem_close(sem);
	sem_unlink(sem_name);
}
