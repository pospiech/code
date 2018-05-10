#ifndef QCAMERASMX_GLOBAL_H
#define QCAMERASMX_GLOBAL_H

#include <QtGlobal>

#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning( disable : 4100 )
#endif

#if defined(Q_WS_WIN)

#ifdef SMXCAMERA_DLL

#if defined(SMXCAMERA_NODLL)     // Do not create a DLL library
    #define QSMXCAMERA_EXPORT
#else
    #if defined(SMXCAMERA_MAKEDLL) // create a DLL library
        #define QSMXCAMERA_EXPORT  __declspec(dllexport)
    #else                           // use a DLL library
        #define QSMXCAMERA_EXPORT  __declspec(dllimport)
    #endif
#endif

#endif // SMXCAMERA_DLL

#endif // Q_WS_WIN

#ifndef QSMXCAMERA_EXPORT
#define QSMXCAMERA_EXPORT
#endif

#endif // QCAMERASMX_GLOBAL_H
