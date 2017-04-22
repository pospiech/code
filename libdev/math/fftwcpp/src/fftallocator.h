/** \class fftalloc
  *
  * \brief custom allocator for FFTW
  *
  * Usage Example:
  * \code
  * #include "fftallocator.h"
  * // create a vector, using fftalloc<> as allocator
  *  std::vector<double,fftwcpp::fftalloc<double> > v;
  *
  * \endcode
  *
  * This code is derived from an code example taken from the book
  * "The C++ Standard Library - A Tutorial and Reference"
  * by Nicolai M. Josuttis, Addison-Wesley, 1999
  *
  * The alloc and deallocators are replaced by the corresponding FFTW
  * function:
  * \code
  * // allocate memory using the special fftw_malloc
  * // this is necessary since the lib requires special alignment
  * in  =  (fftw_complex*)  fftw_malloc(sizeof(fftw_complex)  *  N);
  * out  =  (fftw_complex*)  fftw_malloc(sizeof(fftw_complex)  *  N);
  * // clean up using special functions
  * fftw_free(in);
  * \endcode
  */


#ifndef FFTALLOCATOR_H
#define FFTALLOCATOR_H

// define debug tool
#include <iostream>
//#define FFTALLOC_ALLOCDEBUG

#ifdef FFTALLOC_ALLOCDEBUG
#  define ALLOCDEBUG std::cerr
#else
#  define ALLOCDEBUG if (false) std::cerr
#endif


/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference"
 * by Nicolai M. Josuttis, Addison-Wesley, 1999
 *
 * (C) Copyright Nicolai M. Josuttis 1999.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <limits>
#include <fftw3.h>

namespace fftwcpp {
   template <class T>
   class fftalloc {
     public:
       //! type definitions
       typedef T              value_type;
       typedef T*             pointer;
       typedef const T*       const_pointer;
       typedef T&             reference;
       typedef const T&       const_reference;
       typedef std::size_t    size_type;
       typedef std::ptrdiff_t difference_type;

       //! rebind allocator to type U
       template <class U>
       struct rebind {
           typedef fftalloc<U> other;
       };

       //! return address of value
       pointer address (reference value) const {
           return &value;
       }
       //! return address of value
       const_pointer address (const_reference value) const {
           return &value;
       }

       /*! constructors and destructor
        * - nothing to do because the allocator has no state
        */
       fftalloc() throw() {}
       fftalloc(const fftalloc&) throw() {}
       template <class U>
       fftalloc (const fftalloc<U>&) throw() {}
       ~fftalloc() throw() {}

       //! return maximum number of elements that can be allocated
       size_type max_size () const throw() {
           return std::numeric_limits<std::size_t>::max() / sizeof(T);
       }

       //! allocate but don't initialize num elements of type T
       pointer allocate (size_type num, const void* = 0) {
           // print message and allocate memory with global new
           ALLOCDEBUG << "allocate " << num << " element(s)"
                      << " of size " << sizeof(T) << std::endl;
           // pointer ret = (pointer)(::operator new(num*sizeof(T)));
           pointer ret = (pointer) fftw_malloc(num*sizeof(T));

           ALLOCDEBUG << " allocated at: " << (void*)ret << std::endl;
           ALLOCDEBUG << " aligned 16: " << bool((reinterpret_cast<size_t>(ret)&15) == 0) << std::endl;
           return ret;
       }

       //! initialize elements of allocated storage p with value value
       void construct (pointer p, const T& value) {
           // initialize memory with placement new
           new((void*)p)T(value);
       }

       //! destroy elements of initialized storage p
       void destroy (pointer p) {
           // destroy objects by calling their destructor
           p->~T();
       }

       //! deallocate storage p of deleted elements
       void deallocate (pointer p, size_type num) {
           // print message and deallocate memory with global delete
           ALLOCDEBUG << "deallocate " << num << " element(s)"
                      << " of size " << sizeof(T)
                      << " at: " << (void*)p << std::endl;
           // ::operator delete((void*)p);
           fftw_free(p);
       }
   }; // end of class fftalloc

   //! return that all specializations of this allocator are interchangeable
   template <class T1, class T2>
   bool operator== (const fftalloc<T1>&,
                    const fftalloc<T2>&) throw() {
       return true;
   }
   template <class T1, class T2>
   bool operator!= (const fftalloc<T1>&,
                    const fftalloc<T2>&) throw() {
       return false;
   }
} // end of namespace: fftwcpp

#endif // FFTALLOCATOR_H
