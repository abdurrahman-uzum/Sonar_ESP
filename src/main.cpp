#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include "HCSR04.h"
#include "sonar.h"
#include "car.h"


#define BUFFER_SIZE 8

WiFiServer server(80);

Sensor_HCSR04 front_sensor( 23, 35 );
Sensor_HCSR04 rear_sensor( 22, 34 );

Sonar sonar( &front_sensor, &rear_sensor );
Car car;


void parseInputBuffer( const char* buffer, uint8_t& keystroke, uint8_t& speed, uint8_t& servo_posEnforced )
{
	char holder[9];
	strncpy( holder, buffer, 8  );
	holder[8] = '\0';

	uint32_t input = strtoull( holder, nullptr, 16 );

	keystroke = input & 0x000000FF;
	input >>= 8;

	speed = input & 0x000000FF;
	input >>= 8;

	servo_posEnforced = input & 0x000000FF;
}



void setup()
{
	Serial.begin(115200);

	sonar.init();
	car.init();

	WiFi.softAP( "ESP32", "donotconnectthisplease");
	IPAddress myIP = WiFi.softAPIP();
	server.begin();

	Serial.println("Server started");

	
}



void loop()
{
	char output_buffer[BUFFER_SIZE] = { 0 };
	char input_buffer [BUFFER_SIZE] = { 0 };

	uint8_t keystroke			= 0;
	uint8_t speed				= 0;
	uint8_t movement_enforced	= 0;

	int front_distance	= 0;
	int rear_distance	= 0;
	int angle			= 0;


	WiFiClient radio = server.available();  //listen for incoming clients

	if ( radio )
	{								   
		while ( radio.connected() )
		{ 	
			delay(10); 

			if ( radio.available() )
			{
				/// RECEIVE AND PARSE INPUT ///
				radio.read( (uint8_t*)input_buffer, BUFFER_SIZE );				
							
				parseInputBuffer( input_buffer, keystroke, speed, movement_enforced );				
				car.set( keystroke, speed );				
				sonar.setMode( movement_enforced );


				/// UPDATE ///
				sonar.scan( front_distance, rear_distance, angle );
				car.move();		

				Serial.printf( "Front: %d,  Rear: %d,  Angle: %d\n", front_distance, rear_distance, angle );
				Serial.printf( "                                       Keystroke: %x   Speed: %x   Angle lock: %x\n", keystroke, speed, movement_enforced );


				/// SET AND SEND OUTPUT ///
				sonar.write( output_buffer );
				radio.write( output_buffer, 8 ); 


				/// CLEAR BUFFERS ///
				memset( input_buffer, 0, BUFFER_SIZE );
				memset( output_buffer, 0, BUFFER_SIZE );						
			}	
		}
		//Close the connection:
		radio.stop();
		Serial.println("Client Disconnected.");
	}
} 






