
#include <stdio.h>

#include "sysutils.h"
#include "worker.h"

static int __my_worker()
{
        DBUG("in my worker\n");
        return 0;
}


static int __my_worker2()
{
        DBUG("in my worker 2\n");
        return 0;
}

int main(int argc, char *argv[])
{
        int ret;

        worker_handler_t worker_hander1;
        worker_handler_t worker_hander2;
        worker_init();

        worker_create(&worker_hander1, "my_work1", __my_worker, NULL, NULL, WORKER_TYPE_SEM, 0);
        worker_create(&worker_hander2, "my_work2", __my_worker2, NULL, NULL, WORKER_TYPE_SEM, 0);

        ret = 5;
        while(ret--) {
                worker_post(&worker_hander1);
                sleep(1);
                worker_post(&worker_hander2);
        }


        return 0;
}
