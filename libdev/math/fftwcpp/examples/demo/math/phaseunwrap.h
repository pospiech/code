#ifndef PHASEUNWRAP_H
#define PHASEUNWRAP_H

#include <vector>

void removePhaseFlipping(std::vector<double> & dataPhase)
{
    for (size_t i=0; i < (dataPhase.size()); ++i)
    {
        // -pi -> +pi -> -pi
        const double thresh = (2*pi - 2e-12);
        for (size_t i=0; i < (dataPhase.size()-1); ++i)
        {
            if ((dataPhase[i+1] - dataPhase[i]) > thresh)
            {
                dataPhase[i+1] = -dataPhase[i+1];
            }
        }
        // +pi -> -pi -> -pi
        for (size_t i=0; i < (dataPhase.size()-1); ++i)
        {
            if ((dataPhase[i] - dataPhase[i+1]) > thresh)
            {
                dataPhase[i] = -dataPhase[i];
            }
        }
    }
}

#endif // PHASEUNWRAP_H
