# SJF-CPU-scheduling-algorithm
Implementation of Shortest Job First (SJF) CPU scheduling algorithm.

sjf_input.txt contains test cases. Every line of the input file represents one individual test case. Every input line consists of several processes and their corresponding information such as process name, arrival time, and burst time.

For every test case in sjf_input.txt, applies SJF Scheduling and outputs the process schedule details including average turnaround time and average waiting time.

## Run project
* Download sjf.c and sjf_input.txt and put in the same folder
* Navigate to the folder
    * cd /path/to/files
* Compile the C file
    * gcc -o sjf sjf.c
* Run the executable
    * ./sjf
