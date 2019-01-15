#ifndef AVANTESSPECTROMETER_H
#define AVANTESSPECTROMETER_H

#include <QObject>
#include <QTime>

class AvantesSpectrometer : public QObject
{
    Q_OBJECT
public:
    explicit AvantesSpectrometer(QObject *parent = nullptr);


public slots:
    void processSpectrumAvailable(int WParam, int LParam);
    void processConnStatus(int WParam, int LParam);

private:
    unsigned int l_Time;
    unsigned int m_PreviousTimeStamp;
    long m_DeviceHandle;
    double l_pSpectrum[4096];
    unsigned short m_NrPixels;
    unsigned int m_Measurements;
    unsigned int m_Failures;
    QTime m_Time;
    unsigned short m_StartPixel;
    unsigned short m_StopPixel;



};

#endif // AVANTESSPECTROMETER_H
