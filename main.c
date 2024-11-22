#include <assert.h>
#include <stdio.h>
 #include <unistd.h>

#define __SJS_IMPLE__
#include "sjson.h"

JsonStruct(User,
           int(j),
           int(c),
           string(name),
           float(gpa),
           DArrayFloat(marks),
           string(class),
           DArrayString(subject),
);


/*
typedef struct{
        int j;
        int c;
        char* name;
        float gpa;
        float * marks;
        char *class;
}User;
*/

JsonStruct(Error,int(status_code),string(status_message));


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
                .class = "awdwad",
            };
    Error er = {
            .status_code = 200,
            .status_message = "Message Sucessfull",
    };
    // json_encode(User,user,buf);
    // printf("%s\n",buf );
    // json_encode(Error,er,buf);
    // printf("%s\n",buf );
    FILE *fp = fopen("sample.json","r");
    char json_string1[500]={0};
    size_t file_size = 0;
    fseek(fp,0,SEEK_SET);
    file_size = ftell(fp);
    fseek(fp,0,SEEK_END);
    file_size = ftell(fp)-file_size;
    read(0,json_string1,file_size);
    printf("file has %zu chars  |%s| \n",file_size,json_string1);
    user = json_decode(User,json_string1);
    printf("%s %d\n", float_array_to_string(user.marks,user.marks_count),user.j); 
    fclose(fp);
    // char *json_string = "{\"status_code\":400,\"status_message\":\"Message Falied\"}";
    // er = json_decode_Error(json_string);
    // json_encode_Error(er,buf);
    // printf("%s\n",buf);

    // float a = json_get_var_double(json_string,"status_code");  
    // // if(a==NULL) return  1;
    // printf("%f\n",a); 
    return 0;
}