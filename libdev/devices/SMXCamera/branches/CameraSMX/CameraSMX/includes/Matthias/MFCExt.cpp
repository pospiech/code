#include "StdAfx.h"
#include "MFCExt.h"




void delay(long ms)  //  Zeit im ms
{
#ifndef WIN32
clock_t cl;

	cl=clock();
	
	while (1000.0*(double)(clock()-cl)/(double)CLOCKS_PER_SEC<(double)ms);

#else

HANDLE wait_timer;
MSG message;

	if (ms>0)
	{
		wait_timer=CreateWaitableTimer(NULL,TRUE,NULL);
		WaitForSingleObject(wait_timer,ms);
		CloseHandle(wait_timer);
	}

	if(PeekMessage(&message,NULL,0,0,PM_REMOVE))  // Message queue abfragen
	{	
		TranslateMessage(&message);		
		DispatchMessage(&message); 	
	}


#endif
}	


double CStringToDouble(CString value)
{
	int result = value.Replace(_T(","),_T("."));
	return atof(value);
}

float CStringToFloat(CString value)
{
	return (float)CStringToDouble(value);
}


int CStringToInt(CString value)
{
	return atoi(value);
}


bool checkMinMax(int value, int min, int max)
{
	if ((value >= min) && (value <= max))
	{
		return true;
	} else {
		return false;
	}
}
bool checkMinMax(float value, float min, float max)
{
	if ((value >= min) && (value <= max))
	{
		return true;
	} else {
		return false;
	}
}
bool checkMinMax(double value, double min, double max)
{
	if ((value >= min) && (value <= max))
	{
		return true;
	} else {
		return false;
	}
}

int CorrectRangeMinMax(int value, int min, int max)
{
	if (value < min) { return min;	} 
	else if (value > max) {	return max;	}
	else { return value; }
}

float CorrectRangeMinMax(float value, float min, float max)
{
	if (value < min) { return min;	} 
	else if (value > max) {	return max;	}
	else { return value; }
}

double CorrectRangeMinMax(double value, double min, double max)
{
	if (value < min) { return min;	} 
	else if (value > max) {	return max;	}
	else { return value; }
}
