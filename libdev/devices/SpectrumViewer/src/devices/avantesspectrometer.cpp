#include "avantesspectrometer.h"
#include "avaspec.h"

#include "Logger.h"

#include <QApplication>
#include "applicationcallbacks.h"

AvantesSpectrometer::AvantesSpectrometer(QObject *parent) : QObject(parent)
{
//    QObject::connect(qApp, &ApplicationCallbacks::SpectrumAvailable,
//                      this, &AvantesSpectrometer::processSpectrumAvailable);

    connect(qApp, SIGNAL(SpectrumAvailable(int, int)), this, SLOT(processSpectrumAvailable(int, int)));
    connect(qApp, SIGNAL(ConnStatus(int, int)), this, SLOT(ReceiveConnStatus(int, int)));

}

void AvantesSpectrometer::processSpectrumAvailable(int WParam, int LParam)
{
    unsigned char l_saturated[MAX_NR_PIXELS];
    int	l_Ticks;
    unsigned int l_Dif;
    QString l_Line;
    double l_AvgScantimeRAM;
    int l_Result = ERR_SUCCESS;

    if (WParam < ERR_SUCCESS) // WParam < 0 indicates an error
    {
        LOG_ERROR() << "Meas.Status: failed. Error " << WParam;
        return;
    }

    if ((LParam != m_DeviceHandle) || not(m_NrPixels > 0))
    {
        LOG_ERROR() << "Invalid Parameter";
        return;
    }
    if (WParam == ERR_SUCCESS) // normal measurements
    {
        m_Measurements++;
        if (ERR_SUCCESS == AVS_GetScopeData(m_DeviceHandle,&l_Time,l_pSpectrum))
        {
            // ui.plot->update_plot();
            if (ERR_SUCCESS == AVS_GetSaturatedPixels(m_DeviceHandle,l_saturated))
            {
                bool saturated = false;
                for (int j = 0; j <= m_StopPixel-m_StartPixel; j++) {
                    saturated = saturated || (l_saturated[j] != 0);
                }
                ui.SaturatedChk->setChecked(saturated);
            }
            l_Dif = l_Time - m_PreviousTimeStamp;  // l_Time in 10 us ticks
            m_PreviousTimeStamp = l_Time;
            if (l_Dif != 0)
            {
                LOG_INFO << "Last scan time / ms: " << QString("%1").arg(l_Dif/100.0,7,'f',2);
            }
        }
        else
        {
            LOG_INFO() << Meas.Status: no data available;
        }
        l_Ticks = m_Time.elapsed();
        //                ui.TimeSinceStartEdt->setText(QString("%1").arg(l_Ticks/1000));
        //                ui.NrScansEdt->setText(QString("%1").arg(m_Measurements));
        double timeperscan = double(l_Ticks) / double(m_Measurements);
        //                ui.TimePerScanEdt->setText(QString("%1").arg(timeperscan,7,'f',2));
        //                if (ui.FixedNrRBtn->isChecked())
        //                    ui.StartMeasBtn->setEnabled(QString(ui.NrMeasEdt->text()).toUInt() == m_Measurements);
        //                if (ui.RepetitiveRBtn->isChecked() && !ui.StartMeasBtn->isEnabled())
        //                    on_StartMeasBtn_clicked();
    }
    else	// StoreToRam measurements
    {
        l_AvgScantimeRAM = 0.0;
        LOG_INFO() << "Meas.Status: Reading RAM";
        for (int j = 1; j <= WParam; j++)
        {
            l_Result = AVS_GetScopeData(m_DeviceHandle, &l_Time, l_pSpectrum);
            if (ERR_SUCCESS == l_Result)
            {
                ui.plot->update_plot();
                l_Dif = l_Time - m_PreviousTimeStamp;  //l_Time in 10 us ticks
                m_PreviousTimeStamp = l_Time;
                if (l_Dif != 0)
                {
                    LOG_INFO() << "Time of last Scan / ms: " << QString("%1").arg(l_Dif/100.0,7,'f',2);
                }
                if (j>1)
                {
                    l_AvgScantimeRAM = (l_AvgScantimeRAM*(j-2)+l_Dif/100.0) / (j-1);
                    LOG_INFO() << "Time per Scan / ms: " << QString("%1").arg(l_AvgScantimeRAM,7,'f',3);
                }
            }
            else
            {
                LOG_ERROR() << "Meas.Status: No Data Available";
                break;
            }
            l_Ticks = m_Time.elapsed();
            LOG_INFO() << "Time since Start: " << l_Ticks/1000;
            LOG_INFO() << "Number of Scans: " << j;
        }
        if (l_Result == ERR_SUCCESS)
        {
            LOG_INFO() << "Meas.Status: Finished Reading RAM";
        }
    }

}

//-------------------------------------------------------------------------------------------
void AvantesSpectrometer::processConnStatus(int WParam, int LParam)
{
    extern long m_DeviceHandle;

    if (LParam == m_DeviceHandle){
        switch (WParam) {
            case ETH_CONN_STATUS_CONNECTING:
            {
                for (int i = 0; i < SPECTR_LIST_COLUMN_COUNT; i++) {
                    if (ui.SpectrometerList->item(mSelectedDevRow, i) != NULL)
                        ui.SpectrometerList->item(mSelectedDevRow, i)->setBackground(Qt::yellow);
                }
            }
            break;
            case ETH_CONN_STATUS_CONNECTED:
            {
                for (int i = 0; i < SPECTR_LIST_COLUMN_COUNT; i++) {
                    if (ui.SpectrometerList->item(mSelectedDevRow, i) != NULL)
                        ui.SpectrometerList->item(mSelectedDevRow, i)->setBackground(Qt::green);
                }
            }
            break;
            case ETH_CONN_STATUS_CONNECTED_NOMON:
            {
                for (int i = 0; i < SPECTR_LIST_COLUMN_COUNT; i++) {
                    if (ui.SpectrometerList->item(mSelectedDevRow, i) != NULL)
                        ui.SpectrometerList->item(mSelectedDevRow, i)->setBackground(Qt::lightGray);
                }
            }
            break;
            case ETH_CONN_STATUS_NOCONNECTION:
            {
                for (int i = 0; i < SPECTR_LIST_COLUMN_COUNT; i++) {
                    if (ui.SpectrometerList->item(mSelectedDevRow, i) != NULL)
                        ui.SpectrometerList->item(mSelectedDevRow, i)->setBackground(Qt::red);
                }
            }
            break;
            default:
                statusBar()->showMessage(tr("Unknown connection status received: %1").arg(WParam));
                break;
        }
    }
}
