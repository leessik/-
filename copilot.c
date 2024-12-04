// WSL 2에서 실행 가능한 C/C++ 이진 탐색 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX 100
#define MAX_PATH 1000

int main(int argc, char *argv[]) {
    char path[MAX_PATH];
    char *pathv[MAX];
    char *token;
    int i = 0;
    int status;
    pid_t pid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    // PATH 환경 변수를 읽어서 pathv 배열에 저장
    token = strtok(getenv("PATH"), ":");
    while (token != NULL) {
        pathv[i] = token;
        token = strtok(NULL, ":");
        i++;
    }
    pathv[i] = NULL;

    // 입력받은 명령어가 존재하는지 확인
    for (i = 0; pathv[i] != NULL; i++) {
        sprintf(path, "%s/%s", pathv[i], argv[1]);
        if (access(path, F_OK) == 0) {
            break;
        }
    }

    if (pathv[i] == NULL) {
        fprintf(stderr, "%s: command not found\n", argv[1]);
        exit(1);
    }

    // 자식 프로세스 생성
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // 자식 프로세스
        execv(path, argv + 1);
        perror("execv");
        exit(1);
    } else {
        // 부모 프로세스
        wait(&status);
    }

    return 0;
}

// 실행 예시
// $ gcc -o copilot copilot.c
// $ ./copilot ls -l
// $ ./copilot cat copilot.c
// $ ./copilot notexist
// notexist: command not found
// $ ./copilot
// Usage: ./copilot <command>

// WSL 2에서 실행할 때는 /mnt/c/ 경로를 사용할 수 있음
// $ ./copilot /mnt/c/Windows/System32/notepad.exe
// $ ./copilot /mnt/c/Windows/System32/cmd.exe
// $ ./copilot /mnt/c/Windows/System32/ping.exe
// $ ./copilot /mnt/c/Windows/System32/whoami.exe
// $ ./copilot /mnt/c/Windows/System32/tasklist.exe
// $ ./copilot /mnt/c/Windows/System32/taskkill.exe
// $ ./copilot /mnt/c/Windows/System32/reg.exe
// $ ./copilot /mnt/c/Windows/System32/regedit.exe
// $ ./copilot /mnt/c/Windows/System32/regsvr32.exe
// $ ./copilot /mnt/c/Windows/System32/regini.exe
// $ ./copilot /mnt/c/Windows/System32/regsvr32.exe /?
// $ ./copilot /mnt/c/Windows/System32/reg.exe /?
// $ ./copilot /mnt/c/Windows/System32/regedit.exe /?
// $ ./copilot /mnt/c/Windows/System32/regini.exe /?
// $ ./copilot /mnt/c/Windows/System32/regsvr32.exe /?
