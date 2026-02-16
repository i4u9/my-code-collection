#include "stringtype.h"
#include <time.h>

#define ELEMENTS_QUANTITY_MAXDIGITS 21
#define ELEMENT_MAXSIZE 255

#define MODEL_MAXSIZE 510

uint32_t get_elements_quant(FILE* file);
String*  get_elements(uint32_t elements_quant, FILE* file);
bool     load_elements(uint32_t* elem_quant, String** e, char* filepath);
void     unload_elements(String** e, uint32_t elements_quantity);

char     get_placeholder(FILE* modelfile);
bool     get_model(String* model, FILE* modelfile, uint64_t* modelsize);
bool     load_model(char* placeholder, String* model, uint32_t* placeholder_quant, char* filepath);
void     unload_model(String* mod);

uint32_t gen_random_num(uint32_t limit);
void     init_random_arr(int32_t* array, uint32_t size);
void     gen_random_array(int32_t* array, uint32_t size, uint32_t limit);

int main()
{
    uint32_t elements_quant = 0;
    String*  elements       = NULL;
    String   model = {0};
    char     placeholder = 0;
    uint32_t placeholder_quant = 0;

    if (!load_elements(&elements_quant, &elements, "src.txt")) return EXIT_FAILURE;

    if (!load_model(&placeholder, &model, &placeholder_quant, "model.txt")) return EXIT_FAILURE;
    
    //////////////////////////////
    uint32_t elements_to_insert = 0;
    (placeholder_quant >= elements_quant) ? (elements_to_insert = elements_quant) : (elements_to_insert = placeholder_quant);
    
    int32_t random_arr[elements_to_insert];
    init_random_arr(random_arr, elements_to_insert);

    srand(time(0));
    gen_random_array(random_arr, elements_to_insert, elements_quant);
    
    for (uint32_t i = 0; i < elements_to_insert; i++)
    {
        insert_string(&model, elements[random_arr[i]], strcspn(model.buffer, "1"));
    }
    
    printf("%s\n", model.buffer);

    unload_elements(&elements, elements_quant);
    unload_model(&model);
    return EXIT_SUCCESS;
}


uint32_t get_elements_quant(FILE* file)
{
    char buff[ELEMENTS_QUANTITY_MAXDIGITS];
    if (!fgets(buff, ELEMENTS_QUANTITY_MAXDIGITS, file))
    {
        fprintf(stderr, "error: failed getting elements quantity\n");
        return 0;
    }

    return (uint32_t)atol(buff);
}

String* get_elements(uint32_t elements_quant, FILE* file)
{
    String* elements = (String*)calloc(elements_quant, sizeof(String));
    if (!elements)
    {
        fprintf(stderr, "error: failed to alocate size to elements array\n");
        return NULL;
    }

    for (uint32_t i = 0; i < elements_quant; i++)
    {
        elements[i] = create_string_s(ELEMENT_MAXSIZE);
        if (!fgets(elements[i].buffer, ELEMENT_MAXSIZE, file))
        {
            fprintf(stderr, "error: failed to get elements; stoped at %d\n", i);
            return NULL;
        }

        uint64_t newline_at = strcspn(elements[i].buffer, "\n");
        if (!reallocate_string(&elements[i], newline_at + 1, newline_at)) return NULL;        
    }

    return elements;
}

bool load_elements(uint32_t* elem_quant, String** e, char* filepath)
{ 
    FILE* sourcefile = fopen(filepath, "r");
    if (!sourcefile)
    {
        fprintf(stderr, "error: failed to open source file\n");
        return false;
    }

    *elem_quant = get_elements_quant(sourcefile);
    if (*elem_quant == 0) return false;

    *e = get_elements(*elem_quant, sourcefile);
    if (!*e) return false;
    
    fclose(sourcefile);
    return true;
}

void unload_elements(String** e, uint32_t elements_quantity)
{
    for (uint32_t i = 0; i < elements_quantity; i++)
    {
        destroy_string(&(*e)[i]);
    }
    free(*e);
    *e = NULL;
}

char get_placeholder(FILE* modelfile)
{
    char buff[2];
    if (!fgets(buff, 2, modelfile))
    {
        fprintf(stderr, "error: failed to get placeholder\n");
        return 0;
    }
    fseek(modelfile, 1, SEEK_CUR);
    
    return buff[0];
}

bool get_model(String* model, FILE* modelfile, uint64_t* modelsize)
{
    char tmp = 0;
    uint32_t i = 0;
    while ((tmp = fgetc(modelfile)) != EOF)
    {
        model->buffer[i] = tmp;
        i++;
    }

    *modelsize = i - 1;

    return true;
}

bool load_model(char* placeholder, String* model, uint32_t* placeholder_quant, char* filepath)
{
    FILE* modelfile = fopen(filepath, "r");
    if (!modelfile)
    {
        fprintf(stderr, "error: failed to open model file\n");
        return false;
    }

    // placeholders can only be 1 character
    *placeholder = get_placeholder(modelfile);
    if (*placeholder == 0) return false;

    uint64_t newsize = 0;
    *model = create_string_s(MODEL_MAXSIZE);
    if (!get_model(model, modelfile, &newsize)) return false;
    if (!reallocate_string(model, newsize + 1, newsize)) return false;
    
    fclose(modelfile);

    for (uint32_t i = 0; i < model->size; i++)
    {
        if (model->buffer[i] == *placeholder)
        {
            (*placeholder_quant)++;
        }
    }
    return true;
}

void unload_model(String* mod)
{ 
    destroy_string(mod);
}

uint32_t gen_random_num(uint32_t limit)
{
    return (uint32_t)(rand() % limit);
}

void init_random_arr(int32_t* array, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        array[i] = -1;
    }
}

void gen_random_array(int32_t* array, uint32_t size, uint32_t limit)
{
    if (limit < size)
    {
        fprintf(stderr, "error: risc of inf loop generating random array\n");
        return;
    }

    for (uint32_t i = 0; i < size; i++)
    {
        int32_t num = 0;

        random_again:
            num = gen_random_num(limit);
        for (uint32_t j = 0; j < i; j++)
        {
            if (num == array[j])
            {
                goto random_again;
            }
        }   
        array[i] = num;
    }
}