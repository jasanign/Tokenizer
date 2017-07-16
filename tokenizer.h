/**
 * tokenizer.h - Header file for the tokenizer project
 *
 * @author Gajjan Jasani
 * @version 03/08/2016
 *
 */
/* Constants */
#define LINE 100
#define TSIZE 20
#define TRUE 1
#define FALSE 0
#define FIRST_ELE 0

/* Lexemes array */
char lexemes[12] = {'+', '-', '*', '/', '(', ')', '^', '=', '<', '>', '!', ';'};

/**
* Function prototypes
*/
void get_token(char *);
int check_lexeme(char c);
void statement_num();
void clean_input_line();
void handle_digit_token(char c, char *token_ptr);
void handle_special_char_token(char c, char* token_ptr);
void handle_error(char c, char* token_ptr);


