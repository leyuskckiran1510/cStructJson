#include "sjson.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define TO_PUS(x) x=='{' || x=='[' || x=='"'
#define TO_POP(x,y) x=='{' && y=='}' || x==y && x=='"' || x=='['&& y==']'


void clean_null_terminators(char *buffer) {
	char stack = 0;
	int p1 = 0, p2 = 0;
	while (stack == '"' || buffer[p1]) {
		char c = buffer[p1];
		if (stack == c) {
			stack = 0;
		} else if (c == '"') {
			stack = c;
		}
		if (buffer[p1]) {
			buffer[p2++] = buffer[p1];
		}
		p1++;
	}
    // remove tailing comma and null terminate the string
    //       },
    buffer[p2-1-1] = buffer[p2-1];
	buffer[p2-1] = 0;
}

const char* valid_by_empty_stack(const  char *cur_pointer,const char cur_symbol){
    char stack_symbol[1024]={0};
    int sp = 0,ptr=0;

    stack_symbol[sp++] = cur_symbol;
    while(sp > 0 && *(cur_pointer+ptr)){
        char c =  *(cur_pointer+ptr);
        if(TO_PUS(c)){
            stack_symbol[sp++] = c;
        }else if(TO_POP(stack_symbol[sp-1],c)){
            stack_symbol[sp-1] = 0;
            sp-=2;
        }
        ptr++;
    }
    if(sp<1){
        return (cur_pointer+ptr);
    }else{
        return NULL;
    }
}

const char * json_get_var(const char *json_string,const char *key,size_t *length){

    const char *key_start=json_string+1;
    // search for key like string, until we reach end of json_string
    // or we find key like string wraped arround with double quote ("")
    while(key_start){
        key_start = strstr(key_start, key);
        if (key_start == NULL) {
            return NULL;
        }
        if((*(key_start-1)=='"' && *(key_start+strlen(key))=='"')){
            break;
        }
        key_start+=strlen(key);
    }

    //  search for `:`
    const char *colon_pos = strchr(key_start, ':');
    if (colon_pos == NULL) {
        return NULL;
    }
    // consume `:`
    colon_pos++;
    //consume ` `(space) or `\t`(tab)
    while (*colon_pos == ' ' || *colon_pos == '\t') {
        colon_pos++;
    }

    // now the next char must be `"`
    int quoted_value = 0;
    if (*colon_pos == '"') {
        //consume `"`
        colon_pos++;
        quoted_value = 1;
    }
    
    // now for array and nested json
    char nested_symbol = 0;
    if(*colon_pos=='[' || *colon_pos == '{'){
        // store and consume this symbol
        nested_symbol = *colon_pos;
        colon_pos++;
    }

    const char *value_start = colon_pos;
    const char *value_end = value_start;
    if(quoted_value){
        value_end = strchr(value_start, '"');
    } else if(nested_symbol=='['|| nested_symbol == '{'){
        value_end = valid_by_empty_stack(value_start,nested_symbol);
    }
    else{
       while(*value_end && *value_end!=' ' && *value_end!='\t' && *value_end!=','){
        value_end++;
       }

    }
    if (value_end == NULL) {
        return NULL;
    }

    
    *length = value_end - value_start;
    return value_start;
}

#define MAX_INT_SIZE 100
int json_get_var_int(const char * json_string,const char *key_string){
    char value_buffer[MAX_INT_SIZE];
    const char *start_from;
    size_t length = 0;
    start_from = json_get_var(json_string, key_string, &length);
    if(length < MAX_INT_SIZE){
        strncpy(value_buffer,start_from,length);
        return atoi(value_buffer);
    }
    assert(1!=1 && "Please Increase the MAX_INT_SIZE");
}

char json_get_var_char(const char * json_string,const char *key_string){
    const char *start_from;
    size_t length = 0;
    start_from = json_get_var(json_string, key_string, &length);
    return *start_from;
}


#define MAX_LONG_SIZE 100
long json_get_var_long(const char * json_string,const char *key_string){
    char value_buffer[MAX_LONG_SIZE];
    const char *start_from;
    size_t length = 0;
    start_from = json_get_var(json_string, key_string, &length);
    if(length < MAX_LONG_SIZE){
        strncpy(value_buffer,start_from,length);
        return atol(value_buffer);
    }
    assert(1!=1 && "Please Increase the MAX_LONG_SIZE");
}

#define MAX_DOUBLE_SIZE 100
double json_get_var_double(const char * json_string,const char *key_string){
    char value_buffer[MAX_DOUBLE_SIZE];
    const char *start_from;
    size_t length = 0;
    start_from = json_get_var(json_string, key_string, &length);
    if(length < MAX_DOUBLE_SIZE){
        strncpy(value_buffer,start_from,length);
        return atof(value_buffer);
    }
    assert(1!=1 && "Please Increase the MAX_DOUBLE_SIZE");
}

char * json_get_var_string(const char * json_string,const char *key_string){
    const char *start_from;
    size_t length = 0;
    start_from = json_get_var(json_string, key_string, &length);

    char *value_buffer = calloc(sizeof(char),length);
    
        __SJS_A[__SJS_C++] = value_buffer;
    
    strncpy(value_buffer, start_from,length);   
    return value_buffer;
}


#define IS_int(x)  x>='0' && x<='9'
#define IS_long(x)  IS_int(x)
#define IS_SPACE(x) x==' ' || x=='\t' || x=='\v' || x=='\f' || x=='\n'


#define whole_number_consume(x,to_call) \
x consume_##x(const char* string,size_t *consumed){\
    char buffer[MAX_INT_SIZE]={0};\
    size_t buff_ptr = 0;\
    size_t ptr=0;\
    while(IS_SPACE(string[ptr])){\
        ptr++;\
    }\
    while(string[ptr] && IS_##x(string[ptr])){\
        buffer[buff_ptr] = string[ptr];\
        ptr++;\
        buff_ptr++;\
    }\
    buffer[buff_ptr]=0;\
    if(string[ptr]){\
        *consumed  =  ptr;\
        return to_call(buffer);\
    }else{\
        *consumed = -1;\
        return -1;\
    }\
}\

whole_number_consume(int,atoi);
whole_number_consume(long,atol);

#define IS_float(x) '0'<=x && x<='9' || x=='.'
#define IS_double(x) IS_float(x)

#define decimal_number_consume(x,to_call) \
x consume_##x(const char* string,size_t *consumed){\
    char buffer[MAX_INT_SIZE]={0};\
    size_t buff_ptr = 0;\
    size_t ptr=0;\
    while(IS_SPACE(string[ptr])){\
        ptr++;\
    }\
    int decimal=0;\
    while(string[ptr] && IS_##x(string[ptr]) || (!decimal && string[ptr]=='.')){\
        buffer[buff_ptr] = string[ptr];\
        ptr++;\
        buff_ptr++;\
    }\
    buffer[buff_ptr]=0;\
    if(string[ptr]){\
        *consumed  =  ptr;\
        return to_call(buffer);\
    }else{\
        *consumed = -1;\
        return -1;\
    }\
}\

decimal_number_consume(float,atof);
decimal_number_consume(double,atof);



#define array_parser(x) \
x* parse_##x##_array(const char* string_ptr,size_t length){\
    if(string_ptr==NULL) return NULL;\
    x* array  = calloc(sizeof(x),length/2);\
    size_t a_ptr=0;\
    size_t ptr=0;\
    while(string_ptr[ptr] && string_ptr[ptr]!=']'){\
        size_t shift_by=0;\
        array[a_ptr++] = consume_##x((string_ptr+ptr),&shift_by);\
        if(shift_by<0){\
            return array;\
        }\
        ptr+=shift_by;\
        while(string_ptr[ptr] && (IS_SPACE(string_ptr[ptr]) || string_ptr[ptr]==',')){\
            ptr++;\
        }\
    }\
        __SJS_A[__SJS_C++] = array;\
    __SJS_recent_array_size = a_ptr;\
    return array;\
}\

#define list U(int) U(float) U(double) U(long)
#define U(x) array_parser(x)
list
#undef U

#define JsonToTypeArray(x) \
x * json_get_var_##x##_array(const char * json_string, const char* key_string){\
    const char *start_from;\
    size_t length = 0;\
    start_from = json_get_var(json_string, key_string, &length);\
    return parse_##x##_array(start_from, length);\
}
#define U(x) JsonToTypeArray(x)
list
#undef U

#define TypeArrayToString(type,max_string,fmt) \
char * type##_array_to_string(type *items,size_t count){\
    char * string = calloc(sizeof(char),count*(max_string + 2));\
    char *temp = string;\
    __SJS_A[__SJS_C++] = string;\
    string[0] = '[';\
    temp+=1;\
    for(size_t i=0;i<count;i++){\
        size_t write_count=0;\
        write_count = sprintf(temp,fmt,items[i]);\
        temp+=write_count;\
        *temp++=',';\
        *temp++=' ';\
    }\
    *(temp-2)=']';\
    *(temp-1) = 0;\
    return string;\
}\


TypeArrayToString(int,11, "%d");
TypeArrayToString(long,32, "%ld");
TypeArrayToString(float,16, "%f");
TypeArrayToString(double,32, "%lf");




// memory free related task
// use costume arena for memeory allocatoin


void free_recent_malloc(){
    __SJS_C--;
    free(__SJS_A[__SJS_C]);
    __SJS_A[__SJS_C]=NULL;

}
void free_all_malloc(){
    for(size_t i=0;i<__SJS_C;i++){
        if(__SJS_A[i]!=NULL){
            free(__SJS_A[i]);
            __SJS_A[i]=NULL;
        }
    }
}
void free_n_recent_malloc(size_t n){
    // chose the smaller one
    size_t until = __SJS_C<n?__SJS_C:n;
    for(size_t i=__SJS_C-1;i>=__SJS_C-until;i--){
        if(__SJS_A[i]!=NULL){
            free(__SJS_A[i]);
            __SJS_A[i]=NULL;
        }
    }
}







