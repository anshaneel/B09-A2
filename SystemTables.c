#include <stdio.h>
#include <stdbool.h>

struct process_info {
    pid_t PID;
    int FD;
    char file_name[1024];
    int Inode;

}

void display(){


}

int main(int argc, char* argv[]){

    int threshold;
    bool per_process = false, systemWide = false, Vnodes = false, composite = false;

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
    }
}