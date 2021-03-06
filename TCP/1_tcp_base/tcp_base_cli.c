#include <stdio.h>
#include <stdlib.h> //EXIT_FAILURE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
static int parse_args(int argc, char **argv)
{
    //getopt_long
    return 0;
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        int i;
        for(i = 1; i < argc; i++)
        {
            fprintf(stdout, "argv[%d]=%s\t", i, argv[i]);
        }
        fprintf(stdout, "\n");
    }
    //创建套接字
    int socket_fd;
    do{
        //socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        fprintf(stdout, "socket creating\n");
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        
    }while(socket_fd == -1);
    fprintf(stdout, "socket create success ,%d\n",socket_fd);
    //初始化套接字
    const char* port = "8183";
    const char* ip = "192.168.90.48";
    struct sockaddr_in sin;//在客户端为目的地址的信息
    sin.sin_family = PF_INET;
    sin.sin_port = htons(atoi(port));
    sin.sin_addr.s_addr = inet_addr(ip);
    //sin.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    fprintf(stdout, "init sockaddr_in success,port=%s,ip=%s\n", port, ip);
    //连接服务器
    int ret;
    do{
        fprintf(stdout, "server connecting\n");
        ret = connect(socket_fd, (struct sockaddr *)&sin, sizeof(sin));
    }while(ret);//失败返回-1
    fprintf(stdout, "connect success\n");
    //阻塞、接收、发送
    char recvline[4096], sendline[4096];
    int num;
    while(1)
    {
        fprintf(stdout, "send msg to server: \n");
        fgets(sendline, 4096, stdin);
        do{
            fprintf(stdout, "sending msg ...\n");
            ret = send(socket_fd, sendline, strlen(sendline), 0);
        }while(ret == -1);
        fprintf(stdout, "send msg to server success,recving reply...\n");
        num =  recv(socket_fd, recvline, 4096, 0);//缓冲区无内容会阻塞
        recvline[num] = '\0';  
        fprintf(stdout, "recv msg from server: %s\n", recvline); 

    }
    close(socket_fd);
    //关闭套接字
    close(socket_fd);
    fprintf(stdout, "close(socket_fd)\n");
    return 0;
}   
#if 0
protocol
socket protocol 一般不填,默认值为 0,自动适应
传输层：IPPROTO_TCP、IPPROTO_UDP、IPPROTO_ICMP

SOCK_STREAM SOCK_DGRAM
SOCK_STREAM是基于TCP的，数据传输比较有保障。SOCK_DGRAM是基于UDP的.

AF_INET PF_INET
在windows中AF_INET与PF_INET完全一样. 
而在Unix/Linux系统中，在不同的版本中这两者有微小差别.对于BSD,是AF,对于POSIX是PF.
理论上建立socket时是指定协议，应该用PF_xxxx，设置地址时应该用AF_xxxx。当然AF_INET和PF_INET的值是相同的，混用也不会有太大的问题。

listen的第二个参数
未完成队列的大小
假设未完成队列设置为100，  有并发1000个请求过来。因为未完成队列只有100个。先放100个请求过来处理三次握手。其他的请求直接拒绝。

recv recvfrom
recv的recvfrom是可以替换使用的，只是recvfrom多了两个参数，可以用来接收对端的地址信息，
这个对于udp这种无连接的，可以很方便地进行回复。而换过来如果你在udp当中也使用recv，那么就不知道该回复给谁了，
如果你不需要回复的话，也是可以使用的。另外就是对于tcp是已经知道对端的，就没必要每次接收还多收一个地址，没有意义，要取地址信息，
在accept当中取得就可以加以记录了。
#endif