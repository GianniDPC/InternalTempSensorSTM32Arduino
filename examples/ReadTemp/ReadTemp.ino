#include "InternalTempSensor.h"

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    Serial.println(internalTemp.GetTemperature(30));
    delay(1000);
}
