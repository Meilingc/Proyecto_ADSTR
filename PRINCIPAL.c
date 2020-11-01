/**
* @file     principal.c
* @brief
* Inici : Martes 13 OCT 2020
* Grup : G
* Clase : AT-20
* Signature : ADRST
* Membre:  Xavi Pueyo Castillo/  Meiling Chen
 *
 */

 /**
 * @file		principal.c
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
  * @file				principal.c
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
#include <netdb.h> //incorporat per Fita2
#include <getopt.h>

#define REPLY_MSG_SIZE		500
  \endverbatim
  */


  /**
   ** @file				principal.c
   *
   *						Variables i constants a utilitzar en el programa
   *						-----------------------------------------------
   *
   \verbatim
	char    	nom_servidor[REPLY_MSG_SIZE];//guarda la ip del servidor de mail
	char	    email_destinatari[REPLY_MSG_SIZE];
	char	    email_remitent[REPLY_MSG_SIZE];
	char	    text_email[REPLY_MSG_SIZE];
	char	    serverName[REPLY_MSG_SIZE];//guarda la ip del servidor de http
	char		missatge[REPLY_MSG_SIZE];//comando de consulta a servidor
	char 	    header[500];
	char		dades[4700];
	char 		URLServidor[50];//afegit per Fita2. Guarda la adreça URL del servidor que volem connectar
	char 		Recurs[40];//afegit per Fita2.Guarda la denominacio del recurs del servidor al que es vol accedir
	char		missatge1[REPLY_MSG_SIZE];
	char		missatge2[REPLY_MSG_SIZE];

     #define REPLY_MSG_SIZE		5000

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
#include <netdb.h> /*incorporat per Fita2*/
#include <getopt.h>

#define REPLY_MSG_SIZE		5000

int main(int argc, char* argv[]) {
 
	char* nom_servidor1 = NULL;
    char* email_destinatari1 = NULL;
    char* email_remitent1 = NULL;
    
	char    	nom_servidor[REPLY_MSG_SIZE];/*guarda la ip del servidor de mail*/
	char	    	email_destinatari[REPLY_MSG_SIZE];
	char	    	email_remitent[REPLY_MSG_SIZE];
	char	    	text_email[REPLY_MSG_SIZE];
	
	char	    	serverName[REPLY_MSG_SIZE];/*guarda la ip del servidor de http*/
	char		missatge[REPLY_MSG_SIZE];/*comando de consulta a servidor*/
	char 	    	header[500];
	char		dades[4700];
	char 		URLServidor[50];/*afegit per Fita2. Guarda la adreça URL del servidor que volem connectar*/
	char 		Recurs[40];/*afegit per Fita2.Guarda la denominacio del recurs del servidor al que es vol accedir*/
	char		missatge1[REPLY_MSG_SIZE];
	char		missatge2[REPLY_MSG_SIZE];

	/*DEFINICIO DE ORDRES PER CONSTITUIR EL MISSATGE AL SERVIDOR*/
	char		ordre[] = "GET / HTTP/1.1\r\nHost: ";
	char		ajust[] = "\r\n\r\n";

	/*DEFINICIO DE VARIABLES PER OBTENIR INFORMACIO DEL HOST*/
	struct hostent* info;
	struct in_addr* addr;

	/*DEFINICIO DE VARIABLES PER GETOPT*/
	static struct option long_options[] = {
		 {"servidor",required_argument,0,'s'},
		 {"recurs",required_argument,0,'r'},
		 {"IP",     required_argument,       0, 'a'},
         	 {"remitent",  required_argument,       0, 'b'},
         	 {"destinatari",  required_argument, 0, 'c'},
		 {0,0,0,0}
	};



	/*SECCIO DE GESTIO DE CLIENT DE EMAIL*/

	/*strcpy(nom_servidor, "172.20.0.21");  //Asignar IP 
	strcpy(email_destinatari, "1424692@campus.euss.org");	//Asignar email destinario
	strcpy(email_remitent, "usuari@campus.euss.org");	//Asignar  email remitente (puede ser inventado)*/
	
   /*SECCIO DE GESTIO DE CLIENT HTTP. ENTRADA DE DADES*/
	int option = 0;
	while ((option = getopt_long(argc, argv, "s:r:a:b:c:", long_options, NULL)) != -1)
	{
		switch (option) {
		case 's': strcpy(URLServidor, optarg);
			break;
		case 'r': strcpy(Recurs, optarg);
			break;
		case 'a':   //caso a: guardar el ip de servidor a través de optarg(option argument)
                nom_servidor1=optarg;  
                printf("Nom de servidor es %s\n", nom_servidor1);
                break;  //Terminar el caso
        case 'b':   //caso b: guardar el email remitente a través de optarg(option argument)
                email_remitent1=optarg;  
                printf("El email remitente es %s\n", email_remitent1);
                break;  //Terminar el caso
        case 'c':   //caso c: guardar el email destinatario a través de optarg(option argument)
                email_destinatari1=optarg;  
                printf("El email destinatario es %s\n", email_destinatari1);
                break;  //Terminar el caso              
		case '?': printf("\nOpción seleccionada errónea-%c.\n", optopt);
			break;
		}

	}
	
	strcpy(nom_servidor,nom_servidor1);
        strcpy(email_remitent,email_remitent1);
        strcpy(email_destinatari,email_destinatari1);
 
	strcpy(text_email, "Hola,\nSoy un email que envia automáticamente.\nSaludos\n");	//el texto que queremos enviar
	email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email
      

	/*OBTENCIO DE INFORMACIO DEL SERVIDOR IP PER EL DNS*/

	info = gethostbyname(URLServidor);

	if (info == NULL)
	{
		printf("\rHost no localizado. %s\n", URLServidor);
	}
	else {
		addr = (struct in_addr*)(info->h_addr);
		printf("\r %s tiene por dirección %s\n", URLServidor, inet_ntoa(*addr));
	}
	strcpy(serverName, inet_ntoa(*addr));

	/*CREACIO DE L'ORDRE GET PER EL SERVIDOR*/

	/*strcpy(missatge1, ordre);	//Asignar comando inicial
	strcat(missatge1, URLServidor);
	strcpy(missatge2, ajust);
	strcat(missatge1, missatge2);
	strcpy(missatge, missatge1);
	printf("%s", missatge);*/
	sprintf(missatge,"GET %s HTTP/1.1\r\nHost: %s",Recurs,URLServidor);
	strcat(missatge,ajust);
	printf("%s",missatge);
	
	http_get(serverName, missatge, header, dades);

	return 0;

}

