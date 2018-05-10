#ifndef QCAMERASMXSETTINGS_H
#define QCAMERASMXSETTINGS_H

class QCameraSMXSettings
{
public:
        QCameraSMXSettings(void)
        {
                setBrightness(0);
                setGamma(0);
                setContrast(0);
                setBayerAlg(0);
                setUse10Bits(false);
        }
        ~QCameraSMXSettings(void)	{	}


private:
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

public:

        void setFrequency(const int value){
                m_Frequency = value;
                //if (value == 3){ m_Frequency = 9; }
                //if 	(value == 6) { m_Frequency = 8; }
                //if 	(value == 8) { m_Frequency = 7; }
                //if 	(value == 10) { m_Frequency = 6; }
                //if 	(value == 12) { m_Frequency = 5; }
                //if 	(value == 13) { m_Frequency = 4; }
                //if 	(value == 16) { m_Frequency = 3; }
                //if 	(value == 20) { m_Frequency = 2; }
                //if 	(value == 24) { m_Frequency = 1; }
                //if 	(value == 40) { m_Frequency = 0; }
                //if 	(value == 48) { m_Frequency = 10; }
        }

        void setSnapshotExposureLimits(const int minvalue, const int maxvalue)
        {
                m_SnapshotExposureLimits.Min = minvalue;
                m_SnapshotExposureLimits.Max = maxvalue;
        }
        void setSnapshotExposureLimitsMs(const float minvalue, const float maxvalue)
        {
                m_SnapshotExposureLimitsMs.Min = minvalue;
                m_SnapshotExposureLimitsMs.Max = maxvalue;
        }

        void setExposureLimits(const int minvalue, const int maxvalue)
        {
                m_ExposureLimits.Min = minvalue;
                m_ExposureLimits.Max = maxvalue;
        }
        void setExposureLimitsMs(const float minvalue, const float maxvalue)
        {
                m_ExposureLimitsMs.Min = minvalue;
                m_ExposureLimitsMs.Max = maxvalue;
        }
        void setSnapshotExposureMs(const float value)
        {
                if (checkMinMax(value, m_SnapshotExposureLimitsMs.Min, m_SnapshotExposureLimitsMs.Max))
                {
                        m_SnapshotExposureMs  = value;
                }
        }
        void setSnapshotExposure(const int value)
        {
                if (checkMinMax(value, m_SnapshotExposureLimits.Min, m_SnapshotExposureLimits.Max))
                {
                        m_SnapshotExposure = value;
                }
        }
        void setExposure(const int value)
        {
                if (checkMinMax(value, m_ExposureLimits.Min, m_ExposureLimits.Max))
                {
                        m_Exposure = value;
                }
        }
        void setExposureMs(const float value)
        {
                if (checkMinMax(value, m_ExposureLimitsMs.Min, m_ExposureLimitsMs.Max))
                {
                        m_ExposureMs = value;
                }
        }
        void setSnapshotExposureMultiplier(const int value)
        {
                if ((value >= 0) && (value < 4))
                {
                        m_SnapshotExposureMultiplier = value;
                }
        }
        void setPictureMirroring(const bool vertvalue, const bool horzvalue)
        {
                m_PictureMirroring.Vertical = vertvalue;
                m_PictureMirroring.Horizontal = horzvalue;
        }
        void setDecimation(const int value)
        {
                if ((value == 1) || (value == 2))
                {
                        m_Decimation = value;
                }
        }
        void setCameraFrameSize(const int widthvalue, const int heightvalue)
        {
                m_CameraFrameSize.Width = widthvalue;
                m_CameraFrameSize.Height = heightvalue;
        }
        void setViewPort(const QRect value){ m_ViewPort = value; }
        void setSensortype(const int value){ m_Sensortype = value; }
        void setColorDepth(const int value)
        {
                if ((value == 8) || (value == 24))
                {
                        m_ColorDepth = value;
                }
        }
        void setUse10Bits(const bool value){ m_Use10Bits = value; }
        void setBayerAlg(const int value){ m_BayerAlg = value;}

        /* ////////////// Get ////////////// */
        StructLimits<int>	 getSnapshotExposureLimits()   { return m_SnapshotExposureLimits; 	}
        StructLimits<float> getSnapshotExposureLimitsMs() { return m_SnapshotExposureLimitsMs; 	}
        StructLimits<int>	 getExposureLimits() 		   { return m_ExposureLimits; 	}
        StructLimits<float> getExposureLimitsMs()		   { return m_ExposureLimitsMs; }
        float getSnapshotExposureMs() { return m_SnapshotExposureMs; }
        int   getSnapshotExposure()   { return m_SnapshotExposure; }
        int   getExposure()	  { return m_Exposure; }
        float getExposureMs() { return m_ExposureMs; }
        int getSnapshotExposureMultiplier(){ return m_SnapshotExposureMultiplier; }
        StructVertHorz<BOOLEAN> getPictureMirroring() { return m_PictureMirroring; }
        int getDecimation(){ return m_Decimation; }

        StructSize<int> getCameraFrameSize(){ return m_CameraFrameSize; }
        QRect getViewPort(){ return m_ViewPort; }
        int getSensortype(){ return m_Sensortype; }
        int getColorDepth(){ return m_ColorDepth; }
        bool getUse10Bits(){ return m_Use10Bits; }
        int getBayerAlg(){ return m_BayerAlg;}
private:
        int m_Frequency;
        int m_Gain;
        float m_SnapshotExposureMs;
        int  m_SnapshotExposure;
        float m_ExposureMs;
        int m_Exposure;
        StructLimits<int> m_SnapshotExposureLimits;
        StructLimits<float> m_SnapshotExposureLimitsMs;
        StructLimits<int> m_ExposureLimits;
        StructLimits<float> m_ExposureLimitsMs;
        int m_SnapshotExposureMultiplier;
        StructVertHorz<BOOLEAN> m_PictureMirroring;
        int m_Decimation; // 1 or 2
        int m_Brightness;
        int m_Gamma;
        int m_Contrast;
        char m_DeviceName[64];
        StructSize<int> m_CameraFrameSize;
        QRect m_ViewPort;
        int m_Sensortype; // 0 black, 1 color
        int m_ColorDepth;
        bool m_Use10Bits;
        int m_BayerAlg;
};

#endif // QCAMERASMXSETTINGS_H
