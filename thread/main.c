#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int async_function_true = 0;
int async_function_need_true = 0;

void *async_function() {
    int i = 0;
    while (!async_function_need_true) {
        printf("async_function_true: %d\n", ++i);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, async_function, NULL);

    sleep(10);
    async_function_need_true = 1;

    pthread_join(thread, NULL);
    return 0;
}
