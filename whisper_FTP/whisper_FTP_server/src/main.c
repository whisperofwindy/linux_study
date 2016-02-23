/***********************************************************************
	> File Name: main.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 08:36:14 AM CST
 ************************************************************************/

#include"factory.h"

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("error args\n");
		return -1;
	}
	
	int thread_num = atoi(argv[3]);
	int capacity = atoi(argv[4]);
	
	factory tf;                      // 初始化线程池 及 队列
	factory_init(&tf, thread_num, capacity, thread_handle);  // 初始化 队列 及 线程池数据结构
	factory_start(&tf);
	
	// 网络套接字
	
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1)
	{
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in serv_sockaddr;
	memset(&serv_sockaddr, 0, sizeof(serv_sockaddr));
	serv_sockaddr.sin_family = AF_INET;
	serv_sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_sockaddr.sin_port = htons(atoi(argv[2]));
 
	int ret;
	ret = bind(sock_fd, (struct sockaddr*)&serv_sockaddr, sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("bind");
		return -1;
	}
	ret = listen(sock_fd, capacity);
	if(ret == -1)
	{
		perror("listen");
		return -1;
	}

	// 初始化 epoll
	int epoll_fd = epoll_create(1);
	if(epoll_fd == -1)
	{
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev, evs[2];    // 此处的问题很大
	ev.events = EPOLLIN;
	ev.data.fd = sock_fd;
	ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &ev);
	if(ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

    printf("wait to connect\n");
	int client_sock_fd;


	for(;;)                         // 等待事件的发生，并进行之前的初始化
	{								// 线程事件
		ret = epoll_wait(epoll_fd, evs, 2, -1);
		if(ret > 0)
		{
			if(evs[0].data.fd == sock_fd && evs[0].events == EPOLLIN)
			{ 
				client_sock_fd = accept(sock_fd, NULL, NULL);
				pnode pnew_client_node;
				pnew_client_node = (pnode)calloc(1, sizeof(node));
				pnew_client_node->new_fd = client_sock_fd;
				factory_que_set(&tf.fd_que, pnew_client_node);
				pthread_cond_signal(&tf.cond);
			}
		}
	}
}

void* thread_handle(void*arg)
{
	pfactory pf = (pfactory)arg;
//	printf("the start_flag is %d\n", pf->start_flag);
	pque_t pq = &pf->fd_que;
	pnode pcur;
	if(factory_que_empty(pq))
	{
		pthread_cond_wait(&pf->cond, &pq->mutex);
		pthread_mutex_unlock(&pq->mutex);
	}

	printf("client connect!\n");
	char buf[1000] = {0};
	factory_que_get(pq, &pcur);   //  核心操作 ls cd pwd 等的实现
	int ret;
	for(;;)
	{
		ret = recv(pcur->new_fd, buf, sizeof(buf), 0);
        if(ret == 0)
		{
			break;
		}
		if(ret == -1)
		{
			perror("recv");
			return;
		}

		if(strncmp(buf, "pwd", 3) == 0)
		{
			char dir[128] = " ";
			getcwd(dir, sizeof(dir));
			ret = send(pcur->new_fd, dir, sizeof(dir), 0);
			if(ret == -1)
			{
  				perror("send");
				close(pcur->new_fd);
				return;
			}
		}
		if(strncmp(buf, "ls", 2) == 0)   /// ls command
		{
			DIR *pdir;
			pdir = opendir(".");
			if(pdir == NULL)
			{
 				perror("opendir");
				return;
			}
			struct dirent *pt;
			while(pt = readdir(pdir))
			{
				
			}
		}

		if(strncmp(buf, "cd", 2) == 0)
		{
			//strcpy(buf, "I can hear you!\n");
			char dir[128] = " ";
			sscanf(buf+3, "%s", dir);
			chdir(dir, 128);
			ret = send(pcur->new_fd, dir, sizeof(dir), 0);
			if(ret == -1)
			{
   				perror("send");
				return;
			}
		}
	}
	free(pcur);
	printf("client exit!\n");
}

