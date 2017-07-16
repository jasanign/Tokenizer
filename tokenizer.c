/**
 * tokenizer.c - A simple token recognizer.
 *
 * NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
 *       program.
 *
 * @author Gajjan Jasani
 * @version 03/08/2016
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"

/* Global pointer to line of input */
char *line;
/* Global variable for statement number */
int num_of_statements = 1;
/* Global variable for number of tokens */
int token_cnt = -1; // -1 because first token is stored as token 0
/* Global pointer for output file */
FILE *out;

/**
 *  Function: main
 *  Purpose : The entry point to this program. This function
 *  makes sure the number of arguments provided matches the
 *  program requirements, calls low level functions to find
 *  all the tokens from the input file
 *  argc  : number of arguments provided
 *  argv[]: array to hold arguments
 */

int main(int argc, char* argv[]) {
   char  token[TSIZE];      /* Spot to hold a token, fixed size */
   char  input_line[LINE];  /* Line of input, fixed size */
   FILE  *in_file = NULL;   /* File pointer */
   FILE  *out_file = NULL;	/* File pointer */
   int   line_count,        /* Number of lines read */
         start,             /* start of new statement */
         count;             /* count of tokens */
   count = token_cnt;
   start = num_of_statements;

  if (argc != 3) {			// checking if 3 arguments are provided
    printf("Usage: tokenizer inputFile outputFile\n");
    exit(1);
  }

  in_file = fopen(argv[1], "r");
  if (in_file == NULL) {	// checking if input file is accessible
    fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
    exit(1);
  }

  out_file = fopen(argv[2], "w");
  out = out_file;
  if (out_file == NULL) {	// checking if output file is accessible
    fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
    exit(1);
  }
  // Reading one line at a time from the input file
  while (fgets(input_line, LINE, in_file) != NULL){
	   line_count++;
	   if(input_line[FIRST_ELE] == '\n'){	// if line is empty,
		   continue;  	   	   	   	   	   	//skip this iteration

	   }
      line = input_line;  	// Sets a global pointer to the memory location
                           	// where the input line resides.
      int tok_length;		// To keep track of character in token
      memset(token, 0, TSIZE); // Making sure token is clear before we use
      clean_input_line();	// Clearing out white spaces from input lint

      while (strlen(line)>0){   // iterate through the entire input line
    	  start++;
		  get_token(token);		// Find all the tokens in input line
		  count++;
		  tok_length = strlen(token);
		  memset(token, 0, TSIZE); // clean the token after each token
		  strcpy(line, line + tok_length); // remove the discovered token
      }									   // from the input line
   }
   fclose(in_file);		// close the inpute file
   fclose(out_file);	// close the output file
   return 0;
}

/**
* Function: get_token
* Purpose : To find one token or one error at a time from the input line
* and record that token or error on the output file
* token_ptr:	Pointer to the token string
*/
void get_token(char *token_ptr){
	char c;		// holding char from input line
	c = line[FIRST_ELE];

	if(isdigit(c)){		// if the first char is a digit
		handle_digit_token(c, token_ptr);
	}else if(check_lexeme(c)){ 	// If the char is part of our lexemes array
		handle_special_char_token(c, token_ptr);
	}else { 	// if the char is not a digit or a defined lexeme
		handle_error(c, token_ptr);
	}
}

//==================== HELPER FUNCTIONS =========================

/**
 * Function: check_lexeme
 * Purpose : Check if the char read from input line is part of our
 * lexeme definition (lexems array)
 * c: The char that needs to be checked
 * Return: 1 if c is part of our lexeme definition, 0 if not
 */
int check_lexeme(char c){

	int i = 0;
	while(i < strlen(lexemes)){
		if(lexemes[i] == c){
			return 1;	// early termination if c is part of our definition
		}
		i++;
	}
	return 0;
}

/**
 * Function: statement_num
 * Purpose: Check if a new statement has started or not. If new statement
 * is started, denote it in the output file right away
 */
void statement_num(){

	if(token_cnt == -1){
		fprintf(out, "Statement #%d\n", num_of_statements);
	};
}

/**
 * Function: clean_input_line
 * Purpose : Remove all the white spaces from the input line
 */
void clean_input_line(){

	int i=0, j=0;
	int len = (int)strlen(line);
	while (i != len) {
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'){
			line[j++] = line[i];
		}
		i++;
	}
	line[j]=0; // making sure there is a null character at the end of line
}

/**
 * Function: handle_digit_token
 * Purpose : If the char read from input line is a digit, check for more
 * subsequent digits to find the whole integer
 * c		: the first char that is a digit
 * token_ptr: pointer to the array that hold the token
 */
void handle_digit_token(char c, char *token_ptr){

	int k, j;	// counters
	j = k = 0;
	statement_num();
	token_cnt++; // increase token just by one no matter how many digits
				 // we find subsequently. It will be just 1 integer
	token_ptr[j]=c; // put the first digit on token array
	j++;
	k = FIRST_ELE + 1;
	while(k < strlen(line)){ // keep looking for next chars until we find
							 // a char that is not digit
		c = line[k];	// next char on the input line
		if(!isdigit(c)){
			fprintf(out, "Token %d is %s\n", token_cnt,token_ptr);
			return; // if next char is not a digit
					// end the token
		} else {	// if next char is a digit, keep looking
			token_ptr[j] = c;
			j++;
			k++;
		}
	}
	fprintf(out, "Token %d is %s\n", token_cnt,token_ptr);
}

/**
 * Function: handle_special_char_token
 * Purpose : If the char read from input line is a lexeme defined on our
 * lexemes array, write that on the output file
 * c		: the first char that is a legit lexeme
 * token_ptr: pointer to the array that hold the token
 */
void handle_special_char_token(char c, char* token_ptr){

	statement_num();
	token_cnt++; // incrementing token count
	if(c == ';'){ // special case 1: if ';', that means end of statement
		num_of_statements++; // incrementing number of statements
		token_ptr[FIRST_ELE] = c;
		fprintf(out, "Token %d is %s\n", token_cnt,token_ptr);
		fprintf(out, "---------------------------------------"
				"------------------\n");
		token_cnt = -1;
	}else {
		token_ptr[FIRST_ELE] = c;
		// special case 2: if the first char is <, >, !, or = then
		// check if the next char is = or not
		if((c == '<' || c == '>' || c == '!' || c == '=') &&
				(FIRST_ELE+1 < strlen(line))){
			if(line[FIRST_ELE+1] == '='){
				token_ptr[FIRST_ELE+1] = '=';
			}
		}
		fprintf(out, "Token %d is %s\n", token_cnt,token_ptr);
	}
}

/**
 * Function: handle_error
 * Purpose : If the char read from input line is not a lexeme defined on our
 * lexemes array, or not a digit, denote it as an error on the output file
 * c		: the first char that is a legit lexeme
 * token_ptr: pointer to the array that hold the token
 */
void handle_error(char c, char* token_ptr){

	token_ptr[FIRST_ELE] = c;
	fprintf(out, "===> '%c'\n", c);
	fprintf(out, "Lexical Error: not a lexeme\n");
}
