/**
* @file     informe.c
* @brief
* Inici : Martes 13 OCT 2020
* Grup : G
* Clase : AT-20
* Signature : ADRST
* Membre:  Xavi Pueyo Castillo/  Meiling Chen
 *
 */

 /**
 * @file		informe.c
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
 * Aquest fitxer forma part de el projecte\n
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
 *
 * @file				informe.c
 *
 *						Descripció programa
 *						-----------------------------------------------
	Aquest fitxer es tracta de guardar Data/hora inici, Data/hora final, valor màxim, valor mínim i valor mig en un fitxer de format txt.
*/

/**
  *
  * @file				informe.c
  *
  *						Llibreries a incloure en el programa
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
	#include <time.h>
	#include <sqlite3.h>
	#include <malloc.h>
  \endverbatim
  */


  /**
   ** @file				informe.c
   *
   *						Variables i constants a utilitzar en el programa
   *						-----------------------------------------------
   *
   \verbatim
	char numid;//identificador del sensor amb el que s'està treballanat
	char *pnumid;//puntero a la posición donde se guarda numid;

	char llistaidsens[10]; //vector on es guarden els identificadors de sensors  que existeixen fins a 9;
	int j; //index per recorrer vector llistaidsens
	char nomBD[]="basedades_adstr.db"; //nom de la base de dades on es guarden les mesures de temperatura
	int trovat=1; //variable per controlar si s'ha trobat o no el nom del sensor en la taula de sensors
	char indsensor[5]; //guarda el valor de l'id_sensor, registres comú de les dues taules mesures i sensors
	char descripcio[50]; //camp per omplir al donar d'alta un nou sensor amn una descripcio fins a 50 caracters
	int seleccio=0; //permet fer servir la mateixa funció per diferents accions aprofitant la part comú
	float valmin=3000.00;
	float valmax=-20.00;
	float suma=0;
	float media=0;
	int cont=0;
	char *TempIni,*TempFin;
   \endverbatim
*/


/**
  *
  * \fn static int callback(void *data, int argc, char **argv, char **azColName) {
  *
  *	\brief La funció callback es reitera tantes vegades com registres hi ha per cada vegada que es crida a sqlite_exe
  *	@param Els paràmetres introduïts són propis d'aquesta funció normalitzada.
  *	\return No retorna cap valor.
  *
	\verbatim

   int i;
   float temp;
   
   printf("HAY %d COLUMNAS(s)\n", argc);
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      printf("El registro en la columna \"%s\" es: %s\n y se verifica con: %s\n", azColName[i], argv[i],pnumid);
      //part del programa per obtenir max, min i mitjana de l'informe//
      if ((strcmp(argv[i],pnumid)==0)&&(seleccio==1)&&(i==0)){
		  trovat=0;
		  printf("Hi ha mesures fetes amb aquest sensor: %s\n", pnumid);
		  printf("La mesura es: %s\n", argv[i+1]);
		  temp=(atof(argv[i+1]));//converteix un string a float
      	  if (temp<valmin){
			  valmin=temp;
			  }
		  if (temp>valmax){
			  valmax=temp;
			  }
		  printf("La data de la mesura es: %s\n", argv[i+2]);
		  printf("El maximo hasta ahora es:%.2f\n", valmax);
		  printf("El minimo hasta ahora es:%.2f\n", valmin);
		  if (cont==0){//Entra quan s'ha realitzat la primera lectura del sensor. Quan canvia de sensor, la variable cont es reinicia a 0.
			  strcpy(TempIni,argv[i+2]);
			  strcpy(TempFin,argv[i+2]);//quan es posa el primer registre cal considerar el temps inicial i el final el mateix
			  }else{ 					//en previsió de que només hi hagi un registre assegurar que haurà data apuntada.
				  strcpy(TempFin,argv[i+2]);
				  }
		  suma=suma+temp;
		  printf("La suma acumulada vale:%.2f\n", suma);
		  printf("Cont vale: %i\n", cont);
		  cont++;
		  }
		//part del programa per determinar quants identificadors de sensors diferents hi ha//  
		if (seleccio==2){
			llistaidsens[j]=*argv[i];
			printf("El id de sensor en el vestor llista es: %c\n",llistaidsens[j]);
			j++;
			}
	  }
   printf("\n");
   if ((trovat!=0)&&(seleccio==1)){
	   printf("No hi han mesures amb aquest sensor: %c\n.", numid);
	   }
	
   return 0;
}
	\endverbatim
  */

/**
  *
  * \fn int quants_sensors(char *BD) {
  *
  *	\brief La funció quants_sensor es tracta de analitzar quants sensors tenim pel projecte
  *	@param Els paràmetres introduïts són propis d'aquesta funció normalitzada.
  *	\return No retorna cap valor.
  *
	\verbatim

 
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";
  
   
   sql=malloc (sizeof(char)*90);
     
   // Open database 
   rc = sqlite3_open(BD, &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /// Crea consulta per determinar quants sensors hi ha 
    
   	sprintf(sql,"SELECT id_sensor FROM sensors\n");
  // Execute SQL statement 
	seleccio=2;
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    seleccio=0;
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	
   sqlite3_close(db);
   free(sql);
   printf("\nSensors comptats\n");
   return 0;
}

	\endverbatim
*/

/**
  *
  * \fn int filtre_dades(char *BD, char numid) {
  *
  *	\brief La funció filtre_dades es tracta de crear consulta per accedir a les dades necessaries per fer l'informe 
  *	@param Els paràmetres introduïts són propis d'aquesta funció normalitzada.
  *	\return No retorna cap valor.
  *
	\verbatim

   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";
  
   
   sql=malloc (sizeof(char)*90);
      
   // Open database 
   rc = sqlite3_open(BD, &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
	//------inicialització de les variables per l'informe.---------
	// S'han de inicialitzar per cada sensor que es vulgui informar
	valmin=3000.00;
	valmax=-20.00;
	suma=0;
	media=0;
	cont=0;

   // Crea consulta per accedir a les dades necessaries per fer l'informe 
   
	sprintf(sql,"SELECT id_sensor, valor,temps FROM mesures WHERE id_sensor= %c\n",numid);
  
  // Execute SQL statement 
  
	seleccio=1;
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    seleccio=0;
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	
   sqlite3_close(db);
   free(sql);
   printf("\nDades filtrades\n");
   return 0;
}
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
#include <time.h>
#include <sqlite3.h>
#include <malloc.h>


char numid;//identificador del sensor amb el que s'està treballanat
char *pnumid;//puntero a la posición donde se guarda numid;

char llistaidsens[10];//vector on es guarden els identificadors de sensors  que existeixen fins a 9;
int j;/*index per recorrer vector llistaidsens*/
char nomBD[]="basedades_adstr.db";//nom de la base de dades on es guarden les mesures de temperatura
int trovat=1;//variable per controlar si s'ha trobat o no el nom del sensor en la taula de sensors
char indsensor[5];//guarda el valor de l'id_sensor, registres comú de les dues taules mesures i sensors
char descripcio[50];//camp per omplir al donar d'alta un nou sensor amn una descripcio fins a 50 caracters
int seleccio=0;//permet fer servir la mateixa funció per diferents accions aprofitant la part comú
float valmin=3000.00;
float valmax=-20.00;
float suma=0;
float media=0;
int cont=0;
char *TempIni,*TempFin;



//----Comprovació de l'existència de sensor en la base de dades--------------//
//-- y en cas de no ser-hi, el dona d'alta per seguir amb el registre-------//
//la funció callback es reitera tantes vegades com registres hi ha per cada vegada que es crida a sqlite_exe//

static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   float temp;
   
   printf("HAY %d COLUMNAS(s)\n", argc);
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      printf("El registro en la columna \"%s\" es: %s\n y se verifica con: %s\n", azColName[i], argv[i],pnumid);
      //part del programa per obtenir max, min i mitjana de l'informe//
      if ((strcmp(argv[i],pnumid)==0)&&(seleccio==1)&&(i==0)){
		  trovat=0;
		  printf("Hi ha mesures fetes amb aquest sensor: %s\n", pnumid);
		  printf("La mesura es: %s\n", argv[i+1]);
		  temp=(atof(argv[i+1]));//converteix un string a float
      	  if (temp<valmin){
			  valmin=temp;
			  }
		  if (temp>valmax){
			  valmax=temp;
			  }
		 /*printf("La data de la mesura es: %s\n", argv[i+2]);
		  printf("El maximo hasta ahora es:%.2f\n", valmax);
		  printf("El minimo hasta ahora es:%.2f\n", valmin);*/
		  if (cont==0){//Entra quan s'ha realitzat la primera lectura del sensor. Quan canvia de sensor, la variable cont es reinicia a 0.
			  strcpy(TempIni,argv[i+2]);
			  strcpy(TempFin,argv[i+2]);//quan es posa el primer registre cal considerar el temps inicial i el final el mateix
			  }else{ 					//en previsió de que només hi hagi un registre assegurar que haurà data apuntada.
				  strcpy(TempFin,argv[i+2]);
				  }
		  suma=suma+temp;
		/*  printf("La suma acumulada vale:%.2f\n", suma);
		  printf("Cont vale: %i\n", cont);*/
		  cont++;
		  }
		//part del programa per determinar quants identificadors de sensors diferents hi ha//  
		if ((seleccio==2)/*&&(llistaidsens[j]!=*argv[i])*/){
			llistaidsens[j]=*argv[i];
			printf("El id de sensor en el vestor llista es: %c\n",llistaidsens[j]);
			j++;
			}
	  }
   printf("\n");
   if ((trovat!=0)&&(seleccio==1)){
	   printf("No hi han mesures amb aquest sensor: %c\n.", numid);
	   }
	
   return 0;
}

int quants_sensors(char *BD) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";
  
   
   sql=malloc (sizeof(char)*90);
     
   /* Open database */
   rc = sqlite3_open(BD, &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Crea consulta per determinar quants sensors hi ha */
    
   	sprintf(sql,"SELECT id_sensor FROM sensors\n");
  /* Execute SQL statement */
	seleccio=2;
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    seleccio=0;
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	
   sqlite3_close(db);
   free(sql);
   printf("\nSensors comptats\n");
   return 0;
}




int filtre_dades(char *BD, char numid) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";
  
   
   sql=malloc (sizeof(char)*90);
      
   /* Open database */
   rc = sqlite3_open(BD, &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
	/*------inicialització de les variables per l'informe.---------*/
	/* S'han de inicialitzar per cada sensor que es vulgui informar*/
	valmin=3000.00;
	valmax=-20.00;
	suma=0;
	media=0;
	cont=0;

   /* Crea consulta per accedir a les dades necessaries per fer l'informe */
   
	sprintf(sql,"SELECT id_sensor, valor,temps FROM mesures WHERE id_sensor= %c\n",numid);
  
  /* Execute SQL statement */
  
	seleccio=1;
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    seleccio=0;
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	
   sqlite3_close(db);
   free(sql);
   printf("\nDades filtrades\n");
   return 0;
}

int informe(){
	
	j=0;
	pnumid = &numid;
	memset(llistaidsens,'0',10);//reinicia el llistat de sensors existents
	quants_sensors(nomBD);//obte el llistst de sensors existents
	j=0;
	while ((llistaidsens[j]!='0')&&(j<10)){
		*pnumid=llistaidsens[j];
		printf("Estamos con el id_sensor: %c\n",*pnumid);
		TempIni=malloc(sizeof(char)*25);
		TempFin=malloc(sizeof(char)*25);
		filtre_dades(nomBD,*pnumid);
		media=suma/cont;
		/*printf("La medida mínima es : %.2f\n",valmin);
		printf("La medida máxima es:  %.2f\n",valmax);	
		printf("La medida promedio es: %.2f\n",media); 
		printf("El inicio de la medida es: '%s'\n",TempIni); 
		printf("El fin de la medida es : '%s'\n",TempFin);*/
		
		
		//crearfichero(y);
		FILE *archivo;
		archivo=fopen("basedades_adstr.txt","at");//mode "a" crea el fitxer si no existeix i si no afegeix dades al final- mode text
		fprintf(archivo,"Sensor: %c\nData/hora inici: '%s'\nData/hora final: '%s'\nValor màxim: %.2f V\nValor mínim: %.2f V\nValor mig: %.2f V\n\n\n",*pnumid,TempIni,TempFin,valmax,valmin,media);
		fclose(archivo);
		//printf("text_archivo:%s\n",archivo);
		j++;
		free(TempIni);
		free(TempFin);

     }
    
   return 0;
	
	}
