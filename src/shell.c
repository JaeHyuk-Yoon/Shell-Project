/* mini_shell.c */

/*
* 2022. 11. 30
* 동의대학교 컴퓨터소프트웨어공학과
* 김희진, 윤재혁
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

int checkTaskOption(char *argv){  //실행 인자 포함 여부 확인 / 0=없음, -1 = &, 1 = pipe, 2 = <, 3 = >
    int opt = 0;
    if(argv == NULL){
        return opt;
    }
    for(int i=0; argv[i] != NULL; i++){
        if (argv[i] == '&'){
            opt = -1;
            return opt;
        }
        if (argv[i] == '|'){
            opt = 1;
            return opt;
        }
        if (argv[i] == '<'){
            opt = 2;
            return opt;
        }
        if (argv[i] == '>'){
            opt = 3;
            return opt;
        }
    }
    
    return opt;
}

void my_ls(){
    DIR *pdir;
    struct dirent *pde;
    int i = 0;

    if ( (pdir = opendir(".")) < 0 ) {
        perror("opendir");
        exit(1);
    }
    while ((pde = readdir(pdir)) != NULL) {
        printf("%20s ", pde->d_name);
        if (++i % 3 == 0)
            printf("\n");
    }
    printf("\n");
    closedir(pdir);
}

void my_pwd(){
    char buf[1024];
    getcwd(buf, 1024);
    printf("%s\n", buf);
}

void pwd_print(){
    char buf[1024];
    getcwd(buf, 1024);
    printf("%s", buf);
}

void my_cd(char *path){
    if(chdir(path) < 0){
        perror("chdir");
        exit(1);
    }
    else{
        printf("Move to ");
        my_pwd();
    }
}

void my_mkdir(char *path){
    if(mkdir(path, 0777) < 0){
        perror("rmdir");
    }
}

void my_rmdir(char *dir){
    if(rmdir(dir) < 0){
        perror("rmdir");
    }
}

void my_ln(char *src, char *target){
    if (link(src, target) <0){
        perror("link");
    }
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
            int t_opt = checkTaskOption(argv[i + 1]);    //-1 = &, 1 = pipe, 2 = <, 3 = >
            if(t_opt == 1){
                run_pipe(i, argv);
                i += 2;
            }
            else{
                run(i, t_opt, argv);
            }
            if(t_opt > 1){  //it's optional arg
                i += 2;
            }
        }
    }
}
