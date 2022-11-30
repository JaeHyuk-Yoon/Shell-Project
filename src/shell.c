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
