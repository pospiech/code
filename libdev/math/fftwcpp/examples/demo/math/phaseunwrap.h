#ifndef PHASEUNWRAP_H
#define PHASEUNWRAP_H

#include <vector>

void removePhaseFlipping(std::vector<double> & dataPhase)
{
    // set almost zero value zero
    for (size_t i=0; i < (dataPhase.size()); ++i)
    {
        if (abs(dataPhase[i]) < 1e-12){
            dataPhase[i] = 0;
        }
        if (dataPhase[i] >= (pi - 1e-12)){
            //qDebug() << "Nr: " << i << " " << dataPhase[i]-pi ;
            dataPhase[i] = -pi;
        }
    }
    return;
    // bypass this code
    vector<double> dataPhaseNoFlipping = dataPhase;
    // remove flipping phase jumps
    bool dataChanged = true;
    size_t iteration = 0;
    while (dataChanged && (iteration < 10)) {
        iteration++;
        dataChanged = false;
        for (size_t i=0; i < (dataPhase.size()-2); ++i)
        {
            // diff > 2pi
            if ( abs(dataPhase[i+1] - dataPhase[i]) >= 2*pi ) {
                // flippung up + down
                // if size big enough for 3 point checking
                if ( (i+1) < dataPhase.size() ) {
                    if (dataPhase[i] < 0 )  {
                        if ((dataPhase[i+1] > 0 ) && (dataPhase[i+2] <= pi )) {
                            // data: -pi,  pi,  -pi -> -pi -pi -pi
                            dataPhaseNoFlipping[i+1] = -pi;
                            qDebug() << "Nr: " << i << " " << dataPhase[i]/pi << ", " << dataPhase[i+1]/pi << ", " << dataPhase[i+2]/pi;
                            ++i;
                            dataChanged = true;
                        }
                    } else if (dataPhase[i] > 0 )  {
                        if ((dataPhase[i+1] < 0 ) && (dataPhase[i+2] >= pi )) {
                            // data: +pi,  -pi,  -pi -> +pi +pi +pi
                            dataPhaseNoFlipping[i+1] = +pi;
                            qDebug() << "Nr: " << i << " " << dataPhase[i]/pi << ", " << dataPhase[i+1]/pi << ", " << dataPhase[i+2]/pi;
                            ++i;
                            dataChanged = true;
                        }
                    }
                }
            }
        }
        dataPhase = dataPhaseNoFlipping;
    }
    // remove remaining 2pi changes    
    dataChanged = true;
    iteration = 0;
    while (dataChanged && (iteration < 10) ) {
        iteration++;
        dataChanged = false;
        for (size_t i=0; i < (dataPhase.size()-1); ++i)
        {
//            qDebug() << "Nr: " << i << " " << abs(dataPhase[i+1] - dataPhase[i])/(1.0*pi) ;
            // diff > 2pi
            if ( abs(dataPhase[i+1] - dataPhase[i]) >= 2.0*pi ) {
                qDebug() << "Nr: " << i << " " << dataPhase[i]/pi << ", " << dataPhase[i+1]/pi ;
                // 2pi jump up or down
                if (dataPhase[i] < 0 )  {
                    dataPhaseNoFlipping[i+1] = -pi;
                    ++i;
                    dataChanged = true;
                }
                else if (dataPhase[i] > 0 )  {
                    dataPhaseNoFlipping[i+1] = +pi;
                    ++i;
                    dataChanged = true;
                }
            }
        }
        dataPhase = dataPhaseNoFlipping;
    }

}

#endif // PHASEUNWRAP_H
