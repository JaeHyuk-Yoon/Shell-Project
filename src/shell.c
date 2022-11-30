/* mini_shell.c */

/*
* 2022. 11. 30
* 동의대학교 컴퓨터소프트웨어공학과
* 김희진, 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void my_ctrlc(int sig){
    signal(sig, SIG_IGN);
    printf("  -> Ctrl_C 가 입력되었습니다. - 쉘 종료\n");
    exit(1);
}
void my_ctrlz(int sig, int flag){
    signal(sig, SIG_IGN);
        printf("  -> Ctrl_z 가 입력되었습니다. - 쉘 일시정지\n");
    printf(" fg 명령으로 재개 가능..\n");
    raise(SIGSTOP);
    printf(" 쉘 재개..\n");
    signal(sig, my_ctrlz);
}

int getargs(char *cmd, char **argv) {
    int narg = 0;
    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' '&& *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}

void main() {
    char buf[256];
    char *argv[50];
    int narg;
    
    struct sigaction ctrlc_act;
    struct sigaction ctrlz_act;
    ctrlc_act.sa_handler = my_ctrlc;
    ctrlz_act.sa_handler = my_ctrlz;
    sigaction(SIGINT, &ctrlc_act, NULL);
    sigaction(SIGTSTP, &ctrlz_act, NULL);

    printf("         MYShell을 시작합니다.\n");
    printf("========================================\n");

    while (1) {
        pwd_print();
        printf(" : shell> ");
        gets(buf);
        narg = getargs(buf, argv);  //들어온 인자 갯수
        int t_opt = 0;  //task option
        
        for (int i = 0; i < narg; i++) {

            if(!strcmp(argv[i], "exit")){
                printf("쉘을 종료합니다../\n");
                exit(1);
            }
        }
    }
}
