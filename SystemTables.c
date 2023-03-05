#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct process_info {

    pid_t PID;
    int FD;
    char file_name[1024];
    int Inode;

} process_info;

void getFileDescriptors(process_info processes[1024], char path[1024], int* count, int pid){

    DIR *dp2;
    struct dirent *ep2;
    dp2 = opendir(path);

    if (dp2 != NULL){
    
        while ((ep2 = readdir(dp2)) != NULL) {
            if (ep2 -> d_type == DT_LNK) {

                // Gets file descriptors for process
                int fd = atoi(ep2 -> d_name);

                // Reads the symbolic link of target file
                char link[1024];

                snprintf(path, 1024, "/proc/%d/fd/%d", pid, fd);
                ssize_t link_len = readlink(path, link, sizeof(link) -1);

                link[link_len] = '\0';

                struct stat file_stat;

                if (fstat(fd, &file_stat) == -1) {
                    perror("fstat");
                    exit(EXIT_FAILURE);
                }

                processes[*count].PID = pid;
                processes[*count].FD = fd;
                strncpy(processes[*count].file_name, link, 1024);
                processes[*count].Inode = file_stat.st_ino;
                (*count)++;

            }
        }
    }

    closedir(dp2);
}

int getProcesses(process_info processes[1024], bool specific_process_chosen, int process_chosen){

    //Populates the process array with all the current running processes
    int count = 0;

    DIR *proc;
    struct dirent *ep;
    char path[1024];
    proc = opendir("/proc");

    if (specific_process_chosen){
        getFileDescriptors(processes, path, &count, process_chosen);
        return count;
    }

    while ((ep = readdir(proc)) != NULL && count < 1024) {
        if (ep -> d_type == DT_DIR) {
            // Gets pid of process
            pid_t pid = atoi(ep -> d_name);
            
            if (pid <= 0){
                continue;
            }
            // Create path to access file descriptors of process then calls function getFileDescrioptors to add the information to processes
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);
            getFileDescriptors(processes, path, &count, pid);
        }
    }

    closedir(proc);

    return count;
}

void display(process_info processes[1024], int count){

    for (int i = 0; i < count; i++){
        printf("%d     %d     %d      %s       %d\n", count, processes[i].PID, processes[i].FD, processes[i].file_name, processes[i].Inode);
    }
}

int main(int argc, char* argv[]){

    process_info processes[1024];

    int threshold; int process_chosen;
    bool selected = false, per_process = false, systemWide = false, Vnodes = false, composite = false, specific_process_chosen = false;

    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--per-process") == 0 && !selected){
            per_process = true;
        }
        else if (strcmp(argv[i], "--systemWide") == 0 && !selected){
            systemWide = true;
        }
        else if (strcmp(argv[i], "--Vnodes") == 0 && !selected){
            Vnodes = true;
        }
        else if (strcmp(argv[i], "--composite") == 0 && !selected){
            composite = true;
        }
        else if (strcmp(argv[i], "--output_TXT") == 0){

        }
        else if (strcmp(argv[i], "--output_binary") == 0){

        }
        else if (strncmp(argv[i], "--threshold=", 12) == 0){
            threshold = atoi(argv[i] + 12);
        }
        else if (isdigit(*argv[i])){
            process_chosen = atoi(argv[i]);
            specific_process_chosen = true;
        }
    }

    int count = getProcesses(processes, specific_process_chosen, process_chosen);
    display(processes, count);
}