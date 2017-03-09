/**
 * fftw for C++
 *
 *  Copyright 2016 by Matthias Pospiech <matthias@pospiech.eu>
 *  Copyright (C) 2009 Mark Borgerding mark a borgerding net
 *
 * This file is derived from Eigen, a lightweight C++ template library
 * for linear algebra.
 *
 * Modified by Matthias Pospiech,
 * code stripped down for FFTW usage and compiles without use of EIGEN
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef EIGEN_FFT_H
#define EIGEN_FFT_H

#include <complex>
#include <vector>
#include <map>
#include <iostream>
#include <ctime>     // clock

using std::vector;
using std::complex;

#include "fftallocator.h"
using namespace fftwcpp;

/** \class NumTraits
  *
  * \brief Stripped NumTraits class of EIGEN. Only IsComplex is required
  *
  * \param T the numeric type at hand
  *
  * The provided data consists of:
  * \li An enum value \a IsComplex. It is equal to 1 if \a T is a \c std::complex
  *     type, and to 0 otherwise.
  */

template<typename T>
struct GenericNumTraits { enum { IsComplex = 0 }; };
// any other type = generic
template<typename T>
struct NumTraits : GenericNumTraits<T> {};
// specialize: float = non complex
template<>
struct NumTraits<float>  : GenericNumTraits<float> {};
// specialize: double = non complex
template<>
struct NumTraits<double> : GenericNumTraits<double> {};
// specialize: long double = non complex
template<>
struct NumTraits<long double> : GenericNumTraits<long double> {};
// specialize: std::complex = complex
template<typename _Real>
struct NumTraits<std::complex<_Real> >
        : GenericNumTraits<std::complex<_Real> >
{
    enum {
        IsComplex = 1
    };
};


/**
  * \defgroup FFT_Module Fast Fourier Transform module
  *
  * \code
  * #include <unsupported/Eigen/FFT>
  * \endcode
  *
  * This module provides Fast Fourier transformation, with a FFTW backend
  * implementation.
  *
  * - fftw (http://www.fftw.org) : faster, GPL -- incompatible with Eigen in LGPL form, bigger code size.
  *
  * \section FFTDesign Design
  *
  * The following design decisions were made concerning scaling and
  * half-spectrum for real FFT.
  *
  * The intent is to facilitate generic programming and ease migrating code
  * from  Matlab/octave.
  * We think the default behavior of Eigen/FFT should favor correctness and
  * generality over speed. Of course, the caller should be able to "opt-out" from this
  * behavior and get the speed increase if they want it.
  *
  * 1) %Scaling:
  * Other libraries (FFTW,IMKL,KISSFFT)  do not perform scaling, so there
  * is a constant gain incurred after the forward&inverse transforms , so
  * IFFT(FFT(x)) = Kx;  this is done to avoid a vector-by-value multiply.
  * The downside is that algorithms that worked correctly in Matlab/octave
  * don't behave the same way once implemented in C++.
  *
  * How Eigen/FFT differs: invertible scaling is performed so IFFT( FFT(x) ) = x.
  *
  * 2) Real FFT half-spectrum
  * Other libraries use only half the frequency spectrum (plus one extra
  * sample for the Nyquist bin) for a real FFT, the other half is the
  * conjugate-symmetric of the first half.  This saves them a copy and some
  * memory.  The downside is the caller needs to have special logic for the
  * number of bins in complex vs real.
  *
  * How Eigen/FFT differs: The full spectrum is returned from the forward
  * transform.  This facilitates generic template programming by obviating
  * separate specializations for real vs complex.  On the inverse
  * transform, only half the spectrum is actually used if the output type is real.
  */

// FFTW: faster, GPL -- incompatible with Eigen in LGPL form, bigger code size
#  include "ei_fftw_impl.h"

/*! define FFT implementation. Here fftw is the the only provided implementation */
//template <typename T> typedef struct internal::fftw_impl  default_fft_impl; this does not work
template <typename T> struct default_fft_impl : public Eigen::internal::fftw_impl<T> {};

/*! Define fft_vector type
 *  This is a shortcut for
 *  \code vector<complex<double>,fftalloc<complex<double> > >
 *  \example fft_vector<complex<double> >
*/
template <typename T> using fft_vector = std::vector<T, fftalloc<T>>;

/*! class FFT */
template <typename T_Scalar,
          typename T_Impl=default_fft_impl<T_Scalar> >
class FFT
{
public:
    typedef T_Impl impl_type;
    typedef int Index; // usage ??
    typedef typename impl_type::Scalar Scalar;
    typedef typename impl_type::Complex Complex;

    enum Flag {
        Default=0, // goof proof
        Unscaled=1,
        HalfSpectrum=2,
        // SomeOtherSpeedOptimization=4
        Speedy=32767
    };
    /* copy constructor */
    FFT( const impl_type & impl=impl_type(), Flag flags=Default )
        : m_impl(impl), m_flag(flags) { }

    inline
    bool HasFlag(Flag f) const { return (m_flag & (int)f) == f;}

    inline
    void SetFlag(Flag f) { m_flag |= (int)f;}

    inline
    void ClearFlag(Flag f) { m_flag &= (~(int)f);}

    inline
    impl_type & impl() {return m_impl;}


    /*! Creates Complex vector using fftw allocator
     *  If the vector goes out of scope its destructor calls the correct
     *  deallocator.
     */
    inline
    vector<Complex,fftalloc<Complex > > * createVector(size_t nfft)
    {
        vector<Complex,fftalloc<Complex > > * data = new vector<Complex,fftalloc<Complex > >(nfft);
        // return reinterpret_cast<vector<Complex> *>(data);
        return data;
    }

    /*! 1D fwd scalar->complex */
    inline
    void fwd( Complex * dst, const Scalar * src, Index nfft)
    {
        this->startClock();
        m_impl.fwd(dst,src,static_cast<int>(nfft));
        if ( HasFlag(HalfSpectrum) == false)
            ReflectSpectrum(dst,nfft);
        this->stopClock();
    }

    /*! 1D fwd complex->complex */
    inline
    void fwd( Complex * dst, const Complex * src, Index nfft)
    {
        this->startClock();
        m_impl.fwd(dst,src,static_cast<int>(nfft));
        this->stopClock();
    }

    /*
    inline
    void fwd2(Complex * dst, const Complex * src, int n0,int n1)
    {
      m_impl.fwd2(dst,src,n0,n1);
    }
    */
    /*! 1D fwd vector<type>->vector<complex> */
    template <typename _Input>
    inline
    void fwd( std::vector<Complex,fftalloc<Complex> > & dst, const vector<_Input,fftalloc<_Input> > & src)
    {
        if ( NumTraits<_Input>::IsComplex == 0 && HasFlag(HalfSpectrum) )
            dst.resize( (src.size()>>1)+1); // half the bins + Nyquist bin
        else
            dst.resize(src.size());
        fwd(&dst[0],&src[0],src.size());
    }

    /*! 1D fwd vector<type>->vector<complex> */
    template <typename _Input>
    inline
    void fwd( std::vector<Complex> & dst, const std::vector<_Input> & src)
    {
        if ( NumTraits<_Input>::IsComplex == 0 && HasFlag(HalfSpectrum) )
            dst.resize( (src.size()>>1)+1); // half the bins + Nyquist bin
        else
            dst.resize(src.size());
        fwd(&dst[0],&src[0],src.size());
    }

    /*! 1D inv complex->complex */
    inline
    void inv( Complex * dst, const Complex * src, Index nfft)
    {
        this->startClock();
        m_impl.inv( dst,src,static_cast<int>(nfft) );
        if ( HasFlag( Unscaled ) == false)
            scale(dst,Scalar(1./nfft),nfft); // scale the time series
        this->stopClock();
    }

    /*! 1D inv complex->scalar */
    inline
    void inv( Scalar * dst, const Complex * src, Index nfft)
    {
        this->startClock();
        m_impl.inv( dst,src,static_cast<int>(nfft) );
        if ( HasFlag( Unscaled ) == false)
            scale(dst,Scalar(1./nfft),nfft); // scale the time series
        this->stopClock();
    }


    /*! 1D fwd vector<complex>->vector<type> */
    template <typename _Output>
    inline
    void inv( std::vector<_Output,fftalloc<_Output> > & dst, const std::vector<Complex,fftalloc<Complex> >& src, Index nfft=-1)
    {
        if (nfft<1)
            nfft = ( NumTraits<_Output>::IsComplex == 0 && HasFlag(HalfSpectrum) ) ? 2*(src.size()-1) : src.size();
        dst.resize( nfft );
        inv( &dst[0],&src[0],nfft);
    }

    /*! 1D fwd vector<complex>->vector<type> */
    template <typename _Output>
    inline
    void inv( std::vector<_Output> & dst, const std::vector<Complex> & src,Index nfft=-1)
    {
        if (nfft<1)
            nfft = ( NumTraits<_Output>::IsComplex == 0 && HasFlag(HalfSpectrum) ) ? 2*(src.size()-1) : src.size();
        dst.resize( nfft );
        inv( &dst[0],&src[0],nfft);
    }


    /*
    // TODO: multi-dimensional FFTs
    inline
    void inv2(Complex * dst, const Complex * src, int n0,int n1)
    {
      m_impl.inv2(dst,src,n0,n1);
      if ( HasFlag( Unscaled ) == false)
          scale(dst,1./(n0*n1),n0*n1);
    }
  */

private:

    template <typename T_Data>
    inline
    void scale(T_Data * x,Scalar s,Index nx)
    {
        for (int k=0;k<nx;++k)
            *x++ *= s;
    }

    inline
    void ReflectSpectrum(Complex * freq, Index nfft)
    {
        // create the implicit right-half spectrum (conjugate-mirror of the left-half)
        Index nhbins=(nfft>>1)+1;
        for (Index k=nhbins;k < nfft; ++k )
            freq[k] = conj(freq[nfft-k]);
    }

    clock_t m_clockStart;
    clock_t m_clockEnd;

    /*! save current time for measurement of speed at start */
    inline
    void startClock()
    {
        m_clockStart=clock();
    }

    /*! save current time for measurement of speed at stop */
    inline
    void stopClock()
    {
        m_clockEnd=clock();
    }

public:
    /*! time elapsed for FFT Calculation in seconds */
    inline
    double speed()
    {
        return (double)(m_clockEnd  - m_clockStart) / CLOCKS_PER_SEC;
    }

private:
    impl_type m_impl;
    int m_flag;
};

#endif
