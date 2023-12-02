#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    printf("hello world (pid:%d) \n", (int)getpid());
}