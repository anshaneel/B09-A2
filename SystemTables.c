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

                char fd_path[1024];

                snprintf(fd_path, 1024, "/proc/%d/fd/%d", pid, fd);
                ssize_t link_len = readlink(fd_path, link, sizeof(link) -1);

                link[link_len] = '\0';

                struct stat file_stat;

                if (stat(fd_path, &file_stat) == -1) {
                    perror("stat");
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

void outputTXT(process_info processes[1024], int count){

    File *fp = fopen("compositeTable.txt", "r");
    fprintf(fp, "%s\n", "         PID     FD      Filename                Inode");
    fprintf(fp, "%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        fprintf(fp, "%d        %d  %d        %s        %d\n", i, processes[i].PID, processes[i].FD, processes[i].file_name, processes[i].Inode);
    }
    fprintf("%s\n", "        ===============================================");
    fclose(fp);

}

void outputBinary(process_info processes[1024], int count){

    FILE *fp = fopen("compositeTable.bin", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // write header
    char header[] = "         PID     FD      Filename                Inode\n"
                    "        ===============================================\n";
    fwrite(header, sizeof(header), 1, fp);

    // write data
    for (int i = 0; i < count; i++) {
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&processes[i].PID, sizeof(int), 1, fp);
        fwrite(&processes[i].FD, sizeof(int), 1, fp);
        fwrite(processes[i].file_name, sizeof(char), sizeof(processes[i].file_name), fp);
        fwrite(&processes[i].Inode, sizeof(int), 1, fp);
    }

    // write footer
    char footer[] = "        ===============================================\n";
    fwrite(footer, sizeof(footer), 1, fp);

    fclose(fp);
    
}

void displayComposite(process_info processes[1024], int count){

    printf("%s\n", "         PID     FD      Filename                Inode");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d        %d  %d        %s        %d\n", i, processes[i].PID, processes[i].FD, processes[i].file_name, processes[i].Inode);
    }
    printf("%s\n", "        ===============================================");
}

void displayVnodes(process_info processes[1024], int count){

    printf("%s\n", "           FD            Inode");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d        %d              %d\n", i, processes[i].FD, processes[i].Inode);
    }
    printf("%s\n", "        ===============================================");
}

void displaySystemWide(process_info processes[1024], int count){

    printf("%s\n", "         PID     FD      Filename");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d        %d  %d        %s\n", i, processes[i].PID, processes[i].FD, processes[i].file_name);
    }
    printf("%s\n", "        ===============================================");
}

void displayPerProcess(process_info processes[1024], int count){

    printf("%s\n", "         PID     FD ");
    printf("%s\n", "        ============");

    for (int i = 0; i < count; i++){
        printf("%d        %d  %d\n", i, processes[i].PID, processes[i].FD);
    }
    printf("%s\n", "        ============");
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
    displaySystemWide(processes, count);
}