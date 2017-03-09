#include "fftshift.h"
//#include "fftshift_unused.h"
#include <ctime>        // std::clock
#include <iostream>     // std::cout
#include <vector>

using namespace std;

void startClock();
void stopClock();
double speed();
void fillData(vector<complex<double> > & data, size_t count, size_t offset);
void printData(vector<complex<double> > & data);
bool dataEqual(vector<complex<double> > & data, vector<complex<double> > & dataOrig);


clock_t clockStart;
clock_t clockEnd;

void testfftshift(){
    std::vector<complex<double> > dataOrig;
    std::vector<complex<double> > data;
    std::vector<complex<double> > dataOut;
    size_t Nx = 128 + 0;
    size_t Ny = 128 + 0;
    size_t N = Nx * Ny;
    size_t Iter = 1e4;

    fillData(data, N, 1);
    dataOut.resize(data.size());
    dataOrig = data;

//    printArray2D(&data[0], Nx, Ny);
    startClock();
    for (size_t i = 0; i < Iter; ++i) {
        fftshift(data, Nx, Ny);
        ifftshift(data, Nx, Ny);
    }
    stopClock();
    cout << "2D Shift (circ): Time elapsed: " << speed() << endl;
    if (!dataEqual(data,dataOrig)) {
        cout << "Error: Data forth and back calculation not eqal" << endl;
    }


    // create Data:
    fillData(data, N, 1);
    dataOrig = data;

    // --- memcpy / shift ---
    startClock();
    for (size_t i = 0; i < Iter; ++i) {
        fftshift(data);
        ifftshift(data);
    }
    stopClock();
    cout << "1D Shift (rotate): Time elapsed: " << speed() << endl;
    if (!dataEqual(data,dataOrig)) {
        cout << "Error: Data forth and back calculation not eqal" << endl;
    }
}



bool dataEqual(vector<complex<double> > & data, vector<complex<double> > & dataOrig)
{
    bool isequal = true;
    for(size_t k = 0; k < data.size(); ++k) {
        if (data[k] != dataOrig [k]) {
            isequal = false;
            break;
        }
    }
    return isequal;
}

void printData(vector<complex<double> > & data)
{
    // print out content:
    std::cout << "data contains:";
    for (std::vector<complex<double> >::iterator it=data.begin(); it!=data.end(); ++it)
      std::cout << ' ' << std::abs(*it);
    std::cout << '\n';
}


void fillData(vector<complex<double> > & data, size_t count, size_t offset = 0)
{
    data.resize(0);
    // set some values: 10: 0 1 2 3 4 5 6 7 8 9
    for (size_t i=0; i<count; ++i) {
        complex<double> c = std::polar(double(i + offset), 0.0);
        data.push_back(c);
    }
}

/*! save current time for measurement of speed at start */
void startClock()
{
    clockStart=clock();
}

/*! save current time for measurement of speed at stop */
void stopClock()
{
    clockEnd=clock();
}
/*! time elapsed for FFT Calculation in seconds */
double speed()
{
    return (double)(clockEnd  - clockStart) / CLOCKS_PER_SEC;
}
