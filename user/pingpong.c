#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int p[2];
    pipe(p);

    if(fork()==0)
    {
        //child
        char buf[5];
        read(p[0],buf,4);
        buf[4]='\0';
        printf("%d:received %s\n",getpid(),buf);
        write(p[1],"pong",4);
        close(p[1]);
        close(p[0]);
        exit(0);
    }
    else 
    {
        //parent
        write(p[1],"ping",4);
        close(p[1]);
        wait(0);
        char buf[5];
        read(p[0],buf,4);
        buf[4]='\0';
        close(p[0]);
        printf("%d:received %s\n",getpid(),buf);
        exit(0);
    }
}