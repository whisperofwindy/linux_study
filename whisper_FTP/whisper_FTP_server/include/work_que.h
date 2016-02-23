/*************************************************************************
	> File Name: work_que.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 08:30:23 AM CST
 ************************************************************************/

#ifndef __WORK_QUE__
#define __WORK_QUE__

#include"head.h"

typedef struct node_fd   // 队列节点
{
	int new_fd;
	struct node_fd *pnext;
}node, *pnode;

typedef struct que_fd   // 队列的数据结构
{
	pnode que_head, que_tail;
	pthread_mutex_t mutex;
	int size;
	int que_capacity;
}que_t, *pque_t;

void factory_que_init(pque_t, int);  
void factory_que_set(pque_t, pnode);
void factory_que_get(pque_t, pnode*);
int factory_que_empty(pque_t);

#endif
