#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void get_prime(int p[2])
{
        /* close the write end of incoming pipe; we only read from p[0] */
        close(p[1]);
        int a;
        /* read an integer (binary) from the pipe */
        if (read(p[0], &a, sizeof(a)) > 0) {
            printf("prime %d\n", a);
        } else {
            close(p[0]);
            wait(0);
            exit(0);
        }

        int pp[2];
        pipe(pp);
        /* Use an indirect call to avoid -Werror=infinite-recursion: the child
         * will run get_prime(pp) after fork, but the compiler cannot prove
         * recursion on an indirect call. */
        void (*fp)(int *);
        fp = (void (*)(int *))get_prime;
        if (fork() == 0) {
            /* child: will read from pp[0] */
            fp(pp);
        } else {
            /* parent: read remaining numbers from p[0], write filtered numbers to pp[1] */
            close(pp[0]);
            int b;
            while (read(p[0], &b, sizeof(b)) > 0) {
                if (b % a != 0)
                    write(pp[1], &b, sizeof(b));
            }
            close(pp[1]);
            wait(0);
            close(p[0]);
            exit(0);
        }
}

int main(void)
{
    int p[2];
    pipe(p);
    for(int i=2;i<=35;++i)
    {
        write(p[1],&i,4);
    }
    get_prime(p);
    wait(0);
    exit(0);
}