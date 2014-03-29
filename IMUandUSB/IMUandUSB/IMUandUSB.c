/*
 * IMUandUSB.c
 *
 * Created: 10/24/2012 1:09:13 PM
 *  Author: yunkai
 */ 


//#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"
#include "wireless.h"
#include "Balance_Kalman.h"

#define MAXI 32767
#define PERIOD 5 //ms
#define PERIOD2 100 //ms
#define PERIOD3 10 //ms
#define INTERRUPT10MS PERIOD3/PERIOD-1
#define INTERRUPT1S PERIOD2/PERIOD-1
#define ACOFFSET 42
#define GYOFFSET -54

volatile int Timer3Flag=0;
volatile int Timer3Flag2=0;
volatile int Timer3Flag3=0;

void TimerSetup(void){
	OCR1A = 4000;
	OCR1B = 2000;
	OCR1C = 2000;
	set(DDRB,6);
	set(DDRB,7);
	TCCR1A = 0b00101011;
	TCCR1B = 0b00011001;

	//Timer1 is used to generate the PWM
	OCR3A = (int)(15625*((float)PERIOD/1000));		//set frequency, f=1/((OCR1A+1)*(1/CLK));
	//OCR1B = (int)(COUNTER/2);		//set duty cycle, which can be calculate by (OCR1B+1)/(OCR1A+1)
	TCCR3A = 0b00000000;//0x23; //set mode to (mode 15) UP to OCR1A, PWM mode, operating at clearing at OCR1B, seting at rollover
	TCCR3B = 0b00001101;//0x19;//clock source /1024
	set(TIMSK3 , OCIE3A);
}	

int main(void)
{
	int Data[9]={0};
	int CalData[9] ={0};
	int RealAngle[3] = {0}; 
	int tmp=0;
	int output;
	int angleHis1=0,angleHis2=0;
	int inputK=0,inputI=0,inputD=0;
	int Output=0;
	int OutputHis=2000;
	m_clockdivide(0);
	wirelessinitial();	
	TimerSetup();
	m_usb_init();
	while(!m_imu_init(1,0));
	sei();
	while(!m_usb_isconnected()); // wait for a connection
	//m_green(ON);
    while(1)
    {
		if (Timer3Flag==1)
		{
			if(m_imu_raw(Data))
			{
				m_red(TOGGLE);
			}
			
			Kalman_Filter(1.5*(Data[1]-ACOFFSET),0.5*(Data[3]-GYOFFSET));	//Using the Kalman Filter to get the reliable output of the angle

			float Kp = 1;
			float Ki = 0;
			float Kd = 0.5;
			inputK = angle-angleHis1;
			inputI = angle;
			inputD = angle +angleHis2-2*angleHis1;
			
			Output += Kp*inputK + Ki*inputI + Kd*inputD;	//incremental PID
			//OutputHis +=Output;
			OutputHis =2000+Output;
			if (OutputHis>4000)		//Detect the limitation of the output value
			{
				OutputHis=4000;
			}else{
				if (OutputHis<3)
				{
					OutputHis=3;
				}
			}
			OCR1B = OutputHis;
			OCR1C = OutputHis;
			
			angleHis2 = angleHis1;
			angleHis1 = angle;
			
			Timer3Flag=0;
		}
		if (Timer3Flag2==INTERRUPT1S)
		{
			tmp = angle;
			m_green(TOGGLE);
			m_usb_tx_int(tmp);
			m_usb_tx_char('\t');
			m_usb_tx_int(Data[1]);
			m_usb_tx_char('\n');
			Timer3Flag2=0;
		}
    }
}
ISR(TIMER3_COMPA_vect) //ms interrupt
{
	Timer3Flag = 1;
	if (Timer3Flag3<INTERRUPT10MS)
	{
		Timer3Flag3++;
	}
	if (Timer3Flag2<INTERRUPT1S)
	{
		Timer3Flag2++;
	}
}	
