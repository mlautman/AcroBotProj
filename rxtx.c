
#include <avr/io.h>
#include "m_imu.h"
#include "m_bus.h"
#include "m_usb.h"
#include "m_general.h"


int main(void)
{
	m_clockdivide(0);
	m_usb_init();

	while(1) {
		if (m_usb_rx_available() ){
			char a = m_usb_rx_char();
			m_usb_tx_char(a);
		}	
	}
}

