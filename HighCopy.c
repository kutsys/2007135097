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
	printf("done\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	char block[1024], fin[256], fout[256];
	FILE *in, *out;
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
		printf("Error: fork() failed\n");
		exit(1);
	case 0:
		if((in=fopen(fin, "r"))==NULL)
		{
			printf("Error: No such file\n");
			exit(1);
		}
		out=fopen(fout, "w");
		while((nread=fread(block, sizeof(char), sizeof(block), in))>0)
			fwrite(block, sizeof(char), nread, out);
		fclose(in);
		fclose(out);
		exit(0);
	}

	(void)signal(SIGCHLD, exit_parent);
	while(1)
	{
		sleep(1);
		printf("*");
	}
}
