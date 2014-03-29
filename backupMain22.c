#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h> 
#include "m_general.h"
#include "m_bus.h"
#include "m_usb.h"
#include "m_imu.h"

#define 	PI		3.14159265
#define 	SYSCLK		16000000
#define 	CLKDIV 		0   		
#define 	ACC_SCALE	1 		
#define 	GYRO_SCALE	1

bool newPacket;
int data[9] = {0,0,0,0,0,0,0,0,0};

void setupVals(void);
void setup_timer(void);
bool setup_IMU(int, int );
void loop(void);
void runTest(void);
void update_measurements(void );
void setup_usb(void);
void print_vals(void );

int main(void) {
	setup_usb();
	m_bus_init();
	setupVals();
	setup_timer();
	while(1){
		loop();	
	} return 0;   /* never reached */
}

void loop(){
	if ( m_imu_raw( data ) ){
		update_measurements();
	}
	print_vals();
}

void setupVals(){
	m_clockdivide( CLKDIV );
}

void  setup_timer(){
	clear(TCCR1B,CS12); clear(TCCR1B,CS11); set(TCCR1B, CS10);			// Setting up clock divider		
	set(TCCR1B,WGM13); set(TCCR1B,WGM12); set(TCCR1A,WGM11); set(TCCR1A,WGM10); 	// Set mode15 on timer 1
	set(DDRB,6); 			 						// Controll B6 with the timer
	set(TCCR1A,COM1B1); set(TCCR1A,COM1B0);						// Match b/w TCNT1 and OCR1B clears OCR1B, set at rollover
	set(TIMSK1,OCIE1A);								// call an interupt when timer 1 rolls over
}


void setup_usb(){
	m_usb_init();
	m_green(on);
	m_wait(100);
	while (!m_usb_isconnected());
}

bool setup_IMU( int AccelScale, int GyroScale){
	if (  (0 <= AccelScale)  &&  (AccelScale <= 3)  && (0 <= GyroScale) && (GyroScale <= 3)  ){
		return  m_imu_init( AccelScale , GyroScale );
	} else return false;
}

void update_measurements() {
	m_imu_raw( data );
}

void print_vals(){
	m_wait(10000);
	m_imu_raw(data);
	m_usb_tx_int(data[0]);
	m_usb_tx_char('\t');
	m_usb_tx_int(data[1]);
	m_usb_tx_char('\t');
	m_usb_tx_int(data[2]);
	m_usb_tx_char('\t');
	m_usb_tx_int(data[3]);
	m_usb_tx_char('\t');
	m_usb_tx_int(data[4]);
	m_usb_tx_char('\t');
	m_usb_tx_int(data[5]);
	m_usb_tx_char('\r');
}

ISR( TIMER1_COMPA_vect ){ 
}
