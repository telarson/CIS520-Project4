#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 8
#define LINE_LENGTH 2001

float find_avg(char* line, int nchars, void* id) {
   int i, j, startPos, endPos;
   float sum = 0;

   if((int) id != -1)
   {
      startPos = ((int)id) * (nchars / NUM_THREADS);
  	   endPos = startPos + (nchars / NUM_THREADS);
   }
   else
   {
      startPos = 0;
      endPos = nchars;
   }
   

   for ( i = startPos; i < endPos; i++ ) {
      sum += ((int) line[i]);
   }

   if (nchars > 0) 
	return sum / (float) nchars;
   else
	return 0.0;
}

double myclock() {
   static time_t t_start = 0;  // Save and subtract off each time

   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   if( t_start == 0 ) t_start = ts.tv_sec;

   return (double) (ts.tv_sec - t_start) + ts.tv_nsec * 1.0e-9;
}

int main()
{
   int nlines = 0, maxlines = 1000000;
   int i, err;
   float  charsum=0.0; 
   int nchars = 0; 
   double tstart, ttotal;
   FILE *fd;
   char *line = (char*) malloc( LINE_LENGTH ); // no lines larger than 2000 chars

// Read in the lines from the data file

   fd = fopen( "/homes/dan/625/wiki_dump.txt", "r" );
   
   tstart = myclock();  // Set the zero time
   tstart = myclock();  // Start the clock

   for ( i = 0; i < maxlines; i++ )  {
      err = fscanf( fd, "%[^\n]\n", line);
      if( err == EOF ) break;
      nchars = strlen( line );
      float found_avg = find_avg(line, nchars, -1);
      printf("%d: %.1f\n", nlines, found_avg);
      nlines++;
   }

   ttotal = myclock() - tstart;
   printf("TIME, %d", ttotal);

   fclose( fd );

}

