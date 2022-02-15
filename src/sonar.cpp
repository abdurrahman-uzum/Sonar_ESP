#include "sonar.h"


Sonar::Sonar( Sensor_HCSR04* front, Sensor_HCSR04* rear )
{
    m_front_sensor = front;
    m_rear_sensor = rear;

    m_front_distance = 0;
    m_rear_distance = 0;
    m_angle = 0;
    m_direction = 1;

}

void Sonar::init()
{
    m_front_sensor->init();
    m_rear_sensor->init();

    m_servo.attach( 18 );
    //m_servo.write( 90 );
}

void Sonar::setMode( uint8_t scan_enforced )
{
    switch ( scan_enforced )
    {
        case 0x00:
            m_mode = Scanmode::FREE;
        break;

        case 0x11: case 0x01: case  0x10:
            m_mode = Scanmode::HALT;
        break;

        case 0x0F: case 0x1F:
            m_mode = Scanmode::CW;
        break;

        case 0xF0: case 0xF1:
            m_mode = Scanmode::CCW;
        break;

        default:
            m_mode = Scanmode::ERROR;
        break;
    }
}

void Sonar::scan( int& front_distance, int& rear_distance, int& angle )
{
    Serial.printf( "-------------------> Servo angle: %d\n", m_angle );
    m_servo.write( m_angle );
    
    m_front_distance = m_front_sensor->measure();
    m_rear_distance = m_rear_sensor->measure();

    front_distance = m_front_distance;
    rear_distance = m_rear_distance;
    angle = m_angle;

    switch ( m_mode )
    {
        case Scanmode::FREE:
            m_angle += m_direction;
            if ( m_angle == 0 || m_angle == 180 ) { m_direction *= -1; }
        break;

        case Scanmode::CCW:
            m_angle++;
            if ( m_angle > 180 ) { m_angle = 180; }
        break;

        case Scanmode::CW:
            m_angle--;
            if ( m_angle < 0 ) { m_angle = 0; }
        break;

        case Scanmode::HALT:
            ;
        break;        

        case Scanmode::ERROR:
            Serial.printf( "ERROR! Scanmode undefined!\n" );
        break;
    }

}

void Sonar::write( char* buffer )
{
    uint32_t output = 0;
	
	output |= m_rear_distance;

	output <<= 8;
	output |= m_front_distance;

	output <<= 8;
	output |= m_angle;

	itoa( output, buffer, 16 );
}