#include<hiredis/hiredis.h>
#include<stdio.h>

#define REDIS_SERVER_IP "10.3.1.200"
#define REDIS_SERVER_PORT 6379

const char* command_set = "set teststr testvalue";
const char* command_get = "get teststr";


void print_reply(const char* cmd, const redisReply* reply){

    int i = 0;

    switch(reply->type){
        case REDIS_REPLY_STATUS:
            printf("cmd:%s, reply:%s\n", cmd, reply->str);
            break;
        case REDIS_REPLY_ERROR:
            printf("cmd:%s, reply:%s\n", cmd, reply->str);
            break;
        case REDIS_REPLY_INTEGER:
            printf("cmd:%s, reply:%lld\n", cmd,  reply->integer);
            break;
        case REDIS_REPLY_NIL:
            printf("cmd:%s, reply:nil\n", cmd);
            break;

        case REDIS_REPLY_STRING:
            printf("cmd:%s, reply:%s\n", cmd, reply->str);
            break;
        case REDIS_REPLY_ARRAY:
            printf("cmd:%s, reply:\n", cmd);
            for(i = 0; i < reply->elements; ++i)
            {
                print_reply(cmd, reply->element[i]);
            }
            break;
         default:
            printf("unknown type:%d \n", reply->type);
            break;
    }
}
void test(){
    redisReply* r = NULL;    
    redisContext* c = redisConnect("10.3.1.200", 6379);

    if(NULL == c || c->err){
        redisFree(c);
        printf("connect redis server failed ！");
    }

    //exec set command
    r = (redisReply*) redisCommand(c, command_set);

    if(NULL==r){
        printf("exe c cmd:%s failed !\n", command_set);
    }
/*
    if( REDIS_REPLY_STATUS == r->type)
    {
        printf("cmd:%s, reply:%s \n", command_get, r->str);
    }
    else
    {
        printf("cmd:%s, reply type:%d \n", command_get, r->type);
    }
*/
    print_reply(command_set, r);
    freeReplyObject(r);

   
    //exec get command
    r = (redisReply*) redisCommand(c, command_get);

    if(NULL==r){
        printf("exe c cmd:%s failed !\n", command_get);
    }

    print_reply(command_get, r);
    freeReplyObject(r);
 
    redisFree(c);
    printf("test over\n");

    return;
}

int main()
{
    test();
    return 0;
}


