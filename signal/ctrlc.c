// 此程式會先註冊signal 訊號 如果收到signal會用ouch()來做回覆(只限一次) 
//signal的用法可以先用man 7 signal 去取得
// 可以知道 即使在迴圈當中 若先有註冊signal 那程式也是可以跳去先處理signal

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void ouch(int sig)		//signal handler
{				
	printf("ouch!---I got signal %d\n",sig);
      signal(SIGUSR1,SIG_DFL);  //SIG_DFL reset to default handler
}

int main()
{
    int i, user_pid;
    char buf[50];
    user_pid=getpid();
    signal(SIGUSR1,ouch);
#if 0
    sprintf(buf,"insmod ./signalmod.o user_pid=%d",user_pid);
#endif
    for(i=0;i<30;i++) {
        printf("%d:pid:%d\n",i,user_pid);
        sleep(1);
    }
#if 0
    system(buf);
#endif
}
