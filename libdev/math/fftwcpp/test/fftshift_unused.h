#ifndef FFTSHIFT_UNUSED_H
#define FFTSHIFT_UNUSED_H

/*! swap arrays, required for __non standard__ complex types with double[2] */
template<typename T, std::size_t N>
static inline void swap(T (&x)[N],T (&y)[N])
{
    for ( std::size_t i = 0; i != N; ++i )
        std::swap( x[i], y[i] );
}


/*! swaps the left and right halfes of the data array (forward shift)
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 34 012
 *  \param [in/out] complexArray, count
 */
template <typename _Real>
static inline
void  shift(complex<_Real> * complexArray, size_t count)
{
    int center = (int) floor((float)count/2);
    // For odd and for even numbers of element use different algorithm
    if (count % 2 == 0) {
        complex<_Real> * swapArray = new complex<_Real>[center];

        // swap = comp(0)-comp(0 + c) = 012
        memcpy(swapArray, complexArray, center * sizeof(complex<_Real>));
        // comp = comp(c)-comp(c + c) = 345 345
        memcpy(complexArray, complexArray + center, center * sizeof(complex<_Real>));
        // comp = swap(0) - swap(c) = 345 012
        memcpy(complexArray + center, swapArray, center * sizeof(complex<_Real>));

        delete [] swapArray;
    }
    else {
        // 012 34 changes to 34 012
        // center = 5/2 -> 2,5 -> 2
        complex<_Real> * swapArray = new complex<_Real>[center + 1];
        // swap = comp(0)-comp(0 + c + 1) = 012
        memcpy(swapArray, complexArray, (center + 1) * sizeof(complex<_Real>));
        // comp(0) = comp(c+1)-comp(c+1 + c) = 34 234
        memcpy(complexArray, complexArray + center + 1, (center) * sizeof(complex<_Real>));
        // comp(c) = swap(0) - swap(c+1) = 34 012
        memcpy(complexArray + center, swapArray, (center + 1) * sizeof(complex<_Real>));

        delete [] swapArray;
    }
}

/*! swaps the left and right halfes of the data array (invers shift)
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 234 01
 *  \param [in/out] complexArray, count
 */
template <typename _Real>
static inline
void  ishift(complex<_Real> * complexArray, size_t count)
{
    int center = (int) floor((float)count/2);
    // For odd and for even numbers of element use different algorithm
    if (count % 2 == 0) {
        // identical with forward shift
        shift(complexArray, count);
    }
    else {
        // 01 2 34 changes to 234 01
        // center = 5/2 -> 2,5 -> 2
        complex<_Real> * swapArray = new complex<_Real>[center];
        // swap = comp(0)-comp(0 + c ) = 01
        memcpy(swapArray, complexArray, (center) * sizeof(complex<_Real>));
        // comp(0) = comp(c+1)-comp(c+1 + c) = 234 34
        memcpy(complexArray, complexArray + center, (center + 1) * sizeof(complex<_Real>));
        // comp(c) = swap(0) - swap(c+1) = 234 01
        memcpy(complexArray + center + 1, swapArray, (center) * sizeof(complex<_Real>));

        delete [] swapArray;
    }
}

/*! \sa shift2D(vector<complex<double> > & complexArray) */
template <typename _Real>
static inline
void  shift2D(complex<_Real> * Array, size_t Nx, size_t Ny)
{
    // shift columns first
    for (size_t ix = 0; ix < Nx; ix++) {
        shift(Array + ix * Nx, Nx);
    }

    // Shift each row
    size_t c = Nx/2;
    for (size_t iy=0;iy<Ny;++iy)
    {
        for (size_t ix=0;ix < c ;++ix)
        {
            int pos1 = iy + Nx * (ix + c);
            int pos2 = iy + Nx * ix;
            swap(Array[pos1], Array[pos2]);
        }
    }
}



// http://stackoverflow.com/questions/5915125/fftshift-ifftshift-c-c-source-code
template<class _Real>
static inline
void circshift(complex<_Real>  *out, const complex<_Real>  *in, size_t xdim, size_t ydim, size_t xshift, size_t yshift)
{
  for (size_t x = 0; x < xdim; x++) {
    size_t outX = (x + xshift) % xdim;
    for (size_t y = 0; y < ydim; y++) {
      size_t outY = (y + yshift) % ydim;
      // row-major order
      // cout << "(" << outX << "," << outY << ") <= (" << x << "," << y <<  ")" << endl;
      out[outX + xdim * outY] = in[x + xdim * y];
    }
  }
}

template<class _Real>
static inline
void fftshift2D(complex<_Real> *out, const complex<_Real> *in, size_t xdim, size_t ydim)
{
    size_t xshift = xdim / 2;
    size_t yshift = ydim / 2;
    circshift(out, in, xdim, ydim, xshift, yshift);
}

template<class _Real>
static inline
void ifftshift2D(complex<_Real> *out, const complex<_Real> *in, size_t xdim, size_t ydim)
{
    size_t xshift = xdim / 2;
    if (xdim % 2 != 0) {
        xshift++;
    }
    size_t yshift = ydim / 2;
    if (ydim % 2 != 0) {
        yshift++;
    }
    circshift(out, in, xdim, ydim, xshift, yshift);
}



// http://stackoverflow.com/questions/5915125/fftshift-ifftshift-c-c-source-code
template<class T>
static inline
void ifftShift2D(T *out, const T* in, size_t nx, size_t ny)
{
    const size_t hlen1 = (ny+1)/2;
    const size_t hlen2 = ny/2;
    const size_t shft1 = ((nx+1)/2)*ny + hlen1;
    const size_t shft2 = (nx/2)*ny + hlen2;

    const T* src = in;
    for(T* tgt = out; tgt < out + shft1 - hlen1; tgt += ny, src += ny) { // (nx+1)/2 times
        copy(src, src+hlen1, tgt + shft2);          //1->4
        copy(src+hlen1, src+ny, tgt+shft2-hlen2); } //2->3
    src = in;
    for(T* tgt = out; tgt < out + shft2 - hlen2; tgt += ny, src += ny ){ // nx/2 times
        copy(src+shft1, src+shft1+hlen2, tgt);         //4->1
        copy(src+shft1-hlen1, src+shft1, tgt+hlen2); } //3->2
};


template<class T>
static inline
void ifftShift2DCopy(T* in, size_t nx, size_t ny)
{
    const size_t hlen1 = (ny+1)/2;
    const size_t hlen2 = ny/2;
    const size_t shft1 = ((nx+1)/2)*ny + hlen1;
    const size_t shft2 = (nx/2)*ny + hlen2;

    std::vector<T> outVector;
    outVector.resize(nx * ny);
    T* out = &outVector[0];

    const T* src = in;
    for(T* tgt = out; tgt < out + shft1 - hlen1; tgt += ny, src += ny) { // (nx+1)/2 times
        copy(src, src+hlen1, tgt + shft2);          //1->4
        copy(src+hlen1, src+ny, tgt+shft2-hlen2); } //2->3
    src = in;
    for(T* tgt = out; tgt < out + shft2 - hlen2; tgt += ny, src += ny ){ // nx/2 times
        copy(src+shft1, src+shft1+hlen2, tgt);         //4->1
        copy(src+shft1-hlen1, src+shft1, tgt+hlen2); } //3->2
    copy(outVector.begin(), outVector.end(), &in[0]);
};


/*! swaps the left and right halfes of the data array (invers shift)
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 234 01
 *  \param [in/out] data
 */
template <typename _Real>
static inline
void ifftshift(std::complex<_Real> *data, int count)
{
    int k = 0;
    int c = (int) floor((float)count/2);
    if (count % 2 == 0)
    {
        for (k = 0; k < c; k++)
            swap(data[k], data[k+c]);
    }
    else
    {
        // data = 1 2 3 4 5
        // c = 2
        // count = 5
        // tmp = @(5-1) = 5
        std::complex<_Real> tmp = data[count - 1];
        // c-1:-1:0
        for (k = c-1; k >= 0; k--)
        {
            data[c + k + 1] = data[k];
            // 1: @(2 + 1 + 1) = @(1) -> 1 2 3 4 [2]
            // 0: @(2 + 0 + 1) = @(0) -> 1 4 3 [1] 2
            data[k] = data[c + k];
            // 1: @(1) = @(2 + 1) -> 1 [4] 3 4 2
            // 0: @(0) = @(2 + 0) -> [3] 4 3 1 2
        }
        data[c] = tmp;
        //  @(2) = tmp -> 3 4 5 1 2
    }
}

/*! swaps the left and right halfes of the data array (forward shift)
 *  even example: 012 345 changes to 345 012
 *   odd example: 01234 changes to 34 012
 *  \param [in/out] data
 */
template <typename _Real>
static inline
void fftshift(std::complex<_Real> *data, size_t count)
{
    // count = 6
    // c = 3
    int c = (int) floor((float)count/2);
    // For odd and for even numbers of element use different algorithm
    if (count % 2 == 0)
    {
        for (int k = 0; k < c; k++)
            swap(data[k], data[k+c]);
        // 0: swap(  1 @(0), 4 @(0+3)) -> 4 2 3 1 5 6
        // 1: swap(  2 @(1), 5 @(1+3)) -> 4 5 3 1 2 6
        // 2: swap(  3 @(2), 6 @(2+3)) -> 4 5 6 1 2 3
    }
    else
    {
        // data = 1 2 3 4 5
        // c = 2
        complex<_Real> tmp = data[0];
        for (int k = 0; k < c; k++)
        {
            data[k] = data[c + k + 1];
            // 0: @(0) = @(2 + 0 + 1) -> 4 2 3 4 5
            // 1: @(1) = @(2 + 1 + 1) -> 4 5 3 4 5
            data[c + k + 1] = data[k + 1];
            // 0: @(2 + 0 + 1) = @(0 + 1) -> 4 2 3 2 5
            // 1: @(2 + 1 + 1) = @(1 + 1) -> 4 5 3 2 3
        }
        // 1: @(2) = tmp -> 4 5 1 2 3
        data[c] = tmp;
    }
}

#endif // FFTSHIFT_UNUSED_H
