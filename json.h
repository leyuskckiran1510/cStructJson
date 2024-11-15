#ifndef __JSON__
#define __JSON__

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
            U(void);\

#define U(x) x json_get_var_##x(const char * json_string,const char *key_string)
            list
#undef U
#endif