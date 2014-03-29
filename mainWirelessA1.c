/*
 * Wireless.c
 *
 * Created: 2012/10/14 9:36:47
 *  Author: Genius
 */ 


#include "m_general.h"
#include "m_usb.h"
#include "wireless.h"

volatile int ConvertFinishFlag=0;
volatile char buffer[packLength]={0};
void transmitBuff(char* , int );
	
int main(void)
{
	char value[packLength]={0};
	int m;
	char buff;
	m_clockdivide(0);
	wirelessinitial();
	m_usb_init();
	m_green(ON);
	while(!m_usb_isconnected()); // wait for a connection
	m_green(OFF);
	sei();
	m=0;
    	while(1)
    	{
		if (m_usb_rx_available()) {
			buff = m_usb_rx_char();
			if (buff=='\n'){
				transmitBuff(value ,m);	
			} else {
				value[m]=buff;
				m++;
				if (m==packLength-1){
					transmitBuff(value ,packLength);
					m=0;
				}
			}
		}		

		if (ConvertFinishFlag==1) {
			for (int i = 0 ; i < packLength;i++){
				m_usb_tx_char(buffer[i]);
			}
			ConvertFinishFlag=0;
		}
    }
}

void transmitBuff(char* a, int num){
	for (int i = 0 ; i < packLength;i++){
		m_usb_tx_char(buffer[i]);
	}
}

ISR(INT2_vect )
{
	int success;
	success = m_rf_read(buffer, packLength);
	if ((success==1)&&(buffer[0]!=0))
	{
		m_green(ON);
		ConvertFinishFlag=1;
	}else{
		m_green(OFF);
	}
}
