//
// File: colcalc.h
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// this is a C/C++ include file for the colcalc.dll
//
//////////////////////////////////////////////////////////////////////////


#ifndef __COLCALC
#define __COLCALC

//////////////////////////////////////////////////////////////////////////
//// Define C linkage if using a C++ compiler
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


//////////////////////////////////////////////////////////////////////////
// ColCalc_getAllParam
//
// FLOAT SpectralArray[80]: spectral data from 380nm..780nm in 5nm steps
// illuminant according CIE:
//    0 - A
//    1 - B
//    2 - C
//    3 - D50
//    4 - D55
//    5 - D65
//    6 - D75
//
//  returns: all CIE color parameters

//void __declspec(dllimport) __stdcall Colcalc_GetAllParam(
//                                   FLOAT* SpectralArray,    // array of f[80]
//                                   BYTE illuminant_n,       // illuminant type
//                                   FLOAT* L,                // color parameters
//                                   FLOAT* a,
//                                   FLOAT* b,
//                                   FLOAT* hue,
//                                   FLOAT* C,
//                                   FLOAT* X,
//                                   FLOAT* Y,
//                                   FLOAT* Z);
                            
void __declspec(dllimport) __stdcall Colcalc_GetAllParam(
                                   float* SpectralArray,    // array of f[80]
                                   unsigned char illuminant_n,       // illuminant type
                                   float* L,                // color parameters
                                   float* a,
                                   float* b,
                                   float* hue,
                                   float* C,
                                   float* X,
                                   float* Y,
                                   float* Z);

/////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
};
#endif //__cplusplus


#endif //__COLCALC
