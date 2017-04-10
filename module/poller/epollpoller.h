#include <stdio.h>
#include <libgen.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "common.h"


void AddFd(int epollfd, int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	SetNonBlocking(fd);
}

void RemoveFd(int epollfd, int fd)
{
	//�Ƴ���ʱ�����һ����������ֱ�Ӵ���Ϊnull
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
}

constexpr uint32_t kMaxBuffer = 65535;

//ˮƽ����ģʽ(Ĭ��)
void LT_Mode(epoll_event* events, int number, int epollfd, int listenfd)
{
	for (const auto& iter : events)
	{
		int sockfd = iter.data.fd;
		if (sockfd == listenfd)
		{
			//�µ����ӵ���
			struct sockaddr_in con_addr;
			socklen_t len = sizeof(con_addr);
			int connfd = accept(listenfd, static_cast<sockaddr*>(static_cast<void*>(&con_addr)), &len);
			if (connfd == -1)
			{
				std::cout << "accept failed errno: " << errno << " : " << strerror(errno) << std::endl;
				return;
			}
			AddFd(epollfd, connfd);
			std::cout << "new connection come fd: " << connfd << std::endl;
		}
		else if (iter.events & EPOLLIN)
		{
			//�˴ζ�ȡ����, �´���Ȼ�ᴥ��
			char buffer[kMaxBuffer] = { 0 };
			int ret = recv(sockfd, buffer, kMaxBuffer - 1, 0);
			if (ret < 0)
			{
				RemoveFd(epollfd, sockfd);
				close(sockfd);
				std::cout << "recv socket fd: " << sockfd << " error" << std::endl;
			}
			else if (ret == 0)
			{
				RemoveFd(epollfd, sockfd);
				close(sockfd);
				std::cout << "close socket fd: " << sockfd << std::endl;
			}
			else
			{
				uint32_t len = ret;
				std::cout << "receive buf size: " << len << " content: " << buffer << std::endl;
				ret = send(sockfd, buffer, len, 0);
				std::cout << "send " << ret << " bytes to client" << std::endl;
				if (ret < 0)
				{
					std::cout << "write error!" << std::endl;
				}
			}
		}
	}
}
//���ش���ģʽ
void ET_Mode(epoll_event* events, int number, int epollfd, int listenfd)
{
	for (const auto& iter : events)
	{
		int sockfd = iter.data.fd;
		if (sockfd == listenfd)
		{
			//�µ����ӵ���
			struct sockaddr_in con_addr;
			socklen_t len = sizeof(con_addr);
			int connfd = accept(listenfd, static_cast<sockaddr*>(static_cast<void*>(&con_addr)), &len);
			if (connfd == -1)
			{
				std::cout << "accept failed errno: " << errno << " : " << strerror(errno) << std::endl;
				break;
			}
			AddFd(epollfd, connfd);
			std::cout << "new connection come fd: " << connfd << std::endl;
		}
		else if (iter.events & EPOLLIN)
		{
			char buffer[kMaxBuffer] = { 0 };
			int ret = 0;
			while (true)
			{
				ret = recv(sockfd, buffer + strlen(buffer), kMaxBuffer - 1, 0);
				if (ret < 0)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						std::cout << "read finish completely!"
						break;
					}
					RemoveFd(epollfd, sockfd);
					close(sockfd);
					std::cout << "recv socket fd: " << sockfd << " error" << " close"<< std::endl;
				}
				else if (ret == 0)
				{
					RemoveFd(epollfd, sockfd);
					close(sockfd);
					std::cout << "close socket fd: " << sockfd << std::endl;
				}
				else
				{
					uint32_t len = ret;
					std::cout << "receive buf size: " << len << " content: " << buffer << std::endl;
					ret = send(sockfd, buffer, len, 0);
					std::cout << "send " << ret << " bytes to client" << std::endl;
					if (ret < 0)
					{
						std::cout << "write error!" << std::endl;
					}

				}
			}
			
		}
	}
}