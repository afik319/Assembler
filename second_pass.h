
#include "first_pass.h"

#define MEMORY_START_ADDRESS 100
#define BINARY_LENGTH 12
#define BINARY_LAST_INDEX 11
#define RIGHT_BINARY_INDEX 6
#define LEFT_LAST_BINARY_INDEX 5
#define ADDRESS_TYPES 13
#define ERROR -1
#define OPCODE_INDEX 3
#define DEST_INDEX 7
#define A_R_E_INDEX 10
#define SECOND_REG_INDEX 5
#define REG_NUM_STR_INDEX 2
#define REG_BINARY_DIGITS 5
#define ENT_SUFFIX_LEN 5
#define EXT_SUFFIX_LEN 5
#define OB_SUFFIX_LEN 4
#define BINARY_TABLE_COUNT 2
#define BASE_64_DIGITS 64
#define OPCODES_COUNT 16
#define OPCODES_LEN 4
#define CODING_TYPES 4
#define CODING_TYPE_LEN 3
#define A_R_E_TYPES 3
#define A_R_E_DIGITS 2
#define REG_WORD_DIGITS 5
#define DIFF_CODING_OPTIONS 13
#define LINE_CODING_MODES 3
#define BINARY_DIGITS 2
#define SECOND_BINARY_LINE 2
#define THIRD_BINARY_LINE 3
#define MAX_ADDRESS_BIN_DIGITS_STR 11
#define ENTRY_LABEL_ONLY -1
#define EXTERNAL_LABEL_ONLY -1

typedef struct binary_table* binary_table_p;

typedef enum coding_type{
    non_non,
    non_imm,
    non_label,
    non_reg,
    imm_imm,
    imm_label,
    imm_reg,
    label_imm,
    label_label,
    label_reg,
    reg_imm,
    reg_label,
    reg_reg
}coding_type;

/*Converts the lines of ld_arr to binary and then to base 64, also Creates entries and externals files for the labels.
It also checks if all the labels of instructions are defined in the file. */
int second_pass(label_object **symbol_table[], int* st_size , line_data **ld_arr[], int *ic, int *dc , int lines_count,\
                char* file_name);
binary_table_p new_binary_table(int lines_count , int start_address);
/*Get a number and binary line in the binary table and put the binary representaion of the number in the table line.
  Method 1 converts to 12 digits, method 2 converts to 10 digits and put them in the left 10 digits in the line.*/
void intToTwosComplement(int num, int* binary_line , int method);
void free_binary_table(binary_table_p b1 , int lines_count);
/*Creates and fill the entries and externals files*/
void entries_and_externals_file(label_object **symbol_table[], int* st_size , char* file_name);
/*Converts the line to its binary representation and puts it in the binary table When it is instruction line*/
int inst_coding_type(line_data **ld_arr[] , int line_number , int **binary_line , int binary_curr_index ,\
                     label_object **symbol_table[], int* st_size);
bool delete_null_file(char* path);
/*converts the line second and third binary lines by its source and dest types*/
void binary_extra_lines_insert (line_data **ld_arr[] , int line_number , int **binary_line , int binary_curr_index,\
                                label_object **symbol_table[], int* st_size , int s_type , int d_type , int code_type);
/*Gets two binary tables for instruction lines and direction line and create base64 file of it.*/
void binary_to_64(int** inst_binary_table , int** dir_binary_table , char* path , int inst_len , int dir_len);
/*Insert title to the base64file based on the count of instructions lines and count of directions lines in the am files.*/
void binary_file_title_insert(FILE* fptr , int inst_len , int dir_len);
/*Gets both instructions and directions binary table and converts the inst binary table if it is the first time by variable "time"
and the directions binary table if it is the second time by variable, it puts the result to the base64 file.*/
void binary_line_convert(FILE* fptr , int time , int*** curr_table , int** inst_binary_table , int** dir_binary_table,\
                         int* curr_len , int inst_len , int dir_len);
/*Converts instruction line of two registers to binary and puts the result in the binary table */
void two_reg_binary_line(line_data **ld_arr[] , int line_number , int **binary_line , int binary_curr_index ,\
                         int* reg_s_num , int* reg_d_num);
/*Creates the .ent and .ext files without content. */
void labels_files_creation(char** first , char** second , char* file_name , FILE **ent_fptr , FILE **ext_fptr);
/*Converts and put the binary first line of the line inside the binary table. */
void binary_first_line_insert(line_data **ld_arr[] , int line_number , int **binary_line , int binary_curr_index,\
                              int found_index , int* coding_type);
/*Converts ".string" direction line and puts the result inside the binary table. */
void direction_string_convert(line_data **ld_arr[] , int* char_as_ascii , int index ,\
                              binary_table_p direction_binary_table);
/*Converts ".data" direction line and puts the result inside the binary table. */
void direction_data_convert(line_data **ld_arr[] , int* data_count , int index, binary_table_p direction_binary_table);
/*Ensures that label operand is defined in the file */
void direction_label_dec_valid(label_object **symbol_table[], int* st_size , line_data **ld_arr[] , int index);
/*The "operand_str" variable can be the source name or the dest name, also the operand_type variable can be
s_type or d_type, the Method make them point the right option by the extra line number.
line number 2 is for source name and line number 3 is for dest name*/
void binary_extra_lines_adjustment(line_data **ld_arr[] , int line_number , int extra_line_num , char **operand_str,\
                                   int *operand_type  , int s_type , int d_type);
/*When the operand is label, the method converts its value to binary and put it in the instruction binary line. */
void binary_label_line(label_object **symbol_table[] , int **binary_line , int binary_curr_index , int *label_val,\
                       char *operand_str , int* st_size , bool *is_extern_label);
/*When it is instruction line and need to convert one of the operands, the method navigate the operand
to the right convertios - of immediate or label or register convert. */
void binary_opernd_handle(int **binary_line , label_object **symbol_table[] , int* imm , int operand_type,\
                          char *operand_str , int* first_str_index , int binary_curr_index , int* label_val,\
                          int* st_size, bool *is_extern_label, int* reg_num, int extra_line_num, int *reg_start_index);





