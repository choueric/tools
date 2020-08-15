/* The program creates a child process and waits for it to finish. If a timeout
 * elapses the child is killed. Waiting is done using sigtimedwait(). Race
 * condition is avoided by blocking the SIGCHLD signal before fork().
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

static pid_t fork_child(void)
{
    int p = fork ();

    if (p == -1) {
        perror ("fork");
        exit (1);
    }

    if (p == 0) {
        puts ("child: sleeping...");
        sleep (10);
        puts ("child: exiting");
        exit (0);
    }

    return p;
}

static pid_t fork_test(void)
{
    int p = fork ();

    if (p == -1) {
        perror ("fork");
        exit (1);
    }

    if (p == 0) {
        puts ("test: sleeping...");
        sleep (3);
        puts ("test: exiting");
        exit (0);
    }

    return p;
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    sigset_t orig_mask;
    struct timespec timeout;
    pid_t pid;

    sigemptyset (&mask);
    sigaddset (&mask, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
        perror ("sigprocmask");
        return 1;
    }

    pid = fork_child ();
    fork_test();

    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;

    do {
        int ret = sigtimedwait(&mask, NULL, &timeout);
        printf(" ret = %d\n", ret);
        
        if (ret < 0) {
            printf("--> after sigtimedwait\n");
            if (errno == EINTR) {
                /* Interrupted by a signal other than SIGCHLD. */
                printf("got EINTR\n");
                continue;
            }
            else if (errno == EAGAIN) {
                printf ("Timeout, killing child\n");
                kill (pid, SIGKILL);
            }
            else {
                perror ("sigtimedwait");
                return 1;
            }
        }

        break;
    } while (1);

    printf("wait pid...\n");
    if (waitpid(pid, NULL, 0) < 0) {
        perror ("waitpid");
        return 1;
    }
    printf("ok child pid exit\n");

    return 0;
}
