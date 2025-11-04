#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc,char** argv)
{
    char* argvv[MAXARG];
    int i=1;
    for(;i<argc;++i)
    {
        argvv[i-1]=argv[i];
    }

    argvv[argc-1]=malloc(512);
    argvv[argc]=0;

    while(gets(argvv[argc-1],512))
    {
        if(argvv[argc-1][0]==0)break;
        else if(argvv[argc-1][strlen(argvv[argc-1])-1]=='\n')
        argvv[argc-1][strlen(argvv[argc-1])-1]='\0';
        if(fork()==0)
        {
            exec(argv[1],argvv);
        }
        else 
        {
            wait(0);
        }
    }
}