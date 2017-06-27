#ifndef __YLOCK_H__
#define __YLOCK_H__

#include <pthread.h>
#include <stdint.h>

#include "list.h"

#if 0
#define sy_rwlock_t pthread_rwlock_t

#if 0
#ifdef HAVE_PTHREAD_RWLOCK_INIT
#define sy_rwlock_t pthread_rwlock_t
#else
#define sy_rwlock_t pthread_mutex_t

#define pthread_rwlock_init             pthread_mutex_init
#define pthread_rwlock_destroy          pthread_mutex_destroy
#define pthread_rwlock_rdlock           pthread_mutex_lock
#define pthread_rwlock_tryrdlock        pthread_mutex_trylock
#define pthread_rwlock_wrlock           pthread_mutex_lock
#define pthread_rwlock_trywrlock        pthread_mutex_trylock
#define pthread_rwlock_unlock           pthread_mutex_unlock
#endif

#endif

#endif


#define USE_SPIN_LOCK

#ifdef USE_SPIN_LOCK

#define sy_spinlock_t pthread_spinlock_t
#define sy_spin_init(__spin__) \
        pthread_spin_init(__spin__, PTHREAD_PROCESS_PRIVATE)
#define sy_spin_destroy pthread_spin_destroy
#define sy_spin_lock pthread_spin_lock
#define sy_spin_trylock pthread_spin_trylock
#define sy_spin_unlock pthread_spin_unlock

#else

#define sy_spinlock_t pthread_mutex_t
#define sy_spin_init(__spin__) \
        pthread_mutex_init(__spin__, NULL)
#define sy_spin_destroy pthread_mutex_destroy
#define sy_spin_lock pthread_mutex_lock
#define sy_spin_trylock pthread_mutex_trylock
#define sy_spin_unlock pthread_mutex_unlock

#endif

#if 1
#define LOCK_DEBUG
#endif

typedef struct {
        pthread_rwlock_t lock;
        struct list_head queue;
        sy_spinlock_t spin;
        uint32_t priority;
#ifdef LOCK_DEBUG
        int32_t count;
        uint32_t last_unlock;
#endif
} sy_rwlock_t;

#endif
