
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

void appSIGCHLDhandler (int signo, siginfo_t *siginfop, void *contextp)
{
    int status;

    //UNUSEDPARAM ( signo );
    //UNUSEDPARAM ( siginfop );
    //UNUSEDPARAM ( contextp );
    printf("yiduowang child handling!\n");

    /* Allow the SDK process to terminate */
    wait(&status);
    printf( "yiduowang child handling done!\n");
}

void appSignalInstall( unsigned int signo,  void (*functionp)(int, siginfo_t *, void *) )
{
    struct sigaction sa;

    if (functionp == NULL)
        return;
    sa.sa_sigaction = functionp;
    sigemptyset(&sa.sa_mask);

    //sa.sa_flags =  SA_SIGINFO | SA_RESTART;
    sa.sa_flags =  SA_SIGINFO | SA_NODEFER;

    if (sigaction(signo, &sa, NULL) < 0)
    {
        fprintf(stderr, "Signal handling falied");
    }
}

void * M2_Modem_Module_ReActivate1(void *pParam) {
    pid_t ret;
    int count = 10;

    while (count) {
        ret = fork();
        if (ret == 0) {
            while (1) {
                printf("child_process\n");
                system("rm -rf c.txt");
                system("cp log.txt c.txt");
                //sleep(2);
                exit(0);
            }
        } else {
            printf("parent process\n");
            count--;
        }
    }
    while (1) {
        printf("in thread 1 print\n");
        system("cp log.txt c.txt");
        sleep(1);
    }

}

void * M2_Modem_Module_ReActivate2(void *pParam) {
    pid_t ret;
    int count = 1;

    while (count) {
        ret = fork();
        if (ret == 0) {
            while (1) {
                printf("child_process2\n");
                system("rm -rf c.txt");
                system("cp log.txt c.txt");
                //sleep(2);
                exit(0);
            }
        } else {
            printf("parent proces2s\n");
            count--;
        }
    }
    while (1) {
        printf("in thread 2 print\n");
        system("rm  -rf c.txt");
        system("cp log.txt c.txt");
        sleep(1);
    }

}


pid_t proc_find(const char* name)
{
    DIR* dir;
    struct dirent* ent;
    char* endptr;
    char buf[512];

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while ((ent = readdir(dir)) != NULL) {
        /* if endptr is not a null character, the directory is not
         * entirely numeric, so ignore it */
        long lpid = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') {
            continue;
        }

        /* try to open the cmdline file */
        snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
        FILE* fp = fopen(buf, "r");

        if (fp) {
            if (fgets(buf, sizeof(buf), fp) != NULL) {
                /* check the first token in the file, the program name */
                char* first = strtok(buf, " ");
                if (!strcmp(first, name)) {
                    fclose(fp);
                    closedir(dir);
                    return (pid_t)lpid;
                }
            }
            fclose(fp);
        }

    }

    closedir(dir);
    return -1;
}


int main(int argc, char* argv[])
{
    int i;
    pid_t pid ;
    if (argc == 1) {
        fprintf("usage: %s name1 name2 ...\n", argv[0]);
        return 1;
    }


    for (i = 1; i < argc; ++i) {
        pid = proc_find(argv[i]);
        if (pid == -1) {
            printf("%s: not found\n", argv[i]);
        } else {
            printf("%s: %d\n", argv[i], pid);
        }
    }

    return 0;
}


#if 0

void* run(void *arg) {
    pthread_exit(0);
}
int main () {

    pthread_t thread;
    int rc;
    long count = 0;
    while (1) {
        if (rc = pthread_create(&thread, 0, run, 0) ) {
            printf("ERROR, rc is %d, so far %ld threads created\n", rc, count);
            perror("Fail:");
            return -1;
        }
        count++;
    }
    return 0;

}



int main(int argc, char *argv[])
{

    pthread_t reactivate_thread;
    pthread_t reactivate_thread2;
//  mdm_start_sim_monitor();
    appSignalInstall(SIGCHLD, appSIGCHLDhandler);
    pthread_create(&reactivate_thread, NULL, M2_Modem_Module_ReActivate1, NULL);
    pthread_create(&reactivate_thread2, NULL, M2_Modem_Module_ReActivate2, NULL);





//  sprintf(cmd, "pgrep %s | awk '{print \"kill -9 \" $1}' | sh", path);
    //printf("%s:%d cmd name : %s\n",__FUNCTION__, __LINE__, cmd);
    while (1) {
        printf("main\n");
        system("rm -rf b.txt");
        system("cp a.txt b.txt");
        sleep(1);
    }
    return 0;
}
#endif
