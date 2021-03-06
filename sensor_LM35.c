/**
* @file     sensor_LM35.c
* @brief
* Inici : Martes 13 OCT 2020
* Grup : G
* Clase : AT-20
* Signature : ADRST
* Membre:  Xavi Pueyo Castillo/  Meiling Chen
 *
 */

 /**
 * @file		sensor_LM35.c
 *						Copyright
 *  -----------------------------------------------
 * Copyright Meiling | Xavier 2020 <1424692@campus.euss.org|1467071@campus.euss.org> 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 * @author	Xavier Pueyo Castillo<1424692@campus.euss.org> |Meiling Chen <1467071@campus.euss.org>
 * @version	1.0
 * @date		20-OCT-2020
 * @attention
 * Este archivo forma parte del proyecto\n
 * El projecte es tracta de Disseny i implementar un sistema iot de recollida, control i Visualització dades de sensors amb plataformes embedded complexes i anàlisi de la resposta temporal.
 *
 * @htmlonly
 * <span style="font-weight: bold">History</span>
 * @endhtmlonly
 * Version|Auther|Date|Describe
 * -----------|----------------|-----------|----------
 *V1.0|Meiling Chen|Xavier Pueyo Castillo|20-OCT-2020|Create File|
 * <h2><center>&copy;COPYRIGHT 2017 WELLCASA All Rights Reserved. </center>>/h2>
 */

 /**
  * @file				sensor_LM35.c
  *
  *						Librerias a incluir en el programa
  *						-----------------------------------------------
  \verbatim
    #include <stdint.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <getopt.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/types.h>
    #include <linux/spi/spidev.h>
    #include <string.h>

  \endverbatim
  */
 
  /**
   ** @file				sensor_LM35.c
   *
   *						ADC configuracions segons manual MCP3008
   *						-----------------------------------------------
   *
   \verbatim
    #define SINGLE_ENDED_CH0 8
    #define SINGLE_ENDED_CH1 9
    #define SINGLE_ENDED_CH2 10
    #define SINGLE_ENDED_CH3 11
    #define SINGLE_ENDED_CH4 12
    #define SINGLE_ENDED_CH5 13
    #define SINGLE_ENDED_CH6 14
    #define SINGLE_ENDED_CH7 15
    #define DIFERENTIAL_CH0_CH1 0 //Chanel CH0 = IN+ CH1 = IN-
    #define DIFERENTIAL_CH1_CH0 1 //Chanel CH0 = IN- CH1 = IN+
    #define DIFERENTIAL_CH2_CH3 2 //Chanel CH2 = IN+ CH3 = IN-
    #define DIFERENTIAL_CH3_CH2 3 //Chanel CH2 = IN- CH3 = IN+
    #define DIFERENTIAL_CH4_CH5 4 //Chanel CH4 = IN+ CH5 = IN-
    #define DIFERENTIAL_CH5_CH4 5 //Chanel CH4 = IN- CH5 = IN+
    #define DIFERENTIAL_CH6_CH7 6 //Chanel CH6 = IN+ CH7 = IN-
    #define DIFERENTIAL_CH7_CH6 7 //Chanel CH6 = IN- CH7 = IN+

   \endverbatim
*/



#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "funcions.h"


int verbose = 1;

static char *cntdevice = "/dev/spidev0.0";

//ADC configurations segons manual MCP3008

#define SINGLE_ENDED_CH0 8
#define SINGLE_ENDED_CH1 9
#define SINGLE_ENDED_CH2 10
#define SINGLE_ENDED_CH3 11
#define SINGLE_ENDED_CH4 12
#define SINGLE_ENDED_CH5 13
#define SINGLE_ENDED_CH6 14
#define SINGLE_ENDED_CH7 15
#define DIFERENTIAL_CH0_CH1 0 //Chanel CH0 = IN+ CH1 = IN-
#define DIFERENTIAL_CH1_CH0 1 //Chanel CH0 = IN- CH1 = IN+
#define DIFERENTIAL_CH2_CH3 2 //Chanel CH2 = IN+ CH3 = IN-
#define DIFERENTIAL_CH3_CH2 3 //Chanel CH2 = IN- CH3 = IN+
#define DIFERENTIAL_CH4_CH5 4 //Chanel CH4 = IN+ CH5 = IN-
#define DIFERENTIAL_CH5_CH4 5 //Chanel CH4 = IN- CH5 = IN+
#define DIFERENTIAL_CH6_CH7 6 //Chanel CH6 = IN+ CH7 = IN-
#define DIFERENTIAL_CH7_CH6 7 //Chanel CH6 = IN- CH7 = IN+

// -----------------------------------------------------------------------------------------------

static void pabort(const char *s)
{
	perror(s);
	abort();
}

// -----------------------------------------------------------------------------------------------

static void spiadc_config_tx( int conf, uint8_t tx[3] )
{
	int i;

	uint8_t tx_dac[3] = { 0x01, 0x00, 0x00 };
	uint8_t n_tx_dac = 3;
	
	for (i=0; i < n_tx_dac; i++) {
		tx[i] = tx_dac[i];
	}
	
// Estableix el mode de comunicació en la parta alta del 2n byte
	tx[1]=conf<<4;
	
	if( verbose ) {
		for (i=0; i < n_tx_dac; i++) {
			printf("spi tx dac byte:(%02d)=0x%02x\n",i,tx[i] );
		}
	}
		
}

// -----------------------------------------------------------------------------------------------
static int spiadc_transfer(int fd, uint8_t bits, uint32_t speed, uint16_t delay, uint8_t tx[3], uint8_t *rx, int len )
{
	int ret, value, i;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len*sizeof(uint8_t),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	if( verbose ) {

		for (i = 0; i < len; i++) {
			printf("0x%02x ", rx[i]);
		}
		value = ((rx[1] & 0x0F) << 8) + rx[2];
		printf("-->  %d\n", value);
	
	}

	return ret;

}



// -----------------------------------------------------------------------------------------------

static int spiadc_config_transfer( int conf, int *value )
{
	int ret = 0;
	int fd;
	uint8_t rx[3];
	char buffer[255];
	
	/* SPI parameters */
	char *device = cntdevice;
	//uint8_t mode = SPI_CPOL; //No va bé amb aquesta configuació, ha de ser CPHA
	uint8_t mode = SPI_CPHA;
	uint8_t bits = 8;
	uint32_t speed = 500000; //max 1500KHz
	uint16_t delay = 0;
	
	/* Transmission buffer */
	uint8_t tx[3];

	/* open device */
	fd = open(device, O_RDWR);
	if (fd < 0) {
		sprintf( buffer, "can't open device (%s)", device );
		pabort( buffer );
	}

	/* spi mode 	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/* bits per word 	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/* max speed hz  */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	/* build data to transfer */
	spiadc_config_tx( conf, tx );
		
	/* spi adc transfer */
	ret = spiadc_transfer( fd, bits, speed, delay, tx, rx, 3 );
	if (ret == 1)
		pabort("can't send spi message");

	close(fd);

	*value = ((rx[1] & 0x03) << 8) + rx[2];

	return ret;
}

// -----------------------------------------------------------------------------------------------

void LM35ClockUp()
{
	SCK_LOW;
	delayMicroseconds(10);
	SCK_HIGN;
	delayMicroseconds(10);
}


// -----------------------------------------------------------------------------------------------

int sensor_LM35(float *valor_llegit){
    
    
    int ret = 0, value_int;

	ret = spiadc_config_transfer( SINGLE_ENDED_CH1, &value_int ); //guardar valor llegit en binari a value_int

	printf("valor llegit (0-1023) %d\n", value_int); 
	value_llegit=3.3*value_int/1023; //convertir del valor binari a volts
	
	printf("voltatge %.3f V\n", value_llegit);

	return ret;

}



