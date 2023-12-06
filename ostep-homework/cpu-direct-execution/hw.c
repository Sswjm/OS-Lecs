#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sched.h>
#include<fcntl.h>

#define LOOP 10

void syscallMeasure()
{
    struct timeval tv_start, tv_end;

    int start_flag = gettimeofday(&tv_start, NULL);
    if(start_flag < 0){
        fprintf(stderr, "gettimeofday failed");
        exit(1);
    }

    char buffer[1];

    for(int i = 0;i < LOOP;i++){
        ssize_t res = read(STDIN_FILENO, buffer, 0);
    }

    int end_flag = gettimeofday(&tv_end, NULL);
    if(end_flag < 0){
        fprintf(stderr, "gettimeofday failed");
        exit(1);
    }

    long sec = tv_end.tv_sec - tv_start.tv_sec;
    long usec = tv_end.tv_usec - tv_start.tv_usec;
    double total_time = sec * 1e6 + usec;

    printf("Read 0 bytes syscall need %lf us\n", total_time / LOOP);
}

void switchMeasure()
{
    int pipefd_1[2], pipefd_2[2];  // pipe1:parent -> child   pipe2:child -> parent
    struct timeval tv_start, tv_end;

    if(pipe(pipefd_1) == -1 || pipe(pipefd_2) == -1){
        fprintf(stderr, "create pipe failed\n");
        exit(1);
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(sched_getcpu(), &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    pid_t rc = fork();
    
    if(rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){ // child process
        char buffer = '0';
        close(pipefd_1[1]);  // close pipe1 write end
        close(pipefd_2[0]);

        for(int i = 0;i < LOOP;i++){
            read(pipefd_1[0], &buffer, 1);
            write(pipefd_2[1], &buffer, 1);
        }

        close(pipefd_1[0]);
        close(pipefd_2[1]);

        exit(EXIT_SUCCESS);
    }
    else{ // parent process
        char buffer = '0';
        gettimeofday(&tv_start, NULL);
        close(pipefd_2[1]);
        close(pipefd_1[0]);

        for(int i = 0;i < LOOP;i++){
            write(pipefd_1[1], &buffer, 1);    // 父进程要先write再read，为什么？
            read(pipefd_2[0], &buffer, 1);
        }

        close(pipefd_2[0]);
        close(pipefd_1[1]);

        gettimeofday(&tv_end, NULL);
        long sec = tv_end.tv_sec - tv_start.tv_sec;
        long usec = tv_end.tv_usec - tv_start.tv_usec;
        double total_time = sec * 1e6 + usec;

        printf("Context switch needs %lf us\n", total_time / LOOP);

        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    
}

int main()
{
    //syscallMeasure();
    switchMeasure();

    return 0;
}