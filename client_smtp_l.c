/***************************************************************************
                          main.c  -  client
                             -------------------
    begin                : lun feb  4 16:00:04 CET 2002
    copyright            : (C) 2002 by A. Moreno
    email                : amoreno@euss.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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

int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email);
static int verbose_flag;

int main(int argc, char *argv[]){
    int c;
    char *nom_servidor1=NULL; 
    char *email_destinatari1=NULL; 
    char *email_remitent1=NULL;
    
    char    	nom_servidor[REPLY_MSG_SIZE];
    char	    email_destinatari[REPLY_MSG_SIZE];
	char	    email_remitent[REPLY_MSG_SIZE];
    char	    text_email[REPLY_MSG_SIZE];        

while (1)
    {
      static struct option long_options[] =
        {
			
          {"IP",     required_argument,       0, 'a'},
          {"remitent",  required_argument,       0, 'b'},
          {"destinatari",  required_argument, 0, 'c'},
          {0, 0, 0, 0}
        };
    int option_index = 0;
	/*strcpy(nom_servidor, "172.20.0.21");  //Asignar IP 
	strcpy(email_destinatari, "1467071@campus.euss.org");	//Asignar email destinario
	strcpy(email_remitent, "usuari@campus.euss.org");	//Asignar  email remitente (puede ser inventado)*/
	strcpy(text_email, "Hola,\nSoy un email que envia automáticamente.\nSaludos\n");	//el texto que queremos enviar
	
	 c = getopt_long (argc, argv, "a:b:c:",long_options, &option_index);
	 
	 if (c == -1)
     break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
            
          printf ("option %s", long_options[option_index].name);
          
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

            case '?':  //(caso contrario)

                printf("Opción incorrecta\nIntroduzca 'a' para el IP de servidor, b para el email de remitente y c para el email dedestinatario.\n");
                return 1;   //leave the main fuction and return a value one to it. So our program is going to report an error. So we leave the program at this point.

			default:
				abort ();
        }
    }
	

		strcpy(nom_servidor,nom_servidor1);
        strcpy(email_remitent,email_remitent1);
        strcpy(email_destinatari,email_destinatari1);
        
	email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email
 
	return 0;
	
	}
	
	
int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email)
{
	struct 		sockaddr_in	serverAddr;
	char	    BufRead[REPLY_MSG_SIZE];
    char	    BufWrite[REPLY_MSG_SIZE];
	int			sockAddrSize;
	int			sFd;
	int			result;

	
	
	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);
	if(sFd < 0)
	{
		perror("socket");
		exit(1);
	}
		
	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(nom_servidor);

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
        perror("connect");
		exit(1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",	inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	
    //presentacio servidor
    memset(BufRead,0,REPLY_MSG_SIZE);
    result=read(sFd,BufRead,REPLY_MSG_SIZE);
    if (result<0){
        perror("read presentacio");
        exit(1);
    }
    printf("Rebut(%d): -> %s\n",result,BufRead);

    //HELO

    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"HELO Rpi29.iotlab.euss.es\n");		//inicia la comunicación con 'HELO'
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));
    if(result<0){
        perror("write");
        exit(1);
    }
    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);
    
    //MAIL FROM:
    
    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"MAIL FROM:%s\n",email_remitent);		//Indica la dirección de email remitente
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));

    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);
    
    //RCPT TO:
    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"RCPT TO:%s\n",email_destinatari);		//Indica la dirección de email destinatario
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));

    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);   
    
    //DATA:

    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"DATA\n");
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));		//Envia el comandamiento DATA

    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);

    //Message:
    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"Subject: Email de Proba\nFrom:%s\nTo:%s\n%s\n\n.\n",email_remitent,email_destinatari,text_email); 	//Envia la estructra del mensaje
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));

    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);
    
    //QUIT
    memset(BufWrite,0,REPLY_MSG_SIZE);
    sprintf(BufWrite,"QUIT\n");
    printf("Enviat; -> %s\n",BufWrite);
    result = write(sFd, BufWrite,strlen(BufWrite));

    memset(BufRead,0,REPLY_MSG_SIZE);
    result= read(sFd,BufRead,REPLY_MSG_SIZE);
    printf("Rebut(%d): -> %s\n",result,BufRead);    

	close(sFd);

	return 0;		
		
}
