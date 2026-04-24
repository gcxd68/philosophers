# Philosophers 🍝

> "I never thought philosophy would be so deadly."

This project is a 42 school algorithmic project that introduces the basics of multithreading and multiprocessing. We are asked to solve the classic **Dining Philosophers problem** using threads and mutexes.

## Overview

In this simulation, a number of philosophers sit at a round table with a large bowl of spaghetti in the middle. The philosophers alternatively **eat, think, or sleep**.
While they are eating, they are not thinking nor sleeping; while thinking, they are not eating nor sleeping; and, of course, while sleeping, they are not eating nor thinking.

There are also forks on the table. There are as many forks as philosophers.
Because serving and eating spaghetti with only one fork is very inconvenient, a philosopher takes their right and their left forks to eat, one in each hand.

When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.

Every philosopher needs to eat and should never starve. Philosophers don't speak with each other and don't know if another philosopher is about to die.

## Rules

* **Global variables are forbidden.**
* The program should take the following arguments:
  * `number_of_philosophers`: The number of philosophers and forks.
  * `time_to_die` (in milliseconds): If a philosopher didn't start eating `time_to_die` milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
  * `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
  * `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
  * `number_of_times_each_philosopher_must_eat` (optional argument): If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

## The Mandatory Part (`philo`)

The mandatory part must be implemented using **Threads and Mutexes**.
* Each philosopher should be a thread.
* There is one fork between each pair of philosophers. Therefore, if there are several philosophers, each philosopher has a fork on their left side and a fork on their right side. If there is only one philosopher, there should be only one fork on the table.
* To prevent philosophers from duplicating forks, you should protect the forks state with a mutex for each of them.

## Installation & Usage

1. **Clone the repository:**
   ```bash
   git clone <repository_url>
   cd philosophers/philo
   ```

2. **Compile the program:**
   ```bash
   make
   ```
   This will generate the `philo` executable.

3. **Run the simulation:**
   ```bash
   ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
   ```

   **Examples:**
   ```bash
   # 5 philosophers, they die if they don't eat within 800ms, it takes 200ms to eat and 200ms to sleep.
   ./philo 5 800 200 200

   # Simulator stops if all 5 philosophers eat at least 7 times.
   ./philo 5 800 200 200 7
   ```

## Logs

The program will output the state of each philosopher in the following format:
* `timestamp_in_ms X has taken a fork`
* `timestamp_in_ms X is eating`
* `timestamp_in_ms X is sleeping`
* `timestamp_in_ms X is thinking`
* `timestamp_in_ms X died`

*(Where X is the philosopher number from 1 to `number_of_philosophers`)*

Messages are synchronized, and the death of a philosopher will be displayed no more than 10 ms after their actual death.

## Key Concepts Learned
* Mutexes (`pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_destroy`)
* Threads (`pthread_create`, `pthread_join`, `pthread_detach`)
* Process synchronization and avoiding Data Races
* Prevention of Deadlocks
* Time management in C (`gettimeofday`, `usleep`)
