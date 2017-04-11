#ifndef POLL_POLLER_TEST_H
#define POLL_POLLER_TEST_H

#include "../pollpoller.h"
#include <string>

void test_poll(const std::string& ip, uint32_t port)
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
	std::cout << "Server start ..." << std::endl;
	Poll(listenfd);
	close(listenfd);
}

#endif