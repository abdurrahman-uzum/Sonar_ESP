#include <Arduino.h>
#include "HCSR04.h"

Sensor_HCSR04::Sensor_HCSR04( int pin_TRIG, int pin_ECHO ) : pin_TRIG(pin_TRIG), pin_ECHO(pin_ECHO){}

int Sensor_HCSR04::measure()
{
    long int dur = 0;
    int dist = 0;

	digitalWrite( pin_TRIG, LOW );
	delayMicroseconds( 2 );
	digitalWrite( pin_TRIG, HIGH );

	delayMicroseconds( 10 );
	
	digitalWrite( pin_TRIG, LOW );

	dur = pulseIn( pin_ECHO, HIGH );

    dist = (( dur / 2 ) / 29.1);	

    return (dist > 200)? 200 : dist;
}

void Sensor_HCSR04::init()
{
    pinMode( pin_TRIG, OUTPUT );
    pinMode( pin_ECHO, INPUT );
}