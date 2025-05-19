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

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

# define DEBUG_MODE 1

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
	t_fork		*first_fork;
	t_fork		*second_fork;
	long		last_meal_time;
	long		meal_ct;
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
	t_mutex	state_mutex;
	t_mutex	write_mutex;
	t_fork	*fork;
	t_philo	*philo;
};

typedef enum e_philo_state
{
	TAKING_FIRST_FORK,
	TAKING_SECOND_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_ps;

typedef enum e_time_code
{
	MILLISECOND,
	MICROSECOND
}	t_tc;

typedef enum e_output_mode
{
	WRITE,
	PERROR
}	t_om;

void	ft_cleanup_data(t_data *data);
void	ft_error_exit(const char *err_msg, t_om output_mode);

void	ft_sim(t_data *data);
long	ft_get_time(t_tc time_code, t_data *data);
void	*ft_monitor(void *data);
void	ft_write_state(t_ps state, t_philo *philo, bool debug);

void	ft_set_bool(t_mutex *mutex, bool *dest, bool value);
bool	ft_get_bool(t_mutex *mutex, bool *value);
void	ft_set_long(t_mutex *mutex, long *dest, long value);
long	ft_get_long(t_mutex *mutex, long *value);
bool	ft_sim_is_over(t_data *data);

#endif
