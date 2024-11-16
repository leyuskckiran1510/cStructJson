#ifndef __SJS_HEADER__
#define __SJS_HEADER__


#include <stdio.h>

// https://www.scs.stanford.edu/~dm/blog/va-opt.html 
// the FOREACH macro is copied from above URL,
#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__


#define FOR_EACH(macro, ...)                                                   \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, first_arg, ...)                                 \
  macro(first_arg)                                                             \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define GenerateFunctionSig(x) x json_get_var_##x(const char * json_string,const char *key_string);
// double and float are treat as double when parsing from string
FOR_EACH(GenerateFunctionSig,int,char,double,long)
char* json_get_var_string(const char * json_string,const char *key_string);
void clean_null_terminators(char *buffer);


/*
// each define explanation
v_       => v_    will expand given variable to `data_type variable` 
fmt_     => fmt_  will prepear struct member string and it's format string
var_     => var_  will expand to member name
str_     => str_  will expand to member name c-string 
jget_    => jget_ will expand to appropriate json_value_get, for datatype

NOTE:- if you want to add new datatypes you can
       follow above instruction, for you costume type
       and add here and it should be integrated automatically

*/

// int parsing and handeling functions
#define v_int(x) int x
#define fmt_int(x) "\""#x"\":%d"
#define var_int(x) x
#define str_var_int(x) #x
#define jget_int(...) json_get_var_int
// for char
#define v_char(x) char x
#define fmt_char(x) "\""#x"\":\"%c\""
#define var_char(x) x
#define str_var_char(x) #x
#define jget_char(...) json_get_var_char
// for float
#define v_float(x) float x
#define fmt_float(x) "\""#x"\":\"%f\""
#define var_float(x) x
#define str_var_float(x) #x
#define jget_float(...) json_get_var_double

// for string
#define v_string(x) char* x
#define fmt_string(x) "\""#x"\":\"%s\""
#define var_string(x) x
#define str_var_string(x) #x
#define jget_string(...) json_get_var_string

// for long
#define v_long(x) long x
#define fmt_long(x) "\""#x"\":\"%ld\""
#define var_long(x) x
#define str_var_long(x) #x
#define jget_long(...) json_get_var_long

// for double
#define v_double(x) double x
#define fmt_double(x) "\""#x"\":\"%ld\""
#define var_double(x) x
#define str_var_double(x) #x
#define jget_double(...) json_get_var_double


#define ExpandToStructMember(x)      v_##x;
#define ExpandToFormatString(x)      fmt_##x","
#define ExpandToMemberAccess(x)      ,struct_instance.var_##x
#define ExpandToJsonValueGet(x)      .var_##x = jget_##x(buffer,str_var_##x),
 
//__VAR_OPT__(x); will expand to x, if __VA_ARGS__ is not empty
// so in json_encode, we set buffer = "{}", as fallback when 
// the struct has no members 

#define JsonStruct(name,...)                                                   \
                                                                               \
typedef struct {                                                               \
    FOR_EACH(ExpandToStructMember,##__VA_ARGS__)                               \
} name;                                                                        \
                                                                               \
                                                                               \
void json_encode_##name(name struct_instance,char *buffer){                    \
    buffer[0]='{';buffer[1]='}';                                               \
    __VA_OPT__(                                                                \
        char *format = "{"    FOR_EACH(ExpandToFormatString,__VA_ARGS__) "}";  \
        sprintf(buffer,format FOR_EACH(ExpandToMemberAccess,__VA_ARGS__));     \
    )                                                                          \
    clean_null_terminators(buffer);                                            \
}                                                                              \
                                                                               \
name json_decode_##name(char *buffer){                                         \
    name struct_instance = {                                                   \
                    FOR_EACH(ExpandToJsonValueGet,##__VA_ARGS__)               \
    };                                                                         \
    return struct_instance;                                                    \
}                                                                              

#define json_encode(name,item,buf)  json_encode_##name((item),buf);
#define json_decode(name,buf)  json_decode_##name(buf);

#define __SJS_MALLOC_TRACK__
static void * __SJS_A[1024]={0};
static size_t __SJS_C = 0;
void free_recent_malloc();
void free_all_malloc();
void free_n_recent_malloc(size_t n);
#endif

#ifdef __SJS_IMPLE__
#include "sjson.c"
#endif