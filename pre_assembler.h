#include "line_parser.h"

#define MAX_GET_LEN 100
#define MAX_LINE_LEN 82
#define MAX_MCRO_NAME 30
#define START_MAX_MACROS_NUM 3
#define DIFF_INSTRUCTION_COUNT 15
#define DIFF_REG_COUNT 8
#define DIFF_DIR_COUNT 4
#define MCRO_LEN 4
#define ENDMCRO_LEN 7
#define END_LINE_CHARS 2
#define START_INSIDE_EACH_MCRO 4


typedef struct mcro_table* mcro_table_p;

/** Creates .am file after macro breakdown, and after adjustment to the template of 'line_template'. It is assumed
    that a call to a macro can only be in line that contains nothing except the macro name. */
int pre_as(char* path , char* am_file_name);
/** If one of the files is NULL, it prints error message, closes both, free the pointer to the name of second file
    and returns true. */
bool is_null_file(FILE *f1 , FILE *f2 , char **first_file_name , char **second_file_name);
/* Returns line in format: no white chars in the beginning, between every two words exactly one regular space. */
char* line_template(char* line);
/* Gets two strings and allocate memory for the "copy" variable" and then copies the original content to the "copy" variable. */
void str_copy(char **copy , char **original , bool start_no_spaces);
/* Gets two strings with same size of memory allocated and copies the second content to the first include null terminated. */
void string_copy(char *first , char *copy);
/*Given an optional macro declaration line, it ensures that it is new, valid, and has a legal name.*/
bool mcro_dec_line(int* index, int* index_copy, bool* mcro_flag, char** curr_line_index, char* curr_line,
                   char** mcro_name, mcro_table_p m1, char** line_copy, int line_number);
/* Method 1: list of strings to free, Method 2: list of file pointers to close. */
void free_or_close(int method , int count, ...);
/*Returns the index of the macro in the macro table or '-1' if not exist.*/
int is_exist_mcro (mcro_table_p m1 , char *mcro_name_to_check);
/*Method 1 to insert macro name and method 2 to insert macro content.*/
void insert_mcro(mcro_table_p m1 , char *mcro_name_to_check , char *line , int method);
/* Copies the first string to the second except one letter change. */
void strings_letter_change(int letter_index , char new_letter , char* first , char **after_change);
/*Puts to the variable "inside", the content of the macro when it is valid macro call line or with NULL if not valid.
  Valid mcro call is only when the macro name appears alone in line and exist already in the mcro table.*/
void is_mcro_line(mcro_table_p m1 , char* line , char **inside);
/*Ensure that it is not instruction name.*/
bool valid_mcro_name(char* mcro_name);
/*Realloc the count of available line in the mcro table to be twice the previous value*/
void mcro_table_count_increase(mcro_table_p m1);
/*Realloc the count of lines that specific macro in the table can contain to be twice the previous value*/
void mcro_table_line_increase(mcro_table_p m1 , int index);
void free_mcro_table(mcro_table_p m1);
mcro_table_p create_mcro_table();
/*When there is comma that comes after space, it puts the comma first */
void comma_switch(char* line , char* template_line , int* index , int* template_index);
/*Ensures that only one space separates between every two words in the line that will be inserted to the am file */
void one_space_separate(char* line , char* template_line , int* index , int* template_index);
/*Inserts the macro lines inside macro declaration to the mcro table right place till there is "endmcro" line and then
it Prepares the table for the next mcro insertion.*/
void line_inside_mcro_dec(char* curr_line , char* corrected_line , int* index , bool* mcro_flag , char* mcro_name , mcro_table_p m1);
/*Copy lines that are not related to macro, to the am file.*/
void regular_line_add(char* curr_line , char** corrected_line , int* index , FILE* fptr2);



