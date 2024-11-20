#include <assert.h>
#include <stdio.h>

#define __SJS_IMPLE__
#include "sjson.h"

JsonStruct(User,int(j),int(c),string(name),float(gpa),DArrayFloat(marks));

int main(int argc, char const *argv[]){
    char buf[1000];
    float marks[] = {1,2,3,4,5,6,7,8};
    User user = {
                .j = 10,
                .c = 20,
                .name = "user is the one",
                .gpa = 1.2,
                .marks = marks,
                .marks_count = sizeof(marks)/sizeof(int),
            };
    json_encode(User,user,buf);
    printf("%s\n",buf );
    char *json_string = "{\"j\":10,\"c\":20,\"name\":\"user is the one\",\
                         \"gpa\":\"1.200000\",\"a\":[1,2,3,4.122332,5,6,7,8]}";
    user = json_decode(User,json_string);
    printf("%f\n",user.gpa); 
    float *a = json_get_var_float_array(json_string,"array");  
    if(a==NULL) return  1;
    printf("%f\n",a[3]); 
    return 0;
}