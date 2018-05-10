#pragma once

#ifndef MFCEXT_H       // ensure we are only included once
#define MFCEXT_H


void delay(long ms);  //  Zeit im ms

double CStringToDouble(CString value);
float CStringToFloat(CString value);
int CStringToInt(CString value);


bool checkMinMax(int value, int min, int max);
bool checkMinMax(float value, float min, float max);
bool checkMinMax(double value, double min, double max);

int CorrectRangeMinMax(int value, int min, int max);
float CorrectRangeMinMax(float value, float min, float max);
double CorrectRangeMinMax(double value, double min, double max);


#endif   // MFCEXT_H
