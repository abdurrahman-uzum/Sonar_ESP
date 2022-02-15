#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <stepper.h>
#include "HCSR04.h"

enum class Scanmode { FREE, HALT, CCW, CW, ERROR };

class Sonar
{
    public:
        Sonar( Sensor_HCSR04* front, Sensor_HCSR04* rear );
        
        void init();
        void setMode( uint8_t scan_enforced );
        void scan( int& front_distance, int& rear_distance, int& angle );
        void write( char* buffer );

    private:
        void freeScan();
        void enforcedScan();

    private:
        Sensor_HCSR04*  m_front_sensor;
        Sensor_HCSR04*  m_rear_sensor;
        Servo           m_servo;
        Scanmode        m_mode;
        int             m_front_distance;
        int             m_rear_distance;
        int             m_angle;    
        int             m_direction;

};