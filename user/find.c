#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* filename,char* path)
{
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd=open(path,0))<0)
    {
        fprintf(2,"cannot open\n");
        return ;
    }

    if(fstat(fd,&st)<0)
    {
        fprintf(2,"cannot stat\n");
        return ;
    }

    switch(st.type)
    {
        case T_DEVICE:
        case T_FILE:
        fprintf(2,"no dirent\n");
        break;
        case T_DIR:
        if(strlen(path)+1+DIRSIZ+1>sizeof(buf))
        {
            printf("too long\n");
            break;
        }
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++='/';
        while(read(fd,&de,sizeof(de))==sizeof(de))
        {
            if(de.inum==0)continue;
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ]='\0';
            
            if(stat(buf,&st)<0)
            {
                printf("cannot stat\n");
                return ;
            }

            if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
            {
                continue;
            }

            if(strcmp(de.name,filename)==0)
            {
                printf("%s\n",buf);
                continue;
            }

            if(st.type==T_DIR)
            {
                find(filename,buf);
            }
        }
    }
    close(fd);
}

int main(int argc,char** argv)
{
    if(argc!=3)
    {
        fprintf(2,"too few\n");
        exit(0);
    }
    else 
    {
        find(argv[2],argv[1]);
        exit(0);
    }
}

