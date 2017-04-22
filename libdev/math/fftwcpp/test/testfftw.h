#ifndef TESTFFT_H
#define TESTFFT_H

#include "FFTW.h"
#include "fftfunctions.h"
#include <iostream>

using namespace std;

// promote to long double
template < typename T>
complex<long double>  promote(complex<T> x) { return complex<long double>(x.real(),x.imag()); }

complex<long double>  promote(float x) { return complex<long double>( x); }
complex<long double>  promote(double x) { return complex<long double>( x); }
complex<long double>  promote(long double x) { return complex<long double>( x); }

// root mean square error of manual DFT
long double fft_rmse( const complex<double> * input, const complex<double> * output, size_t size)
{
    long double totalpower=0;
    long double difpower=0;
    long double pi = acos((long double)-1 );
    for (size_t i=0;i<size;++i) {
        // manual (slow) dft
        complex<long double> dft = 0;
        long double phinc = -2.0 * pi * i / size;
        for (size_t j=0; j<size; ++j) {
            dft +=  promote( input[j] ) * exp( complex<long double>(0,j*phinc) );
        }
        // intensity in dft
        totalpower += norm(dft);
        // compare with FFT, thus ouput[i]
        complex<long double> x = promote(output[i]);
        complex<long double> dif = dft - x;
        // intensity of error (differences)
        difpower += norm(dif);
        //cerr << i << "\t" << dft << "\t" <<  x << "\t" << sqrt(norm(dif)) << endl;
    }
    cerr << "dft: rmse:" << sqrt(difpower/totalpower) << endl;
    return sqrt(difpower/totalpower);
}

// difference between forth and back calculated fft
long double dif_rmse( const complex<double> * input, const complex<double> * output, size_t nx, size_t ny = 1)
{
    long double totalpower=0;
    long double difpower=0;
    size_t n = nx*ny;

    for (size_t k=0; k<n; ++k) {
        totalpower += (norm( input[k] ) + norm(output[k]) )/2.0;
        difpower += norm(input[k] - output[k]);
        cerr << k << " diff:" << norm(input[k] - output[k]) << " in:" << norm( input[k] ) << " out:" << norm( output[k] ) << endl;
    }
    cerr << "diff: rmse:" << sqrt(difpower/totalpower) << endl;
    return sqrt(difpower/totalpower);
}


void test_complex_fft(size_t nx, size_t ny = 1)
{
    cout << "--- testing size: " << nx << " x " << ny << endl;

    FFTW cfft;
    complex<double>* input = FFTW::createArray(nx*ny);
    complex<double>* output = FFTW::createArray(nx*ny);
    complex<double>* outputinv = FFTW::createArray(nx*ny);

    for (size_t i=0; i<nx*ny; ++i) {
        // value between -0.5 and 0.5
        double real = (rand()/(double)RAND_MAX - 0.5);
        double imag = (rand()/(double)RAND_MAX - 0.5);
        input[i]= complex<double>(real, imag);
    }
    fftutils::printArray(input, nx, ny);

    cfft.setAutoScale(true);
    if (ny == 1) {
        cfft.setSize(nx);
    }
    else {
        cfft.setSize(nx, ny);
    }
    cfft.setThreads(2);
    cfft.setFastestFFT(true);
    cfft.setShiftBefore(false);
    cfft.setShiftAfter(false);

    // cfft
    cfft.forward(input, output);
    // check with manual dft
    //cfft_rmse(input, output, nx);

    fftutils::printArray(input, nx, ny);
    return;

    // cfft
    cfft.invers(output, outputinv);
    // check equal with input
    dif_rmse(input,outputinv, nx);
    cout << "speed: " << cfft.speed() << " ms" << endl;

    FFTW::deleteArray(input);
    FFTW::deleteArray(output);
    FFTW::deleteArray(outputinv);
}

void perfomFFTTests()
{
    cout << "======== testing complex 1D ============" << endl;
    test_complex_fft(32);
//    test_complex_fft(256);
//    test_complex_fft(500);
//    test_complex_fft(4096);
//    test_complex_fft(pow(2,16));
//    test_complex_fft(pow(2,16)+6);
//    cout << "======== testing complex 2D ============" << endl;
//    test_complex_fft(32, 32);
//    test_complex_fft(256, 256);
//    test_complex_fft(500, 500);
//    test_complex_fft(4096, 4096);
}

#endif
