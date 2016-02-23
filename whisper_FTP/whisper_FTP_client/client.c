/************************************************************************
	> File Name: client.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 20 Feb 2016 11:00:03 AM CST
 ************************************************************************/

#include "func.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("error args\n");
		return -1;
	}
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
	ret = connect(sock_fd, (struct sockaddr*)&serv_sockaddr, sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("connect");
		return -1;
	}
	char buf[1000];
	memset(buf, 0, sizeof(buf));
    // 发送命令 并且 ls cd pwd puts gets 等到服务器
	for(;;)
	{
	    read(0, buf, sizeof(buf));
		if(strncmp(buf, "pwd", 3) == 0)
		{
			ret = send(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
 				perror("send");
				close(sock_fd);
				return -1;
			}
			memset(buf, 0, sizeof(buf));
			ret = recv(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("recv");
				close(sock_fd);
				return -1;
			}
			//system("clear");
			printf("%s\n", buf);
		}

		if(strncmp(buf, "cd", 2) == 0)  // cd command
		{
			ret = send(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("send");
				return -1;
			}
			memset(buf, 0, sizeof(buf));
			ret = recv(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("recv");
				return -1;
			}
			system("clear");
			printf("%s", buf);
		}

		if(strncmp(buf, "ls", 2) == 0)
		{
			ret = send(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("send");
				close(sock_fd);
				return -1;
			}
			memset(buf, 0, sizeof(buf));
			ret = recv(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("recv");
				close(sock_fd);
				return -1;
			}
			system("clear");
			printf("%s\n", buf);
		}


		if(strncmp(buf, "remove", 6) == 0)
		{
			ret = send(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("send");
				close(sock_fd);
				return -1;
			}
			memset(buf, 0, sizeof(buf));
			ret = recv(sock_fd, buf, sizeof(buf), 0);
			if(ret == -1)
			{
				perror("recv");
				close(sock_fd);
				return -1;
			}
			system("clear");
			printf("%s\n", buf);
		}
	}
	close(sock_fd);
}
