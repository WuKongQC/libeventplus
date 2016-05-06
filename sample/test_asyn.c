#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<hiredis/hiredis.h>
#include<hiredis/async.h>
#include<hiredis/adapters/libevent.h>

#include<event2/event.h>

//设置get命令的回调函数
void getCallback(redisAsyncContext *c, void *r, void *privdata) {
  printf("in get cmd callback \n");
  redisReply *reply = r;
  if (reply == NULL){
      printf("reply == NULL");
      return;
  }
  printf("get cmd:[%s], reply->str:%s\n", (char*)privdata, reply->str);

  // Disconnect 
  redisAsyncDisconnect(c);
}

//设置连接回调函数
void connectCallback(const redisAsyncContext *c, int status) {
  if (status != REDIS_OK) {
    printf("in connectCallback Error: %s\n", c->errstr);
    return;
  }
  printf("Connected...\n");
}

//设置断开连接回调函数
void disconnectCallback(const redisAsyncContext *c, int status) {
  if (status != REDIS_OK) {
    printf("in disconnectCallback  Error: %s\n", c->errstr);
    return;
  }
  printf("Disconnected...\n");
}

int main (int argc, char **argv) {

  struct event_base *base = event_base_new();//新建一个libevent事件处理

  redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);//新建异步连接
  if (c->err) {
    /* Let *c leak for now... */
    printf("Error: %s\n", c->errstr);
    return 1;
  }

  redisLibeventAttach(c,base);//将连接添加到libevent事件处理
  redisAsyncSetConnectCallback(c,connectCallback);//设置连接回调
  redisAsyncSetDisconnectCallback(c,disconnectCallback);//设置断开连接回调
  redisAsyncCommand(c, NULL, NULL, "SET aaa %b", "bbb", sizeof("bbb"));//发送set命令
  redisAsyncCommand(c, getCallback, (char*)"GET aaa", "GET aaa");//发送get命令
  event_base_dispatch(base); //开始libevent循环。注意在这一步之前redis是不会进行连接的
  return 0;
}
