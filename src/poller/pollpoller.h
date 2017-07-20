#ifndef POLL_POLLER_H
#define POLL_POLLER_H
#include <iostream>
#include <assert.h>
#include <sys/poll.h>
#include <vector>
#include <libgen.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "common.h"

typedef std::vector<struct pollfd> PollFdList;

void AddFd(PollFdList& pollfds, int fd, int events)
{
	SetNonBlocking(fd);
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	pollfds.push_back(pfd);
}

void RemoveFd(PollFdList& pollfds, int fd)
{
	for (auto iter = pollfds.begin(); iter != pollfds.end(); ++iter)
	{
		if (iter->fd == fd)
		{
			pollfds.erase(iter);
			break;
		}
	}
}

void Poll(uint32_t listenfd)
{
	PollFdList pollfds;
	AddFd(pollfds, listenfd, POLLIN);
	while (true)
	{
		int ret = poll(&*pollfds.begin(), pollfds.size(), -1);
		if (ret == 0)
		{
			std::cout << "noting happened!" << std::endl;
			continue;
		}
		else if (ret < 0)
		{
			std::cout << "poll return error: " << errno << " : " << strerror(errno) << std::endl;
			continue;
		}

		for (const auto& iter : pollfds)
		{
			if (iter.revents <= 0 || ret <= 0)
			{
				continue;
			}
			if (iter.revents & POLLIN)
			{
				if (iter.fd == listenfd)
				{
					//新的连接到来
					struct sockaddr_in con_addr;
					socklen_t len = sizeof(con_addr);
					int connfd = accept(listenfd, static_cast<sockaddr*>(static_cast<void*>(&con_addr)), &len);
					if (connfd == -1)
					{
						std::cout << "accept failed errno: " << errno << " : " << strerror(errno) << std::endl;
						break;
					}
					AddFd(pollfds, connfd, POLLIN);
					std::cout << "new connection come fd: " << connfd << std::endl;
				}
				else
				{
					char buffer[kMaxBuffer] = { 0 };
					int ret = recv(iter.fd, buffer, kMaxBuffer - 1, 0);
					if (ret < 0)
					{
						RemoveFd(pollfds, iter.fd);
						close(iter.fd);
						std::cout << "recv socket fd: " << iter.fd << " error: " << errno
							<< " : " << strerror(errno) << std::endl;
					}
					else if (ret == 0)
					{
						RemoveFd(pollfds, iter.fd);
						close(iter.fd);
						std::cout << "close socket fd: " << iter.fd << std::endl;
					}
					else
					{
						uint32_t len = ret;
						std::cout << "receive buf size: " << len << " content: " << buffer << std::endl;
						ret = send(iter.fd, buffer, len, 0);
						std::cout << "send " << ret << " bytes to client" << std::endl;
						if (ret < 0)
						{
							std::cout << "write error!" << std::endl;
						}
					}
				}
			}
			else if (iter.revents & POLLOUT)
			{
				std::cout << "socket " << iter.fd << " writable " << std::endl;
			}
			--ret;
		}
	}
}

#endif
