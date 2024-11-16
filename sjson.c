#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void json_get_var(const char *json_string,const char *key,char *value_buffer){

    const char *key_start=json_string+1;
    // search for key like string, until we reach end of json_string
    // or we find key like string wraped arround with double quote ("")
    while(!(*(key_start-1)=='"' && *(key_start+strlen(key))=='"')){
        key_start = strstr(json_string, key);
        if (key_start == NULL) {
            value_buffer[0] = '\0';
            return;
        }
    }

    //  search for `:`
    const char *colon_pos = strchr(key_start, ':');
    if (colon_pos == NULL) {
        value_buffer[0] = '\0';
        return;
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
    
    const char *value_start = colon_pos;
    const char *value_end = value_start;
    if(quoted_value){
        value_end = strchr(value_start, '"');
    }else{
       while(*value_end && *value_end!=' ' && *value_end!='\t' && *value_end!=','){
        value_end++;
       }

    }
    if (value_end == NULL) {
        value_buffer[0] = '\0';
        return;
    }
    size_t value_length = value_end - value_start;
    strncpy(value_buffer, value_start, value_length);
    value_buffer[value_length] = '\0';
}

int json_get_var_int(const char * json_string,const char *key_string){
    char value_buffer[1024];
    json_get_var(json_string, key_string, value_buffer);
    return atoi(value_buffer);
}

char json_get_var_char(const char * json_string,const char *key_string){
    char value_buffer[1024];
    json_get_var(json_string, key_string, value_buffer);
    return value_buffer[0];
}


long json_get_var_long(const char * json_string,const char *key_string){
    char value_buffer[1024];
    json_get_var(json_string, key_string, value_buffer);
    return atol(value_buffer);
}

double json_get_var_double(const char * json_string,const char *key_string){
    char value_buffer[1024];
    json_get_var(json_string, key_string, value_buffer);
    return atof(value_buffer);
}

char * json_get_var_string(const char * json_string,const char *key_string){
    char *value_buffer = calloc(sizeof(char),1024);
    json_get_var(json_string, key_string, value_buffer);

    #ifdef __SJS_MALLOC_TRACK__
        __SJS_A[__SJS_C++] = value_buffer;
    #endif
    return value_buffer;
}
#ifdef __SJS_MALLOC_TRACK__
void free_recent_malloc(){
    __SJS_C--;
    free(__SJS_A[__SJS_C]);
    __SJS_A[__SJS_C]=NULL;

}
void free_all_malloc(){
    for(size_t i=0;i<__SJS_C;i++){
        if(__SJS_A[i]){
            free(__SJS_A[i]);
            __SJS_A[i]=NULL;
        }
    }
}
void free_n_recent_malloc(size_t n){
    // chose the smaller one
    size_t until = __SJS_C<n?__SJS_C:n;
    for(size_t i=__SJS_C-1;i>=__SJS_C-until;i--){
        if(__SJS_A[i]){
            free(__SJS_A[i]);
            __SJS_A[i]=NULL;
        }
    }
}


#endif





