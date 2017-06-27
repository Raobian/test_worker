
#include "sysutils.h"

inline time_t gettime()
{
        return time(NULL);
}

inline int _sem_wait(sem_t *sem)
{
        int ret;

        while (1) {
                ret = sem_wait(sem);
                if (ret) {
                        ret = errno;
                        if (ret == EINTR)
                                continue;
                        else
                                GOTO(err_ret, ret);
                }

                break;
        }

        return 0;
err_ret:
        return ret;
}

inline int _epoll_wait(int epfd, struct epoll_event *events,
                       int maxevents, int timeout)
{
        int ret, nfds;

        while (1) {
                nfds = epoll_wait(epfd, events, maxevents, timeout);
                if (nfds == -1) {
                        ret = errno;
                        if (ret == EINTR) {
                                DBUG("file recv loop interrupted by signal\n");
                                continue;
                        } else
                                GOTO(err_ret, ret);
                }

                break;
        }

        return nfds;
err_ret:
        return -ret;
}

inline int _epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
        return epoll_ctl(epfd, op, fd, event);
}


inline void *_memset(void *s, int c, size_t n)
{
        return memset(s, c, n);
}

inline void *_memmove(void *dest, const void *src, size_t n)
{
        return memmove(dest, src, n);
}

inline void *_memcpy(void *dest, const void *src, size_t n)
{
        return memcpy(dest, src, n);
}


int ymalloc(void **_ptr, size_t size)
{
        int ret, i;
        void *ptr;

        YASSERT(size != 0);

        if (size == 0) {
                *_ptr = NULL;
                return 0;
        }

        if (size > 4096)
                DBUG("big mem %u\n", (int)size);

        for (i = 0; i < 3; i++) {
                ptr = calloc(1, size);
                if (ptr != NULL)
                        goto out;
        }

        ret = ENOMEM;
        GOTO(err_ret, ret);

out:
        *_ptr = ptr;
        return 0;
err_ret:
        return ret;
}

int yrealloc(void **_ptr, size_t size, size_t newsize)
{
        int ret, i;
        void *ptr;

        if (*_ptr == NULL && size == 0) /*malloc*/ {
                ret = ymalloc(&ptr, newsize);
                if (ret)
                        GOTO(err_ret, ret);

                _memset(ptr, 0x0, newsize);

                *_ptr = ptr;
                return 0;
        }

        if (newsize == 0)
                return yfree(_ptr);

        if (newsize < size) {
                ptr = *_ptr;
                _memset(ptr + newsize, 0x0, size - newsize);
        }

        ret = ENOMEM;
        for (i = 0; i < 3; i++) {
                ptr = realloc(*_ptr, newsize);
                if (ptr != NULL)
                        goto out;
        }

        GOTO(err_ret, ret);
out:
        if (newsize > size)
                _memset(ptr + size, 0x0, newsize - size);

        *_ptr = ptr;

        return 0;
err_ret:
        return ret;
}

int yfree(void **ptr)
{
        if (*ptr != NULL) {
                free(*ptr);
        } else {
                YASSERT(0);
        }

        *ptr = NULL;

        return 0;
}

int ymalloc1(void **_ptr, size_t size)
{
        int ret, i;
        void *ptr;

        YASSERT(size != 0);

        if (size == 0) {
                *_ptr = NULL;
                return 0;
        }

        if (size > 4096)
                DBUG("big mem %u\n", (int)size);

        for (i = 0; i < 3; i++) {
                ptr = calloc(1, size);
                if (ptr != NULL)
                        goto out;
        }

        ret = ENOMEM;
        GOTO(err_ret, ret);
out:
        *_ptr = ptr;

        return 0;
err_ret:
        return ret;
}

int yfree1(void **ptr)
{

        if (*ptr != NULL) {
                free(*ptr);
        } else  {
                YASSERT(0);
        }

        *ptr = NULL;

        return 0;
}

