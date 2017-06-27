#ifndef __DBG_H__
#define __DBG_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <sys/syscall.h> 


#define D_BUG           0x00000001
#define D_INFO          0x00000002
#define D_WARNING       0x00000004
#define D_ERROR         0x00000008
#define D_FATAL         0x00000010

extern int __d_bug__;
extern int __d_info__;
extern int __d_goto__;

static inline void __free(void *ptr)
{
        free(ptr);
}

static inline void *__malloc(size_t size)
{
        return malloc(size);
}

inline static pid_t __gettid(void)
{
        return syscall(SYS_gettid);
}


#define __MSG__(mask) ((mask) & (__d_bug__ | __d_info__ | D_WARNING | D_ERROR | D_FATAL))

#define __D_MSG__(size, mask, format, a...)                             \
        do {                                                            \
                if (__MSG__(mask)) {                                    \
                        time_t __t = time(NULL);                        \
                        char *__d_msg_buf, *__d_msg_time;               \
                                                                        \
                        __d_msg_buf = __malloc(size + 128);             \
                        __d_msg_time = (void *)__d_msg_buf + size;      \
                        strftime(__d_msg_time, 128, "%F %T", localtime(&__t));      \
                        snprintf(__d_msg_buf, size,                     \
                                 "%s/%lu %lu/%lu %s:%d %s() " format,  \
                                 __d_msg_time, (unsigned long)__t,      \
                                 (unsigned long)getpid( ),              \
                                 (unsigned long)__gettid( ),                          \
                                 __FILE__, __LINE__, __FUNCTION__,      \
                                 ##a);                                  \
                                                                        \
                        printf("%s", __d_msg_buf);                 \
                        __free(__d_msg_buf);                            \
                }                                                       \
        } while (0);

#define D_MSG(mask, format, a...)  __D_MSG__(4096 - 128, mask, format, ## a)
#define DINFO(format, a...)        D_MSG(D_INFO, "INFO: "format, ## a)
#define DINFO1(size, format, a...) __D_MSG__(size, D_INFO, "INFO: "format, ## a)
#define DWARN(format, a...)        D_MSG(D_WARNING, "WARNING: "format, ## a)
#define DERROR(format, a...)       D_MSG(D_ERROR, "ERROR: "format, ## a)
#define DFATAL(format, a...)       D_MSG(D_FATAL, "FATAL: "format, ## a)
#define DBUG(format, a...)         D_MSG(D_BUG, format, ## a)

# define YASSERT(exp)                                                   \
        do {                                                            \
                if (!(exp)) {                      \
                        DERROR("!!!!!!!!!!assert fail!!!!!!!!!!!!!!!\n"); \
                            abort(); \
                }                                                       \
        } while (0)

# define GOTO(label, ret)                                               \
        do {                                                            \
                if (__d_goto__) {                                       \
                DWARN("Process leaving via %s (%d)%s\n", #label, ret, strerror(ret)); \
                }                                                       \
                goto label;                                             \
        } while (0)



#endif
