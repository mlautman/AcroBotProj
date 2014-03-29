/*
 * Wireless.c
 *
 * Created: 2012/10/14 9:36:47
 *  Author: Genius
 */ 


#include "m_general.h"
//#include "m_bus.h"
//#include "m_rf.h"
#include "m_usb.h"
#include "wirelessX.h"

//#define packLength 8
//#define Channel 1
//#define Address 0x1D	//17
//#define TXaddress 0x17	//3e

//volatile char Message[packLength]={0};
volatile int ConvertFinishFlag=0;
volatile char buffer[packLength]={0};
	
//int char2num(char *buffer){
	//int i, num=0;
	//for (i=0;i<packLength;i++)
	//{
		//num = num*10+(buffer[i]-'0');
	//}
	//return num;
//}
//void LEDBlink(int num){
	//int i=0;
	//while(i<num){
		//m_red(ON);
		//m_wait(1000);
		//m_red(OFF);
		//m_wait(1000);
		//i++;
	//}
	//
//}

int main(void)
{
	char value[packLength]={0};
	int m;
	//int number, numberHis;
	//int blinkFlag;
	//char SendMessage[packLength]="007";
	m_clockdivide(0);
	wirelessinitial();
	//m_bus_init();
	m_usb_init();
	//m_rf_open(Channel,Address,packLength);
	while(!m_usb_isconnected()); // wait for a connection
	sei();
    while(1)
    {
        //TODO:: Please write your application code 
		//m_rf_send(TXaddress, SendMessage, packLength);
		if (m_usb_rx_available())
		{
			memset(value,0,packLength);
			m=0;
			while(m_usb_rx_available())
			{
				if (m<packLength)
				{
					value[m] = m_usb_rx_char();
					m++;
				}else{
					break;
				}
			}
			m_usb_tx_char('O');
			m_usb_tx_char('K');
			m_usb_tx_char('\n');
			m_wait(100);
			wireless_string(value,packLength);
		}
		//int success;
		//success = m_rf_read(buffer, packLength);
		//if ((success==1)&&(buffer[0]!=0))
		//{
			//m_green(ON);
			//ConvertFinishFlag=1;
		//}else
		//{
			//m_green(OFF);
		//}
		//m_wait(5);
					
		if (ConvertFinishFlag==1)
		{
			//number = char2num(Message);
			for (int i=0;i<packLength;i++)
			{
				if (buffer[i])
				{
					m_usb_tx_char(buffer[i]);
				}
			}
			//m_usb_tx_char(buffer);
			ConvertFinishFlag=0;
			//blinkFlag=1;
		}
		//if ((blinkFlag==1)&&(number!=numberHis))
		//{
			//LEDBlink(number);
			//numberHis = number;
			//blinkFlag=0;
		//}else{
			//blinkFlag=0;
		//}
    }
}
ISR(INT2_vect )
{
	int success;
	//success = m_rf_read(Message, packLength);
	success = m_rf_read(buffer, packLength);
	if ((success==1)&&(buffer[0]!=0))
	{
		m_green(ON);
		ConvertFinishFlag=1;
	}else{
		m_green(OFF);
	}
}
