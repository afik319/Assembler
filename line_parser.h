//
// Created by elifu on 02/08/2023.
//

#ifndef ASSEMBLER_OPENU_PROJECT_LINE_PARSER_H
#define ASSEMBLER_OPENU_PROJECT_LINE_PARSER1_H
#define DIRECTION_NUM 4
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LABEL_SIZE 31
#define MAX_LINE_SIZE 82
#define VALID_ADDRSS_MTHDS 3
typedef enum bool {false, true} bool;

/*Instruction Line Data Structures:*/
typedef enum opcode_names {
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop,
    opcode_length,
    invalid
} opcode;

typedef enum address_type{
    immediate = 1,
    label = 3,
    reg = 5,
    none = 0
}address_type;

typedef struct op_args_method{
    opcode code;
    address_type src[VALID_ADDRSS_MTHDS];
    address_type dest[VALID_ADDRSS_MTHDS];
} op_args_method;

typedef struct instruction {
    op_args_method* op_address_method;
    int lines_keeper;
}instruction;

/*Direction Line Data Structures:*/

typedef struct data_int_arr{
    int* data_args;
    int size;
}data_int_arr;

typedef enum direction_type {
    d_string,
    d_data,
    d_entry,
    d_extern
}direction_type;

typedef union direction_content{
    data_int_arr* data;/*if it's a data*/
    char* string; /*if it's a string*/
    char* entry; /*if it's an entry*/
    char* extern_; /*if it's an extern*/
}direction_content;

typedef struct direction {
    direction_type d_type;
    direction_content* dc;
}direction;

/*LINE DATA STRUCTURES:*/

typedef union line_type {
    instruction* inst;
    direction* dir;
}line_type;

typedef struct line_data{

    char label_name[31];
    bool is_instruction;
    bool is_comment;
    bool is_empty_line;
    line_type* l_type; /*struct of instruction or direction*/
    /*direction* dir;
    direction_content* dir_cont;
    direction_type* d_type;
    data_int_arr* d_arr;
    instruction* inst;*/

}line_data;



/*function declarations*/

line_data* create_line_data(char* temp_line);
bool is_direction (char* word);
bool is_args_as_expected(op_args_method* op_args_to_validate,opcode code);
bool is_valid_string(char* string_line);
bool is_label_def (char* word);
void skip_spaces (int* index,const char* line);
char* copy_word (const char* line, int* index);
bool is_instruction(char* word);
direction_type which_data_type(char* word);
opcode which_instruction(char* word);
bool is_A_group(opcode code);
bool is_B_group(opcode code);
bool is_C_group(opcode code);
bool is_commas_valid(char* args);
char* copy_string(char* line,int* index);
bool is_more_args(char *line, int *index);
bool string_parser(char* line, char* args, line_data* ld, int* index)  ;
char *copy_s_args(char *string_line);
bool data_parser(char* temp_line, data_int_arr* d_arr, line_data* ld, int* index);
data_int_arr* copy_d_args(char* data_line);
bool is_valid_data(char* data_line);
int args_counter (char* inst_line);
bool inst_args_parser(char *temp_line, opcode code, int *index);
bool a_count_as_expected(opcode op, int args_c);
op_args_method* set_op_args(char *data_args, op_args_method* op_args_to_validate);
void src_add_set (char* args,op_args_method* op_add_method);
void dest_add_set (char* args,op_args_method* op_add_method);
bool is_immediate(char* args);
bool is_label(char* args);
bool is_register(char* args);



#endif //ASSEMBLER_OPENU_PROJECT_LINE_PARSER_H

