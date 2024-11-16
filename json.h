#ifndef __JSON__
#define __JSON__

#define string char*

#define list \
            U(int);\
                        \
            U(char);\
                        \
            U(float);\
                        \
            U(double);\
                        \
            U(long);\
                        \
            U(string);\

#define U(x) x json_get_var_##x(const char * json_string,const char *key_string)
            list
#undef U

void clean_null_terminators(char *buffer);

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
#define jget_float(...) json_get_var_float

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


#define Expand(x) v_##x;
#define Expand_fmt(x)  fmt_##x","
#define Expand_var(x) ,__local_var.var_##x
#define DecodeExpand(value) .var_##value = jget_##value(buffer,str_var_##value),
 
#define JsonStruct(name,...) typedef struct {                                   \
                           FOR_EACH(Expand,##__VA_ARGS__)                       \
} name;                                                                         \
void json_encode_##name(name __local_var,char *buffer){                         \
           buffer[0]='{';buffer[1]='}';                                         \
    __VA_OPT__(char *format = "{" FOR_EACH(Expand_fmt,__VA_ARGS__) "}";         \
    sprintf(buffer,format FOR_EACH(Expand_var,__VA_ARGS__));)                   \
    clean_null_terminators(buffer);                                             \
}                                                                               \
name json_decode_##name(char *buffer){                                       \
    name variable = {                                                        \
                    FOR_EACH(DecodeExpand,##__VA_ARGS__)               \
    };                                                                       \
    return variable;                                                         \
}                                                                            \


#define json_encode(name,item,buf)  json_encode_##name((item),buf);
#define json_decode(name,buf)  json_decode_##name(buf);
#endif

#ifdef __JSON_IMPLE__
#include "json.c"
#endif