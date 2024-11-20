# cStructJson
Single line json to c-struct and viceversa

# Features
- c struct to json
```c
json_encode(StructName,struct_instance,char_buffer_to_write_to);
```
- json to c struct
```c
StructName struct_instance = json_decode(StructName,json_string);
```

## whats the catch
You have to define the struct as such
```c
JsonStruct(StructName,data_type(var_name))
```
```c
//example
//in-pure C

typedef struct{
    char *name;
    int id;
    int age;
}User_a;

// with CstructJson

JsonStruct(User_b,string(name),int(id),int(age));
// and Yes, you can use User struct as Like User_a can be used,
```
# New Features
- added support for int,float,long and double array

# Cons/TODOs
- dosenot supports nested struct or union
- only supports following types for struct member, int,char,char*,float,double,long
    int *, long *, float *, double * 
- when doing json_decode, is the provided json string has more fields then it
  treats it like a valid json, and will only populate the sturct fields from it

# Examples

```c
#include <assert.h>
#include <stdio.h>

#define __SJS_IMPLE__
#include "sjson.h"

JsonStruct(User,int(j),int(c),string(name),float(gpa),DArrayFloat(marks));
/*
typedef struct{
        int j;
        int c;
        char* name;
        float gpa;
        float * marks;
}User;


*/
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
                         \"gpa\":\"1.200000\",\"marks\":[1,2,3,4.122332,5,6,7,8]}";
    user = json_decode(User,json_string);
    printf("%s\n", float_array_to_string(user.marks,user.marks_count)); 
    float *a = json_get_var_float_array(json_string,"array");  
    if(a==NULL) return  1;
    printf("%f\n",a[3]); 
    return 0;
}
```


