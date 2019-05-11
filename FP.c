#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

#define Size 1000
int aminute[15]={0};
int bminute[15]={0};
int ahour[15]={0};
int bhour[15]={0};
int adaym[15]={0};
int bdaym[15]={0};
int amonth[15]={0};
int bmonth[15]={0};
int adayw[15]={0};
int bdayw[15]={0};
int line=0;
int charconfig=0;

char tester[Size][Size];
char config[Size];
char *perintah2;

void *fungsirun(void *bari){//jadi thread
// char bash[50]="bash ";
// strcat(bash,config);
// system(bash);
    int baris;
    baris=atoi((char*)bari);
    pid_t child;
    child=fork();
    if(child==0)
	{
	char *argv[]={perintah2,tester[baris],NULL};
	execvp(argv[0],argv);
	}     
}

void readconfig2(){//jadi thread
    FILE *fptr;
    char c;
    char filename[]="crontab.data";
    fptr = fopen(filename, "r");
    if (fptr == NULL) 
    { 
        printf("Cannot open file \n"); 
        exit(0); 
    }
    int x;
    int status=1;
    int status2=0;
    int status3=0;
    int bintang=0;
    c = fgetc(fptr); 
    while (c!= EOF) 
    { 
        if (isdigit(c) && status<=5) { 
            x = c - 48;
            if(status==1 && status2==0){
                if(status3==0){
                    aminute[line]=x;
                    status3++;
                }
                else{
                    aminute[line]=aminute[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==1 && status2==1){
                if(status3==0){
                    bminute[line]=x;
                    status3++;
                }
                else{
                    bminute[line]=bminute[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==2 && status2==0){
                if(status3==0){
                    ahour[line]=x;
                    status3++;
                }
                else{
                    ahour[line]=ahour[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==2 && status2==1){
                if(status3==0){
                    bhour[line]=x;
                    status3++;
                }
                else{
                    bhour[line]=bhour[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==3 && status2==0){
                if(status3==0){
                    adaym[line]=x;
                    status3++;
                }
                else{
                    adaym[line]=adaym[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==3 && status2==1){
                if(status3==0){
                    bdaym[line]=x;
                    status3++;
                }
                else{
                    bdaym[line]=bdaym[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==4 && status2==0){
                if(status3==0){
                    amonth[line]=x;
                    status3++;
                }
                else{
                    amonth[line]=amonth[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==4 && status2==1){
                if(status3==0){
                    bmonth[line]=x;
                    status3++;
                }
                else{
                    bmonth[line]=bmonth[line]*10+x;
                    status3=0;
                }
                charconfig=0;
            }
            else if(status==5 && status2==0){
                if(status3==0){
                    adayw[line]=x;
                    status3++;
                    bdayw[line]=adayw[line];
                }
                else{
                    adayw[line]=adayw[line]*10+x;
                    status3=0;
                    bdayw[line]=adayw[line];
                }
                charconfig=0;
            }
            else if(status==5 && status2==1){
                if(status3==0){
                    bdayw[line]=x;
                    status3++;
                }
                else{
                    bdayw[line]=bdayw[line]*10+x;
                    status3=0;
                }
                charconfig=0;
             }
        }
        else if(c==' '){
            if(status==1 && status2==0 && bintang==0){
                bminute[line]=aminute[line];
            }
            else if(status==2 && status2==0 && bintang==0){
                bhour[line]=ahour[line];
            }
            else if(status==3 && status2==0 && bintang==0){
                bdaym[line]=adaym[line];
            }
            else if(status==4 && status2==0 && bintang==0){
                bmonth[line]=amonth[line];
            }
            else if(status==5 && status2==0 && bintang==0){
                bdayw[line]=adayw[line];
            }
            config[charconfig]=c;
            charconfig++;
            status++;
            status2=0;
            status3=0;
            bintang=0;
        }
        else if(c=='-' && status<=5){
            status2++;
            status3=0;
            charconfig=0;
        }
        else if(c=='*' && status<=5){
            if(status==1 && status2==0){
                aminute[line]=0;
                bminute[line]=59;
            }
            else if(status==2 && status2==0){
                ahour[line]=0;
                bhour[line]=23;
            }
            else if(status==3 && status2==0){
                adaym[line]=1;
                bdaym[line]=31;
            }
            else if(status==4 && status2==0){
                amonth[line]=1;
                bmonth[line]=12;
            }
            else if(status==5 && status2==0){
                adayw[line]=0;
                bdayw[line]=6;
            }
            charconfig=0;
            bintang=1;
        }
        else if(c=='\n'){
            status=1;
            status2=0;
            status3=0;
            bintang=0;
            config[charconfig]='\0';
            line++;
        }
        else if(isalnum(c) || c=='/' || c=='.'){
            if(charconfig==1 && config[charconfig-1]==' '){
                charconfig=0;
            }
            config[charconfig]=c;
            charconfig++;
        }
        c = fgetc(fptr);
        config[charconfig+1]='\0';
        strcpy(tester[line],config);
        //line++;
    }
    fclose(fptr);
    // strcpy(tester[line],config);
} 


int main(int argc, char* argv[])
{
    
    time_t t ; 
    struct tm *tmp ; 
    pthread_t tid[10];
    pid_t pid = 0;
    pid_t sid = 0;

    pid = fork();// fork a new child process
    
    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }

    if (pid > 0)// its the parent process
    {
       printf("pid of child process %d \n", pid);
 
        exit(0); //terminate the parent process succesfully
    }

    umask(0);//unmasking the file mode

    sid = setsid();//set new session
    if(sid < 0)
    {
        exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    char semen[Size][Size];
    char *perintah,simpanan[Size],*check,coba[Size],*jalur,jalor[Size];
    int baris;
    int panjang;
    while(1){
        readconfig2();
        time( &t ); 
        tmp = localtime( &t );
        for(baris=0;baris<line;baris++){
            strcpy(semen[baris],tester[baris]);
            perintah=strtok(semen[baris]," ");
            char temporal[Size];
            sprintf(temporal,"%s",perintah);
            check = strstr(temporal,"/bin/");
            if(check){
                  perintah2=strrchr(temporal,'/');
                  panjang=strlen(perintah2);
                  sprintf(coba,"%s",perintah2);
                  int i;
                  for(i=panjang;i>0;i--){
                      simpanan[i-1]=coba[i];
                  }
                  perintah2=simpanan;
                  jalur=strtok(NULL," ");
                  sprintf(jalor,"%s",jalur);
                  sprintf(tester[baris],"%s",jalur);
            }
            else if(strstr(perintah,"/bin/")==0){
                 perintah2="bash";
            }
            if(tmp->tm_min<=bminute[baris] && tmp->tm_min>=aminute[baris]){
                if(tmp->tm_hour<=bhour[baris] && tmp->tm_hour>=ahour[baris]){
                    if(tmp->tm_mday<=bdaym[baris] && tmp->tm_mday>=adaym[baris]){
                        if(tmp->tm_mon<=bmonth[baris] && tmp->tm_mon>=amonth[baris]){
                            if(tmp->tm_wday<=bdayw[baris] && tmp->tm_wday>=adayw[baris]){
                                printf("%d %s %s\n",line,jalor,tester[baris]);
                                char barus[Size];
                                sprintf(barus,"%d",baris);
                                pthread_create(&(tid[baris]),NULL,&fungsirun,(void *)barus);
                                pthread_join(tid[baris],NULL);
                                // fungsirun(baris);
                            }
                        }
                    }
                }   
            }          
        }
        line=0;
        sleep(60);
    }
    return 0;
}
