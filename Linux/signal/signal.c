#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig);
void keycontrol(int sig);

int main(int argc, char* argv[]) {
    int i;
    
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    
    puts("2sec alarm이 설정되었습니다.");
    alarm(2);

    for(i = 0; i < 3; i++) {
        puts("프로세스를 100sec동안 sleep합니다.");
        sleep(100);
    }

    return 0;
}

void timeout(int sig) {
    if(sig == SIGALRM) {
        puts("alarm 시간이 초과되었습니다.");
    }

    puts("2sec alarm이 설정되었습니다.");
    alarm(2);
}

void keycontrol(int sig) {
    if(sig == SIGINT) {
        puts("CTRL + C pressed: signal이 발생하여 프로세스가 깨어납니다.");
    }
}