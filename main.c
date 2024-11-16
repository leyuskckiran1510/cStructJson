#include <assert.h>
#include <stdio.h>
#include <string.h>

#define __JSON_IMPLE__
#include "json.h"

JsonStruct(Number,int(j),int(c));

int main(int argc, char const *argv[]){
    char buf[1000];
    Number num ={.j=10};
    json_encode(Number,num,buf);
    assert(strcmp(buf,"{\"j\":10,\"c\":0}")==0);
    printf("%s\n",buf );
    num = json_decode(Number,buf);
    printf("%d\n",num.c);
    return 0;
}