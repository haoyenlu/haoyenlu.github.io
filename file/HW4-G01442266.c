// Hao-En Lu G01442266

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>


# define MAX_TOKEN 10

int save_stdout; // saving console output pipeline

void split(char*,const char*,char*[]); // split the command into tokens
void CHILD_PROC(char* []); // Child Process
int PARENT_PROC(void); // Parent Process
int CS531_system(char*); //system

// split the command into tokens
void split(char* str, const char* delim,char * arr[]){

    int index = 0;
    char* token = strtok(str,delim);


    while (token != NULL) {
        arr[index++] = token;
        token = strtok(NULL,delim);
    }
}

// Child Process
void CHILD_PROC(char* argv[]){

    // redirect the output to original stdout
    dup2(save_stdout,STDOUT_FILENO);

    // execute command
    execvp(argv[0],argv);

    exit(EXIT_SUCCESS);
}


// Parent Process
int PARENT_PROC(void){

    // ignore SIGINT and SIGQUIT signal
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);

    int cstatus;
    
    do {
        pid_t w = waitpid(-1,&cstatus,WUNTRACED | WCONTINUED); // wait till the child process finish or stop

        // waitpid error
        if (w == -1){
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // handle child exit status
        if (WIFEXITED(cstatus)){
            printf("Child process exit with status %d\n",WEXITSTATUS(cstatus));
            break;
        }

        // handle child terminate status
        else if (WIFSIGNALED(cstatus)){
            printf("Child process terminate by signal %d\n",WTERMSIG(cstatus));
            break;
        }

        // handle child stop status
        else if (WIFSTOPPED(cstatus)){
            printf("Child process stopped by signal %d\n",WSTOPSIG(cstatus));
        }

        // handle child continue status
        else if (WIFCONTINUED(cstatus)){
            printf("Child process continued.\n");
        }

    } while (!WIFEXITED(cstatus) && !WIFSIGNALED(cstatus)); // only breaks loop when child terminate or exit.

    // return to default action for SIGINT and SIGQUIT handling 
    signal(SIGINT,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);


    return cstatus;
}

// system 
int CS531_system(char *s){

    int status;

    char* commands[MAX_TOKEN] = {NULL};
    split(s," ",commands); // split commands

    // create child process
    int pid = fork();

    switch (pid)
    {
    case -1:
        // fork failed
        printf("Error:Create child process failed.\n");
        break;
    
    case 0:
        // child process
        CHILD_PROC(commands);
        break;

    default:
        // parent process
        status = PARENT_PROC();
        break;
    }  

    return status;
}



int main(int argc, char * argv[]){

    // if redirection
    if (argc > 2){
        // save console standard output
        save_stdout = dup(STDOUT_FILENO);
    }

    int status;
    status = CS531_system(argv[1]);

    printf("Parent process is sleeping..\n");
    sleep(10);

    return status;
}