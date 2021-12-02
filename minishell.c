#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

//implementing ls command
void _ls(char *dir, int depth) {
    DIR *dp;
    struct dirent *entry; 
    struct stat statbuf;
    if((dp=opendir(dir))==NULL) {
        fprintf(stderr,"cannot open directory %s",dir);
        return; 
    }
    chdir(dir);
    while((entry=readdir(dp))!=NULL) {
    lstat(entry->d_name, &statbuf);
    if(S_ISDIR(statbuf.st_mode)) 
    {
        if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
            continue;
        printf("%*s%s/\n",depth,"",entry->d_name); 
        _ls(entry->d_name,depth+4);
    } 
    else
        printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir(".."); 
    closedir(dp);
}


//implementing cat command
void catcommand()
{   
    char line[100];
    char buf[100];
    printf("FILENAME: ");
    gets(line);
    char c;
    int i =0, lineCount=0;
    int sfd = open(line,O_RDONLY);
    if(sfd == -1)
    {
        write(2,"ERROR\n",6);
        exit(1);
    }

    while(read(sfd,&c,1) > 0)
    {
        if(c!='\n')
        {
            buf[i++]=c;
        }
        else
        {
            lineCount++;
            buf[i]='\0';
            printf("%s\n",buf);
            i=0;
        }
    }

    printf("------%d LINES WERE PRINTED--------\n",lineCount);
}

//implementing clear command
void clearconsole()
{
    //using regex
    printf("\e[1;1H\e[2J");
}

//implementing rm command
void deletefile()
{
    char buf[100];
    printf("FILENAME: ");
    gets(buf);
    int ret = remove(buf);
    if(ret == 0)
        printf("File deleted successfuly!\n");
    else
        printf("Failed to delete file\n");
}

int  main(int argc, char *argv[])
{
    char line[1024];
    char buf[100];
    pid_t pid = fork();
    if(pid == -1)
    {
        write(2,"Fork Error",10);
        exit(1);

    }
    else if(pid == 0)
    {
    printf("FORKED.\n");
    while(1)
    {
        printf("Shell in Child> ");
        gets(line);

        if(!strcmp(line,"exit"))
        {
            printf("Exiting Shell Program from Child...\n");
            exit(0);
        }
        else if(!strcmp(line,"ls"))
            _ls(".",0);
        else if(!strcmp(line,"cat"))
            catcommand();
        else if(!strcmp(line,"clear"))
            clearconsole();
        else if(!strcmp(line,"rm"))
            deletefile();
        else
            printf("Command not found!\n");
    }
    }
    else
    {
        wait(NULL);
        printf("In Parent Process.\n");
        printf("Shell in Parent> ");
        gets(line);
        if(!strcmp(line,"exit"))
        {
            printf("Exiting \n");
            return 0;
        }
    }

    return 0;
}