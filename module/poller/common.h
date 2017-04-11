#ifndef POLLER_COMMON_H
#define POLLER_COMMON_H

#include <fcntl.h>

void SetNonBlocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}

#endif
