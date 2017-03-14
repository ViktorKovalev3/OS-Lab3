#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;
struct thread_arg{
    bool thread_ended;
    unsigned thread_num;
    bool rw; //0 - reader, 1 - writer
    int file;
};

#define size_of_buffer 128
static void * thread_func(void *arg){
    thread_arg* parsed_arg = (thread_arg*) arg;
    char msg[size_of_buffer];
    while (!(parsed_arg->thread_ended)){
        if (parsed_arg->rw){
            //1 - writer
            sprintf(msg, "%d", parsed_arg->thread_num);
            write(parsed_arg->file, (void*) msg, size_of_buffer);
        } else {
            //0 - reader
            memset(msg, 0, size_of_buffer);
            read(parsed_arg->file, msg, size_of_buffer);
            printf(" %s ", msg);
            fflush(stdout);
            sleep(100);
        }
    }
    pthread_exit((void*) parsed_arg->thread_num);
}

//Counting start with 0
#define amount_of_threads 2

int main(void)
{
    pthread_t  threads[amount_of_threads];
    thread_arg threads_args[amount_of_threads];
    int fds[2];      // fds[0] - read, fds[1] - write
    pipe(fds);

    for (int i = 0; i < amount_of_threads; ++i){
        threads_args[i].thread_ended = 0;
        threads_args[i].thread_num   = i;
        threads_args[i].rw    = i;
        threads_args[i].file  = fds[i];
        if ( pthread_create( &threads[i], NULL, thread_func, &threads_args[i] ) )
            return 1;
    }

    getchar();

    //Close all threads after press ENTER
    for (int i = 0; i < amount_of_threads; ++i){
        threads_args[i].thread_ended = 1;
    }

    //Waiting while all threads ended
    for (int i = 0; i < amount_of_threads; ++i){
        int* exit_thread_code;
        if ( pthread_join( threads[i], (void**) &exit_thread_code ) )
            return 1;
        printf("\nThread [%d] ended\n", exit_thread_code);
        close(fds[i]);
    }

    return EXIT_SUCCESS;
}
