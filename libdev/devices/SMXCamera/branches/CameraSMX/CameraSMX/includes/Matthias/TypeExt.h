#pragma once

template <class T>
struct CStructXY
{
	T x;
	T y;
};

template <class T>
struct CStructSize
{
	T Width;
	T Height;
};


template <class T>
struct CStructVertHorz
{
	T Vertical;
	T Horizontal;
};

template <class T>
struct CStructLimits
{
	T Min;
	T Max;
};



template <class T>
struct CStructXYZ
{
	T x;
	T y;
	T z;
};

template <class T>
struct CStructStatisticValues{
	CStructXY<T> min;
	CStructXY<T> max;
	CStructXY<T> mean;
};


struct CPointDouble
{
	double x;
	double y;
};
