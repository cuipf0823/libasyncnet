#include "../epollpoller.h"
#include <string>
#include <iostream>

void test_epoll(const std::string& ip, uint32_t port)
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
			if (errno == EINTR)
			{
				continue;
			}
			break;
		}
		//LT or ET

	}
	close(listenfd);
}