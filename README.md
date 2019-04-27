# Fractional Gaussian noise generator
The code in this repository is an implementation of the approximate circulant algorithm used to generate fractional Gaussian noise. This algorithm was proposed in a paper by [Dieker and Mendes (2003)](https://doi.org/10.1017/S0269964803173081).

This repository is a fork of [Aleksejus Kononovicius](https://github.com/akononovicius/fgn-generator-gsl) implementation. The code has been modified to speed up the generation, especially consectutive calls. It's using [FFTW3](http://www.fftw.org/) insted of the much slower GSL implementation of the FFT. A function and a data structure to handle one time initialization is implemented in contrast to that the data structure where allocated at each and every call. It still depends on the GSL for random number generation. File are renamed and a new function that returns Fractional Brownian Motion (FBm) was reimplemented.

This implementation is a fork of [Aleksejus Kononovicius](https://github.com/akononovicius/fgn-generator-gsl) implemenation which heavily reuses code made available by Ton Dieker at [http://www.columbia.edu/~ad3217/fbm.html](http://www.columbia.edu/~ad3217/fbm.html). The main difference from the previous implementation is that this implementation relies on [GNU Scientific Library](https://www.gnu.org/software/gsl/) instead of [Ranlib and Meschach](http://www.netlib.org/). The possibility to generate fractional Brownian motion was also removed as it is redundant. Namely this functionality can be implemented outside this generation algorithm.
