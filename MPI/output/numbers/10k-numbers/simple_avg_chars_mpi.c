#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int NUM_THREADS;

#define ARRAY_SIZE 10000 //Numeber of lines
#define STRING_SIZE 2001 //size of lines

char line_array[ARRAY_SIZE][STRING_SIZE];
int line_avg[ARRAY_SIZE];			// count of individual characters
int local_line_avg[ARRAY_SIZE];

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
   printf("Initializing arrays.\n"); fflush(stdout);
   fd = fopen( "/homes/dan/625/wiki_dump.txt", "r" );
   for ( i = 0; i < ARRAY_SIZE; i++ )  {
      err = fscanf( fd, "%[^\n]\n", line_array[i]);
      if( err == EOF ) break;
   }

  for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	line_avg[i] = 0.0;
  }
}

int find_avg(char* line, int nchars) {
   int i, j;
   int sum = 0;
	//printf("avg1\n");
   for ( i = 0; i < nchars; i++ ) {
      sum += ((int) line[i]);
   }
   	//printf("avg2\n");
   if (nchars > 0)
        return sum / /*(float)*/ nchars;
   else
        return 0.0;
}

void *count_array(void *rank)
{
	char theChar;
	int i, j, charLoc;
	int startPos, endPos;
	int myID = *((int*)rank);
	//printf("ARRAY_SIZE = %d and %d and %d", ARRAY_SIZE, NUM_THREADS, myID);
	startPos = ((long) myID) * (ARRAY_SIZE / NUM_THREADS);
	endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

	printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);

					// init local count array
	for ( i = 0; i < ARRAY_SIZE; i++ ) {
		local_line_avg[i] = 0.0;
	}
					// count up our section of the global array
	for ( i = startPos; i < endPos; i++) {
		local_line_avg[i]=find_avg(line_array[i], strlen(line_array[i]));
	}

					// sum up the partial counts into the global arrays
	//for ( i = 0; i < ARRAY_SIZE; i++ ) {
 	//	line_avg[i] += local_line_avg[i];
	//}
	//printf("count_array = done!\n");
}

void print_results(int the_line_avg[])
{
  int i,j, total = 0;

  					// then print out the totals
  for ( i = 0; i < ARRAY_SIZE; i++ ) {
	printf("LINE, %d: %d\n", i, the_line_avg[i]);
  }
  fflush(stdout);
}

main(int argc, char* argv[]) {
	int i, rc;
	double tstart, ttotal;
	int numtasks, rank;
	MPI_Status Status;

	rc = MPI_Init(&argc,&argv);
	if(rc != MPI_SUCCESS) {
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	NUM_THREADS = numtasks;
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);
	if(rank == 0){
		init_arrays();
	}

	MPI_Bcast(line_array, ARRAY_SIZE * STRING_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	tstart = myclock();  // Set the zero time
	tstart = myclock();  // Start the clock

	count_array(&rank);

	MPI_Reduce(local_line_avg, line_avg, ARRAY_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(rank == 0){
		print_results(line_avg);
		ttotal = myclock() - tstart;
		printf("TIME, %lf\n", ttotal);
		printf("Main: program completed. Exiting.\n");
	}

	MPI_Finalize();
	return 0;
}
