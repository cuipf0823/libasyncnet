#ifndef EPOLL_POLLER_TEST_H
#define EPOLL_POLLER_TEST_H


#include "../epollpoller.h"
#include <string>
#include <iostream>

void test_epoll(const std::string& ip, uint32_t port, bool lt_mode = true)
{
	struct sockaddr_in addr;
	bzero(static_cast<void*>(&addr), sizeof(sockaddr_in));
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	int ret = 0;
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd > 0);
	ret = bind(listenfd, static_cast<const sockaddr*>(static_cast<const void*>(&addr)), sizeof(addr));
	assert(ret != -1);
	ret = listen(listenfd, SOMAXCONN);
	assert(ret != -1);
	constexpr uint32_t kMaxEvents = 1000;
	struct epoll_event events[kMaxEvents];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	if (lt_mode)
	{
		AddFd(epollfd, listenfd, false);
	}
	else
	{
		AddFd(epollfd, listenfd, true);
	}


	std::cout << "Server start ..." << std::endl;
	while (true)
	{
		ret = epoll_wait(epollfd, events, kMaxEvents, -1);
		if (ret < 0)
		{

			std::cout << "epoll wait failed errno: " << errno << std::endl;
			if (errno == EINTR)
			{
				continue;
			}
			break;
		}
		//LT or ET
		if (lt_mode)
		{
			LT_Mode(events, ret, epollfd, listenfd);
		}
		else
		{
			ET_Mode(events, ret, epollfd, listenfd);
		}
	}
	close(listenfd);
}

#endif