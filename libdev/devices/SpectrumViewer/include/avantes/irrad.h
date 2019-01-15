#ifndef IRRAD_H
#define IRRAD_H

//----------------------------------------------------------------------------
// Import overview
//----------------------------------------------------------------------------

#define DLL_API extern "C" __declspec (dllimport)

//#pragma alignment

const int MAX_PIXELS = 3648;

typedef double pixelarray[MAX_PIXELS];

typedef struct
{
    double  wl[MAX_PIXELS] ;
    double  intensity[MAX_PIXELS];
} DataType;

DLL_API short int __stdcall Radio_GetIrradiance
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetEnergyPerCM2
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_inttime,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetReceivedPower
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_surfacecm2,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetReceivedEnergy
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_surfacecm2,
	double a_inttime,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetRadiantIntensity
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_distancecm,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetRadiantEnergy
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_inttime,
	double a_distancecm,
    double *a_integral
);

DLL_API short int __stdcall Radio_CalcEmittedPower
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_distancecm,
	double a_steradians,
    double *a_integral
);

DLL_API short int __stdcall Radio_CalcEmittedEnergy
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_inttime,
	double a_distancecm,
	double a_steradians,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetPhotonFluxPerM2
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetPhotonFlux
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_surfacecm2,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetPhotonsPerM2
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_inttime,
    double *a_integral
);

DLL_API short int __stdcall Radio_GetPhotons
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_surfacecm2,
	double a_inttime,
    double *a_integral
);

DLL_API short int __stdcall Photo_GetIlluminance
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	double *a_Lux

);

DLL_API short int __stdcall Photo_GetReceivedLuminousFlux
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	double a_surfacecm2,
	double *a_Lumen
);

DLL_API short int __stdcall Photo_CalcLuminousFlux
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	double a_distancecm,
	double a_steradians,
	double *a_Lumen
);

DLL_API short int __stdcall Photo_GetLuminousIntensity
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	double a_distancecm,
	double *a_Candela
);

DLL_API short int __stdcall Color_GetColorOfLightParam
(
	DataType *a_data,
	unsigned char   a_CIEResolution,
	unsigned char   a_Observer,
	double *a_smallx,
	double *a_smally,
	double *a_smallz,
	double *a_bigX,
	double *a_bigY,
	double *a_bigZ,
	double *a_u,
	double *a_v,
	double *a_Colortemperature
);

DLL_API short int __stdcall Color_GetLedParamFromxy
(
    double smallx,
    double smally,
	unsigned char  a_Observer,
	double *a_DW,
	double *a_CDW,
	double *a_Purity
);

DLL_API short int __stdcall Color_GetColorRenderingIndex
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	unsigned char   a_Observer,
    double *R_Values,
    double *CRI
);

DLL_API short int __stdcall Color_GetColorQualityScale
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	unsigned char   a_Observer,
    double *Q_Values,
    double *CQS_Qa
);

DLL_API short int __stdcall Radio_GetPeak
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	short int a_Splinefactor,
	double *a_peaknm,
	double *a_peakamp,
	double *a_fwhm,
	double *a_cwlnm,
	double *a_cwlamp,
	double *a_centroidnm,
	double *a_centroidamp
);

DLL_API short int __stdcall Radio_IspGetEmittedPower
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_ispsurfacecm2,
    double *a_integral
);

DLL_API short int __stdcall Radio_IspGetEmittedEnergy
(
    DataType *a_data,
	double a_startwav,
	double a_endwav,
	double a_inttime,
	double a_ispsurfacecm2,
    double *a_integral
);

DLL_API short int __stdcall Photo_IspGetLuminousFlux
(
    DataType *a_data,
	unsigned char   a_CIEResolution,
	double a_ispsurfacecm2,
	double *a_Lumen
);

//----------------------------------------------------------------------------
// End of definitions
//----------------------------------------------------------------------------

#endif  // IRRAD_H


