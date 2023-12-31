# OSTEP作业  

# Ch.4 抽象：进程（Abstraction:Process）  
1 cpu利用率应该是100%；两个进程都没有I/O操作，CPU一直在运行  
2 11个时间单位
3 运行时间明显缩短，为7个时间单位；重要，某个进程由于I/O进入blocked状态时，cpu可以运行其他进程，从而提高cpu利用率  
4 运行时间与2相同，因为一个进程阻塞时，不允许另一个进程调度（先来先服务且不允许抢占）  
5 运行时间与3相同，因为在IO操作时允许进程调度    
6 运行这个进程组合时，设置当IO完成时，发出IO的进程不一定马上运行。PID0进程被阻塞后，先后运行1、2、3，其皆运行结束之后才继续运行进程PID0。这种方式系统资源利用率较低，cpu利用率仅为67.74%。   
7 设置立即运行发出IO的进程。这种方式可以在每次IO进程阻塞时，运行另一个进程，顺序为0、1、0、2、0、3，这样可以充分利用cpu资源，这个例子中达到了100%。  
8 预测随机生成的进程即可。  

# CH.5 插叙：进程API（Interlude:Process API）  
Simulation:  
1、2 思考进程fork时的进程树  
3 根据进程树推测执行哪种syscall  
4 执行`./fork.py -A a+b,b+c,c+d,c+e,c-`子进程退出时，进程c的子进程变为a（根结点）的子进程。加入`-R`参数后（表示reparent to local parent），c的子进程d和e变为b的子进程。  
5 根据进程fork和exit序列创建进程树  
6 根据进程树考虑syscall序列  

Code:  
代码位于ostep-homework/cpu-api，这里我没有写makefile，每次写完一个程序都使用gcc编译来检查结果  
1 父进程和子进程都会单独改变局部变量x的值。父进程中，改变前x = 100， 改变后x = 7， 最终程序输出7；子进程中，改变前x = 100， 改变后x = 6，最终程序输出6。  
2 子进程和父进程都可以访问`open()`返回的文件描述符；`FILE *fd = fopen("test_2.txt", "w+");`我使用w+方式打开文件，测试结果中父进程和子进程的字符串都可以正确写入。然而在没有适当的同步机制的情况下，可能会有写入交错、数据覆盖、文件完整性损坏等问题，同时由于两个进程使用同一个文件描述符fd，可能会对指针便宜量有影响。  
3 使用`wait()`实现的代码见`3-1.c`。可以不使用`wait()`来实现题目中的要求，这时候需要用到管道（`pipe()`），实现代码见`3-2.c`，同样可以确保子进程先输出。  
4 exec所有变体的说明可以使用`man exec`来查看    
5 父进程中的`wait()`返回子进程的pid；如果在子进程中调用`wait()`，则首先执行父进程，再执行子进程，且wait返回值为-1，表示没有子进程      
6 使用`int wc = waitpid(-1, NULL, 0);`  
7 关闭标准输出后，子进程中的print语句无法正常输出，父进程中的print语句可以正常输出  
8 程序`8.c`可以在子进程中使用标准输入读入一个字符，使用通信，在父进程中使用标准输出显示该字符  

# Ch.6 机制：受限直接执行（Direct Execution）  
编写的代码位于ostep-homework/cpu-direct-execution（我自己新开的一个文件夹，原来的作业中没有这个文件夹）  
需要注意子进程是先读后写，父进程是先写后读，否则父进程会被阻塞  

# Ch.7 进程调度：介绍（Scheduling Basics）   
1 SJF j0响应时间0，周转时间200；j1响应时间200，周转时间400；j2响应时间400，周转时间600  
  FCFS与SJF相同，因为程序待运行时间都为200   
2 SJF j0响应时间0，周转时间100；j1响应时间100，周转时间300；j2响应时间300，周转时间600  
  FCFS与SJF相同，因为作业队列就是按照最短运行时间排序的  
3 j0响应时间0，周转时间598；j1响应时间1，周转时间599；j2响应时间2，周转时间600  
4 FCFS的作业队列按照最短运行时间从小到大排序时（或所有作业运行时间一致），SJF提供与FIFO相同的周转时间  
5 所有作业长度相同且时间片大小和作业长度相同时，SJF和RR提供相同的响应时间        
6 工作时间越长的作业，使用SJF算法时的响应时间越长，因为需要不断等待小作业先运行  
7 随着时间片长度的增加，RR的响应时间会变长。$response = \frac{0 + rr + 2\times rr + ... + (n-1)\times{rr}}{N}$，分子可以使用等比数列求和公式来化简。  

# Ch.8 调度：多级反馈队列（MLFQ Scheduling）  
1 使用命令`python3 mlfq.py -j 2 -n 2 -m 10 -M 0 -q 2 -s 100`计算即可  
2 略  
3 使用命令`python3 mlfq.py -j 2 -n 1 -m 10 -M 0 -q 2`设置只有一个优先级队列，即可实现RR算法  
4 `python3 mlfq.py -n 3 -q 10 -l 0,50,0:0,50,9 -i 1 -S -c`  
5 `python3 mlfq.py -n 3 -q 10 -l 0,200,0:0,200,1:0,200,1 -i 1 -S -B 200 -c`  
6 `python3 mlfq.py -n 2 -q 10 -l 0,50,0:0,50,11 -i 1 -S -I -c`

# Ch.9 调度：比例份额（Scheduling Proportional Share）  
1 略，明白彩票调度的执行过程即可。注意winner的产生是根据$随机数 % 彩票总数$来计算的  
2 job0在job1完成之前很难获得处理机；有可能会运行，概率为1/100；导致资源分配不公平  
3 $U = \frac{192}{200} = 0.96$，其他随机种子可以使用-s参数调整，结果略。 
4 随着时间片长度的增加，调度算法的公平性会逐渐降低（可以查看https://github.com/xxyzz/ostep-hw/tree/master/9 中的图像）     
q = 2时，$U = \frac{188}{200} = 0.94$  
q = 3时，$U = \frac{201}{204} = 0.985$
q = 4时，$U = \frac{176}{200} = 0.88$   

# Ch.10 多处理器调度（高级）  
学完并发相关内容再来读  

# Ch.13 抽象：地址空间（Abstraction:Address Spaces）
编程作业位于`ostep-homework/vm-address-space`  
