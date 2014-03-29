#include <avr/io.h>
#include "m_imu.h"
#include "m_bus.h"
#include "m_usb.h"
#include "m_general.h"
#include "m_rf.h"

#define  accel_scale  	0
#define  gyro_scale 		0

#define TXADDRESS			33
#define RXADDRESS			32
#define PACKET_LENGTH 	14
#define CHANNEL			1

void m_rf_init( void );

char packet[PACKET_LENGTH]	=	{0};

volatile bool newPacket = false;

int main(void){
	m_clockdivide(0);
	int data[9] = {0,0,0,0,0,0,0,0,0};
	m_green(ON);
	m_bus_init();
	m_usb_init();
	while(!m_usb_isconnected());
	m_green(OFF);
	m_imu_init(accel_scale, gyro_scale);
	m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH);

	while(1) {
		while	( !newPacket );
		int i,a;
		while (i=0; i<PACKET_LENGTH , i+=2 ){
			a = *(int*)&packet[i*2+2];
			m_usb_tx_int(a);
			m_usb_tx_char('\t');
		}
		m_usb_tx_char('\n');
		m_usb_tx_char('\r');
	}
}

void m_rf_init(){
	m_rf_open(CHANNEL,RXADDRESS,PACKET_LENGTH);
}



ISR( INT2_vect) {
	m_green(2);
	m_rf_read(packet, PACKET_LENGTH);
	newPacket=true;
}
/*
	send_buffer[2]= *(char*)&VL;
	send_buffer[3]= *( ((char*)&VL) + 1);
	a = *(int*)&receive_buffer[i*2+2];
	*/ 
