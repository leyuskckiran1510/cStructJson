#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "json.h"



#define v_int(x) int x
#define fmt_int(x) "\""#x"\":%d"
#define var_int(x) x
#define str_var_int(x) #x
#define jget_int(...) json_get_var_int



#define v_char(x) char x
#define fmt_char(x) "\""#x"\":\"%c\""
#define var_char(x) x
#define str_var_char(x) #x
#define jget_char(...) json_get_var_int

#define Expand(x,...) v_##x; Expand(__VA_ARGS__)

#define Expand_fmt(x,...) "," fmt_##x  Expand_fmt(__VA_ARGS__)
#define Expand_var(name,x,...) ,_##name.var_##x Expand_var(name,__VA_ARGS__)
 
#define JsonStruct(name,value,...) typedef struct {                            \
                        v_##value;                                              \
                        Expand(__VA_ARGS__)                                    \
} name;                                                                        \
void json_encode_##name(name _##name,char *buffer){                            \
    char *format = "{" fmt_##value  Expand_fmt(__VA_ARGS__) "}";               \
    sprintf(buffer,format,_##name.var_##value Expand_var(name,__VA_ARGS__));   \
}                                                                              \
// name json_decode_##name(char *buffer){                                         \
//     name variable = {                                                          \
//         .var_##value = jget_##value(buffer,str_var_##value)                    \
//     };                                                                         \
// }                                                                              \

JsonStruct(Positive,int(m));

JsonStruct(Number,int(x),char(y));

int main(int argc, char const *argv[]){
#define je(name,item,buf)  json_encode_##name((item),buf);
// #define jd(name,buf)  json_decode_##name(buf);
    char buf[1000];
    Number num = {.x=10,};
    je(Number,num,buf);
    printf("%s\n",buf );
    // num = jd(Number,buf)
    // printf("%d\n",num.x);
    return 0;
}