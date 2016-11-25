/* readcsv.c
 *
 * Created by: Shi Chang Zhang
 * Username: shiz2 SID:695434
 *
 * Last modified: 6/9/2015
 *
 * Description:
 * Definitions of error-checked malloc and realloc
 * and functions for reading from the input file
 * and stdin. 
 */
 
#include <stdio.h>
#include "readcsv.h"

void *emalloc(size_t size){
	/* Error checked malloc */
	void *p = malloc(size);
	if(p==NULL){
		exit(EXIT_FAILURE);	
	}
	return p;
}

void *erealloc(void *pointer, size_t size){
	/* Error checked realloc */
	void *p = realloc(pointer, size);
	if(p==NULL){
		exit(EXIT_FAILURE);	
	}
	return p;
}

void readField(char **field, int buffer_size, int max_size, FILE *file_ptr){
	/* Read field from input file until one of the Delimiters or EOF
	   is reached. */
	int c,n=0;
	char *buffer = (char *)emalloc(buffer_size);
	do{
		c=fgetc(file_ptr);
		if((c==DELIMITER1 || c==DELIMITER2 || c==EOF) && n>0){
			buffer[n]='\0';
			*field = (char *)emalloc((n+1)*sizeof(char));
			strcpy(*field, buffer);
		}
		else if(c!='\n' && c!='\r' && c!=EOF){
			/* Don't want to read these characters into
			   the field. */
			buffer[n]=c;
			n++;
			
			if(n>=buffer_size){
				/* reallocates more memory if n >= buffer_size */
				if(buffer_size*2 > max_size){
					buffer_size = max_size;	
				}
				else{
					buffer_size *= 2;
				}
				buffer=(char *)erealloc(buffer, buffer_size);
			}
		}
	}while(!(c==DELIMITER1|| c==DELIMITER2 || c==EOF));
	free(buffer);
	return;
}

int readLine(char **buffer){
	int c, n=0;
	while(n<MAX_NAME_SIZE-1 && (c = getchar())!='\n' && c!=EOF){
		if(c!='\n' && c!='\r' && c!=EOF){
			/* Don't want to read these characters into
			   the field. */
			(*buffer)[n++]=c;
		}
	}
	(*buffer)[n]='\0';
	return n;
}