#include <stdio.h>
#include <iostream>
#include "module/poller/tests/epollpoller_test.h"
int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		std::cout << "usage: " << basename(argv[0]) << " ip address port number" << std::endl;
		return -1;
	}
	const char* kIp = argv[1];
	const uint32_t kPort = atoi(argv[2]);
	test_epoll(kIp, kPort, false);
	getchar();
	return 0;
}