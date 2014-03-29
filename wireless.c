/*
 * m_wireless.c
 *
 * Created: 10/25/2012 10:58:02 PM
 *  Author: yunkai
 */ 
#include <stdlib.h>
#include "m_wireless.h"
#include <string.h>
#define COMMONLENGTH 8

char RXaddress = 0;

char wireless_init(char rxtx[]){
	m_bus_init();
	char a;
	if ( strcmp(rxtx, "rx") ){
		a = m_rf_open(Channel,RXaddress,packLength);
	}else if (strcmp(rxtx, "tx")){
		a =m_rf_open(Channel,TXaddress,packLength);
	}else {
		a = -1;
	}	
	return a;
}
char m_wireless_int(int i)
{
	char string[COMMONLENGTH] = {0};
	itoa(i,string,10);
	return m_rf_send(TXaddress,string, COMMONLENGTH);
}
char m_wireless_uint(unsigned int i)
{
	char string[COMMONLENGTH] = {0};
	utoa(i,string,10);
	return m_rf_send(TXaddress,string, COMMONLENGTH);
}
char m_wireless_char(char i)
{
	char string[COMMONLENGTH] = {0};
	string[0] = i;
	return m_rf_send(TXaddress,string, COMMONLENGTH);
}
char m_wireless_string(char* i,int numC)
{
	char a = 1 ;
	char b = 0 ;
	char string[COMMONLENGTH] = {0};
	while (numC>0){
		for (int m=0; m < min(numC,8) ;m++)
		{
			string[m] = i[m];
		}
		b = m_rf_send(TXaddress,string, COMMONLENGTH);
		numC=numC-8;
		if ( b == 0 )
			a=b;
	}
	return a;
}


int min (int a, int b){
	if ( a<b ){
		return a;
	} else if ( b<=a ){
		return b;
	} else return b;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wirelessinitial(void)
{
	m_bus_init();
	m_rf_open(Channel,Address,packLength);
}

void wireless_int(int i)
{
	char string[COMMONLENGTH] = {0};
	itoa(i,string,10);
	m_rf_send(TXaddress,string, COMMONLENGTH);
	smallDelay(1000);
}
void wireless_uint(unsigned int i)
{
	char string[COMMONLENGTH] = {0};
	utoa(i,string,10);
	m_rf_send(TXaddress,string, COMMONLENGTH);
	smallDelay(1000);
}
void wireless_char(char i)
{
	char string[COMMONLENGTH] = {0};
	string[0] = i;
	m_rf_send(TXaddress,string, COMMONLENGTH);
	smallDelay(1000);
}
void wireless_string(char* i,int numC)	//which can only contain 8 characters
{
	char string[COMMONLENGTH] = {0};
	for (int m=0;m<numC;m++)
	{
		string[m] = i[m];
	}
	m_rf_send(TXaddress,string, COMMONLENGTH);
	smallDelay(1000);
}
void smallDelay(int value)
{
	for (int i=0;i<value;i++);
}
