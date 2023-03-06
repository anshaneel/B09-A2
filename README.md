# B09-A2
Assignment 2: Recreating the System-Wide FD Tables

  A C program that will report different a tool to display the tables used by the OS to keep track of open files, assignation of File Descriptors (FD) and processess.
  
## Libraries Used

- #include <stdio.h>
- #include <stdlib.h>
- #include <string.h>
- #include <dirent.h>
- #include <unistd.h>
- #include <ctype.h>
- #include <stdbool.h>
- #include <sys/types.h>
- #include <sys/stat.h>
- #include <fcntl.h>

## Suported Command Line Arguments

### --per-process

        indicates that only the process FD table will be displayed


### --systemWide

        indicates that only the system-wide FD table will be displayed


### --Vnodes

       indicates that the Vnodes FD table will be displayed


### --composite

        indicates that only the composed table will be displayed

 

### --threshold=X

        where X denotes an integer, indicating that processes which have a number of FD assigned larger than X should be flagged in the output.


### Default behaviour

        if no argument is passed to the program, then the program will display the composite table, i.e. same effect as having used the --composite flag

## How to run the program
1) Compile it: (gcc SystemTables.c -o SystemTables) or using the makefile (make -f makefile.mak)
2) Run the executable file with any of the command line arguments: ex) ./SystemTables --composite --threshold=1500


## Functions
### void getFileDescriptors(process_info processes[1024], char path[1024], int* count, int pid):
    
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
    
    
    
    
### int getProcesses(process_info processes[1024], bool specific_process_chosen, int process_chosen, int threshold):

   
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
    
    
    
    
### void outputTXT(process_info processes[1024], int count):

   
    void outputTXT(process_info processes[1024], int count){
    /**
    *Outputs information about the file descriptors associated with a process to a text file named "compositeTable.txt".
    *@param processes: an array of process_info structs containing the file descriptor information to output
    *@param count: an integer representing the number of file descriptors to output
    *The function opens a new text file named "compositeTable.txt" in write mode and prints a header to the file.
    *The function then iterates through the processes array and prints the PID, file descriptor number, file name, and inode number of each process to the file.
    *The function then prints a footer to the file and closes the file.
    */
    
    
    
    
### void outputBinary(process_info processes[1024], int count): 
    
    void outputBinary(process_info processes[1024], int count){
    /**
    * Outputs information about the file descriptors associated with a process to a binary file named "compositeTable.bin".
    *@param processes: an array of process_info structs containing the file descriptor information to output
    *@param count: an integer representing the number of file descriptors to output
    *The function opens a new binary file named "compositeTable.bin" in write mode and prints a header to the file.
    *The function then iterates through the processes array and writes the PID, file descriptor number, file name, and inode number of each process to the file in binary format.
    *The function then prints a footer to the file and closes the file.
    */
    
    
    
    
### void displayComposite(process_info processes[1024], int count): 
   
    void displayComposite(process_info processes[1024], int count){
    /**
    * Displays information about the file descriptors associated with a process on the console.
    *@param processes: an array of process_info structs containing the file descriptor information to display
    *@param count: an integer representing the number of file descriptors to display
    *The function prints a header to the console and iterates through the processes array, printing the PID, file descriptor number, file name, and inode number of each process to the console.
    *The function then prints a footer to the console.
    */
    
    
    
    
### void displayVnodes(process_info processes[1024], int count): 
   
    void displayVnodes(process_info processes[1024], int count){
    /**
    * Displays the file descriptor and inode number for each entry in an array of process_info structs.
    * @param processes: an array of process_info structs to display
    * @param count: the number of elements in the processes array to display
    * The function prints a table of the file descriptor and inode number for each process_info struct in the array, using printf to format the output.
    */
    
    
    
    
### void displaySystemWide(process_info processes[1024], int count): 
   
    void displaySystemWide(process_info processes[1024], int count){
    /**
    * Displays the system-wide file descriptors information for all processes or a specified threshold.
    * @param processes: an array of process_info structs to display file descriptor information
    * @param count: an integer that tracks the number of file descriptors to display
    * This function iterates through the processes array and displays the process ID, file descriptor number, and file name for each process.
    * If a threshold is specified, processes with a PID equal to or less than the threshold are skipped.
    */
    
    
    
    
### void display(process_info processes[1024], int count, bool per_process, bool systemWide, bool Vnodes, bool composite, bool specific_process_chosen, bool txt, bool bin): 
   
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
    
    
### int main(int argc, char *argv[]): 
   
    int main(int argc, char* argv[]){
    /**
    * Main function that takes command line arguments and calls other functions to retrieve and display process information.
    * @param argc: An integer that contains the number of arguments passed in the command line.
    * @param argv: A character pointer array that contains the actual arguments passed in the command line.
    * @return: An integer value that indicates the status of the program upon exit.
    *
    */
    
    
### int main(int argc, char *argv[]): 
   
    int main(int argc, char *argv[]){
    /**
    * Entry point of the program, parses command line arguments to pass values to display function
    *
    * @argc: Number of command-line arguments
    * @argv: Array of pointers to command-line arguments as strings
    *
    * Return: 0 on success, non-zero on error
    */
    
    
    
    

## How I solved the problem: 

I approached this problem by splitting it up into chunks and building functionality from the ground up. I started by parsing the command line arguments so I could test the other functions in isolation when implemented.

Then I initialized the array of processses according to the specifications given by the user before passing the users selections to the display function.

The display function is where the program was put together, it would call all the functions based on the arguments passed and print according to the desired specifications.

The max number of processes are set to 1024



