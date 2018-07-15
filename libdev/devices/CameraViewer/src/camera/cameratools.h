#ifndef CAMERATOOLS_H
#define CAMERATOOLS_H

struct Limits {
    Limits(): min(0), max(0), intervall(0) {}
    Limits(double nmin, double nmax, double nintervall=0)
        : min(nmin)
        , max(nmax)
        , intervall(nintervall) {}
    double min;
    double max;
    double intervall;
};


#endif // CAMERATOOLS_H
