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
    // Struct stores all the info for each process

    pid_t PID;
    int FD;
    char file_name[1024];
    long Inode;

} process_info;

void getFileDescriptors(process_info processes[1024], char path[1024], int* count, int pid){
    /**

    * Retrieves information about the file descriptors associated with a process and stores it in an array of process_info structs.
    *@param processes: an array of process_info structs to store the file descriptor information
    *@param path: a string containing the path to the /proc directory
    *@param count: a pointer to an integer that tracks the number of file descriptors found
    *@param pid: the PID of the process to retrieve file descriptor information for
    *The function opens the directory specified by the path parameter and iterates through all symbolic links with a file type of DT_LNK.
    *For each symbolic link found, the function extracts the file descriptor number and retrieves information about the target file using the stat function.
    *The function then stores the process ID, file descriptor number, file name, and inode number in a process_info struct and adds it to the processes array.
    *The count parameter is updated with the number of file descriptors found.

    */

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

                if (file_stat.st_ino <= 0){
                    continue;
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

int getProcesses(process_info processes[1024], bool specific_process_chosen, int process_chosen, int threshold){
    /**
    * Retrieves information about the file descriptors associated with a process and stores it in an array of process_info structs.
    *@param processes: an array of process_info structs to store the file descriptor information
    *@param specific_process_chosen: a boolean value indicating if a specific process is chosen or not
    *@param process_chosen: an integer representing the PID of the process to retrieve file descriptor information for if specific_process_chosen is true
    *@param threshold: an integer representing the minimum PID of a process to include in the search if specific_process_chosen is false
    *The function populates the process array with information about the running processes.
    *If specific_process_chosen is true, the function retrieves file descriptor information for the process with PID process_chosen.
    *If specific_process_chosen is false, the function iterates through all running processes and retrieves file descriptor information for each process with a PID greater than threshold.
    *The count variable is updated with the number of file descriptors found.
    *@return the number of file descriptors found
    */

    //Populates the process array with all the current running processes
    int count = 0;

    DIR *proc;
    struct dirent *ep;
    char path[1024];
    proc = opendir("/proc");

    if (specific_process_chosen){
        snprintf(path, sizeof(path), "/proc/%d/fd", process_chosen);
        getFileDescriptors(processes, path, &count, process_chosen);
        return count;
    }

    while ((ep = readdir(proc)) != NULL && count < 1024) {
        if (ep -> d_type == DT_DIR) {
            // Gets pid of process
            pid_t pid = atoi(ep -> d_name);
            
            if (pid <= threshold){
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
    /**
    *Outputs information about the file descriptors associated with a process to a text file named "compositeTable.txt".
    *@param processes: an array of process_info structs containing the file descriptor information to output
    *@param count: an integer representing the number of file descriptors to output
    *The function opens a new text file named "compositeTable.txt" in write mode and prints a header to the file.
    *The function then iterates through the processes array and prints the PID, file descriptor number, file name, and inode number of each process to the file.
    *The function then prints a footer to the file and closes the file.
    */


    printf("Printing to compositeTable.txt\n\n");

    FILE *fp = fopen("compositeTable.txt", "w");
    fprintf(fp, "%s\n", "         PID     FD      Filename                Inode");
    fprintf(fp, "%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        fprintf(fp, "%d\t%d  %d\t%s\t%ld\n", i, processes[i].PID, processes[i].FD, processes[i].file_name, processes[i].Inode);
    }
    fprintf(fp, "%s\n", "        ===============================================");
    fclose(fp);

}

void outputBinary(process_info processes[1024], int count){
    /**
    * Outputs information about the file descriptors associated with a process to a binary file named "compositeTable.bin".
    *@param processes: an array of process_info structs containing the file descriptor information to output
    *@param count: an integer representing the number of file descriptors to output
    *The function opens a new binary file named "compositeTable.bin" in write mode and prints a header to the file.
    *The function then iterates through the processes array and writes the PID, file descriptor number, file name, and inode number of each process to the file in binary format.
    *The function then prints a footer to the file and closes the file.
    */


    printf("Printing to compositeTable.bin\n\n");
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
        fwrite(&processes[i].Inode, sizeof(long), 1, fp);
    }

    // write footer
    char footer[] = "        ===============================================\n";
    fwrite(footer, sizeof(footer), 1, fp);

    fclose(fp);

}

void displayComposite(process_info processes[1024], int count){
    /**
    * Displays information about the file descriptors associated with a process on the console.
    *@param processes: an array of process_info structs containing the file descriptor information to display
    *@param count: an integer representing the number of file descriptors to display
    *The function prints a header to the console and iterates through the processes array, printing the PID, file descriptor number, file name, and inode number of each process to the console.
    *The function then prints a footer to the console.
    */

    printf("%s\n", "         PID     FD      Filename                Inode");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d\t%d  %d\t%s\t%ld\n", i, processes[i].PID, processes[i].FD, processes[i].file_name, processes[i].Inode);
    }
    printf("%s\n\n", "        ===============================================");
}

void displayVnodes(process_info processes[1024], int count){
    /**
    * Displays the file descriptor and inode number for each entry in an array of process_info structs.
    * @param processes: an array of process_info structs to display
    * @param count: the number of elements in the processes array to display
    * The function prints a table of the file descriptor and inode number for each process_info struct in the array, using printf to format the output.
    */

    printf("%s\n", "           FD            Inode");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d\t%d\t%ld\n", i, processes[i].FD, processes[i].Inode);
    }
    printf("%s\n\n", "        ===============================================");
}

void displaySystemWide(process_info processes[1024], int count){
    /**
    * Displays the system-wide file descriptors information for all processes or a specified threshold.
    * @param processes: an array of process_info structs to display file descriptor information
    * @param count: an integer that tracks the number of file descriptors to display
    * This function iterates through the processes array and displays the process ID, file descriptor number, and file name for each process.
    * If a threshold is specified, processes with a PID equal to or less than the threshold are skipped.
    */

    printf("%s\n", "         PID     FD      Filename");
    printf("%s\n", "        ===============================================");

    for (int i = 0; i < count; i++){
        printf("%d\t%d  %d\t%s\n", i, processes[i].PID, processes[i].FD, processes[i].file_name);
    }
    printf("%s\n\n", "        ===============================================");
}

void displayPerProcess(process_info processes[1024], int count){
    /**
    * Displays the process ID and file descriptor number for each file descriptor found in the processes array.
    *@param processes: an array of process_info structs containing the file descriptor information
    *@param count: the number of file descriptors stored in the processes array
    *The function iterates through the processes array and prints the process ID and file descriptor number for each file descriptor.
    *The output is formatted to align the columns of data using tabs.
    */

    printf("%s\n", "         PID     FD ");
    printf("%s\n", "        ============");

    for (int i = 0; i < count; i++){
        printf("%d\t%d  %d\n", i, processes[i].PID, processes[i].FD);
    }
    printf("%s\n\n", "        ============");
}

void display(process_info processes[1024], int count, bool per_process, bool systemWide, bool Vnodes, bool composite, bool specific_process_chosen, bool txt, bool bin){
    /**
    * Displays information about running processes based on user-selected options.
    *@param processes: an array of process_info structs containing file descriptor information for each process
    *@param count: the number of processes in the processes array
    *@param per_process: a boolean indicating whether to display file descriptor information for each process individually
    *@param systemWide: a boolean indicating whether to display system-wide file descriptor information
    *@param Vnodes: a boolean indicating whether to display Vnode information
    *@param composite: a boolean indicating whether to display the composite table
    *@param specific_process_chosen: a boolean indicating whether a specific process has been selected
    *@param txt: a boolean indicating whether to output file descriptor information to a text file
    *@param bin: a boolean indicating whether to output file descriptor information to a binary file
    *The function calls various display and output functions based on user-selected options.
    */
    
    //If the composite option is selected, the function calls the displayComposite function.
    if (composite){
        displayComposite(processes, count);
    }
    //If the system-wide option is selected, the function calls the displaySystemWide function.
    if (systemWide){
        displaySystemWide(processes, count);
    }
    //If the Vnode option is selected, the function calls the displayVnodes function.
    if (Vnodes){
        displayVnodes(processes, count);
    }
    //If the per-process option is selected, the function calls the displayPerProcess function.
    if (per_process){
        displayPerProcess(processes, count);
    }
    //If the txt option is selected, the function calls the outputTXT function to output file descriptor information to a text file.
    if (txt){
        outputTXT(processes, count);
    }
    //If the bin option is selected, the function calls the outputBinary function to output file descriptor information to a binary file.
    if (bin){
        outputBinary(processes, count);
    }

}

int main(int argc, char* argv[]){
    /**
    * Main function that takes command line arguments and calls other functions to retrieve and display process information.
    * @param argc: An integer that contains the number of arguments passed in the command line.
    * @param argv: A character pointer array that contains the actual arguments passed in the command line.
    * @return: An integer value that indicates the status of the program upon exit.
    *
    */ 

    // Initializes an array of process_info structs, and several boolean and integer variables to store command-line arguments.
    process_info processes[1024];

    int threshold; int process_chosen;
    bool selected, per_process, systemWide, Vnodes, composite, specific_process_chosen, txt, bin;

    // The for loop iterates over the command-line arguments, and based on the arguments, sets corresponding boolean flags and integer variables.
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--per-process") == 0){
            per_process = true;
            selected = true;
        }
        else if (strcmp(argv[i], "--systemWide") == 0){
            systemWide = true;
            selected = true;
        }
        else if (strcmp(argv[i], "--Vnodes") == 0){
            Vnodes = true;
            selected = true;
        }
        else if (strcmp(argv[i], "--composite") == 0){
            composite = true;
            selected = true;
        }
        else if (strcmp(argv[i], "--output_TXT") == 0){
            txt = true;
            selected = true;
        }
        else if (strcmp(argv[i], "--output_binary") == 0){
            bin = true;
            selected = true;
        }
        else if (strncmp(argv[i], "--threshold=", 12) == 0){
            threshold = atoi(argv[i] + 12);
        }
        else if (isdigit(*argv[i])){
            process_chosen = atoi(argv[i]);
            specific_process_chosen = true;
        }
    }

    // If no option is selected, the function defaults to composite view.
    if (!selected){
        composite = true;
    }

    // The function calls the getProcesses function, which populates the array of process_info structs with process information, and returns the number of processes found.
    int count = getProcesses(processes, specific_process_chosen, process_chosen, threshold);
    // The function then calls the display function, which prints the process information based on the selected options.
    display(processes , count, per_process, systemWide, Vnodes, composite, specific_process_chosen, txt, bin);

}