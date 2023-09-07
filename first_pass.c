#include "first_pass.h"


char* errors[] = {"",
                  "Error: Invalid first word.\n",
                  "Error: Invalid data name.\n",
                  "Error: Unnecessary label definition before .entry or .extern.\n",
                  "Error: Label name can't be an instruction opcode.\n",
                  "Error: Arguments amount is not valid.\n",
                  "Error: Arguments address method is not valid.\n",
                  "Error: Instruction argument is not valid.\n",
                  "Error: Illegal comma at the end of the direction word.\n",
                  "Error: Missing comma.\n",
                  "Error: Illegal comma.\n",
                  "Error: Multiple consecutive commas.\n",
                  "Error: extern label can't be defined in the same file.\n",
                  "Error: invalid char after closing quote.\n",
                  "Error: Direction line without content.\n",
                  "Error: .data argument value is not in range, should be -2048 to 2047.\n",
                  "Error: label name is longer then 31 characters.",
                  "Error: Label first character should be A-Z or a-z character.\n",
                  "Error: Invalid label name (characters should be digits or alphabets).\n",
                  "Error: Immediate number range should be -512 to 511.\n",
                  "Error: first word is too long for label definition / direction / instruction.",
                  "Error: comma in the line beginning.\n",
                  "Error: label can't be defined in the same file.\n",
                  "Error: out of memory.\n",
                  "Warning: enrty label not defined in this file.\n",
                  "Error: data array contain invalid chars\n",
                  "Error: two non-digits in one number in data array\n",
                  "Error: Wrong First word or Invalid label name\n",
                  "Error: missing quote.\n",
                  "Error: char before quotes.\n"

};

bool first_valid = true;

bool first_pass(FILE *am, label_object **symbol_table[], int *st_size, int *capacity, line_data **ld_arr[], int *ld_arr_size,
           int *ic, int *dc, line_data **ld, label_object **new_label) {
    char line[MAX_LINE_SIZE];
    int i = 0;
    int line_num = 1;

    while (fgets(line, MAX_LINE_SIZE, am)) {/*put each line as a struct in the line_data array*/
        if(line[0] == ','){
            printf("In line %d:\n%s%s\n" , line_num , line , errors[COMMA_BEGIN]);
            first_valid = false;
            line_num++;
            continue;
        }
        (*ld) = (line_data*)safe_calloc(1 , sizeof(line_data));
        (*ld) = create_line_data(line, *ld);

        (*ld_arr)[i] = *ld;
        if ((*ld)->ei != SUCCESS) {
            printf("In line %d:\n", line_num);
            printf("%s", line);
            printf("%s\n", errors[(*ld)->ei]);
            if((*ld)->ei == INVALID_FIRST_WORD || (*ld)->ei == NO_CONTENT_DIRECTION ||
             (*ld)->ei == LONG_FIRST_WORD || (*ld)->ei == OPCODE_LABEL_NAME ||
             (*ld)->ei == WRONG_FIRST){
                safe_free((void**)ld);
                line_num++;
                continue;
            }      
        }
        line_num++;
        i++;
        if (i >= *ld_arr_size) {
            resize_ld_arr(ld_arr, ld_arr_size);
        }
    }
    *ld_arr = (line_data **) realloc(*ld_arr, i * sizeof(line_data *));
    *ld_arr_size = i;
    
    if (create_symbol_table(*ld_arr, *ld_arr_size, symbol_table, st_size, capacity, ic, dc, new_label)) {/*creating the symbol table of the file*/
        int k;
        for (k = 0; k < *st_size; ++k) {
            if((*symbol_table)[k]->is_data){
                (*symbol_table)[k]->label_value += *ic;
            }
        }
        if((*ic + *dc) > MEMORY_MAX_ADDRESS){
            printf("%s Asked: %d byets while only %d Available.\n" , errors[OUT_OF_MEMORY] , \
             (*ic + *dc - START_MEMORY_ADDRESS) , AVAILABLE_MEMORY);
            first_valid = false;
        }
        return first_valid;
    } 
    else first_valid = false;
        return first_valid;
}

bool create_symbol_table(line_data *ld_arr[], int ld_arr_size, label_object **symbol_table[], int* st_size, int* capacity,
                    int* ic, int* dc, label_object** new_label) {
    int i , search_result , search_index , search_value;
    char* temp_label_name;
    bool search_value_bool;

    temp_label_name = NULL;
    search_value_bool = true;
    /***symbol_table = (label_object*)safe_malloc(*capacity * sizeof(label_object));*/
    for (i = 0; i < ld_arr_size; i++) {
        search_result = search_index = search_value = NOT_INITIALIZE;
        if (ld_arr[i]->ei != SUCCESS) {
            return false;/*if there is an error, label table isn't necessary*/
        }
        
        if (ld_arr[i]->is_label_def){
            /*there is a label definition in the line*/
            search_value = label_value_search(ld_arr[i]->label_name , *symbol_table , *st_size , &search_value_bool);
            if(search_value > 0){
                printf("In line %d:\n", i + 1);
                printf("%s" , errors[TWICE_LABEL]);
                return false;
            }
            else if(search_value == 0){
                printf("In line %d:\n", i + 1);
                printf("%s" , errors[EXTERN_DEFINED]);
                return false;
            } 
            if (search_label((ld_arr[i]->label_name), *symbol_table, *st_size) == LABEL_NOT_EXIST){/*label isn't in the table - definition is valid.*/
                *new_label = (label_object *) safe_calloc(1, sizeof(label_object));
                if (ld_arr[i]->is_instruction) { /*instruction line*/
                    strcpy((*new_label)->label_name, ld_arr[i]->label_name);
                    (*new_label)->is_code = true;
                    (*new_label)->type = relocatable;
                    (*new_label)->label_value = *ic;
                    *ic += ld_arr[i]->inst->inst_line_keeper;
                } 
                else if (ld_arr[i]->is_direction) {/*direction line*/
                    (*new_label)->is_data = true;
                    strcpy((*new_label)->label_name, ld_arr[i]->label_name);
                    (*new_label)->type = relocatable;
                    (*new_label)->label_value = *dc;
                    *dc += ld_arr[i]->dir->dir_line_keeper;
                }
                add_to_symbol_table((*new_label), symbol_table, st_size, capacity);
            }/*end if - label isn't in the label table*/
            else if (search_label((ld_arr[i]->label_name), *symbol_table, *st_size) >= 0) { /*in label table and label of entry.*/
                int s_table_index = (search_label((ld_arr[i]->label_name), *symbol_table, *st_size));
                if((*symbol_table)[s_table_index]->is_entry) {/*entry label- update val*/
                    if (ld_arr[i]->is_instruction) {
                        (*symbol_table)[s_table_index]->is_code = true;
                        (*symbol_table)[s_table_index]->label_value = *ic;/*change val from -1 to the ic*/
                        *ic += ld_arr[i]->inst->inst_line_keeper;
                    } else if (ld_arr[i]->is_direction) {
                        (*symbol_table)[s_table_index]->is_data = true;
                        (*symbol_table)[s_table_index]->label_value = *dc;/*change val from -1 to the dc*/
                        *dc += ld_arr[i]->dir->dir_line_keeper;
                    }
                }
            }/* end if - entry label*/
            else if(search_label((ld_arr[i]->label_name), *symbol_table, *st_size) == LABEL_EXTERNAL){/*definition of extern label*/
                printf("In line %d:\n", i + 1);
                printf("Error: extern label: : %s can't be defined in the same file.\n", ld_arr[i]->label_name);
                return false;
            }
            else if(search_label((ld_arr[i]->label_name), *symbol_table, *st_size) == LABEL_RELOCATABLE){/*double label definition*/
                printf("In line %d:\n", i + 1);
                printf("%s" , errors[TWICE_LABEL]);/*label defined twice*/
                return false;
            }
       }
        else {/*not a label definition*/
            if (ld_arr[i]->is_direction) {
                if (ld_arr[i]->dir->d_type == d_entry) {/*add loop*/
                    int k, en_arr_size = ld_arr[i]->dir->d_content->en_arr->en_size;
                    for (k = 0; k < en_arr_size; ++k) {
                        temp_label_name = ld_arr[i]->dir->d_content->en_arr->entry[k];
                        search_result = search_label(temp_label_name , *symbol_table , *st_size);
                        if(search_result > LABEL_NOT_EXIST){
                            if(search_result == LABEL_RELOCATABLE){
                                search_index = label_index_search(temp_label_name , *symbol_table , *st_size);
                                (*symbol_table)[search_index]->is_entry = true;
                            }
                            else if(search_result == LABEL_EXTERNAL){
                                printf("In line %d:\n", i + 1);
                                printf("%s" , errors[EXTERN_DEFINED]);
                                temp_label_name = NULL;
                                return false;
                        } 
                            temp_label_name = NULL;
                            continue;
                        } 
                        (*new_label) = (label_object *) safe_calloc(1, sizeof(label_object));
                        strcpy((*new_label)->label_name, ld_arr[i]->dir->d_content->en_arr->entry[k]);
                        (*new_label)->is_entry = true;
                        (*new_label)->label_value = -1;/*-1 is entry label temp val*/
                        (*new_label)->type = relocatable;
                        add_to_symbol_table((*new_label), symbol_table, st_size, capacity);
                    }
                }
                else if (ld_arr[i]->dir->d_type == d_extern) {
                    int j, ex_arr_size = ld_arr[i]->dir->d_content->ex_arr->ex_size;
                    for (j = 0; j < ex_arr_size; ++j) {
                        temp_label_name = ld_arr[i]->dir->d_content->ex_arr->extern_[j];
                        search_result = search_label(temp_label_name , *symbol_table , *st_size);
                        if(search_result > LABEL_NOT_EXIST && search_result != LABEL_EXTERNAL){
                            printf("In line %d:\n", i + 1);
                            printf("%s" , errors[EXTERN_DEFINED]);
                            temp_label_name = NULL;
                            return false;
                        } 
                        (*new_label) = (label_object *) safe_calloc(1, sizeof(label_object));
                        (*new_label)->type = external;
                        (*new_label)->is_extern = true;
                        strcpy((*new_label)->label_name, ld_arr[i]->dir->d_content->ex_arr->extern_[j]);
                        (*new_label)->label_value = 0;
                        add_to_symbol_table((*new_label), symbol_table, st_size, capacity);
                    }
                }
                else {
                        *dc += ld_arr[i]->dir->dir_line_keeper;
                }
            }
            else if (ld_arr[i]->is_instruction) {
                *ic += ld_arr[i]->inst->inst_line_keeper;
            }
        }/*end of - not label def*/
    }/*end of for loop*/
    /*resize symbol table to the true size*/
    *symbol_table = (label_object **) realloc(*symbol_table, *st_size * sizeof(label_object *));
    return true;
}

void add_to_symbol_table(label_object *label, label_object ***symbol_table, int *size, int *capacity) {
    if (*size == *capacity) {
        resize_symbol_table(symbol_table, capacity);
    }
    (*symbol_table)[*size] = label;
    (*size)++;
}

int label_index_search(char* label_name, label_object *symbol_table[], int s_table_size){
    int i;
    for(i = 0; i < s_table_size; i++)
        if(strcmp(symbol_table[i]->label_name,label_name) == 0) {
            return i;
        }
    return ERROR_VAL;
}



int search_label(char* label_name, label_object *symbol_table[], int s_table_size){
    int i;
    for(i = 0; i < s_table_size; i++){
        if(strcmp(symbol_table[i]->label_name,label_name) == 0){
            if(symbol_table[i]->is_entry == true){
                return i;
            }
            if(symbol_table[i]->type == external){
                return LABEL_EXTERNAL;
            }
            else if(symbol_table[i]->type == relocatable){
                return LABEL_RELOCATABLE;
            }
        }
    }
    return LABEL_NOT_EXIST;/*the label doesn't exist in the symbol table*/
}

void resize_ld_arr(line_data*** arr, int* size) {
    *size += INITIAL_SIZE;
    line_data** temp_arr = (line_data**)realloc(*arr, (*size) * sizeof(line_data*));
    if (temp_arr == NULL) {
        printf("Memory reallocation failed.\n");
        free(*arr);
        exit(1);
    }
    *arr = temp_arr;
    }


void resize_symbol_table(label_object ***symbol_arr, int* capacity) {
    (*capacity) += INITIAL_SIZE;
    label_object **temp_arr = realloc(*symbol_arr, (*capacity) * sizeof(label_object *));
    if (temp_arr == NULL) {
        printf("Memory reallocation failed.\n");
        free(symbol_arr);
        exit(1);
    }
    *symbol_arr = temp_arr;
}

int label_value_search(char* label_name, label_object *symbol_table[], int s_table_size , bool* is_extern){
    int i , value;
    value = ERROR_VAL;
    for(i = 0; i < s_table_size; i++)
        if(strcmp(symbol_table[i]->label_name,label_name) == 0) {
            value = symbol_table[i]->label_value;
            *is_extern = symbol_table[i]->is_extern;
            break;
        }
    return value;
}
