#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

enum timer_elements{hours, mins, secs};

#define MAX_INPUTSIZE 16 // timer XX:YY:ZZ + ('\n' -> '\0)
#define TIMER_STR_MAXSIZE 9 // XX:YY:ZZ + '\0'

#define EXIT_FORMAT_ERROR 2
#define EXIT_PARSE_ERROR 3
#define EXIT_CHILD_ERROR 4
#define EXIT_SUBCHILD_ERROR 5
#define EXIT_PARENT_SUCCESS 6


void clear_screen();
int  start();
void display_time();

int  start_timer();
void get_timer_elements(int timer[3], char timer_str[]);
int  timer_manager(int timer[3]);
int  terminal();
int  get_terminal_stdout_path(char* subchild_stdout_path);
int  timer_display(char subchild_stdout_path[], int timer[3]);
void stop_timer();

int  error_occured(int errorcode, char* errormess);
int  check_format(char timer_str[]);
void invalid_input();


void clear_screen()
{
    printf("\033[H\033[J");
}

int start()
{
    while (true)
    {
        char buffer[MAX_INPUTSIZE];
        fgets(buffer, MAX_INPUTSIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        char* first_arg = strtok(buffer, " ");
        if (first_arg == NULL)
        {
            invalid_input();
        }
        else if (strcmp(first_arg, "timer") == 0)
        {
            int timer_result = start_timer();
            if (timer_result != EXIT_SUCCESS && timer_result != EXIT_PARSE_ERROR && timer_result != EXIT_FORMAT_ERROR) return EXIT_FAILURE;
        } 
        else if (strcmp(first_arg, "time") == 0)
        {
            display_time();
        }
        else if (strcmp(first_arg, "stop") == 0)
        {
            stop_timer();
        }
        else if (strcmp(first_arg, "exit") == 0)
        {
            return EXIT_SUCCESS;
        }
        else 
        {
            invalid_input();
        }
    }
}

void display_time()
{
    time_t current_time;
    time(&current_time);
    printf("%s", ctime(&current_time));
}

int start_timer()
{
    char* tmp = strtok(NULL, " ");
    if (!tmp) return error_occured(EXIT_PARSE_ERROR, "Failed to parse timer argument");

    char timer_str[TIMER_STR_MAXSIZE];   
    strcpy(timer_str, tmp);
    tmp = NULL;
    if (check_format(timer_str) == EXIT_FORMAT_ERROR) return EXIT_FORMAT_ERROR;

    int timer[3] = {0};
    get_timer_elements(timer, timer_str);
    
    int timer_manager_result = timer_manager(timer);
    if (timer_manager_result != EXIT_PARENT_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void get_timer_elements(int timer[3], char timer_str[])
{   
    for (size_t i = 0; i < 3; i++)
    {
        char tmp_buff[2];
        (i == 0) ? strcpy(tmp_buff, strtok(timer_str, ":")) : strcpy(tmp_buff, strtok(NULL, ":"));

        timer[i] = atoi(tmp_buff);
    }
}   

// child process
int timer_manager(int timer[3])
{
    pid_t pid = fork();
    if (pid < 0) return error_occured(EXIT_FAILURE, "Failed to fork parent");

    if (pid == 0)
    {
        // entered child process
        pid_t cpid = fork();
        if (cpid < 0) return error_occured(EXIT_CHILD_ERROR, "Failed to fork child");
        
        if (cpid == 0 && terminal() == EXIT_SUBCHILD_ERROR) return EXIT_CHILD_ERROR;

        char subchild_stdout_path[11]; // /dev/pts/X + ('n' -> '\0')
        if (get_terminal_stdout_path(subchild_stdout_path) != EXIT_SUCCESS) return EXIT_CHILD_ERROR;

        chmod(subchild_stdout_path, S_IWOTH); ////////////////

        if (access(subchild_stdout_path, W_OK) == 0)
        {
            if (timer_display(subchild_stdout_path, timer) != EXIT_SUCCESS) return EXIT_CHILD_ERROR;
        }
        else
        {
            return error_occured(EXIT_CHILD_ERROR, "No permission to write to the subchild_stdout(terminal)");
        }

        wait(NULL);
        return EXIT_SUCCESS;
    }
    else
    {
        // entered parent process
        return EXIT_PARENT_SUCCESS;
    }
}

// subchild proccess
int terminal()
{
    // entered subchild proccess
    pid_t parent_pid = getppid(); 
    kill(parent_pid, SIGSTOP);

    char bash_command[80];
    sprintf(bash_command, "tty > subchild_stdout_path.txt;kill -18 %d; exec bash", parent_pid);
    char* args[] = {
        "/usr/bin/konsole",
        "-e",
        "bash",
        "-c",
        bash_command,
        NULL
    };
    
    // turning subchild into terminal
    execv(args[0], args);
    kill(parent_pid, SIGCONT);                                                          // only executes if execv failed
    return error_occured(EXIT_SUBCHILD_ERROR, "Failed to turn subchild into terminal");
}

int get_terminal_stdout_path(char* subchild_stdout_path)
{
    FILE* file = fopen("subchild_stdout_path.txt", "r");
    if (!file) return error_occured(EXIT_CHILD_ERROR, "Error trying to get subchild(terminal)_stdout_path");

    fgets(subchild_stdout_path, sizeof(subchild_stdout_path), file);
    fclose(file);

    return EXIT_SUCCESS;
}

int timer_display(char subchild_stdout_path[], int timer[3])
{
    FILE* subchild_stdout = fopen(subchild_stdout_path, "w");
    if (!subchild_stdout) return error_occured(EXIT_CHILD_ERROR, "Error getting subchild(terminal) stdout");
    
    fprintf(subchild_stdout, "\033[H\033[J");
    for (size_t i = 0; i < 5; i++)
    {
        fprintf(subchild_stdout, "GSGGGHGA\n");
    }
    
    fclose(subchild_stdout);

    return EXIT_SUCCESS;
}

void stop_timer()
{

}

int error_occured(int errorcode, char* errormess)
{
    fprintf(stderr, errormess);
    fprintf(stderr, ": code %d\n", errno);
    return errorcode;
}

int check_format(char timer_str[])
{
    int count = 0;
    for (size_t i = 0; i < TIMER_STR_MAXSIZE; i++)
    {
        if (timer_str[i] == ':') count++;
    }
    if (count < 2) 
    {
        printf("Not valid format\n");
        return EXIT_FORMAT_ERROR;
    }

    return EXIT_SUCCESS;
}

void invalid_input()
{
    printf("Invalid input\n");
}