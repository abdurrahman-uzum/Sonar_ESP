#pragma once

class Sensor_HCSR04
{
    public:
        Sensor_HCSR04( int pin_TRIG, int pin_ECHO );

        void init();
        int measure();

    private:
        const int pin_TRIG;
        const int pin_ECHO;

};