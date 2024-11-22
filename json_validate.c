#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define IS_SPACE(x) x==' '||x=='\t'||x=='\n'||x=='\v'||x=='\f'||x=='\r'
#define IS_SIGN(x) x=='+' || x=='-'
#define IS_DIGIT(x) x>='0' && x<='9'
#define IS_EXPONEN(x) x=='e' || x=='E'
#define RETURN_FALSE  { printf("at func %s:%d (%c) -- %s -- \n",__FUNCTION__,__LINE__,*(string+ptr),(string+ptr)) ; return false;}

bool consume_object(char*string,size_t *consumed);
bool consume_value(char *string,size_t *consumed);

size_t skip_whitespace(char *string){
    size_t ptr =0;
    while(IS_SPACE(string[ptr])){
        ptr++;
    }
    return ptr;
}


bool consume_string(char *string,size_t *consumed){
    size_t ptr=0;
    //consume white spaces
    ptr+=skip_whitespace((string+ptr));
    
    if(string[ptr]!='"') return 0;
    ptr++;
    while(string[ptr] && string[ptr]!='"'){
        // skipp next character if escape code
        if(string[ptr]=='\\'){
            ptr++;
        }
        ptr++;
    }
    if(string[ptr]=='"'){
        ptr++;
        *consumed+=ptr;
        return true;
    }
    return false;
}

size_t skip_digits(char *string){
    size_t ptr =0;
    while(IS_DIGIT(string[ptr])){
        ptr++;
    }
    return ptr;
}

bool consume_array(char *string,size_t *consumed){
    size_t ptr=0;
    if(string[ptr]!='[') RETURN_FALSE;
    // consume '['
    ptr++;
    // skip white space
    ptr+=skip_whitespace((string+ptr));

    if(string[ptr]==']'){
        *consumed = ptr;
        return true;
    }

    while(consume_value((string+ptr),&ptr)){
        ptr+=skip_whitespace((string+ptr));
        if(string[ptr]==']'){
            ptr++;
            *consumed+=ptr;
            return true;
        }
        if(string[ptr]!=',') RETURN_FALSE;
        ptr++;
        ptr+=skip_whitespace((string+ptr));
    }
    RETURN_FALSE;
}

bool consume_number(char *string,size_t *consumed){
    size_t ptr=0;
    // consume signs
    if(IS_SIGN(string[ptr])){
        ptr++;
    }
    bool is_fraction = false;
    // only fractions can start with zeros
    if(string[ptr]=='0'){
        // consume 0
        ptr++;
        if (string[ptr]!='.')RETURN_FALSE;
        // consume .
        ptr++;
        is_fraction = true;
    }

    ptr+=skip_digits((string+ptr));
    // as we already encountered '.'
    if(string[ptr]=='.' && is_fraction) RETURN_FALSE;
    // else conusme the '.'
    if(string[ptr]=='.'){
        ptr++;
        // we cannot have exponent right after '.'
        if(IS_EXPONEN(string[ptr]))RETURN_FALSE;
        // now consume all numbers
        ptr+=skip_digits((string+ptr));
    }

    if(IS_EXPONEN(string[ptr])){
        ptr++;
        if(IS_SIGN(string[ptr])){
            ptr++;
        }
    }
    ptr+=skip_digits((string+ptr));
    // now skip all white spaces
    ptr+=skip_whitespace((string+ptr));
    // after consuming number their must be either ','  or '}' 
    if(string[ptr]!=',' && string[ptr]!='}'&& string[ptr]!=']') RETURN_FALSE;
    *consumed+=ptr;
    return true;
}

bool consume_value(char *string,size_t *consumed){
    size_t ptr=0;
    ptr+=skip_whitespace(string);
    switch(string[ptr]){
        case '"':{
            if(consume_string((string+ptr),&ptr)){
                ptr+=skip_whitespace(string);
                *consumed+=ptr;
                return true;
            }
            RETURN_FALSE;
        }
        case '{':{
            if(consume_object((string+ptr),&ptr)){
                ptr+=skip_whitespace(string);
                *consumed+=ptr;
                return true;
            }
            RETURN_FALSE;
        }
        case '-': 
        case '+':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':{
            if(consume_number((string+ptr),&ptr)){
                *consumed+=ptr;
                return true;
            }
            RETURN_FALSE;
        }
        case '[':{
            if(consume_array((string+ptr),&ptr)){
                *consumed+=ptr;
                return true;
            }
            RETURN_FALSE;
        }
        case 't':
        case 'f':
        case 'n':{
            if(!strcmp((string+ptr),"true")){
                *consumed+=strlen("true");
                return true;
            }
            if(!strcmp((string+ptr),"false")){
                *consumed+=strlen("false");
                return true;
            }
            if(!strcmp((string+ptr),"null")){
                *consumed+=strlen("null");
                return true;
            }
            RETURN_FALSE;
        }
    }
    RETURN_FALSE;
}

bool consume_object(char *string,size_t *consumed){
    size_t ptr=0;
    // must start from {
    if(string[ptr] != '{') RETURN_FALSE;
    //consume '{' 
    ptr++;
    ptr+=skip_whitespace((string+ptr));
    // if it's empty object return True early
    if(string[ptr]=='}'){
        *consumed+=ptr;
        return true;
    }

    while(string[ptr] && string[ptr]!='}'){
        // consume string -> key
        if(!consume_string((string+ptr),&ptr)) RETURN_FALSE;
        // consume white spaces
        ptr+=skip_whitespace((string+ptr));
        if(string[ptr]!=':') RETURN_FALSE;
        ptr++;
        // consume white spaces
        ptr+=skip_whitespace((string+ptr));
        if(!consume_value((string+ptr),&ptr)) RETURN_FALSE;
        // consume white spaces
        ptr+=skip_whitespace((string+ptr));
        if(string[ptr]!=',' && string[ptr]!='}') RETURN_FALSE;
        if(string[ptr]==','){
            ptr++;
            ptr+=skip_whitespace((string+ptr));
            if(string[ptr]!='}'){
                continue;
            }
            RETURN_FALSE;
        }
    }
    *consumed+=ptr; 
    return true;
}



int main(){
    char *buff;
    size_t consumed=0;
    FILE *fp = fopen("sample.json","r");
    size_t file_size = 0;
    fseek(fp,0,SEEK_SET);
    file_size = ftell(fp);
    fseek(fp,0,SEEK_END);
    file_size = ftell(fp)-file_size;
    buff =calloc(file_size,sizeof(char));
    read(0,buff,file_size);
    printf("provided json is %s\n",consume_object(buff,&consumed )?"valid":"invalid");
    return 0;
}