/*  This program simulates fractional Gaussian noise or fractional
    Brownian motion using the approximate circulant algorithm.
    The C-packages Ranlib and Meschach are used, both available
    via Netlib (http://www.netlib.org).

    Reference:
    A.B. Dieker and M. Mandjes (2002),
    On spectral simulation of fractional Brownian motion,
    submitted for publication.
    
    Copyright Ton Dieker
    Centre of Mathematics and Computer Science (CWI) Amsterdam
    April 2002

    ton@cwi.nl
            
    Modified to be used with GSL (instead of Netlib libraries) by
    Aleksejus Kononovicius   
    https://github.com/akononovicius/fgn-generator-gsl                                    

    Modified to use FFTW3 and optimization of multiple calls by
    Peter Lundén p@apal.se
*/

#include <math.h>
#include <complex.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fftw3.h>

#include "fgn.h"
#include "fgn_private.h"

#ifdef ZIGGURAT
#define GAUSSIAN gsl_ran_gaussian_ziggurat
#else
#define GAUSSIAN gsl_ran_gaussian
#endif

void fgn_spectrum (double *pow_spec, int size, double H);

void fgn_make_plan(int n, long seed, double H, fgn_plan** plan) {
  /* fgn_make_plan setup a data structure to be used in multiple calls to
     fgn to generate fractional Gaussian noise samles.
     Input: 
        n : the sample size in each call, must be a power of two
        H : the Hurst parameter
	seed : seed for the random generator
     Output:
	plan : the allocated data structure
   */
  int N = 2 * n;
  *plan = malloc(sizeof(struct fgn_plan_S));
  (*plan)->n = n;
  (*plan)->H = H;
  (*plan)->mem = 0.0;
  gsl_rng_env_setup();
  (*plan)->rng = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set((*plan)->rng, seed);
  (*plan)->pspec = malloc(sizeof(double) * (n + 1));
  (*plan)->data = fftw_malloc(sizeof(fftw_complex) * (n + 1));
  (*plan)->output = fftw_malloc(sizeof(double) * n);
  (*plan)->plan = fftw_plan_dft_c2r_1d(n, (*plan)->data, (*plan)->output, FFTW_MEASURE);
  fgn_spectrum ((*plan)->pspec, n, H);
  (*plan)->pspec[0] = sqrt(2.*(pow(N,2.*(*plan)->H)-pow(N-1, 2.*(*plan)->H)));
  (*plan)->pspec[n] = sqrt(2.*(*plan)->pspec[n]);
  for(int i=1;i<n;i++) {
    (*plan)->pspec[i]=sqrt((*plan)->pspec[i]);
  }
}

void fgn_free(fgn_plan* plan) {
  /* fgn_free, frees all resources allocated by fgn_make_plan.
     Input:
        plan : the data structure to be freed
  */
  gsl_rng_free(plan->rng);
  free(plan->pspec); 
  fftw_free(plan->data); 
  fftw_free(plan->output); 
  fftw_destroy_plan(plan->plan);
  fftw_cleanup();
  free(plan);
}

void fgn(fgn_plan* plan, double L, double *output) {
  /* function that generates fractional  Gaussian noise sample using
     the approximate circulant method.
     Input:  
        plan : the data structure returned by fgn_make_plan
	L : scale factor
	
     Output: 
        output : the resulting sample is stored in this array, size n
    */
  
  long i, N, halfN;
  double scale;
  double aux;

  // halfN = pow(2,n);
  halfN=plan->n;
  N=2*halfN;
        
  plan->data[0] = plan->pspec[0] * GAUSSIAN(plan->rng, 1.0);
  plan->data[halfN] = plan->pspec[halfN] * GAUSSIAN(plan->rng, 1.0);
  
  for(i=1;i<halfN;i++) {
    plan->data[i] = plan->pspec[i] * (GAUSSIAN(plan->rng,1.0) + I * GAUSSIAN(plan->rng,1.0));
  }
    
  fftw_execute(plan->plan);

  /* rescale to obtain a sample of size 2^(*n) on [0,L] */
  scale=pow(L/halfN,plan->H)/sqrt(2*N);
  for(i=0;i<halfN;i++) {
    output[i]=scale*plan->output[i];
  }
}

void fbm(fgn_plan* plan, double L, double *output) {
  fgn(plan, L, output);
  output[0] = plan->mem;
  for(int i=1;i<plan->n;i++) {
    output[i]+=output[i-1];
  }
  plan->mem = output[plan->n-1];
}


