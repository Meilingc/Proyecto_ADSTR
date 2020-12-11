#include "email.h"
#include "informe.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define REPLY_MSG_SIZE		5000


float valmin=3000.00;
float valmax=-20.00;
float suma=0;
float media=0;
int cont=0;
char *TempIni,*TempFin;
char numid;//identificador del sensor amb el que s'està treballanat
char *pnumid;//puntero a la posición donde se guarda numid;

int main(int argc, char *argv[])
{
	char* 		nom_servidor1 = NULL;
    char* 		email_destinatari1 = NULL;
    char* 		email_remitent1 = NULL;
    
	char    	nom_servidor[REPLY_MSG_SIZE];//guarda la ip del servidor de mail
	char	    email_destinatari[REPLY_MSG_SIZE];
	char	    email_remitent[REPLY_MSG_SIZE];
	char	    text_email[REPLY_MSG_SIZE];
	static struct option long_options[] = {
		 {"IP",     required_argument,       0, 'a'},
         {"remitent",  required_argument,       0, 'b'},
         {"destinatari",  required_argument, 0, 'c'},
		 {0,0,0,0}
	};
	
	int option = 0;
	while ((option = getopt_long(argc, argv, "s:r:a:b:c:", long_options, NULL)) != -1)
	{
		switch (option) {
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
 
	
/*	
	char buf[MAX_LINE];  
	FILE *fp;                        
	if((fp = fopen("basedades_adstr.txt","r")) == NULL)
	{
		perror("fail to read");
		exit(1);
	}
	fgets(buf,MAX_LINE,fp);
	strcat(text_email,buf);
	printf("proba:%s\n",text_email);*/
	//email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email
      
	
	informe();
	//printf("text_archivo:%s\n",archivo);
	sprintf(text_email, "Hola,\nSoy un email que envia el resultado del sensor automáticamente.\nSaludos\n\nINFORME\n\n\tSensor: %c\n\tData/hora inici: '%s'\n\tData/hora final: '%s'\n\tValor màxim: %.2f V\n\tValor mínim: %.2f V\n\tValor mig: %.2f V\n\n\n",*pnumid,TempIni,TempFin,valmax,valmin,media);	//el texto que queremos enviar
	printf("text_email:%s\n",text_email);
	email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email
	
	return 0;
}

