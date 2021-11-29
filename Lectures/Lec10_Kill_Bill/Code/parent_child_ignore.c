#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
typedef void func(int);

void child_signal_handler(int signal){
	printf("I received a signal from my parrent %d\n", getppid());
	printf("The signal is %d\n", signal);
	printf("I don't care about your signal, ma! I simply ignore it\n");
}

int main(int argc, char *argv[]){
	printf("I am a lonely process, pid=%d\n", getpid());	
	int child_pid = fork();
	if(child_pid == -1){
		perror("impossible to have a child!\n");
		exit(1);
	}
	if(child_pid >= 0){//(child_pid != -1)
		if(child_pid > 0)
			printf("I am the parent, pid=%d\n", getpid());
		else{//(child_pid == 0)
			printf("I am the child, pid=%d\n", getpid());
			//subscribe a signal handler to the kernel
			void *res = signal(SIGUSR1, child_signal_handler);
			if(res == SIG_ERR)
				printf("subscription for SIGUSR1 was not successful!\n");

			while(1){}//busy waiting
			exit(0);
		}
	}

	printf("I sleep for 10 second and will be back with you to check on your progress, child.\n");
	sleep(10);
	kill(child_pid, SIGUSR1);

	printf("I sleep for another 10 second. Hopefully you're done by then.\n");
	sleep(10);
	kill(child_pid, SIGUSR1);

	printf("What's up child.\n");
	sleep(10);
	kill(child_pid, SIGUSR1);

	printf("Seems you don't care. I'll kill you\n!");
	kill(child_pid, SIGINT);


	int child_exit;
	wait(&child_exit);
	if (WIFEXITED(child_exit))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(child_exit));
	else if (WIFSIGNALED(child_exit))
		printf("abnormal termination, signal number = %d\n", WTERMSIG(child_exit));

	exit(0);
}
