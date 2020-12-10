 #include "email.h"
 #include "informe.h"
 
 int main()
 {
 	char* 		nom_servidor1 = NULL;
	char* 		email_destinatari1 = NULL;
	char* 		email_remitent1 = NULL;
    
	char    	nom_servidor[REPLY_MSG_SIZE];/*guarda la ip del servidor de mail*/
	char	    	email_destinatari[REPLY_MSG_SIZE];
	char	    	email_remitent[REPLY_MSG_SIZE];
	char	    	text_email[REPLY_MSG_SIZE];
	
	static struct 	option long_options[] = {
		 {"IP",     required_argument,       0, 'a'},
         {"remitent",  required_argument,       0, 'b'},
         {"destinatari",  required_argument, 0, 'c'},
		 {0,0,0,0}
	};
	
	int option = 0;
	while ((option = getopt_long(argc, argv, "s:r:a:b:c:", long_options, NULL)) != -1)
	{
		switch (option) 
		{
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
		case '?': 
			printf("\nOpción seleccionada errónea-%c.\n", optopt);
			break;
		}
	}
	
	informe();		//Llamar la función informe
	
	strcpy(nom_servidor,nom_servidor1);
	strcpy(email_remitent,email_remitent1);   
	strcpy(email_destinatari,email_destinatari1);
	
	char buf[1024];
	FILE *f;
	int len;
	if((f=fopen("basedades_adstr.txt", "r")) == NULL)//Abro el fichero en modo lectura, compruebo que se ha abierto correctamente
	{
	  printf("Error.No se ha podido abrir el fichero");
	  exit(1);
	}
	
	//Mientras no alcancemos el End of line del archivo
	while (!feof(f))	//leer dentro del bufer; fread regresa el número de bytes leídos
	{
		len = strlen(buf);
	}
	
	//sprintf(text_email,"From: <%s>\nTo: <%s>\nSubject:Resultado del sensor\n\n\nHola,\nLe adjunto el resultado del sensor.\nRESULTADOS DEL SENSOR LM35\n\n\t\tSensor: %d\n\t\tData/hora inici: '%s'\n\t\tData/hora final: '%s'\n\t\tValor màxim: %.2f V\n\t\tValor mínim: %.2f V\n\t\tValor mig: %.2f V\n\n\nSaludos\n\r\n",email_remitent,email_destinatari,*pnumid,TempIni,TempFin,valmax,valmin,media);
	sprintf(text_email,"From: <%s>\nTo: <%s>\nSubject:Resultado del sensor\n\n\nHola,\nLe adjunto el resultado del sensor.\nSaludos\n\r\nRESULTADOS DEL SENSOR LM35\n\n\t\t",email_remitent,email_destinatari);
	strcat(text_email,buf);
	//printf("text:%s\n",text_email);
	
	email(nom_servidor, email_destinatari, email_remitent, text_email);		//Llamar la función email
      
  return 0;
 }
