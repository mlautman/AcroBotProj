/*
 * wireless.h
 *
 * Created: 10/25/2012 10:58:20 PM
 *  Author: yunkai
 */ 


#ifndef WIRELESS_H_
#define WIRELESS_H_

#include "m_bus.h"
#include "m_rf.h"
#include <stdlib.h>

#define packLength 8
#define Channel 1
#define Address 0x17	//17
#define TXaddress 0x1D	//1D

extern void wirelessinitial(void);
extern void wireless_int(int i);
extern void wireless_uint(unsigned int i);
extern void wireless_char(char i);

#endif /* WIRELESS_H_ */