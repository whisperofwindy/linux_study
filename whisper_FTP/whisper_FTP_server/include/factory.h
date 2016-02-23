/*************************************************************************
	> File Name: factory.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 09:03:49 AM CST
 ************************************************************************/

#ifndef __FACTORY__
#define __FACTORY__
#include "head.h"
#include "work_que.h"

#define DOWN_FILE "hello.avi"

typedef void* (*pfunc)(void*);

typedef struct thread_pool // 线程池
{
	pthread_t *pth;
	pthread_cond_t cond;
	pfunc entry;           // 线程的处理函数
	que_t fd_que;
	int start_flag;
	int thread_num;
}factory, *pfactory;

typedef struct trans_data  // 传输数据结构信息
{
	int len;
	char buf[1000];
}data_t, *pdata_t;

void factory_init(pfactory, int, int, pfunc);
void factory_start(pfactory);
void *thread_handle(void*);
void send_file(int);
void send_n(int, char*, int);

#endif
