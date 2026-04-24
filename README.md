*This project has been created as part of the 42 curriculum by gdosch.*

# Philosophers 🍝

> "I never thought philosophy would be so deadly."

## Description

This project is an algorithmic exercise that introduces the basics of multithreading and multiprocessing. The goal is to solve the classic **Dining Philosophers problem** using threads and mutexes, and to learn how to manage concurrent execution safely.

### Overview
In this simulation, a number of philosophers sit at a round table with a large bowl of spaghetti in the middle. The philosophers take turns to **eat, think, or sleep**.
While they are eating, they are not thinking nor sleeping; while thinking, they are not eating nor sleeping; and, of course, while sleeping, they are not eating nor thinking.

There are also forks on the table. There are as many forks as philosophers. Every philosopher needs to eat and should never starve. Philosophers don't speak with each other and don't know if another philosopher is about to die.
Because eating spaghetti with only one fork is impractical, a philosopher must take both their right and left forks to eat. When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.

To prevent philosophers from duplicating forks and to avoid data races, the state of the forks is protected using mutexes.

## Instructions

1. **Clone the repository:**
   ```bash
   git clone <repository_url>
   cd philosophers/philo
   ```

2. **Compilation:**
   Run `make` to compile the source code into the `philo` executable.
   ```bash
   make
   ```
   Other available Makefile rules: `clean`, `fclean`, `re`.

3. **Execution:**
   ```bash
   ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
   ```
   * `number_of_philosophers`: The number of philosophers and forks.
   * `time_to_die` (in milliseconds): If a philosopher didn't start eating `time_to_die` milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
   * `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat. During that time, they will hold two forks.
   * `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
   * `number_of_times_each_philosopher_must_eat` (optional argument): If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

   **Examples:**
   ```bash
   # 5 philosophers, they die if they don't eat within 800ms, it takes 200ms to eat and 200ms to sleep.
   ./philo 5 800 200 200

   # Simulator stops if all 5 philosophers eat at least 7 times.
   ./philo 5 800 200 200 7
   ```

## Resources

* [POSIX Threads Programming (pthreads)](https://computing.llnl.gov/tutorials/pthreads/)
* [Dining Philosophers Problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Mutexes and threads explained](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)

### AI Usage
AI tools were used for generating ideas for documentation layout and finding initial references on thread synchronization. Specifically:
- Formatting the README and structuring the documentation.
- Providing brief explanations of standard functions such as `gettimeofday` and `pthread_mutex_init`.
