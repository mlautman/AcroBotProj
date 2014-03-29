/*
 * m_wireless.h
 *
 * Created: 10/25/2012 10:58:20 PM
 *  Author: yunkai
 */ 


#ifndef WIRELESSX_H_
#define WIRELESSX_H_

#include "m_bus.h"
#include "m_rf.h"
#include <stdlib.h>

#define packLength 8
#define Channel 1
#define Address 0x1D	//17
#define TXaddress 0x17	//1D

extern char wireless_init( char rxtx[]);

/* ***********************************************************************************************
		 -------------------- wirelessinital() ----------------------
	initializes the m_bus then upens up rf communication with anther M2 
  		Comm goes over channel 1
	WARNING! This function requires that you have defined the correct 
*********************************************************************************************** */

extern void wirelessinitial( void );
/* ***********************************************************************************************
		 -------------------- wirelessinital() ----------------------
	initializes the m_bus then upens up rf communication with anther M2 over channel 1
	param:
		String equaling either "rx" or "tx" 
	RETURNS:
		1: module acknowledged setup
		0: something went wrong
		-1: incorrect input parameter

*********************************************************************************************** */

extern char m_wireless_int(int i) ;
/* ***********************************************************************************************
		 -------------------- m_wireless_int( int i) ----------------------
	sends an integer value over rf to another M2

	param:
		int i
	
	RETURNS:
		1: 	successful transmission to the mRF module 
		0: 	something went wrong

*********************************************************************************************** */

extern char  m_wireless_uint(unsigned int i);
/* ***********************************************************************************************
		 -------------------- wireless_uint() ----------------------
	sends an unsigned int  value over rf to another M2

	param:
		unsigned int i

	RETURNS:
		1: 	successful transmission to the mRF module 
		0: 	something went wrong

*********************************************************************************************** */

extern char  m_wireless_string(char* i,int numC);
/* ***********************************************************************************************
		 -------------------- wireless_string( char* , int) ----------------------
	sends an string value over rf to another M2

	param:
		char* a string.
		int numC : size of string

	RETURNS:
		1: 	successful transmission to the mRF module 
		0: 	something went wrong


*********************************************************************************************** */

extern char  m_wireless_char(char i);
/* ***********************************************************************************************
		 -------------------- wireless_char( char* , int) ----------------------
	sends an char value over rf to another M2

	param:
		char* a string.
		int numC : size of string
	RETURNS:
		1: 	successful transmission to the mRF module 
		0: 	something went wrong

*********************************************************************************************** */



int min(int a, int b);

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

extern void wireless_int(int i) ;
/* ***********************************************************************************************
		 -------------------- wireless_int() ----------------------
	sends an integer value over rf to another M2

	param:
		int i
	
*********************************************************************************************** */ 

extern void wireless_uint(unsigned int i);
/* ***********************************************************************************************
		 -------------------- wireless_uint() ----------------------
	sends an unsigned int  value over rf to another M2

	param:
		unsigned int i
	
*********************************************************************************************** */

extern void wireless_string(char* i,int numC);
/* ***********************************************************************************************
		 -------------------- wireless_string( char* , int) ----------------------
	sends an string value over rf to another M2

	param:
		char* a string.
		int numC : size of string
	
*********************************************************************************************** */

extern void wireless_char(char i);
/* ***********************************************************************************************
		 -------------------- wireless_char( char* , int) ----------------------
	sends an char value over rf to another M2

	param:
		char* a string.
		int numC : size of string
	
*********************************************************************************************** */
extern void smallDelay(int value);

#endif /* WIRELESS_H_ */
