#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_THREADS 4

#define ARRAY_SIZE 1000 //Numeber of lines
#define STRING_SIZE 2001 //size of lines

pthread_mutex_t mutexsum; 			//mutex for line_avg

char line_array[ARRAY_SIZE][STRING_SIZE];
float line_avg[ARRAY_SIZE];			// count of individual characters

double myclock() {
   static time_t t_start = 0;  // Save and subtract off each time

   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   if( t_start == 0 ) t_start = ts.tv_sec;

   return (double) (ts.tv_sec - t_start) + ts.tv_nsec * 1.0e-9;
}

void init_arrays()
{
  int i, j, err;
  FILE *fd;

   fd = fopen( "/homes/dan/625/wiki_dump.txt", "r" );
   for ( i = 0; i < ARRAY_SIZE; i++ )  {
      err = fscanf( fd, "%[^\n]\n", line_array[i]);
      if( err == EOF ) break;
   }

  for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	line_avg[i] = 0.0;
  }
}

float find_avg(char* line, int nchars) {
   int i, j;
   float sum = 0;

   for ( i = 0; i < nchars; i++ ) {
      sum += ((int) line[i]);
   }

   if (nchars > 0)
        return sum / (float) nchars;
   else
        return 0.0;
}

void *count_array(void *myID)
{
	char theChar;
	int i, j, charLoc;
	float local_line_avg[ARRAY_SIZE];
	int startPos, endPos;
	startPos = ((int) myID) * (ARRAY_SIZE / NUM_THREADS);
	endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

	printf("myID = %d startPos = %d endPos = %d \n", (int)myID, startPos, endPos);

					// init local count array
	for ( i = 0; i < ARRAY_SIZE; i++ ) {
		local_line_avg[i] = 0.0;
	}
					// count up our section of the global array
	for ( i = startPos; i < endPos; i++) {
		local_line_avg[i]=find_avg(line_array[i], strlen(line_array[i]));
	}

					// sum up the partial counts into the global arrays
	pthread_mutex_lock(&mutexsum);
	for ( i = 0; i < ARRAY_SIZE; i++ ) {
 		line_avg[i] += local_line_avg[i];
	}
	pthread_mutex_unlock(&mutexsum);

	pthread_exit(NULL);
}

void print_results(float the_line_avg[])
{
  int i,j, total = 0;

  					// then print out the totals
  for ( i = 0; i < ARRAY_SIZE; i++ ) {
	printf("%d: %.1f\n", i, the_line_avg[i]);
  }
}

main() {
	int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	double tstart, ttotal;
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	init_arrays();

	tstart = myclock();  // Set the zero time
	tstart = myclock();  // Start the clock

	for ( i = 0; i < NUM_THREADS; i++ )
	{
		rc = pthread_create(&threads[i], &attr, count_array, (void *)i);
		if (rc) {
			printf("Error: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	pthread_attr_destroy(&attr);
	for ( i = 0; i < NUM_THREADS; i++ ) {
		rc = pthread_join(threads[i], &status);
		if (rc) {
			printf("Error: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	print_results(line_avg);

	ttotal = myclock() - tstart;
	printf("TIME, %lf\n", ttotal);

	pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
}
