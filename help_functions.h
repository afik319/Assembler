
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>


typedef enum {false, true} bool;

extern char* inst_Arr[16];
extern char* reg_names_arr[8];
extern char* directions_names_arr[4];

char* copy_word (const char* line, int* index);
void skip_spaces (int* index,const char* line);
int args_counter (char* inst_line);
int string_to_sign_int(char* data_line, int* index);
void skip_commas(int* index, const char* line);
void* safe_malloc(size_t size);
void safe_free_double_p(void **ptr);
void safe_free(void **ptr);
char* path_string (const char *filename,char* suffix);
FILE* safe_fopen (const char *filename, const char *mode, char* suffix);
void* safe_calloc(size_t num_elements, size_t element_size);
void resize_int_arr(int** arr, int* size);
/*Method d: for char** realloc , Method c: for char* , Method i: for int*.   */
void safe_realloc(char *** c_3ptr , char** c_2ptr , int** i_ptr , size_t size , char method);
void bool_safe_fopen(FILE** fptr , char* path , bool* valid);
bool is_null_file_content(char* path);
int is_all_white(char* line);
/*Checks whether the char is space or one of those: \t\f\v\r */
int is_white(char c);
/* Increases index to the index of the first non white char in the line (till \n) */
void clean_white(char* line , int* index);
