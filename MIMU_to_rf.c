
#include <avr/io.h>
#include "m_imu.h"
#include "m_bus.h"
#include "m_rf.c"
#include "m_usb.h"
#include "m_general.h"

#define RX_Add  12
#define TX_Add 	3
#define channel 1




volatile int data[9] = {0};

int main(void)
{
	m_clockdivide(0);
	unsigned char accel_scale = 0;
	unsigned char gyro_scale = 0;
	m_usb_init();
	m_imu_init(accel_scale, gyro_scale);

    while(1)
    {

		tx_IMU_data();

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
		m_usb_tx_char('\n');
		m_usb_tx_char('\r');
    }
}
