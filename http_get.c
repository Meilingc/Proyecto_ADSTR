/**
* @file     http_get.c
* @brief
* Inici : Martes 13 OCT 2020
* Grup : G
* Clase : AT-20
* Signature : ADRST
* Membre:  Xavi Pueyo Castillo/  Meiling Chen
 *
 */

 /**
 * @file		http_get.c
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
  * @file				http_get.c
  *
  *						Librerias a incluir en el programa
  *						-----------------------------------------------
  \verbatim
	#ifdef HAVE_CONFIG_H
	#include <config.h>
	#endif
	#include "funcions.h"

	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <string.h>
  \endverbatim
  */


  /**
   ** @file				http_get.c
   *
   *						Variables i constants a utilitzar en el programa
   *						-----------------------------------------------
   *
   \verbatim
	int flag = 0;// variable que diferencia la zona de http header de la zona de dades. 0=Header 1=dades
	int ind_buffer = 0;
	int ind_header = 0;//index per recórrer el vector de header
	int ind_dades = 0;//index per recórrer el vectr de dades
	struct sockaddr_in	serverAddr;
	int			sockAddrSize;
	int			sFd;
	int			mlen;
	int 		result;
	char		buffer[8000];


   \endverbatim
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "funcions.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


#define SERVER_PORT_NUM1	80

/**
		*@brief
		*Funció http_get: fer un "HTTP GET"
		* @param[in]	nom_servidor: IP del servidor
		* @param[in]	cadena_URL: cadana URL
		* @param[in]	resposta_header: resposta del header
		* @param[in]	resposta_data: respotsta de les dates
		*
		*/


int http_get(char* nom_servidor, char* cadena_URL, char* resposta_header, char* resposta_data)
{
	int flag = 0;// variable que diferencia la zona de http header de la zona de dades. 0=Header 1=dades
	int ind_buffer = 0;
	int ind_header = 0;//index per recórrer el vector de header
	int ind_dades = 0;//index per recórrer el vectr de dades
	struct sockaddr_in	serverAddr;
	int			sockAddrSize;
	int			sFd;
	int			mlen;
	int 		result;
	char		buffer[8000];

	/*Inicialitzar vector buffer
	for(ind_buffer=0;ind_buffer<4255;ind_buffer++){
		buffer[ind_buffer]=' ';
		} */

		/*Inicialitzar vector header*/
	for (ind_header = 0;ind_header < 500;ind_header++) {
		resposta_header[ind_header] = ' ';
	}
	ind_header = 0;

	/*Inicialitzar vector data*/
	for (ind_dades = 0;ind_dades < 4700;ind_dades++) {
		resposta_data[ind_dades] = ' ';
	}
	//resposta_data[0]='\0';
	ind_dades = 0;


	/*Crear el socket*/
	sFd = socket(AF_INET, SOCK_STREAM, 0);

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero((char*)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT_NUM1);
	serverAddr.sin_addr.s_addr = inet_addr(nom_servidor);


	/*Conexió*/
	result = connect(sFd, (struct sockaddr*)&serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	/*Enviar*/
	memset(buffer,0,8000);
	strcpy(buffer, cadena_URL); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s", result, cadena_URL);

	/*Rebre*/
	memset(buffer,0,8000);
	result = read(sFd, buffer, 4256);
	//printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);

	/*Distribuir resposta entre header i dades*/
	memset(resposta_header,0,500);
	memset(resposta_data,0,4700);
	
	printf("\nLA RESPOSTA DEL HEAD DE HTTP ES:\n");
	for (mlen = 0;mlen < 8000;mlen++) {
		if (buffer[mlen] == '<') {
			flag = 1;//significa que hem entrat en la zona de dades
		}
		if (flag != 1) {
			resposta_header[ind_header]=buffer[mlen];
			ind_header++;			
		}else{
			if (buffer[mlen])resposta_data[ind_dades++]=buffer[mlen];
		}
	}/*tanca for*/
	
	printf("%s\n",resposta_header);
	
	/*Tancar el socket*/
	close(sFd);
	//printf("\r\n %s \r\n",resposta_header);
	printf("\nLA RESPOSTA DE LES DADES ES:\n");
	printf("\r\n %s \r\n", resposta_data);


	return 0;
}

