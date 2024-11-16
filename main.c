#include <assert.h>
#include <stdio.h>

#define __SJS_IMPLE__
#include "sjson.h"

JsonStruct(User,int(j),int(c),string(name),float(gpa));

int main(int argc, char const *argv[]){
    char buf[1000];
    User user ={
                .j=10,
                .c=20,
                .name="user is the one",
                .gpa=1.2,
            };
    json_encode(User,user,buf);
    printf("%s\n",buf );
    char *json_string = "{\"j\":10,\"c\":20,\"name\":\"user is the one\",\"gpa\":\"1.200000\"}";
    user = json_decode(User,json_string);
    printf("%f\n",user.gpa);    
    return 0;
}