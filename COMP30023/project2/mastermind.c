/* mastermind.c
 *
 * COMP30023 Computer Systems
 * Project 2 
 * Semester 1, 2016
 *
 * by Shi Chang Zhang, 695434, shiz2
 *
 * Last modified: 25/5/2016
 *
 * Description: This contains the functions needed for
 * the Mastermind game. 
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "mastermind.h"

// Checks if code is valid.
int validcode(char *code){
	if(strlen(code)!=CODE_SIZE){
		return NOT_VALID;
	}
	int i,j,valid;
	char valid_colours[] = VALID_COLOURS;
	for(i=0;i<CODE_SIZE;i++){
		valid = 0;
		for(j=0;j<NUM_COLOURS;j++){
			if(toupper(code[i])==valid_colours[j]){
				valid = 1;
			}
		}
		if(!valid){
			return NOT_VALID;
		}
	}

	return VALID;
}

// Checks if two codes are the same, returns -1 when out of moves
int makeguess(char *guess, char *code, int moves){
	if(rightincode(guess,code) == CODE_SIZE){
		return CORRECT;
	}
	if(moves == 1){
		return OUT_OF_MOVES;
	}
	return INCORRECT;
}

// Counts how many characters in guess are in code.
int countincode(char *guess, char *code){
	int i,j;
	int count = 0;
	int guess_count;
	int code_count;
	char valid_colours[] = VALID_COLOURS;	
	for(i=0;i<NUM_COLOURS;i++){
		guess_count = 0;
		code_count = 0;
		for(j=0;j<CODE_SIZE;j++){
			if(toupper(guess[j]) == valid_colours[i]){
				guess_count++;
			}
			if(toupper(code[j]) == valid_colours[i]){
				code_count++;
			}
		}
		if(code_count <= guess_count){
			count += code_count;
		} else {
			count += guess_count;
		}
	}
	return count;
}

// Counts how many characters in guess match with code.
int rightincode(char *guess, char *code){
	int i;
	int count = 0;
	for(i=0;i<CODE_SIZE;i++){
		if(toupper(guess[i]) == code[i]){
			count++;
		}
	}
	return count;
}

// Generates random code.
void randomcode(char *code){
	srand(time(NULL));
	int i,r;
	char colours[] = VALID_COLOURS;
	for(i=0;i<CODE_SIZE;i++){
		r = rand()%NUM_COLOURS;
		code[i] = colours[r];
	}	
}
