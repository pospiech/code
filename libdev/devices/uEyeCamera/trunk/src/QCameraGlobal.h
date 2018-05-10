#ifndef QCAMERA_GLOBAL_H
#define QCAMERA_GLOBAL_H

#include <QtGlobal>
#if defined(Q_WS_WIN)

#ifdef UEYECAMERA_DLL

#if defined(UEYECAMERA_NODLL)     // Do not create a DLL library
    #define QCAMERA_EXPORT
#else
    #if defined(UEYECAMERA_MAKEDLL) // create a DLL library
        #define QCAMERA_EXPORT  __declspec(dllexport)
    #else                           // use a DLL library
        #define QCAMERA_EXPORT  __declspec(dllimport)
    #endif
#endif

#endif // UEYECAMERA_DLL

#endif // Q_WS_WIN

#ifndef QCAMERA_EXPORT
#define QCAMERA_EXPORT
#endif

#endif // QCAMERA_GLOBAL_H
