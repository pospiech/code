/**
 * fftw for C++
 *
 *  Copyright 2016 by Matthias Pospiech <matthias@pospiech.eu>
 *
 * This file is part of fftwcpp, a C++ template library for FFTW
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef FFTSHIFT_H
#define FFTSHIFT_H


#include <complex>
#include <vector>
#include <algorithm> // std::rotate
#include <iostream>

using std::vector;
using std::complex;

/*! Forward FFT shift. For vectors, fftshift swaps the left and right halves of a vector
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 34 012
 *  \param [in/out] complexArray, count
 */
template <typename _Real>
static inline
void fftshift(complex<_Real> * complexArray, const size_t count)
{
    size_t center = (size_t) floor((float)count/2);
    if (count % 2 != 0) {
        center++;
    }
    // odd: 012 34 changes to 34 012
    // center = 5/2 -> 2,5 -> 2 = 2 + 1
    std::rotate(complexArray, complexArray + center, complexArray + count);
    // std::rotate(complexVector.begin(),complexVector.begin()+center,complexVector.end());
}

/*! Invers FFT shift. For vectors, ifftshift swaps the left and right halves of a vector.
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 234 01
 *  \param [in/out] complexArray, count
 */
template <typename _Real>
static inline
void ifftshift(complex<_Real> * complexArray, const size_t count)
{
    size_t center = (size_t) floor((float)count/2);
    // odd: 01 234 changes to 234 01
    // center = 5/2 -> 2,5 -> 2
    std::rotate(complexArray, complexArray + center,complexArray + count);
    // std::rotate(complexVector.begin(),complexVector.begin()+center,complexVector.end());
}

/*! \sa fftshift(complex<_Real> & complexArray,const size_t count) */
template <typename _Real>
static inline
void fftshift(vector<complex<_Real> > & data)
{
    fftshift(&data[0], data.size());
}

/*! \sa ifftshift(complex<_Real> & complexArray,const size_t count) */
template <typename _Real>
static inline
void ifftshift(vector<complex<_Real> > & complexVector)
{
    ifftshift(&complexVector[0], complexVector.size());
}


/*! \sa fftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void fftshift(fftwf_complex * complexArray, size_t arraySize)
{
    fftshift(reinterpret_cast<complex<float> *>(complexArray), arraySize);
}

/*! \sa fftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void fftshift(fftw_complex * complexArray, size_t arraySize)
{
    fftshift(reinterpret_cast<complex<double> *>(complexArray), arraySize);
}

/*! \sa fftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void fftshift(fftwl_complex * complexArray, size_t arraySize)
{
    fftshift(reinterpret_cast<complex<long double> *>(complexArray), arraySize);
}

/*! \sa ifftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void  ifftshift(fftwf_complex * complexArray, size_t arraySize)
{
     ifftshift(reinterpret_cast<complex<float> *>(complexArray), arraySize);
}

/*! \sa  ifftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void  ifftshift(fftw_complex * complexArray, size_t arraySize)
{
     ifftshift(reinterpret_cast<complex<double> *>(complexArray), arraySize);
}

/*! \sa  ifftshift(complex<_Real> & complexArray, const size_t count) */
static inline
void  ifftshift(fftwl_complex * complexArray, size_t arraySize)
{
     ifftshift(reinterpret_cast<complex<long double> *>(complexArray), arraySize);
}

/*! Forward FFT shift. For matrices, fftshift swaps the first quadrant with
 *  the third and the second quadrant with the fourth.
 *  This is identical with the circshift algorithm using a shift of half the dimensions.
 *  This is importent for an odd number of elements.
 *  example:  1 2      4 3
 *            3 4  ->  2 1
 *
 *  example:  1 2 3    9 7 8
 *            4 5 6    3 1 2
 *            7 8 9 -> 6 4 5
 *
 *  \param [in/out] complexArray
 *  \note Array is accessed with A[x + Nx * y] (= A[x][y])
 */
template<class _Real>
static inline
void fftshift(complex<_Real> *data, size_t xdim, size_t ydim)
{
    size_t xshift = xdim / 2;
    size_t yshift = ydim / 2;
    if ((xdim*ydim) % 2 != 0) {
        // temp output array
        std::vector<complex<_Real> > out;
        out.resize(xdim * ydim);
        for (size_t x = 0; x < xdim; x++) {
            size_t outX = (x + xshift) % xdim;
            for (size_t y = 0; y < ydim; y++) {
                size_t outY = (y + yshift) % ydim;
                // row-major order
                out[outX + xdim * outY] = data[x + xdim * y];
            }
        }
        // copy out back to data
        copy(out.begin(), out.end(), &data[0]);
        }
    else {
        // in and output array are the same,
        // values are exchanged using swap
        for (size_t x = 0; x < xdim; x++) {
            size_t outX = (x + xshift) % xdim;
            for (size_t y = 0; y < yshift; y++) {
                size_t outY = (y + yshift) % ydim;
                // row-major order
                swap(data[outX + xdim * outY], data[x + xdim * y]);
            }
        }
    }
}

/*! Invers FFT shift. For matrices, ifftshift swaps the first quadrant with
 *  the third and the second quadrant with the fourth.
 *  This is identical with the circshift algorithm using a shift of half the dimensions.
 *  This is importent for an odd number of elements.
 *  example:  1 2      4 3
 *            3 4  ->  2 1
 *
 *  example:  1 2 3    5 6 4
 *            4 5 6    8 9 7
 *            7 8 9 -> 2 3 1
 * \note Array is accessed with A[x + Nx * y] (= A[x][y])
 * \note ifftshift undoes the results of fftshift.
 *       If the matrix X contains an odd number of elements,
 *       ifftshift(fftshift(X)) must be done to obtain the original X.
 */
template<class _Real>
static inline
void ifftshift(complex<_Real> *data, size_t xdim, size_t ydim)
{
    size_t xshift = xdim / 2;
    if (xdim % 2 != 0) {
        xshift++;
    }
    size_t yshift = ydim / 2;
    if (ydim % 2 != 0) {
        yshift++;
    }
    if ((xdim*ydim) % 2 != 0) {
        // temp output array
        std::vector<complex<_Real> > out;
        out.resize(xdim * ydim);
        for (size_t x = 0; x < xdim; x++) {
            size_t outX = (x + xshift) % xdim;
            for (size_t y = 0; y < ydim; y++) {
                size_t outY = (y + yshift) % ydim;
                // row-major order
                out[outX + xdim * outY] = data[x + xdim * y];
            }
        }
        // copy out back to data
        copy(out.begin(), out.end(), &data[0]);
        }
    else {
        // in and output array are the same,
        // values are exchanged using swap
        for (size_t x = 0; x < xdim; x++) {
            size_t outX = (x + xshift) % xdim;
            for (size_t y = 0; y < yshift; y++) {
                size_t outY = (y + yshift) % ydim;
                // row-major order
                swap(data[outX + xdim * outY], data[x + xdim * y]);
            }
        }
    }
}

/*! \sa fftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
template <typename _Real>
static inline
void fftshift(vector<complex<_Real> > & data, size_t xdim, size_t ydim)
{
    fftshift(&data[0], xdim, ydim);
}

/*! \sa ifftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
template <typename _Real>
static inline
void ifftshift(vector<complex<_Real> > & data, size_t xdim, size_t ydim)
{
    ifftshift(&data[0], xdim, ydim);
}


/*! \sa fftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void fftshift(fftwf_complex * complexArray, size_t xdim, size_t ydim)
{
    fftshift(reinterpret_cast<complex<float> *>(complexArray), xdim, ydim);
}

/*! \sa fftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void fftshift(fftw_complex * complexArray, size_t xdim, size_t ydim)
{
    fftshift(reinterpret_cast<complex<double> *>(complexArray), xdim, ydim);
}

/*! \sa fftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void fftshift(fftwl_complex * complexArray, size_t xdim, size_t ydim)
{
    fftshift(reinterpret_cast<complex<long double> *>(complexArray), xdim, ydim);
}

/*! \sa ifftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void  ifftshift(fftwf_complex * complexArray, size_t xdim, size_t ydim)
{
     ifftshift(reinterpret_cast<complex<float> *>(complexArray), xdim, ydim);
}

/*! \sa  ifftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void  ifftshift(fftw_complex * complexArray, size_t xdim, size_t ydim)
{
     ifftshift(reinterpret_cast<complex<double> *>(complexArray), xdim, ydim);
}

/*! \sa  ifftshift(complex<_Real> & complexArray, size_t xdim, size_t ydim) */
static inline
void  ifftshift(fftwl_complex * complexArray, size_t xdim, size_t ydim)
{
     ifftshift(reinterpret_cast<complex<long double> *>(complexArray), xdim, ydim);
}


/*! Array index of 2D data using row-major order */
static inline
size_t arrayIndex(const size_t x, const size_t y, const size_t Nx)
{
    return x + Nx * y;
}

/*! Print complex array to std out. Usefull for debugging. */
template <typename _Real>
static inline
void  printArray2D(complex<_Real> * Array, const size_t Nx, const size_t Ny)
{
    std::cout << std::endl << "----------- Array: ( row-major order )---------------" << std::endl;
    for (size_t y = 0; y < Ny; y++) {
        for (size_t x = 0; x < Nx; x++) {
            size_t index = arrayIndex(x, y, Nx);
            cout << " c:(" << Array[index].real() << "," << Array[index].imag() << ")  ";
        }
        cout << std::endl;
    }
}

#endif // FFTSHIFT_H
