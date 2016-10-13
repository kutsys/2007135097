#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

void exit_parent(int sig)
{
	printf("Done\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	char block[1024], fin[256], fout[256];
	int in, out;
	int nread;
	pid_t pid;

	if(argc>1) strcpy(fin, argv[1]);
	else 
	{
		printf("Error: No input filename specified\n");
		exit(1);
	}

	if(argc>2) strcpy(fout, argv[2]);
	else strcpy(fout, "file.out"); 	

	pid=fork();
	switch(pid)
	{
	case -1:
		printf("Error: fork() failed");
		exit(1);
	case 0:
		if((in=open(fin, O_RDONLY))==-1)
		{
			printf("Error: No such file\n");
			exit(1);
		}
		out=open(fout, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
		while((nread=read(in, block, sizeof(block)))>0)
			write(out, block, nread);
		close(in);

		close(out);
		exit(0);
	}

	(void)signal(SIGCHLD, exit_parent);
	while(1)
	{
		sleep(1);
		printf(".");
	}
}
