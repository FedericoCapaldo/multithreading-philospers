#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/*Global variables */
int num_threads;
pthread_mutex_t *mutexes;

/* For representing the status of each philosopher */
typedef enum{
	none,   // No forks
	one,    // One fork
	two     // Both forks to consume
} utensil;

/* Representation of a philosopher */
typedef struct phil_data{
	int phil_num;
	int course;
	utensil forks;
}phil_data;

/* ****************Change function below ***************** */
void *eat_meal(void *phil){
/* 3 course meal: Each need to acquire both forks 3 times.
 *  First try for fork in front.
 * Then for the one on the right, if not fetched, put the first one back.
 * If both acquired, eat one course.
 */
  phil_data *philosopher = (phil_data *)phil;
	int num = philosopher->phil_num;
	while (philosopher->course < 3) {
		pthread_mutex_lock(&mutexes[num]);
		philosopher -> forks = one;

		int second_fork = (num) + 1;
		if(num == num_threads-1) {
			second_fork = 0;
		}

		if(pthread_mutex_trylock(&mutexes[second_fork]) == 0) {
			philosopher -> forks = two;
			sleep(1);
			printf("Philosper %ld Eating meal %ld\n", num, philosopher->course);
			philosopher -> course += 1;
			pthread_mutex_unlock(&mutexes[num]);
			pthread_mutex_unlock(&mutexes[second_fork]);
		} else {
			pthread_mutex_unlock(&mutexes[num]);
		}
		philosopher -> forks = none;
	}

	pthread_exit(NULL); // this referes to the current thread
}

/* ****************Add the support for pthreads in function below ***************** */
int main( int argc, char **argv ){
	int num_philosophers, error;

	if (argc < 2) {
          fprintf(stderr, "Format: %s <Number of philosophers>\n", argv[0]);
          return 0;
     }

    num_philosophers = num_threads = atoi(argv[1]);
	pthread_t threads[num_threads];
	phil_data philosophers[num_philosophers]; //Struct for each philosopher
	mutexes = malloc(sizeof(pthread_mutex_t)*num_philosophers); //Each mutex element represent a fork

	/* Initialize structs */
	for( int i = 0; i < num_philosophers; i++ ){
		philosophers[i].phil_num = i;
		philosophers[i].course   = 0;
		philosophers[i].forks    = none;
	}
/* Each thread will represent a philosopher */

/* Initialize Mutex, Create threads, Join threads and Destroy mutex */

	for( int i=0; i<num_philosophers; i++) {
		pthread_mutex_init(&mutexes[i], NULL);
	}

	for(long i=0; i<num_threads; i++ ) {
		printf("In main: creating thread %ld\n", i);
		// need to pass philosopher struct as a paramenter
		int err = pthread_create(&threads[i], NULL, eat_meal, (void *)&philosophers[i]);
		if(err) {
			printf("ERROR; return code from pthread_create() is %d\n", err);
			exit(-1);
		}
	}

	for (int i=0; i<num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	for(int i=0; i<num_philosophers; i++) {
		pthread_mutex_destroy(&mutexes[i]);
	}
	pthread_exit(NULL);
	return 0;
}
