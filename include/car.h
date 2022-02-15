#pragma once
#include <Arduino.h>

enum class Mask
{
    FORWARD     = 0b00000001,
    BACKWARD    = 0b00000010,
    LEFT        = 0b00000100,
    RIGHT       = 0b00001000

};

enum class Direction { FORWARD, BACKWARD, HALT };

class Car
{
    public:
        static const int LEFT_F = 2;
        static const int LEFT_B = 3;
        static const int RIGHT_F = 4;
        static const int RIGHT_B = 5;

        static const int P_LEFT_F = 13;
        static const int P_LEFT_B = 12;
        static const int P_RIGHT_F = 14;
        static const int P_RIGHT_B = 27;


    public:
        Car();

        void init();
        void move();
        void set( uint8_t keystroke, uint8_t speed );

    private:
        int         m_speed;
        int         m_turnOffset;
        Direction   m_direction;
        
    
        






};