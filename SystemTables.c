#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

typedef process_info {

    pid_t PID;
    int FD;
    char file_name[1024];
    int Inode;

} process_info;

void defineProcess(process_info *p, char path[]){

    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        char buf[1024];

        while (fgets(buf, sizeof(buf), fp) != NULL) {
            if (strncmp(buf, "pos:", 4) == 0) {
                long int pos = atol(buf + 4);
                *p.PID = pid;
                *p.FD = fd;
                strncpy(*p.file_name, file_name, sizeof(*p.file_name));
                *p.Inode = pos;
                count++;
                break;
            }
        }
        fclose(fp);
    }
}

void getFileDescriptors(process_info processes[MAX_PROCESSES], char path[], int* count, int pid){

    DIR *dp2;
    struct dirent *ep2;
    dp2 = opendir(path);
    
    while ((ep2 = readdir(dp2)) != NULL) {

        // Gets file descriptors for process
        int fd = atoi(ep2 -> d_name);

        // Creates path to access file descriptors
        snprintf(path, sizeof(path), "/proc/%d/fd/%d", pid, fd);

        char link[1024];
        ssize_t link_len = readlink(path, link, sizeof(link) -1);

        link[link_len] = '\0';

        char *file_name = strrchr(link, '/');

        file_name++;

        // Creates path for file descriptor info
        snprintf(path, sizeof(path), "/proc/%d/fdinfo/%d", pid, fd);

        defineProcess(&processes[count], path);
    }

    closedir(dp2);
}

void getProcesses(process_info processes[MAX_PROCESSES], bool specific_process_chosen, int process_chosen){

    //Populates the process array with all the current running processes
    int count = 0;

    DIR *proc;
    struct dirent *ep;
    char path[1024];
    proc = opendir("/proc");

    while ((ep = readdir(proc)) != NULL && count < MAX_PROCESSES) {
        // Gets pid of process
        pid_t pid = atoi(ep -> d_name);
        // Create path to access file descriptors of process then calls function getFileDescrioptors to add the information to processes
        snprintf(path, sizeof(path), "/proc/%d/fd", pid);
        getFileDescriptors(processes, path, &count, pid);
    }

    closedir(proc);

    return count;
    }

void display(){

}

int main(int argc, char* argv[]){

    process_info processes[MAX_PROCESSES];

    int threshold; int process_chosen;
    bool per_process = false, systemWide = false, Vnodes = false, composite = false, specific_process_chosen = false;

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

    getProcesses(process, specific_process_chosen, process);
    display();
}