#ifndef INTERNALTEMPSENSOR_H
#define INTERNALTEMPSENSOR_H

#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7B8))
#define VDD_CALIB ((uint16_t)(330))
#define VDD_APPLI ((uint16_t)(300))

class InternalTempSensorClass
{
public:
    int32_t GetTemperature(int calibration = 30);
    InternalTempSensorClass();
    ~InternalTempSensorClass();

private:
    int32_t ComputeTemperature(uint32_t measure);
    void InitializeADC(void);
    void CalibrateADC(void);
    void ConfigureADC(void);
    void EnableADC(void);
    void DisableADC(void);
};

extern InternalTempSensorClass internalTemp;

#endif
