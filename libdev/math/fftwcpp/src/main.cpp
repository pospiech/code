//#include <QCoreApplication>

#include "fftwcpp.h"
#include "fftshift.h"
#include "fftallocator.h"

#include <iostream>     // std::cout
#include <cstdio>       // std::getchar

#include <cmath>
#define M_PI		3.14159265358979323846


using namespace std;
#include <vector>

using namespace std;
using namespace fftwcpp;

int main(int argc, char*argv[])
{
        // create FFT class implementation
        // each implementation has its own plan map.
        FFT<double> fft;
        // create some data
        size_t N = 1024 * 1024;
        std::vector<complex<double>,fftalloc<complex<double> > > data(N);
        std::vector<complex<double>,fftalloc<complex<double> > > dataFourier(N);
        std::vector<complex<double>,fftalloc<complex<double> > > dataCalc(N);
        std::cout << "1) --- original data ---" << endl;
        for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
            data[i] = polar(sin(double(i)/N * M_PI * 2), 0.0);
//            std::cout << i << data[i] << endl;
        }

        // fft
        std::cout << "2) --- fft data ---" << endl;
        fft.fwd(dataFourier, data);
        std::cout << "speed / ms: " << fft.speed()/1000 << endl;
//        for(std::vector<complex<double> >::size_type i = 0; i != dataFourier.size(); i++) {
//            std::cout << i << dataFourier[i] << endl;
//        }

        // ifft
        std::cout << "3) --- ifft data ---" << endl;
        fft.inv(dataCalc, dataFourier);
        std::cout << "speed / ms: " << fft.speed()/1000 << endl;
//        for(std::vector<complex<double> >::size_type i = 0; i != dataCalc.size(); i++) {
//            std::cout << i << dataCalc[i] << endl;
//        }
//        std::cout << "4) --- comparison data ---" << endl;
//        for(std::vector<complex<double> >::size_type i = 0; i != dataCalc.size(); i++) {
//            std::cout << i << data[i] - dataCalc[i] << endl;
//        }

    std::getchar();
    return 0;
}
