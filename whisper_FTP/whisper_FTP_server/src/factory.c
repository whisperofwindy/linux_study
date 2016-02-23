/************************************************************************
	> File Name: factory.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 09:15:07 AM CST
 ************************************************************************/

#include "factory.h"

void factory_init(pfactory pf, int th_num, int que_capacity, pfunc entry) // 线程池的初始化
{
	int ret;
	ret = pthread_cond_init(&pf->cond, NULL);
	if(ret == -1)
	{
		perror("pthread_cond_init");
		return ;
	}
	pf->pth = (pthread_t*)calloc(th_num, sizeof(pthread_t));  //  整型数组
	pf->entry = entry;
	factory_que_init(&pf->fd_que, que_capacity);  // 初始化队列
	pf->thread_num = th_num;                      // 线程的个数
	pf->start_flag = 0;
}

void factory_start(pfactory pf)
{
	if(pf->start_flag == 0)
	{
		int i, ret;
		for(i = 0; i < pf->thread_num; i++)
		{
			ret = pthread_create(&pf->pth[i], NULL, pf->entry, (void*)pf);
			if(ret != 0)
			{
 				printf("pthread_create fail\n");
				return ;
			}
			//printf("this is %dth thread\n", i+1);
		}
		pf->start_flag = 1;
	}
}

