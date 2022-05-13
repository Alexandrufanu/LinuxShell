
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <stdbool.h>
#include <menu.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

extern char **environ;
char tempf[50];
int WriteOutput = 0;
int AppendOutput = 0;
char outputFile[180]="";


void add_history(char queue[][180], int occupied, char element[])
{
    if(occupied<99){
        strcpy(queue[occupied], element);

    } else
    {
        for (int i = 99; i>0;i--)
        {
            strcpy(queue[i-1], queue[i]);

        }
        strcpy(queue[99], element );

    }
}



void clear_screen()
{
    int x,y;
    getyx(stdscr, y, x);
    move(y, 0);
    clrtoeol();
    printw(">");
    move(y, 1);
    refresh();
}


char normalizedInput[180][100][100] ;



int executeCat(char input[180][180] , int fdout) {

    char buffer[BUFSIZ]="";

    long bytes;

    int  fd[180], i = 2, j=0, k=0, sentenceCounter = 1, suppressed = 0, skip = 0, receivedFileParam = 0 ;


    char lastWritten[BUFSIZ+10];
    char writingNow[BUFSIZ+10]="";
    char ch;


    for (int index=0 ; index< strlen(input[1]);index++)
    {
        if ( input[1][index]!='b' && input[1][index]!='E' &&input[1][index]!='n' &&input[1][index]!='s' )
        {
            char temp[29]="CAT ERROR: invalid option -";

            temp[27]=input[1][index];
            temp[28]='\0';
            write(fdout, temp, strlen(temp));
            close(fdout);
            exit(0);

        }

    }
    while (input[i][0]!='\0' && input[i]!=NULL)
    {

        if(input[i][0]!='-' && strlen(input[i]))
        {
            receivedFileParam = 1;

            fd[i] = open(input[i], O_RDONLY);

            if (fd[i] == -1 )
            {
                char temp[100] = "CAT ERROR: COULD NOT OPEN ";
                strcat(temp, input[i]);
                write(fdout, temp, strlen(temp));

                close(fd[i]);
                close(fdout);

                exit(0);
            }

            while( (bytes = read(fd[i], buffer, sizeof (buffer))) > 0) {
                buffer[bytes] = '\0';

                j=0; 
                k=0; 
                while ((ch =buffer[j]) !='\0' )
                {

                    if (ch =='\n')
                    {

                        writingNow[k++] = ch;
                        writingNow[k] = '\0';


                        
                        if (strchr(input[1], 's'))
                        {
                            if (strlen(writingNow)==1)
                            {
                                if (suppressed == 0)
                                {
                                    suppressed = 1;

                                }
                                else {
                                    writingNow[0] = '\0';
                                    k = 0;
                                    j++;
                                    continue;
                                    skip =1;
                                }

                            } else
                                suppressed = 0;

                        }



                        
                        if (strchr(input[1], 'b'))
                        {
                            if (strlen(writingNow)>1) {
                                char temp[BUFSIZ+10];
                                snprintf(temp, sizeof temp, "   %d ", sentenceCounter++);

                                strcat(temp, writingNow);

                                strcpy(writingNow, temp);
                            }
                        }
                        else 
                        if (strchr(input[1], 'n'))
                        {


                            if (strlen(writingNow)>1) {
                                char temp[BUFSIZ+10];
                                snprintf(temp,sizeof temp, "   %d ", sentenceCounter++);

                                strcat(temp, writingNow);

                                strcpy(writingNow, temp);
                            } if (strlen(writingNow) == 1)
                                snprintf(writingNow, sizeof writingNow,"   %d \n", sentenceCounter++);


                        }

                        
                        if (strchr(input[1], 'E'))
                        {
                            if(strlen(writingNow)>=1) {
                                char temp[BUFSIZ+10];

                                writingNow[strlen(writingNow)-1] = '\0';
                                strcat(writingNow, "$\n");



                            }
                        }



                        write(fdout, writingNow, strlen(writingNow));

                        writingNow[0] = '\0';
                        k = 0;

                    }else
                        writingNow[k++] = ch;

                    j++;

                }



            }


            close(fd[i]);
        }

        i++;
    }


    close(fdout);

    exit(0);

}



int inputReader(char buffer[BUFSIZ])
{

    int userInput;
    buffer[0] = '\0';
    
    int sentenceSize = 0;
    int y, x;

//    strcpy(buffer, "\n");
//    printw(buffer);

    while(1) {

        userInput = getch();


        if (userInput == KEY_F(3))
        {

            return 1;





            return 0;
        }
        if (userInput=='\n') {


            printw("\n");
            buffer[sentenceSize++] = (char) userInput;

            buffer[sentenceSize] = '\0';
            return 0;



        }
        else
        if(userInput == '\b' || userInput == 263 || userInput == 127)
        {

            if (sentenceSize >0) {

                buffer[--sentenceSize] = '\0';
                getyx(stdscr, y, x);
                move(y, x - 1);
                printw(" ");
                move(y, x - 1);
            }
        }
        else
        if(userInput == 259 || userInput == 258 || userInput==261 || userInput == 260){
            getch();
            continue;
        }
        else
        {
            printw("%c", userInput) ;
            buffer[sentenceSize++] = (char) userInput;
            buffer[sentenceSize] = '\0';
        }

    }


}






void executeCatNoArgs(char input[180][180] , int fdout) {

    char buffer[BUFSIZ] = "";
    ssize_t readBytes;

    long bytes;

    int i = 2, j = 0, k = 0, sentenceCounter = 1, suppressed = 0, skip = 0, receivedFileParam = 0;


    char lastWritten[BUFSIZ];
    char writingNow[BUFSIZ] = "";
    char ch;

    printw("\n");
    while (inputReader(buffer)  == 0) {

        j = 0; 
        k = 0; 
        while ((ch = buffer[j]) != '\0') {

            if (ch == '\n') {

                writingNow[k++] = ch;
                writingNow[k] = '\0';


                if (strchr(input[1], 's')) {
                    if (strlen(writingNow) == 1) {
                        if (suppressed == 0) {
                            suppressed = 1;

                        } else {
                            writingNow[0] = '\0';
                            k = 0;
                            j++;
                            continue;
                            skip = 1;
                        }

                    } else
                        suppressed = 0;

                }



                
                if (strchr(input[1], 'b')) {
                    if (strlen(writingNow) > 1) {
                        char temp[BUFSIZ];
                        sprintf(temp, "   %d ", sentenceCounter++);

                        strcat(temp, writingNow);

                        strcpy(writingNow, temp);
                    }
                } else 
                if (strchr(input[1], 'n')) {


                    if (strlen(writingNow) > 1) {
                        char temp[BUFSIZ];
                        sprintf(temp, "   %d ", sentenceCounter++);

                        strcat(temp, writingNow);

                        strcpy(writingNow, temp);
                    }
                    if (strlen(writingNow) == 1)
                        sprintf(writingNow, "   %d \n", sentenceCounter++);


                }

                
                if (strchr(input[1], 'E')) {
                    if (strlen(writingNow) >= 1) {
                        char temp[BUFSIZ];

                        writingNow[strlen(writingNow) - 1] = '\0';
                        strcat(writingNow, "$\n");



                    }
                }

                printw(writingNow);


                writingNow[0] = '\0';
                k = 0;

            } else
                writingNow[k++] = ch;

            j++;

        }




    }






    printw("\n");



}


void executeEnv(char input[180][180] , int fdout)
{
    char writingNow[BUFSIZ*2]="";
    char varName[BUFSIZ]="";
    int inVar = 1, i=0, j=0, ignore=0;
    FILE * file;
    char c;
    char buf[2];
    buf[1] = '\0';
    file = fopen(tempf, "r");





    while((c = (char)getc(file)) != EOF) {
        buf[0]= c;

        if (strchr(input[1], 'u') == 0)
        {

            if (c == '\n')
            {
                strcat(writingNow, buf);
                inVar = 1;

                for (int m = 2; input[m][0]!='\0'; ++m) {
                    if(input[m][0]!='-')
                    {
                        if(strcmp(input[m], varName) == 0)
                        {
                            ignore = 1;
                        }
                    }
                }

                if(!ignore)
                    write(fdout, writingNow, strlen(writingNow));

                ignore = 0;

                writingNow[0] ='\0';

            } else if (c == '=' && inVar)
            {
                strcat(writingNow, buf);
                inVar = 0;
            } else {
                strcat(writingNow, buf);

                if(inVar){
                    strcat( varName, buf);
                }

            }


        }
        else write(fdout, buf, strlen(buf));

    }








    close(fdout);


    exit(0);
}

void getEnv()
{
    time_t now = time(0);


    snprintf(tempf, sizeof tempf, "%ld.txt", now);

    FILE * file;

    file = fopen(tempf, "w");

    int i=1;
    while(environ[i]) {


        fputs( environ[i],file);
        fputs("\n", file );
        i++;
    }


    fclose(file);


}



void executeEnv_(char input[180][180] , int fd)
{




    int i=0;

    while(environ[i]) {

        if (strchr(input[1], 'u'))
        {
            char varName[200];
            int j = 0, ignore = 0;

            while (environ[i][j] != '=')
            {
                varName[j] = environ[i][j];
                j++;
            }
            varName[j]='\0';

            for (int m = 2; input[m][0]!='\0'; ++m) {
                if(input[m][0]!='-')
                {
                    if(strcmp(input[m], varName) == 0)
                    {
                        ignore = 1;
                    }
                }
            }

            if(!ignore)
            {
                write(fd, environ[i], strlen(environ[i]));
                write(fd, "\n", strlen("\n"));
            }

        }
        else {
            write(fd, environ[i], strlen(environ[i]));
            write(fd, "\n", strlen("\n"));
        }


        i++;
    }





    close(fd);
    exit(0);
}









int executeHead(char input[180][180] , int fdout) {

    char buffer[BUFSIZ]="";
    ssize_t readBytes;
    FILE *fptr;
    fptr = fopen("aa.txt", "r");

    long bytes;

    int  fd[180], i = 2, j=0, k=0, sentenceCounter = 1, suppressed = 0, skip = 0, receivedFileParam = 0 ;


    char lastWritten[BUFSIZ+10];
    char writingNow[BUFSIZ+10]="";
    char ch;

    int printFirstXElem = 1;
    int printLines = 1;
    int X = 10;

    int verbose = -1;





    int ii = 1;


    for (int index =2; input[index][0]!=0;index ++)
    {
        if (input[index][0]!='-')
        {
            verbose++;
        }

    }

    for (int index=0 ; index< strlen(input[1]);index++)
    {
        if ( input[1][index]!='c' && input[1][index]!='n' &&input[1][index]!='q' &&input[1][index]!='v' &&input[1][index]!='-' && !isdigit(input[1][index]) )
        {
            char temp[29]="HEAD ERROR: invalid option: -";

            temp[27]=input[1][index];
            temp[28]='\0';

            write(fdout, temp, strlen(temp));
            close(fdout);
            exit(0);

        }

        if (input[1][index] == 'q')
            verbose = 0;
        if(input[1][index] == 'v')
            verbose = 1;



    }



    
    for (int index=0 ; index< strlen(input[1]);index++)
    {

        if (input[1][index] == 'c' || input[1][index] == 'n' )
        {
            X=0;
            if (input[1][index] == 'c')
                printLines = 0;
            else printLines = 1;

            index++;
            if(index< strlen(input[1]))
            {
                if (input[1][index] == '-')
                {
                    printFirstXElem = 0;
                    index ++;
                }
                else if(isdigit(input[1][index]))
                    printFirstXElem = 1;
                else
                {
                    char temp[60]="HEAD ERROR: after options -n and -c a number is expected!";



                    write(fdout, temp, strlen(temp));
                    close(fdout);
                    exit(0);
                }
                int temp=0;
                while (isdigit(input[1][index]))
                {

                    temp = (input[1][index] - '0') +temp*10;
                    index++;
                }
                X = temp;



            }
            else
            {
                char temp[60]="HEAD ERROR: after options -n and -c a number is expected";
                write(fdout, temp, strlen(temp));
                close(fdout);
                exit(0);
            }



        }
    }





    while (input[i][0]!='\0' && input[i]!=NULL)
    {

        if(input[i][0]!='-' && strlen(input[i]))
        {
            int copyX = X;

            fd[i] = open(input[i], O_RDONLY);

            if (fd[i] == -1 )
            {
                char temp[100] = "HEAD ERROR: COULD NOT OPEN ";
                strcat(temp, input[i]);
                write(fdout, temp, strlen(temp));

                close(fd[i]);
                close(fdout);

                exit(0);
            }

            if (strchr(input[1], 'v'))
            {
                char temp[60]="++>";
                snprintf(temp, sizeof temp,"==>%s<==\n", input[i]);


                write(fdout, temp, strlen(temp));
            }

            

            
            


            if (!printFirstXElem) {
                int tempfd = open(input[i], O_RDONLY), nr = 0;

                while ((bytes = read(tempfd, buffer, sizeof(buffer))) > 0) {

                    j = 0; 
                    k = 0; 

                    while ((ch = buffer[j]) != '\0') {

                        if (printLines) {

                            if (ch == '\n') {
                                nr++;

                            } else;


                        } else nr++;

                        j++;

                    }

                }
                close(tempfd);

                copyX = nr - X;






            }

            while( (bytes = read(fd[i], buffer, sizeof (buffer))) > 0) {
                buffer[bytes] = '\0';

                j=0; 
                k=0; 

                while ((ch =buffer[j]) !='\0' )
                {
                    if (printLines) {

                        if (copyX > 0) {

                            if (ch == '\n') {

                                writingNow[k++] = ch;




                                writingNow[k] = '\0';

                                char temp[10];



                                write(fdout, writingNow, strlen(writingNow));

                                writingNow[0] = '\0';
                                k = 0;
                                copyX--;

                            } else
                                writingNow[k++] = ch;

                        } else break;


                    }
                    else {

                        if (copyX > 0) {

                            char temp[2];
                            temp[0] = ch;
                            temp[1] ='\0';

                            write(fdout, temp, strlen(temp));



                            copyX--;

                        } else break;


                    }




                    j++;


                }



            }


            close(fd[i]);
        }

        i++;
    }


    close(fdout);

    exit(0);

}

int executeHeadNoArgs(char input[180][180] , int fdout) {

    char buffer[BUFSIZ]="";
    ssize_t readBytes;

    long bytes;

    int  fd[180], i = 2, j=0, k=0, sentenceCounter = 1, suppressed = 0, skip = 0, receivedFileParam = 0 ;


    char lastWritten[BUFSIZ+10];
    char writingNow[BUFSIZ+10]="";
    char ch;

    int printFirstXElem = 1;
    int printLines = 1;
    int X = 10;

    int verbose = -1;





    int ii = 1;


    for (int index =2; input[index][0]!=0;index ++)
    {
        if (input[index][0]!='-')
        {
            verbose++;
        }

    }

    for (int index=0 ; index< strlen(input[1]);index++)
    {
        if ( input[1][index]!='c' && input[1][index]!='n' &&input[1][index]!='q' &&input[1][index]!='v' &&input[1][index]!='-' && !isdigit(input[1][index]) )
        {
            char temp[29]="HEAD ERROR: invalid option: -";

            temp[27]=input[1][index];
            temp[28]='\0';

            printw("%s", temp);
            close(fdout);
            return (0);

        }

        if (input[1][index] == 'q')
            verbose = 0;
        if(input[1][index] == 'v')
            verbose = 1;



    }





    int copyX = X;




    if (strchr(input[1], 'v'))
    {

        printw("\n++>Standard Input<++\n");
    }

    

    
    


    if (!printFirstXElem) {

        int  nr = 0;


        j = 0; 
        k = 0; 

        while ((ch = buffer[j]) != '\0') {

            if (printLines) {

                if (ch == '\n') {
                    nr++;

                } else;


            } else nr++;

            j++;

        }


        copyX = nr - X;


    }
    while (inputReader(buffer)  == 0)
    {

        j=0; 
        k=0; 

        while ((ch =buffer[j]) !='\0' )
        {
            if (printLines) {

                if (copyX > 0) {

                    if (ch == '\n') {

                        writingNow[k++] = ch;


                        writingNow[k] = '\0';

                        char temp[10];


                        printw(writingNow);


                        writingNow[0] = '\0';
                        k = 0;
                        copyX--;

                    } else
                        writingNow[k++] = ch;

                } else break;


            }
            else {

                if (copyX > 0) {

                    char temp[2];
                    temp[0] = ch;
                    temp[1] ='\0';

                    printw(temp);




                    copyX--;

                } else break;


            }


            j++;


        }
        if(copyX == 0) break;


    }

}





void cleanInput(char s[180][180])
{

    if (strcmp(s[0], "cat")==0 || strcmp(s[0], "env")==0) {
        int i = 1;
        while (s[i] != NULL && s[i][0] != '\0') {
            i++;
        }

        int j;
        for (j = i; j > 1; j--) {

            strcpy(s[j], s[j - 1]);
        }


        i = 1;
        char temp[100] = "";
        int k = 0;
        int argsPresent = 0;
        while (s[i][0] != '\0' && s[i] != NULL) {
            argsPresent = 1;
            if (s[i][0] == '-') {
                int j = 1;

                while (s[i][j] != '\0') {
                    temp[k++] = s[i][j];
                    j++;
                }

            }

            i++;
        }
        temp[k] = '\0';

        strcpy(s[1], temp);
    }
    else if(strcmp(s[0], "head")==0)
    {
        int i = 1;
        while (s[i] != NULL && s[i][0] != '\0') {
            i++;
        }

        int j;
        for (j = i; j > 1; j--) {

            strcpy(s[j], s[j - 1]);
        }


        i = 1;
        char temp[100] = "";
        int k = 0;
        while (s[i][0] != '\0' && s[i] != NULL) {
            if (s[i][0] == '-' && !isdigit( s[i][1] )) {


                int j = 1;

                while (s[i][j] != '\0') {
                    temp[k++] = s[i][j];
                    j++;
                }
                s[i][0]='-';
                s[i][1]='\0';

            }else
            if(isdigit( s[i][0] ) || (s[i][0] == '-' && isdigit( s[i][1] ))){
                int j = 0;

                while (s[i][j] != '\0') {
                    temp[k++] = s[i][j];
                    j++;
                }

                s[i][0]='-';
                s[i][1]='\0';
            }

            i++;
        }
        temp[k] = '\0';

        strcpy(s[1], temp);

    }


}

bool executedManually(char input[180][180],  int fdin, int fdout, int hasArgs)
{



    if (hasArgs) {
        if (strcmp(*input, "cat") == 0) {
            cleanInput(input);
            



            executeCat(input, fdout);
            return true;
        } else if (strcmp(*input, "head") == 0) {
            cleanInput(input);

            executeHead(input, fdout);
            return true;

        }
        else if (strcmp(*input, "env")==0)
        {
            cleanInput(input);

            
            executeEnv_(input, fdout);
            return true;

        }


    }
    else
    {

        if (strcmp(*input, "cat") == 0) {


            int hasargs=0;
            for (int i=1; strlen(input[i])>0;i++)
                if(input[i][0]!='-' && input[i][0]!=0 && input[i][0]!=' ')
                    hasArgs = 1;

            if (!hasArgs) {
                cleanInput(input);

                executeCatNoArgs(input, fdout);
                return true;
            }
        }else  if (strcmp(*input, "head") == 0) {

            int hasargs=0;
            for (int i=1; strlen(input[i])>0;i++)
                if(input[i][0]!='-' && input[i][0]!=0 && input[i][0]!=' ')
                    hasArgs = 1;

            if (!hasArgs) {
                cleanInput(input);

                executeHeadNoArgs(input, fdout);
                return true;
            }
        }



    }
    return false;

}


void if_else_nested(int *pipes, int forkNr, int currentFork ) { 


    int err;
    char *argumentsVector_[180], argStr_[180][180];
    for (int i = 0; i < normalizedInput[currentFork][0][0]; i++)
        argumentsVector_[i] = normalizedInput[currentFork][i + 1], strcpy(argStr_[i], normalizedInput[currentFork][i +
                                                                                                                   1]);;

    argumentsVector_[normalizedInput[currentFork][0][0]] = NULL;
    argStr_[normalizedInput[currentFork][0][0]][0] = 0;

    int status = 0;

    if (executedManually(argStr_, 0, pipes[1], 0)) { ; }
    else {
        if (strcmp(*argStr_, "env") == 0) {
            getEnv();
        }
        if (currentFork == 0) 
        {


            if (fork() == 0) {




                dup2(pipes[1], 1);



                for (int i = 0; i < forkNr * 2; i++) {
                    if (i != 1)
                        close(pipes[i]);

                }

                char *argumentsVector[180], argStr[180][180];
                for (int i = 0; i < normalizedInput[currentFork][0][0]; i++)
                    argumentsVector[i] = normalizedInput[currentFork][i + 1], strcpy(argStr[i],
                                                                                     normalizedInput[currentFork][i +
                                                                                                                  1]);;

                argumentsVector[normalizedInput[currentFork][0][0]] = NULL;


                if (executedManually(argStr, 0, pipes[1], 1)) {
                } else
                    status = execvp(*argumentsVector, argumentsVector);



                if(status == -1)
                {
                    char t[150];
                    strcpy(t, *argumentsVector);
                    strcat(t, " is not a valid command ");
                    write(pipes[1], t , strlen(t));
                    close(pipes[1]);
                    exit(0);
                }


            } else {

                if_else_nested(pipes, forkNr, currentFork + 1);

            }
        } else if (currentFork == forkNr) {


            char buffer[BUFSIZ*2] = "";

            for (int i = 0; i < forkNr * 2; i++) {

                if (i != (currentFork - 1) * 2)
                    close(pipes[i]);

            }

            printw("\n");
            long len;
            if(AppendOutput)
            {
                FILE * file;
                file = fopen(outputFile, "a");
                int i=1;
                while((len = read(pipes[(currentFork - 1) * 2], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[len] = 0;

                    fputs( buffer,file);
                    i++;
                }

                fclose(file);
            } else if (WriteOutput)
            {
                FILE * file;

                file = fopen(outputFile, "w");

                int i=1;
                while((len = read(pipes[(currentFork - 1) * 2], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[len] = 0;

                    fputs( buffer,file);
                    i++;
                }


                fclose(file);
            }
            else
                while ((len = read(pipes[(currentFork - 1) * 2], buffer, sizeof(buffer) - 1)) > 0) {

                    buffer[len] = 0;

                    printw("%s",buffer);
                }

            printw("\n");


            close(pipes[(currentFork - 1) * 2]);




        } else if (currentFork < forkNr) {
            if (fork() == 0) {


                dup2(pipes[(currentFork - 1) * 2], 0);

                dup2(pipes[(currentFork - 1) * 2 + 3], 1);


                for (int i = 0; i < forkNr * 2; i++) {
                    
                    close(pipes[i]);

                }

                char *argumentsVector[10];
                for (int i = 0; i < normalizedInput[currentFork][0][0]; i++)
                    argumentsVector[i] = normalizedInput[currentFork][i + 1];
                argumentsVector[normalizedInput[currentFork][0][0]] = NULL;


                status = execvp(*argumentsVector, argumentsVector);
                if(status == -1)
                {



                    exit(0);
                }

            } else {

                if_else_nested(pipes, forkNr, currentFork + 1);

            }
        }

    }
}



void f_tester(int forksNr)
{
    int status;

    int pipes[(forksNr-1)*2];

    for (int i =0 ; i<= (forksNr-1)*2; i+=2)
        pipe(pipes +i);

    if_else_nested(pipes ,  forksNr, 0);


    for (int  i= 0 ; i< forksNr *2; i++)
    {
        close(pipes[i]);

    }

    for (int i = 0; i <= forksNr; i++) {
        wait(&status);

    }

}



void clearInput()
{
    for (int i=0;i<180;i++)
        for (int j=0;j<100;j++)
            normalizedInput[i][j][0]=0;


}

int remove_pipes(char *statement) {



    char *result[180] = {NULL};
    char *copy = malloc(strlen(statement) * sizeof(char));
    strcpy(copy, statement);

    strtok(copy, "|");


    int i = 0;
    while (copy != NULL) {
        
        result[i++] = copy;

        copy = strtok(NULL, "|");
    }
    result[i] = 0;


    for (i = 0; result[i] != 0; i++) {

        int j = 0, k = 1, spaced = 1, l = 0, innewword = 1;
        normalizedInput[i][0][0] = 0;
        while (result[i][j] != 0) {
            if (result[i][j] != ' ') {
                spaced = 0;
                normalizedInput[i][k][l++] = result[i][j];
                if (innewword == 1) {
                    normalizedInput[i][0][0]++;
                    innewword = 0;
                }


            } else {
                if (spaced == 0) {
                    spaced = 1;
                    normalizedInput[i][k][l] = '\0';
                    k++;
                    l = 0;
                    innewword = 1;
                }
            }
            j++;
        }

    }


    return i;

}


void getRedirections( char input[180])
{
    strcpy(outputFile, "");
    WriteOutput = 0;
    AppendOutput = 0;
    int k=0, pos= strlen(input);
    char c;

    int i=0,j=0;
    while (input[i]!='\0')
    {
        if(input[i] == '>' && WriteOutput==0)
        {
            WriteOutput = 1;
            j=i+1;
            pos = i;

        } else if (WriteOutput && input[i]!=' '){
            outputFile[k++]=input[i];
        }

        i++;
    }
    outputFile[k]='\0';

    while (input[j]!='\0')
    {
        if(input[j] == '>')
        {
            AppendOutput = 1;
            k=0;

        } else if (AppendOutput && input[j]!=' '){
            outputFile[k++]=input[j];
        }
        j++;
    }

    outputFile[k]='\0';

    input[pos] = '\0';


}


int main() {

    int c, occupied = 0;
    int queue_pos = 0;



    char queue[300][180];

    int userInput;

    initscr();


    raw();
    keypad(stdscr, TRUE);



    char currentSentence[180];
    int sentenceSize = 0;

    noecho();
    scrollok(stdscr, TRUE);

    int y, x, hard_clear = 0;
    bool f1_pressed = false;

    printw("In order to switch to commands already used use F1 and F2; Press F3 for exit;\n");
    printw(">");
    while(1) {

        userInput = getch();


        if (userInput == KEY_F(2)) {
            if (f1_pressed)
            {

                if (queue_pos<occupied-1){

                    clear_screen();
                    printw(queue[++queue_pos]);
                    strcpy(currentSentence, queue[queue_pos]);
                    sentenceSize = strlen(currentSentence);
                }
            }
        }
        else
        if (userInput == KEY_F(1)) {
            if(f1_pressed) {
                clear_screen();

                if (queue_pos>0)
                    queue_pos--;
                printw(queue[queue_pos]);
                strcpy(currentSentence, queue[queue_pos]);

            } else {
                if (occupied>0) {
                    clear_screen();
                    f1_pressed = true;
                    queue_pos = occupied-1;
                    printw(queue[queue_pos]);
                    strcpy(currentSentence, queue[queue_pos]);
                    sentenceSize = strlen(currentSentence);

                }
            }
        }
        else
        if (userInput == KEY_F(3))
        {
            endwin();
            endwin();
            endwin();

            exit(0);
            return 0;
        }
        else {
            f1_pressed = false;
            if (userInput=='\n') {

                char temp[180];

                strcpy(temp, currentSentence);

                for (int i = sentenceSize; i<179 ;i++)
                    currentSentence[i]=' ';
                currentSentence[179] ='\0';

                clearInput();

                getRedirections(currentSentence);

                for (int i = sentenceSize; i<179 ;i++)
                    currentSentence[i]=' ';
                currentSentence[179] ='\0';


                int nr = remove_pipes(currentSentence);

                f_tester(nr);

                clear_screen();

                add_history(queue, occupied++, temp);

                hard_clear = 1;


                currentSentence[0] = '\0';
                sentenceSize = 0;


            }
            else
            if(userInput == '\b' || userInput == 263 || userInput == 127)
            {
                
                if (sentenceSize >0) {

                    currentSentence[--sentenceSize] = '\0';
                    normalizedInput[0][0][0] = 0;


                    getyx(stdscr, y, x);
                    move(y, x - 1);
                    printw(" ");
                    move(y, x - 1);
                }
            }
            else
            if(userInput == 259 || userInput == 258 || userInput==261 || userInput == 260 ){
                getch();
                continue;
            } else
            if(userInput == 410)
            {
                continue;
            }
            else
            {
                printw("%c", userInput) ;
                currentSentence[sentenceSize++] = (char) userInput;
                currentSentence[sentenceSize] = '\0';

            }
        }


        refresh();




    }





}

