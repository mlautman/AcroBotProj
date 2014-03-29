/*
 * wireless.c
 *
 * Created: 10/25/2012 10:58:02 PM
 *  Author: yunkai
 */ 
#include "wirelessX.h"

:define COMMONLENGTH 8

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
