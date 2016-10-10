#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char *dir,int depth) 
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	int tab;

	if((dp=opendir(dir))==NULL) 
	{
		fprintf(stderr,"cannot open directory: %s\n",dir);
		return;
	}

	chdir(dir);
	while((entry=readdir(dp))!=NULL) 
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			/* Found a directory, but ignore . and .. */
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
				continue;

			tab=depth;
			while(tab--) printf("\t");
			printf("%s/\n",entry->d_name);
			/* Recurse at a new indent level */
			printdir(entry->d_name,depth+1);
		}
		else if(entry->d_name[0]!='.')
		{

			tab=depth;
			while(tab--) printf("\t");
			printf("%s\n",entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}

int main()
{
	printf("Directory scan of current location:\n");
	printdir("./",0);
	printf("done.\n");
	exit(0);
}
