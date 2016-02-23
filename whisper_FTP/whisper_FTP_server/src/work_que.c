/*************************************************************************
	> File Name: work_que.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 09:39:30 AM CST
 ************************************************************************/

#include"work_que.h"

void factory_que_init(pque_t pq, int capacity)
{
	int ret;
	pq->que_head = NULL;
	pq->que_tail = NULL;
	ret = pthread_mutex_init(&pq->mutex, NULL);
	if(ret != 0)
	{
		printf("pthread_mutex_init fail\n");
		return ;
	}
	pq->size = 0;
	pq->que_capacity = capacity;
}

void factory_que_set(pque_t pq, pnode pnew)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->que_tail == NULL)
	{
		pq->que_head = pnew;
		pq->que_tail = pnew;
	}
	else
	{
		(pq->que_tail)->pnext = pnew;
		pq->que_tail = pnew;
	}
	pq->size++;
	pthread_mutex_unlock(&pq->mutex);
}

void factory_que_get(pque_t pq, pnode* pcur)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->que_head != NULL)
	{
		*pcur = pq->que_head;
		pq->que_head = (*pcur)->pnext;
		pq->size--;
	}
	pthread_mutex_unlock(&pq->mutex);
}

int factory_que_empty(pque_t pq)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->size == 0)
	{	
		return 1;
	}
	pthread_mutex_unlock(&pq->mutex);
	return 0;
}
