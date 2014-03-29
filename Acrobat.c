/*
 * Acrobat_V1.c
 *
 * Created: 2012/10/27 11:06:36
 *  Author: Edward and Mike L
 */ 
//##########################################################################
// this is the third version of our code, with comments
// this code is modified last by Michael  on 11/02/2012 Wed.
// this code is compiled last on
// Any one who will modify this code please mark all the code added 
// and write something here to make every body know.
// Please describe all the function you added 
// The functions that already exist are:
// 	void TimerSetup(void); initialize the timer
//	void systemInitial(void); initialize some function of the system
// 	int main(void); the main function 
// 	ISR(TIMER3_COMPA_vect) periodical interrupt
//##########################################################################


#include "m_general.h"
#include "m_usb.h"
#include "wireless.h"
#include "Balance_Kalman.h"

/****************************************************************************
                          System Configuration                                 
 ****************************************************************************/

//constant about period interrupt
#define PERIOD 5	//ms period1 which 
#define PERIOD2 100 //ms
#define PERIOD3 10	//ms
#define INTERRUPT10MS PERIOD3/PERIOD-1
#define INTERRUPT1S PERIOD2/PERIOD-1

//constant about dealing output data of the sensor
#define ACOFFSET 42
#define GYOFFSET -30

//parameter about kalman complementary
#define ACCPART 1
#define GYRPART 2-ACCPART

//Initial value of PID
#define KPINI 1000
#define KIINI 0
#define KDINI 5000


/****************************************************************************
                            Global Variables                                  
 ****************************************************************************/

volatile int Timer3Flag=0;
volatile int Timer3Flag2=0;
volatile int Timer3Flag3=0;
volatile int ConvertFinishFlag=0;
volatile char buffer[packLength]={0};
volatile int EncoderCount=0;


/****************************************************************************
 =========================  functions  ================================ 
 ****************************************************************************/
void TimerSetup(void); 					//initialize the timer
void systemInitial(void);				//initialize some function of the syste
int main(void); 						//the main function 
void transmitEdsCode(void);				//Use Ed's terminal interface
//==========================================================================

int main(void) {
	//original data
	int Data[9]={0};
	int RealinputACC = 0;
	int RealinputGyr = 0;

	//variables of flags
	int switchNum = 0;
	int paraDeter = 0;

	//temporary variables for test
	int tmp=0;
	int getValue=0;

	//variables for PID control
	int output;
	int angleHis1=0,angleHis2=0;
	int inputK=0,inputI=0,inputD=0;
	int Output=0;
	int OutputHis1=2000;
	int OutputHis2=2000;
	int KpIni=KPINI,KiIni=KIINI,KdIni=KDINI;
	float Kp = 1;
	float Ki = 0;
	float Kd = 0.5;
	int AngleCali = 0;
	int AngleActual=0;
	
	systemInitial();
	// send back comfirm information to prove that wireless has been connected
	wireless_string(haha,5);
	wireless_char('\n');
	clear(DDRB,0);
	clear(PORTB,0);
	// using wireless to change parameters of PID controller, and if DIP switch 1 is ON, this can be skipped
	if (!check(PINB,0)) {
		transmitEdsCode();

	}
	m_green(OFF);
	// Initializing IMU
	while(!m_imu_init(1,0));
	int AngleHis=0;
	// calibrate the balance angle value, the code here is trying to find out offset of the angle
	for (int numpoint=0;numpoint<499;numpoint++)
	{
		if(m_imu_raw(Data))
		{
			RealinputACC = ACCPART*(Data[1]-ACOFFSET);
			RealinputGyr = GYRPART*(GYOFFSET-Data[3]);
			Kalman_Filter(RealinputACC,RealinputGyr);
		}
		AngleHis+=angle;
	}
	AngleCali = AngleHis/500;
	// Enable timer interrupt and global interrupt
	set(TIMSK3 , OCIE3A);
	set(PCICR,0);
	set(PCMSK0,5);
//	set(PCMSK0,4);
	sei();
    while(1)
    {
		if (Timer3Flag==1){	//Here is the control loop, all the data sample process and output should be here
		
		cli();
			// read data from IMU
			if(m_imu_raw(Data)) {
				m_red(TOGGLE);
			}
			// make the input value get rid of the offset
			RealinputACC = ACCPART*(Data[1]-ACOFFSET);	//The acceleration input without offset
			RealinputGyr = GYRPART*(Data[3]-GYOFFSET);	//The anglar velocity input without offset
			Kalman_Filter(RealinputACC,RealinputGyr);	//Using the Kalman Filter to get the reliable output of the angle

			// read the changed parameter, because the wireless cannot send float, so change it to float here
			Kp = (float)KpIni/1000;
			Ki = (float)KiIni/1000;
			Kd = (float)KdIni/1000;
			// calibrate the angle value by using the balance offset of the angle
			AngleActual = angle-AngleCali;
			inputK = AngleActual-angleHis1;
			inputI +=AngleActual;

			// PID controller, which you will find that Ki always be zero
			Output = Kp*AngleActual + Ki*inputI + Kd*inputK;
			
			// dead region, which means the value in this region won't give the wheels a speed, so remove it from output to make the output speed linear with the input angle
			if (AngleActual>0) {
				OutputHis1 =1823-Output;
			}
			if (AngleActual<0) {
				OutputHis1 =2193-Output;
			}
			if (AngleActual==0) {
				if (angleHis1>0) {
					OutputHis1 =1823;
				}
				if (angleHis1<0) {
					OutputHis1 =2193;
				}
				if (angleHis1=0){
					OutputHis1 =OutputHis1;
				}
			}

			if (AngleActual>0)
				OutputHis2 =1857-Output;

			if (AngleActual<0)
				OutputHis2 =2148-Output;

			if (AngleActual==0) {
				if (angleHis1>0) 
					OutputHis2 =1857;
				if (angleHis1<0)
					OutputHis2 =2148;
				if (angleHis1=0)
					OutputHis2 =OutputHis2;
			}
			// Try to check whether the output value in the limitation
			if (OutputHis1>4000)		//Detect the limitation of the output value
			{
				OutputHis1=4000;
			}else{
				if (OutputHis1<3)
				{
					OutputHis1=3;
				}
			}
			if (OutputHis2>4000){		//Detect the limitation of the output value
				OutputHis2=4000;
			}else{
				if (OutputHis2<3)	{
					OutputHis2=3;
				}
			}

			// set the output duty cycle
			OCR1B = OutputHis1;	// for B6
			OCR1C = OutputHis2;	// for B7
			
			angleHis2 = angleHis1;	//record value of the angle which will be used in D 
			angleHis1 = AngleActual;//record value of the angle which will be used in D
			
			Timer3Flag=0;
			sei();
		}



	//*****************************************************************
	// Because the holes on our encoder are few, so in the 5 ms period it 
	// probably won't get any data. So my advice is to write the code for 
	// encoder here, which period is 100ms, then we may get some points,
	// or if we like we can use flag3 and set it to 1000ms to get more 
	// points, 1s is enough short to get precise position
	//*****************************************************************



	//condition used to send the wireless data or usb data, 
	// 	because the frequency here is below 10Hz
		if (Timer3Flag2==INTERRUPT1S)	{ 	
			cli();
			tmp = angle;
			// send back value from IMU of acceleration of Y
			wireless_int(RealinputACC);	
			wireless_char('\t');
			// send back value from IMU of anglar speed around X
			wireless_int(RealinputGyr);	
			wireless_char('\t');
			wireless_int(AngleActual);	// send back the current angle value
			wireless_char('\n');
			m_green(TOGGLE);
			Timer3Flag2=0;
			sei();
		}		
    }
}

void systemInitial(void){
	m_clockdivide(0);
	wirelessinitial();
	TimerSetup();
}

void TimerSetup(void){
	//Timer1 is used to generate the PWM
	OCR1A = 4000;
	OCR1B = 2000;
	OCR1C = 2000;
	set(DDRB,6);
	set(DDRB,7);
	TCCR1A = 0b00101011;
	TCCR1B = 0b00011001;
	//Timer3 is used to generate periodical interrupt
		//The equation to calculate the period of interrupt wich determined by PERIOD
	OCR3A = (int)(15625*((float)PERIOD/1000));
	TCCR3A = 0b00000000;
	TCCR3B = 0b00001101;// /1024
	clear(TIMSK3 , OCIE3A);
		// The setup for the input capture should be here
}



ISR(TIMER3_COMPA_vect) //ms interrupt
{
	Timer3Flag = 1;
	if (Timer3Flag2<INTERRUPT1S)
	{
		Timer3Flag2++;
	}
	//if (Timer3Flag3<INTERRUPT10MS)
	//{
		//Timer3Flag3++;
	//}
}

ISR(PCINT0_vect){
	EncoderCount++;
	
}











void transmitEdsCode(){

	char haha[5]="Hello";
	while(1) {
		int success;
		success = m_rf_read(buffer, packLength);
		if ((success==1)&&(buffer[0]>=45)) {
			m_green(ON);
			ConvertFinishFlag=1;
		}
		else{
			m_green(OFF);
		}
		if (ConvertFinishFlag==1) {
			if (switchNum==0) {
				// send back current value of parameter and determine which one should be changed
				wireless_string(buffer,packLength);
				wireless_char(':');
				if ((buffer[0]=='k')&&(buffer[1]=='p')) {
					paraDeter = 1;
					wireless_int(KpIni);
				}
				if ((buffer[0]=='k')&&(buffer[1]=='i')) {
					paraDeter = 2;
					wireless_int(KiIni);
				}
				if ((buffer[0]=='k')&&(buffer[1]=='d')) {
					paraDeter = 3;
					wireless_int(KdIni);
			}
				m_wait(50);
				wireless_char('\n');
				switchNum++;
			}else{
				// change the parameter, and if the input is not a value, it will send back "SayAgain" and wait untill the input is a value
				getValue = atoi(buffer);
				if ((getValue==0)&&(buffer[0]!='0')) {
					char Sorry1[8] = "SayAgain";
					wireless_string(Sorry1,8);
					m_wait(50);
					wireless_char('\n');
					m_wait(50);
					switchNum=1;
				}else{
					wireless_string(buffer,packLength);
					m_wait(50);
					wireless_char('\n');
					m_wait(50);
					switchNum=0;
					switch (paraDeter) {
						case 1:KpIni=getValue;break;
						case 2:KiIni=getValue;break;
						case 3:KdIni=getValue;break;
					}
				}
			}
			// when the input is "start", the robot will start
			if ((buffer[0]=='s')
				&&(buffer[1]=='t')
					&&(buffer[2]=='a')
						&&(buffer[3]=='r')
							&&(buffer[4]=='t')) {
				break;
			}
			ConvertFinishFlag=0;
			memset(buffer,0,packLength);
			m_wait(100);
		}
	}
}
