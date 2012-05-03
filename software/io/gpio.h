/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef GPIO_H
#define GPIO_H

#include "../datastructures/hashTable.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>


using namespace std;

#define GPIO_0 		0
#define GPIO_1		GPIO_0 + 32
#define GPIO_2		GPIO_1 + 32
#define GPIO_3		GPIO_2 + 32


namespace gpio {

	struct Pin {
		string name;
		int gpio;
		bool led;
		string mux;
	};

	HashTable<Pin> Bone;
	Pin p;

	void gpio_datastruct_init() {
		static bool initialized = false;

		if(initialized) return;
		initialized = true;

		p.led = false;

		//P8
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P8_1",p);
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P8_2",p);
		p.name = "GPIO1_6"; p.gpio = GPIO_1+6; p.mux = "gpmc_ad6"; Bone.put("P8_3",p);
		p.name = "GPIO1_7"; p.gpio = GPIO_1+7; p.mux = "gpmc_ad7"; Bone.put("P8_4",p);
		p.name = "GPIO1_2"; p.gpio = GPIO_1+2; p.mux = "gpmc_ad2"; Bone.put("P8_5",p);
		p.name = "GPIO1_3"; p.gpio = GPIO_1+3; p.mux = "gpmc_ad3"; Bone.put("P8_6",p);
		p.name = "TIMER4"; p.gpio = GPIO_2+2; p.mux = "gpmc_advn_ale"; Bone.put("P8_7",p);
		p.name = "TIMER7"; p.gpio = GPIO_2+3; p.mux = "gpmc_oen_ren"; Bone.put("P8_8",p);
		p.name = "TIMER5"; p.gpio = GPIO_2+5; p.mux = "gpmc_ben0_cle"; Bone.put("P8_9",p);
		p.name = "TIMER6"; p.gpio = GPIO_2+4; p.mux = "gpmc_wen"; Bone.put("P8_10",p);
		p.name = "GPIO1_13"; p.gpio = GPIO_1+13; p.mux = "gpmc_ad13"; Bone.put("P8_11",p);
		p.name = "GPIO1_12"; p.gpio = GPIO_1+12; p.mux = "gpmc_ad12"; Bone.put("P8_12",p);
		p.name = "EHRPWM2B"; p.gpio = GPIO_0+23; p.mux = "gpmc_ad9"; Bone.put("P8_13",p);
		p.name = "GPIO0_26"; p.gpio = GPIO_0+26; p.mux = "gpmc_ad10"; Bone.put("P8_14",p);
		p.name = "GPIO1_15"; p.gpio = GPIO_1+15; p.mux = "gpmc_ad15"; Bone.put("P8_15",p);
		p.name = "GPIO1_14"; p.gpio = GPIO_1+14; p.mux = "gpmc_ad14"; Bone.put("P8_16",p);
		p.name = "GPIO0_27"; p.gpio = GPIO_0+27; p.mux = "gpmc_ad11"; Bone.put("P8_17",p);
		p.name = "GPIO2_1"; p.gpio = GPIO_2+1; p.mux = "gpmc_clk"; Bone.put("P8_18",p);
		p.name = "EHRPWM2A"; p.gpio = GPIO_0+22; p.mux = "gpmc_ad8"; Bone.put("P8_19",p);
		p.name = "GPIO1_31"; p.gpio = GPIO_1+31; p.mux = "gpmc_csn2"; Bone.put("P8_20",p);
		p.name = "GPIO1_30"; p.gpio = GPIO_1+30; p.mux = "gpmc_csn1"; Bone.put("P8_21",p);
		p.name = "GPIO1_5"; p.gpio = GPIO_1+5; p.mux = "gpmc_ad5"; Bone.put("P8_22",p);
		p.name = "GPIO1_4"; p.gpio = GPIO_1+4; p.mux = "gpmc_ad4"; Bone.put("P8_23",p);
		p.name = "GPIO1_1"; p.gpio = GPIO_1+1; p.mux = "gpmc_ad1"; Bone.put("P8_24",p);
		p.name = "GPIO1_0"; p.gpio = GPIO_1+0; p.mux = "gpmc_ad0"; Bone.put("P8_25",p);
		p.name = "GPIO1_29"; p.gpio = GPIO_1+29; p.mux = "gpmc_csn0"; Bone.put("P8_26",p);
		p.name = "GPIO2_22"; p.gpio = GPIO_2+22; p.mux = "lcd_vsync"; Bone.put("P8_27",p);
		p.name = "GPIO2_24"; p.gpio = GPIO_2+24; p.mux = "lcd_pclk"; Bone.put("P8_28",p);
		p.name = "GPIO2_23"; p.gpio = GPIO_2+23; p.mux = "lcd_hclk"; Bone.put("P8_29",p);
		p.name = "GPIO2_25"; p.gpio = GPIO_2+25; p.mux = "lcd_ac_bias_en"; Bone.put("P8_30",p);
		p.name = "UART5_CTSN"; p.gpio = GPIO_0+10; p.mux = "lcd_data14"; Bone.put("P8_31",p);
		p.name = "UART5_RTSN"; p.gpio = GPIO_0+11; p.mux = "lcd_data15"; Bone.put("P8_32",p);
		p.name = "UART4_RTSN"; p.gpio = GPIO_0+9; p.mux = "lcd_data13"; Bone.put("P8_33",p);
		p.name = "UART3_RTSN"; p.gpio = GPIO_2+17; p.mux = "lcd_data11"; Bone.put("P8_34",p);
		p.name = "UART4_CTSN"; p.gpio = GPIO_0+8; p.mux = "lcd_data12"; Bone.put("P8_35",p);
		p.name = "UART3_CTSN"; p.gpio = GPIO_2+16; p.mux = "lcd_data10"; Bone.put("P8_36",p);
		p.name = "UART5_TXD"; p.gpio = GPIO_2+14; p.mux = "lcd_data8"; Bone.put("P8_37",p);
		p.name = "UART5_RXD"; p.gpio = GPIO_2+15; p.mux = "lcd_data9"; Bone.put("P8_38",p);
		p.name = "GPIO2_12"; p.gpio = GPIO_2+12; p.mux = "lcd_data6"; Bone.put("P8_39",p);
		p.name = "GPIO2_13"; p.gpio = GPIO_2+13; p.mux = "lcd_data7"; Bone.put("P8_40",p);
		p.name = "GPIO2_10"; p.gpio = GPIO_2+10; p.mux = "lcd_data4"; Bone.put("P8_41",p);
		p.name = "GPIO2_11"; p.gpio = GPIO_2+11; p.mux = "lcd_data5"; Bone.put("P8_42",p);
		p.name = "GPIO2_8"; p.gpio = GPIO_2+8; p.mux = "lcd_data2"; Bone.put("P8_43",p);
		p.name = "GPIO2_9"; p.gpio = GPIO_2+9; p.mux = "lcd_data3"; Bone.put("P8_44",p);
		p.name = "GPIO2_6"; p.gpio = GPIO_2+6; p.mux = "lcd_data0"; Bone.put("P8_45",p);
		p.name = "GPIO2_7"; p.gpio = GPIO_2+7; p.mux = "lcd_data1"; Bone.put("P8_46",p);

		//P9
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P9_1",p);
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P9_2",p);
		p.name = "VDD_3V3"; p.gpio = -1; p.mux = ""; Bone.put("P9_3",p);
		p.name = "VDD_3V3"; p.gpio = -1; p.mux = ""; Bone.put("P9_4",p);
		p.name = "VDD_5V"; p.gpio = -1; p.mux = ""; Bone.put("P9_5",p);
		p.name = "VDD_5V"; p.gpio = -1; p.mux = ""; Bone.put("P9_6",p);
		p.name = "SYS_5V"; p.gpio = -1; p.mux = ""; Bone.put("P9_7",p);
		p.name = "SYS_5V"; p.gpio = -1; p.mux = ""; Bone.put("P9_8",p);
		p.name = "PWR_BUT"; p.gpio = -1; p.mux = ""; Bone.put("P9_9",p);
		p.name = "SYS_RESETn"; p.gpio = -1; p.mux = ""; Bone.put("P9_10",p);
		p.name = "UART4_RXD"; p.gpio = GPIO_0+30; p.mux = "gpmc_wait0"; Bone.put("P9_11",p);
		p.name = "GPIO1_28"; p.gpio = GPIO_1+28; p.mux = "gpmc_ben1"; Bone.put("P9_12",p);
		p.name = "UART4_TXD"; p.gpio = GPIO_0+31; p.mux = "gpmc_wpn"; Bone.put("P9_13",p);
		p.name = "EHRPWM1A"; p.gpio = GPIO_1+18; p.mux = "gpmc_a2"; Bone.put("P9_14",p);
		p.name = "UART3_RXD"; p.gpio = GPIO_1+16; p.mux = "mii1_rxd3"; Bone.put("P9_15",p);
		p.name = "EHRPWM1B"; p.gpio = GPIO_1+19; p.mux = "gpmc_a3"; Bone.put("P9_16",p);
		p.name = "I2C1_SCL"; p.gpio = GPIO_0+5; p.mux = "spi0_cs0"; Bone.put("P9_17",p);
		p.name = "I2C1_SDA"; p.gpio = GPIO_0+4; p.mux = "spi0_d1"; Bone.put("P9_18",p);
		p.name = "I2C2_SCL"; p.gpio = GPIO_0+13; p.mux = "uart1_rtsn"; Bone.put("P9_19",p);
		p.name = "I2C2_SDA"; p.gpio = GPIO_0+12; p.mux = "uart1_ctsn"; Bone.put("P9_20",p);
		p.name = "UART2_TXD"; p.gpio = GPIO_0+3; p.mux = "spi0_d0"; Bone.put("P9_21",p);
		p.name = "UART2_RXD"; p.gpio = GPIO_0+2; p.mux = "spi0_sclk"; Bone.put("P9_22",p);
		p.name = "UART3_TXD"; p.gpio = GPIO_0+7; p.mux = "ecap0_in_pwm0_out"; Bone.put("P9_23",p);
		p.name = "UART1_TXD"; p.gpio = GPIO_0+15; p.mux = "uart1_txd"; Bone.put("P9_24",p);
		p.name = "GPIO3_21"; p.gpio = GPIO_3+21; p.mux = "mcasp0_ahclkx"; Bone.put("P9_25",p);
		p.name = "UART1_RXD"; p.gpio = GPIO_0+14; p.mux = "uart1_rxd"; Bone.put("P9_26",p);
		p.name = "GPIO3_19"; p.gpio = GPIO_3+19; p.mux = "mcasp0_fsr"; Bone.put("P9_27",p);
		p.name = "SPI1_CSO"; p.gpio = GPIO_3+17; p.mux = "mcasp0_ahclkr"; Bone.put("P9_28",p);
		p.name = "SPI1_D0"; p.gpio = GPIO_3+15; p.mux = "mcasp0_fsx"; Bone.put("P9_29",p);
		p.name = "SPI1_D1"; p.gpio = GPIO_3+16; p.mux = "mcasp0_axr0"; Bone.put("P9_30",p);
		p.name = "SPI1_SCLK"; p.gpio = GPIO_3+14; p.mux = "mcasp-_aclkx"; Bone.put("P9_31",p);
		p.name = "VDD_ADC"; p.gpio = -1; p.mux = ""; Bone.put("P9_32",p);
		p.name = "AIN4"; p.gpio = -1; p.mux = ""; Bone.put("P9_33",p);
		p.name = "GNDA_ADC"; p.gpio = -1; p.mux = ""; Bone.put("P9_34",p);
		p.name = "AIN6"; p.gpio = -1; p.mux = ""; Bone.put("P9_35",p);
		p.name = "AIN5"; p.gpio = -1; p.mux = ""; Bone.put("P9_36",p);
		p.name = "AIN2"; p.gpio = -1; p.mux = ""; Bone.put("P9_37",p);
		p.name = "AIN3"; p.gpio = -1; p.mux = ""; Bone.put("P9_38",p);
		p.name = "AIN0"; p.gpio = -1; p.mux = ""; Bone.put("P9_39",p);
		p.name = "AIN1"; p.gpio = -1; p.mux = ""; Bone.put("P9_40",p);
		p.name = "CLKOUT2"; p.gpio = GPIO_0+20; p.mux = "xdma_eve_intr1"; Bone.put("P9_41",p);
		p.name = "ID_1"; p.gpio = -1; p.mux = ""; Bone.put("P9_42",p);
		p.name = "ID_0"; p.gpio = -1; p.mux = ""; Bone.put("P9_43",p);
		p.name = "ID_2"; p.gpio = -1; p.mux = ""; Bone.put("P9_44",p);
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P9_45",p);
		p.name = "DGND"; p.gpio = -1; p.mux = ""; Bone.put("P9_46",p);

		//User LEDs
		p.led = true;
		p.name = "USR0"; p.gpio = GPIO_1+21; p.mux = "gpmc_a5"; Bone.put("USR0",p);
		p.name = "USR1"; p.gpio = GPIO_1+22; p.mux = "gpmc_a6"; Bone.put("USR1",p);
		p.name = "USR2"; p.gpio = GPIO_1+23; p.mux = "gpmc_a7"; Bone.put("USR2",p);
		p.name = "USR3"; p.gpio = GPIO_1+24; p.mux = "gpmc_a8"; Bone.put("USR3",p);

	}
}


class GPIO {

	private:
		gpio::Pin pin;
		int status;

	public:
		
		GPIO();
		~GPIO();

		int init(const char *pin);
		int detach();
		int set_dir(const char *dir);
		int set_value(bool state);
		int get_value(unsigned int &state);
		int set_edge(const char *edge);
};

GPIO::GPIO() {
	gpio::gpio_datastruct_init();
	status = -1;
}

GPIO::~GPIO() {
	detach();
}

int GPIO::init(const char *pin) {

	if((status = gpio::Bone.get(pin,this->pin)) != 0) {
		cout << "Invalid Pin" << endl;
		return status;
	}

	int fd,len;
	char buf[64];

	if((fd = open("/sys/class/gpio/export",O_WRONLY)) < 0) {
		cout << "Error Opening Pin" << endl;
		status = fd;
		return status;
	}

	len = snprintf(buf,sizeof(buf),"%d",this->pin.gpio);
	write(fd,buf,len);
	close(fd);

	status = 1;
	return status;
}

int GPIO::detach() {
	if(status != 1) return -1;

	int fd,len;
	char buf[64];

	if((fd = open("/sys/class/gpio/unexport",O_WRONLY)) < 0) {
		cout << "error closing pin" << endl;
		return fd;
	}

	len = snprintf(buf,sizeof(buf),"%d",pin.gpio);
	write(fd,buf,len);
	close(fd);

	return status;
}

int GPIO::set_dir(const char *dir) {

	int fd,len;
	char buf[64];

	len = snprintf(buf,sizeof(buf),"/sys/class/gpio/gpio%d/direction",pin.gpio);

	if((fd = open(buf,O_WRONLY)) < 0) {
		cout << "Error Setting Direction" << endl;
		return fd;
	}

	write(fd,dir,strlen(dir));

	close(fd);
	return 0;
}


int GPIO::set_value(bool state) {

	int fd,len;
	char buf[64];

	len = snprintf(buf,sizeof(buf),"/sys/class/gpio/gpio%d/value",pin.gpio);
	if((fd = open(buf,O_WRONLY)) < 0) {
		cout << "error opening file" << endl;
		return fd;
	}

	if(state) {
		write(fd,"1",1);
	} else {
		write(fd,"0",1);
	}

	close(fd);
	return 0;
}

int GPIO::get_value(unsigned int &value) {

	int fd,len;
	char buf[64];

	len = snprintf(buf,sizeof(buf),"/sys/class/gpio/gpio%d/value",pin.gpio);

	if((fd = open(buf,O_RDONLY)) < 0 ) {
		cout << "error opening file" << endl;
		return fd;
	}

	read(fd,buf,1);
	value = atoi(buf);

	close(fd);
	return 0;
}

int GPIO::set_edge(const char *edge) {

	int fd,len;
	char buf[64];

	len = snprintf(buf,sizeof(buf),"/sys/class/gpio/gpio%d/edge",pin.gpio);

	if((fd = open(buf,O_WRONLY)) < 0) {
		cout << "Error Setting Direction" << endl;
		return fd;
	}

	write(fd,edge,strlen(edge));

	close(fd);
	return 0;
}
#endif
