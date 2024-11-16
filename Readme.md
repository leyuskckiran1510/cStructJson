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

# Cons/TODOs
- dosenot supports nested struct or union
- only supports following types for struct member, int,char,char*,float,double,long
- when doing json_decode, is the provided json string has more fields then it
  treats it like a valid json, and will only populate the sturct fields from it

# Examples

```c
#define __SJS_IMPLE__
#include "sjson.h"


JsonStruct(User,int(id),string(name),int(age),string(address))

int main(){
    User user={
        .id = 1,
        .name = "The User 1",
        .age = 19,
    };
    char buffer[1024];
    json_encode(User,user,buffer);
    printf("%s\n",buffer); // {"id":1,"name":"The User 1","age":19,"address":""}
    char *json_strig = "{\"id\":123,\"name\":\"user1\",\"age\":12,\"address\":\"localhost\""
                        ",\"other_random_keys\":\"random_value\"}";
    // absence of key will raise error,
    // but presence of additional keys are ignored
    user = json_decode(User,json_string);
    printf("%d",user.id);//123
    // SJS allocates memeory for all string type
    // and when you are done with them you can do
    // any of the following
    free_all_malloc(); // free all alloacter memory
    // free_recent_malloc(); // frees single last allocated memory
    // free_n_recent_malloc(size_t n); // free n-number of recently allocated memory
    return 0;
}

```


