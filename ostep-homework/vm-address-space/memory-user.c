#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    if(argc > 2){
        fprintf(stderr, "parameters error");
        exit(1);
    }

    int mem = atoi(argv[1]);  // how many MB you should alloc
    int size = mem * 1e6 / 4;
    int *arr = (int*)malloc(sizeof(int) * size);  // int:4B
    while(1){
        for(int i = 0;i < size;i++){
            arr[i] = 0;
        }
    } 

    return 0;
}