#include "assembler.h"

int main(int argc, char* argv[]) {
    int i , j , ic , dc;
    bool pre_valid , first_valid;

    FILE *am;
    char *as_file_path;
    char *as_suffix = ".as";
    char *am_suffix = ".am";
    char am_name[FILE_NAME];

    int capacity;
    int ld_arr_size;
    int st_size;

    line_data** ld;
    label_object** new_label;
    line_data** ld_arr;
    label_object** symbol_table;

    ld = NULL;


    for (i = 1; i < argc; i++) {
        capacity = INITIAL_SIZE;
        ld_arr_size = capacity;
        st_size = 0;

        ic = 100, dc = 0;
        pre_valid = true;
        new_label = NULL;
        ld_arr = NULL;
        symbol_table = NULL;
        ld = NULL;
        
        if (strlen(argv[i]) > FILE_NAME) {
            printf("Filename is too long - %d characters max is allowed\n", FILE_NAME);
            continue;
        }

        as_file_path = path_string(argv[i], as_suffix);
        printf("\"%s\" file converting starts:\n\n" , as_file_path);
        pre_valid = pre_as(as_file_path , am_name);
        
        safe_free((void**)&as_file_path);
        
        if(pre_valid) {
            am = safe_fopen(argv[i], "r", am_suffix);
            symbol_table = (label_object **) safe_calloc(capacity, sizeof(label_object*));
            ld_arr = (line_data **) safe_malloc((ld_arr_size) * sizeof(line_data *));
            ld = (line_data **) safe_malloc(sizeof(line_data*));
            
            new_label = (label_object**)safe_malloc(sizeof(label_object*));

            first_valid = first_pass(am, &symbol_table, &st_size, &capacity, &ld_arr, &ld_arr_size, &ic, &dc, ld, new_label);
            if(first_valid) {
                second_pass(&symbol_table, &st_size, &ld_arr, &ic, &dc, ld_arr_size , argv[i]);            
            }
            free_ld_structs(ld_arr, ld_arr_size);
            for (j = 0; j < st_size ; ++j) {
                safe_free((void**)&symbol_table[j]);
            }
            safe_free((void**)&symbol_table);
            safe_free((void **)&new_label);
            safe_free((void **)&ld_arr);
            safe_free_double_p((void**)&ld);
            fclose(am);
            am = NULL;
        }
        else remove(am_name);
        printf("________________________________________\n\n");
    }
    return 0;
    }


void free_ld_structs(line_data **ld_arr, int ld_arr_size) {
    int i , j;

    for (i = 0; i < ld_arr_size; i++) {

        if (ld_arr[i]->is_direction && ld_arr[i]->dir != NULL) {
            if (ld_arr[i]->dir->d_type == d_string && ld_arr[i]->dir->d_content != NULL) {
                if(ld_arr[i]->dir->d_content->string != NULL)
                    safe_free_double_p((void **) &ld_arr[i]->dir->d_content->string->string);
                safe_free_double_p((void **) &ld_arr[i]->dir->d_content->string);
            }
            else if (ld_arr[i]->dir->d_type == d_data && ld_arr[i]->dir->d_content != NULL) {
                if(ld_arr[i]->dir->d_content->d_arr != NULL)
                    safe_free_double_p((void **) &ld_arr[i]->dir->d_content->d_arr->data_arr);
                safe_free_double_p((void **) &ld_arr[i]->dir->d_content->d_arr);
            } else if (ld_arr[i]->dir->d_type == d_entry && ld_arr[i]->dir->d_content != NULL && ld_arr[i]->dir->d_content->en_arr != NULL) {
                for (j = 0; j < ld_arr[i]->dir->d_content->en_arr->en_size ; j++)
                    safe_free_double_p((void**) &ld_arr[i]->dir->d_content->en_arr->entry[j]);
                if(ld_arr[i]->dir->d_content->en_arr != NULL) 
                    safe_free_double_p((void **) &ld_arr[i]->dir->d_content->en_arr->entry);
                safe_free_double_p((void **) &ld_arr[i]->dir->d_content->en_arr);
            } else if (ld_arr[i]->dir->d_type == d_extern && ld_arr[i]->dir->d_content != NULL && ld_arr[i]->dir->d_content->ex_arr != NULL) {
                for (j = 0; j < ld_arr[i]->dir->d_content->ex_arr->ex_size ; j++)
                    safe_free((void**)&ld_arr[i]->dir->d_content->ex_arr->extern_[j]);
                safe_free_double_p((void **) &ld_arr[i]->dir->d_content->ex_arr->extern_);
                safe_free_double_p((void **) &ld_arr[i]->dir->d_content->ex_arr);
            }
            safe_free_double_p((void **) &ld_arr[i]->dir->d_content);
            safe_free_double_p((void **) &ld_arr[i]->dir);
            safe_free_double_p((void **) &ld_arr[i]);
        }
        else if (ld_arr[i]->is_instruction && ld_arr[i]->inst != NULL) {
            safe_free((void **) &ld_arr[i]->inst->op_args_type);
            safe_free((void **) &ld_arr[i]->inst->src_name);
            safe_free((void **) &ld_arr[i]->inst->dest_name);
            safe_free((void **) &ld_arr[i]->inst);
            safe_free((void **) &ld_arr[i]);
        }
    }
}

