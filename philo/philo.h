/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:54 by gdosch            #+#    #+#             */
/*   Updated: 2025/03/06 13:34:56 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <pthread.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <strings.h>
# include <time.h>
# include <unistd.h>
# include <bsd/string.h>
# include <sys/wait.h>
# include <sys/time.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_data	t_data;

typedef struct s_fork
{
	int			id;
	t_mutex		mutex;
}	t_fork;

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	t_fork		left_fork;
	t_fork		right_fork;
	long		last_meal_time;
	int			meal_ct;
	bool		is_full;
	t_data		*data;
}	t_philo;

struct s_data
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	max_meals;
	long	start_time;
	bool	end_sim;
	t_fork	fork;
	t_philo	philo;
};

#endif
