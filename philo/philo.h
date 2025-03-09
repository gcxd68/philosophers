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

// write function debug
# define DEBUG_MODE 0

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
	t_fork		*left_fork;
	t_fork		*right_fork;
	long		last_meal_time;
	int			meal_ct;
	bool		is_full;
	t_mutex		mutex;
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
	bool	all_threads_ready;
	t_mutex	mutex;// renommer en rd_mutex ?
	t_mutex wr_mutex;// renommer ?? wr_mutex ?
	t_fork	*fork;
	t_philo	*philo;
};

typedef enum e_philo_status
{
	TAKING_LEFT_FORK,
	TAKING_RIGHT_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_philo_status;

typedef enum e_time_code // GICLER SECOND
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}	t_time_code;

void	ft_sim(t_data *data);
void	ft_set_bool(t_mutex *mutex, bool *dest, bool value);
bool	ft_get_bool(t_mutex *mutex, bool *value);
void	ft_set_long(t_mutex *mutex, long *dest, long value);
long	ft_get_long(t_mutex *mutex, long *value);
bool	ft_sim_is_over(t_data *data);
void	ft_error_exit(const char *error);
void	ft_usleep(long sleep_time, t_data *data);
void	ft_write_status(t_philo_status status, t_philo *philo, bool debug);
long	ft_get_time(t_time_code time_code);

#endif
