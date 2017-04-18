/*
 * StringAlg.h
 *
 *  Created on: Jul 30, 2014
 *      Author: alexandru
 */

#ifndef STRINGALG_H_
#define STRINGALG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#ifdef ASTOX_COMP_CPP
//	#include <iostream>
//	#include <stdlib.h>
//#else
//
//#endif

static inline char * append(char * data, char rest)
{
	size_t len = strlen(data);
	char *str2 = (char *) malloc(len + 1 + 1 ); /* one for extra char, one for trailing zero */
	strcpy(str2, data);
	str2[len - 2] = rest;
	str2[len - 1] = '\0';

	/*
	size_t len = strlen(response);
			char *str2 = (char *) malloc(len + 1 + 1 ); // one for extra char, one for trailing zero 
			strcpy(str2, response);
			str2[len] = input[i];
			str2[len + 1] = '\0';
			response = str2;
	*/
	return str2;
}


static inline char * getStem(const char * input)
{

	char * response = (char *) malloc(1*sizeof(char));
	// memset (response,0x0,sizeof(char));
	//char response[2];
	 int len = strlen(input);
	for(int i = len ; i > 0 ; i--)
	{
		
		if(input[i] == '.')
		{
			break;
		}
		printf("index: %i, char: %c\n",i, input[i]);
		

		int size = strlen(response);
		char* tmp = (char *) malloc(size + 2); // one for null, two for pp 
		*tmp = input[i]; 
		strcpy(tmp+1, response);
		response = tmp;
		//memmove(response+1,response,strlen(response));
		//response[0] = input[i];

		printf("response: %s length = %i \n",response, strlen(response));
		//strcat();
		//size_t len = strlen(response);
		//	char *str2 = (char *) malloc(len + 1 + 1 ); // one for extra char, one for trailing zero 
		//	strcpy(str2, response);
		//	str2[len] = input[i];
		//	str2[len + 1] = '\0';
		//	response = str2;
		//free(str2);
	}


	return response;
}



struct CharList{
	char * list[3];
	int length;

	CharList(){
		length=0;

	};

	void pushBack(char * data)
	{
		list[length] = data;
		length = length+1;
		//list =  realloc(list, length * sizeof(list));
	};
};

static inline void split(char str[], const char  * delimiter, const int tmp)
{

//	char * in = "test";
//	char c = '2';
//	char * result = append(in, c);
//	printf("%s", result);
//		 ="- This, a sample string.";

	//copiaza inputul ca sa nu fie afectat:



	int i = 0;
	const int numOfStrings = 128;
	char* strArray[numOfStrings];

	char *buf1 = (char*)calloc(strlen(str)+1, sizeof(char));
	strcpy(buf1, str);

	char * pch;

	  printf ("Splitting string \"%s\" into tokens using |%s| :\n",str, delimiter);
	  pch = strtok (buf1, delimiter);


	  while (pch != NULL)
	  {
		strcpy(strArray[i], pch);
	    printf ("-----%s\n",pch);
	    pch = strtok (NULL, delimiter);
	    i++;
	  }
	  printf("last token: %s\n", pch);
	  //sterge temporarele:
	  free(buf1);
	  free(pch);




}

#endif /* STRINGALG_H_ */
