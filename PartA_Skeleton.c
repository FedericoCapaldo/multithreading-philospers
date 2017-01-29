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
	printf("Eating meal %ld\n", philosopher->phil_num);
	pthread_exit(NULL);
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

	/*
		1. create threads
		2. while meals eaten are less than N*3
		3. loop for some condition (number of threads)
		4. acquire mutex
		5. if acuired pthread_create(eatmeal)

		10. Terminate threads (pthread_exit(NULL))
	*/

	for(long i=0; i<num_threads; i++ ) {
		printf("In main: creating thread %ld\n", i);
		// need to pass philosopher struct as a paramenter
		int err = pthread_create(&threads[i], NULL, eat_meal, (void *)&philosophers[i]);
		if(err) {
			printf("ERROR; return code from pthread_create() is %d\n", err);
			exit(-1);
		}
	}

	pthread_exit(NULL);
	return 0;
}