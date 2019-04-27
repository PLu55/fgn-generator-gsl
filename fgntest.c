#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fgn.h"
#include "fgn_private.h"

#define DUMP

int main(int argc, char** args)
{
  int n=10;
  int N=pow(2,n);
  fgn_plan* plan;
  long seed = 123467;
  double H = 0.8;
  int i = 0;
  
  while( ++i < argc ){
    if (strcmp(args[i], "-n") == 0 ){
      n=atoi(args[++i]);
      printf("-n %d\n", n);
    } else if (strcmp(args[i], "-H") == 0 ){
      H = atof(args[++i]);
      printf("-H %5.3f\n", H);
    } else if (strcmp(args[i], "-s") == 0 ){
      seed = atoi(args[++i]);
      printf("-s %ld\n", seed);
    }
  }
  
  fgn_make_plan( N, seed, H, &plan);
  if(plan->n != N){
    printf("Error: n is not N\n");
  }
 
  if(plan->H != H){
    printf("Error: H is not H\n");
  }
  
  /*
  gsl_rng* rng;
  fftw_plan plan;
  double* pspec;
  double* output;
  fftw_complex* data;
  */

  double *res = (double*) malloc(N*sizeof(double));

  fgn(plan, H, res);
#ifdef DUMP
  FILE * fp;
  fp = fopen ("pspec.txt", "w+");
  for (int i=0; i<N+1; i++)
    fprintf(fp, "%f\n", plan->pspec[i]);
  fclose(fp);
  fp = fopen ("out.txt", "w+");
  for (int i=0; i<N; i++)
    fprintf(fp, "%f\n", res[i]);
  fclose(fp);
#endif
  free(res);
  fgn_free(plan);
}
