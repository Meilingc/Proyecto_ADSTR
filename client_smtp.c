/**
* @file     client_smtp.c
* @brief
* Inici : Martes 13 OCT 2020
* Grup : G
* Clase : AT-20
* Signature : ADRST
* Membre:  Xavi Pueyo Castillo/  Meiling Chen
 *
 */

 /**
 * @file		client_smtp.c
 *						Copyright
 *  -----------------------------------------------
 * Copyright Meiling 2020 <1467071@campus.euss.org>
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
 * @author	Meiling Chen <1467071@campus.euss>
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
 *V1.0|Meiling Chen|20-OCT-2020|Create File|
 * <h2><center>&copy;COPYRIGHT 2017 WELLCASA All Rights Reserved. </center>>/h2>
 */

 /**
  * @file				client_smtp.c
  *
  *						Librerias a incluir en el programa
  *						-----------------------------------------------
  \verbatim
         #ifdef HAVE_CONFIG_H
         #include <config.h>
         #endif
         #include <stdio.h>
         #include <stdlib.h>
         #include <sys/types.h>
         #include <sys/socket.h>
         #include <string.h>
         #include <sys/socket.h>
         #include <netinet/in.h>
         #include <arpa/inet.h>
         #include <unistd.h>
  \endverbatim
  */


  /**
   ** @file				client_smtp.c
   *
   *						Variables i constants a utilitzar en el programa
   *						-----------------------------------------------
   *
   \verbatim
     int c=0;
     char *nom_servidor1=NULL;
     char *email_destinatari1=NULL;
     char *email_remitent1=NULL;
     char    	nom_servidor[REPLY_MSG_SIZE];
     char	    email_destinatari[REPLY_MSG_SIZE];
     char	    email_remitent[REPLY_MSG_SIZE];
     char	    text_email[REPLY_MSG_SIZE];
     struct 		sockaddr_in	serverAddr;
     char	    BufRead[REPLY_MSG_SIZE];
     char	    BufWrite[REPLY_MSG_SIZE];
     int			sockAddrSize;
     int			sFd;
     int			result;
     #define REQUEST_MSG_SIZE	1024
     #define REPLY_MSG_SIZE		500
     #define SERVER_PORT_NUM		25


   \endverbatim
   */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>



#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		25

int email(char* nom_servidor, char* email_destinatari, char* email_remitent, char* text_email);


int main(int argc, char* argv[]) {
    int c = 0;
    char* nom_servidor1 = NULL;
    char* email_destinatari1 = NULL;
    char* email_remitent1 = NULL;

    char    	nom_servidor[REPLY_MSG_SIZE];
    char	    email_destinatari[REPLY_MSG_SIZE];
    char	    email_remitent[REPLY_MSG_SIZE];
    char	    text_email[REPLY_MSG_SIZE];


    /*strcpy(nom_servidor, "172.20.0.21");  //Asignar IP
    strcpy(email_destinatari, "1467071@campus.euss.org");	//Asignar email destinario
    strcpy(email_remitent, "usuari@campus.euss.org");	//Asignar  email remitente (puede ser inventado)*/
    strcpy(text_email, "Hola,\nSoy un email que envia automáticamente.\nSaludos\n");	//el texto que queremos enviar

    while ((c = getopt(argc, argv, "a:b:c:")) != -1) {
        switch (c) {

        case 'a':   //caso a: guardar el ip de servidor a través de optarg(option argument)

            nom_servidor1 = optarg;
            printf("Nom de servidor es %s\n", nom_servidor1);
            break;  //Terminar el caso

        case 'b':   //caso b: guardar el email remitente a través de optarg(option argument)

            email_remitent1 = optarg;
            printf("El email remitente es %s\n", email_remitent1);
            break;  //Terminar el caso

        case 'c':   //caso c: guardar el email destinatario a través de optarg(option argument)

            email_destinatari1 = optarg;
            printf("El email destinatario es %s\n", email_destinatari1);
            break;  //Terminar el caso

        case '?':  //(caso contrario)

            printf("Opción incorrecta\nIntroduzca 'a' para el IP de servidor, b para el email de remitente y c para el email dedestinatario.\n");
            return 1;   //leave the main fuction and return a value one to it. So our program is going to report an error. So we leave the program at this point.

        }
    }

    strcpy(nom_servidor, nom_servidor1);
    strcpy(email_remitent, email_remitent1);
    strcpy(email_destinatari, email_destinatari1);

    email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email

    return 0;

}


        /**
                *@brief
                *Funció email: enviar un correu
                * @param[in]	nom_servidor: IP del servidor
                * @param[in]	email_destinatari: adreça del e-mail destinatari
                * @param[in]	email_remitent: adreça del e-mail remitent
                * @param[in]	text_email: text del email que volem enviar
                *
                */


int email(char* nom_servidor, char* email_destinatari, char* email_remitent, char* text_email)
{
    struct 		sockaddr_in	serverAddr;
    char	    BufRead[REPLY_MSG_SIZE];
    char	    BufWrite[REPLY_MSG_SIZE];
    int			sockAddrSize;
    int			sFd;
    int			result;



    /*Crear el socket*/
    sFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sFd < 0)
    {
        perror("socket");
        exit(1);
    }

    /*Construir l'adreça*/
    sockAddrSize = sizeof(struct sockaddr_in);
    bzero((char*)&serverAddr, sockAddrSize); //Posar l'estructura a zero
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT_NUM);
    serverAddr.sin_addr.s_addr = inet_addr(nom_servidor);

    /*Conexió*/
    result = connect(sFd, (struct sockaddr*)&serverAddr, sockAddrSize);
    if (result < 0)
    {
        perror("connect");
        exit(1);
    }
    printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

    //presentacio servidor
    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    if (result < 0) {
        perror("read presentacio");
        exit(1);
    }
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //HELO

    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "HELO Rpi29.iotlab.euss.es\n");		//inicia la comunicación con 'HELO'
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));
    if (result < 0) {
        perror("write");
        exit(1);
    }
    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //MAIL FROM:

    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "MAIL FROM:%s\n", email_remitent);		//Indica la dirección de email remitente
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));

    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //RCPT TO:
    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "RCPT TO:%s\n", email_destinatari);		//Indica la dirección de email destinatario
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));

    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //DATA:

    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "DATA\n");
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));		//Envia el comandamiento DATA

    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //Message:
    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "Subject: Email de Proba\nFrom:%s\nTo:%s\n%s\n\n.\n", email_remitent, email_destinatari, text_email); 	//Envia la estructra del mensaje
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));

    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    //QUIT
    memset(BufWrite, 0, REPLY_MSG_SIZE);
    sprintf(BufWrite, "QUIT\n");
    printf("Enviat; -> %s\n", BufWrite);
    result = write(sFd, BufWrite, strlen(BufWrite));

    memset(BufRead, 0, REPLY_MSG_SIZE);
    result = read(sFd, BufRead, REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n", result, BufRead);

    close(sFd);

    return 0;

}
