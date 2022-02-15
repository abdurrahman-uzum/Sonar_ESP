#include "car.h"
#include <Arduino.h>

Car::Car()
{
    m_speed = 0;
    m_turnOffset = 0;
    m_direction = Direction::HALT;
}



void Car::init()
{
    pinMode( P_LEFT_F, OUTPUT );
    pinMode( P_LEFT_F, OUTPUT );
    pinMode( P_RIGHT_F, OUTPUT );
    pinMode( P_RIGHT_B, OUTPUT );

    ledcSetup( LEFT_F, 490, 8 );
    ledcSetup( LEFT_B, 490, 8 );
    ledcSetup( RIGHT_F, 490, 8 );
    ledcSetup( RIGHT_B, 490, 8 );

    ledcAttachPin( P_LEFT_F, LEFT_F );
    ledcAttachPin( P_LEFT_B, LEFT_B );
    ledcAttachPin( P_RIGHT_F, RIGHT_F );
    ledcAttachPin( P_RIGHT_B, RIGHT_B );
}



void Car::move()
{
    Serial.printf( "offset: %d speed: %d ", m_turnOffset, m_speed );
    if ( m_direction == Direction::FORWARD )
    {
        ledcWrite( LEFT_F,  m_speed - m_turnOffset * (m_turnOffset > 0) );
        ledcWrite( RIGHT_F, m_speed - m_turnOffset * (m_turnOffset < 0) );

        Serial.printf( "FORWARD\n");
    }
    else if ( m_direction == Direction::BACKWARD )
    {
        ledcWrite( LEFT_B,  m_speed - m_turnOffset * (m_turnOffset > 0) );
        ledcWrite( RIGHT_B, m_speed - m_turnOffset * (m_turnOffset < 0) );

        Serial.printf( "BACKWARD\n" );
    }
    else
    {
        ledcWrite( LEFT_F,  0 );
        ledcWrite( LEFT_B,  0 );
        ledcWrite( RIGHT_F, 0 );
        ledcWrite( RIGHT_B, 0 );

        Serial.printf( "HALT\n" );
    }
    
}



void Car::set( uint8_t keystroke, uint8_t speed )
{
    if      ( keystroke & 0b00000001 ) { m_direction = Direction::FORWARD; }
    else if ( keystroke & 0b00000010 ) { m_direction = Direction::BACKWARD; }
    else                               { m_direction = Direction::HALT; }

    m_speed = speed;

    if      ( keystroke & 0b00000100 ) { m_turnOffset =  0.5 * speed; }  //Left turn
    else if ( keystroke & 0b00001000 ) { m_turnOffset = -0.5 * speed; }  //Right turn
    else                               { m_turnOffset =  0; }            //Straight
}