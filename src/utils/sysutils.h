#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <sys/epoll.h>
#include "dbg.h"

#define MAX_BUF_LEN             (1024 * 64)
#define MAX_LINE_LEN            (1024 * 2)
#define MAX_PATH_LEN            (1024 * 4)
#define MAX_NAME_LEN            (256)
#define MAX_INFO_LEN            (512)

inline time_t gettime();
inline int _sem_wait(sem_t *sem);
inline int _epoll_wait(int epfd, struct epoll_event *events,
                       int maxevents, int timeout);
inline int _epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

#endif
