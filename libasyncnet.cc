#include <stdio.h>
#include <libgen.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <assert.h>
#include <vector>

void SetNonBlocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}

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
	//移除的时候最后一个参数可以直接传递为null
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
}

int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		std::cout << "usage: " << basename(argv[0]) << " ip address port number" << std::endl;
		return -1;
	}
	const char* kIp = argv[1];
	const uint32_t kPort = atoi(argv[2]);

	struct sockaddr_in addr;
	bzero(static_cast<void*>(&addr), sizeof(sockaddr_in));
	addr.sin_port = htons(kPort);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, kIp, &addr.sin_addr);

	int ret = 0;
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd > 0);
	ret = bind(listenfd, static_cast<const sockaddr*>(static_cast<const void*>(&addr)), sizeof(addr));
	assert(ret != -1);
	ret = listen(listenfd, SOMAXCONN);
	assert(ret != -1);

	constexpr uint32_t kMaxBuffer = 65535;
	constexpr uint32_t kMaxEvents = 1000;
	struct epoll_event events[kMaxEvents];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	AddFd(epollfd, listenfd);
	

	std::cout << "Server start ..." << std::endl;
	while (true)
	{
		ret = epoll_wait(epollfd, events, kMaxEvents, -1);
		if (ret < 0)
		{
			std::cout << "epoll wait failed errno: " << errno << std::endl;
			break;
		}
		for (const auto& iter : events)
		{
			int sockfd = iter.data.fd;
			if (sockfd == listenfd)
			{
				//新的连接到来
				struct sockaddr_in con_addr;
				socklen_t len = sizeof(con_addr);
				int connfd = accept(listenfd, static_cast<sockaddr*>(static_cast<void*>(&con_addr)), &len);
				assert(connfd != -1);
				AddFd(epollfd, connfd);
				std::cout << "new connection come fd: " << connfd << std::endl;
			}
			else if (iter.events & EPOLLIN)
			{
				char buffer[kMaxBuffer] = { 0 };
				ret = recv(sockfd, buffer, kMaxBuffer - 1, 0);
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
					if (ret < 0)
					{
						std::cout << "write error!" << std::endl;
					}

				}
			}
		}
	}
	close(listenfd);
	getchar();
	return 0;
}