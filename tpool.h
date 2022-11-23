//
//  tpool.h
//  WorkerThread
//
//  Created by srujan chilukunuri on 20/11/22.
//

#ifndef tpool_h
#define tpool_h

#include <stdbool.h>
#include <stddef.h>

struct tpool;
typedef struct tpool tpool_t;

typedef void (*thread_func_t)(void *arg);

tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *tm);

bool tpool_add_work(tpool_t *tm, thread_func_t func, void *arg);
void tpool_wait(tpool_t *tm);

#endif /* tpool_h */
