#include <complex.h>
#include <fftw3.h>
#include <gsl/gsl_rng.h>

struct fgn_plan_S {
  int n;
  double H;
  double mem;
  gsl_rng* rng;
  fftw_plan plan;
  double* pspec;
  double* output;
  fftw_complex* data;
};
