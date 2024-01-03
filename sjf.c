#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 1000
#define MAX_NUM_PROCESSES 100

// define struct to hold information about a process
struct Process {
	char process[5];
	int art;
	int brt;
};

// function that swaps processes
void swap(struct Process *process_a, struct Process *process_b) {
	struct Process temp = *process_a;
	*process_a = *process_b;
	*process_b = temp;
}

// function that adds processes to the ready queue
void add_to_ready_queue(int time, struct Process processes[], struct Process ready_queue[], int *num_ready_queue, int *total_ready_queue) {

	// get the index of the start of the processes in the processes array that have not been added to the ready queue yet
	int i = *total_ready_queue;

	// add to the ready queue all of the processes with the arrival time the same as time
	while (processes[i].art == time) {
		int j = 0;

		// find the spot in the ready queue where the process should be added (ready queue is sorted in increasing order by the burst time)
		while (processes[i].brt >= ready_queue[j].brt && j < *num_ready_queue) {
			j++;
		}
		
		// if needed, shift processes in the ready queue so that it can fit the new process
		for (int k = *num_ready_queue; k > j; k--) {
			ready_queue[k] = ready_queue[k - 1];
		}

		// add the process to the ready queue
		ready_queue[j] = processes[i];

		i++;
		*num_ready_queue = *num_ready_queue + 1;
		*total_ready_queue = *total_ready_queue + 1;
	}
}

// function that prints the processes that have arrived at a given time 
void arrived(int time, struct Process processes[], int *total_arrived) {
	
	// get the index of the start of the processes in the processes array that have not printed that they have arrived
	int i = *total_arrived;

	// print all of the processes that have arrived at the same time as time
	while (processes[i].art == time) {
		printf("[%c%s Arrived] ", toupper(processes[i].process[0]), processes[i].process + 1);
		i++;
		*total_arrived = *total_arrived + 1;
	}
}

// function that removes processes from the ready queue
void remove_from_ready_queue(struct Process ready_queue[], int *num_ready_queue) {
	// remove the first process in the ready queue and shift the rest of the processes in the ready queue
	for (int i = 0; i < *num_ready_queue - 1; i++) {
		ready_queue[i] = ready_queue[i + 1];
	}
	*num_ready_queue = *num_ready_queue - 1;
}

// function that performs shortest job first CPU scheduling algorithm
void shortest_job_first(struct Process processes[], int num_processes) {
	int time = 0; // stores the CPU time
 	float turnaround_time = 0.0; // stores the turnaround time of all the processes
 	float waiting_time = 0.0; // stores the waiting time of all the processes
	int processes_completed = 0; // stores the number of completed processes
 	struct Process ready_queue[num_processes]; // stores the processes that are ready
 	int num_ready_queue = 0; // stores the number of processes that are currently in the ready queue 
 	int total_ready_queue = 0; // stores the total number of processes that have been in the ready queue
 	int total_arrived = 0; // stores the total number of processes that have printed they have arrived

	// sort array of processes by arrival time
	for (int i = 0; i < num_processes - 1; i++) {
		for (int j = 0; j < num_processes - i - 1; j++) {
			if (processes[j].art > processes[j + 1].art) {
				swap(&processes[j], &processes[j + 1]);
			}
		}
 	}

 	printf("Process Scheduling Started:\n");

 	// add any processes that are ready at time = 0 to the ready queue
 	add_to_ready_queue(time, processes, ready_queue, &num_ready_queue, &total_ready_queue);

 	// output the process scheduling details for each process
 	while (processes_completed != num_processes) {
 		// if the ready queue is empty, does the following
 		if (num_ready_queue == 0) {
 			// print None and increase time
 			printf("CPU Time %d: None\n", time);
 			time++;

 			// add any processes that are ready at the time to the ready queue
 			add_to_ready_queue(time, processes, ready_queue, &num_ready_queue, &total_ready_queue);
 		}

 		// if there is a process in the ready queue, output the process scheduling details
 		else {
 			// get the information for the process with the shortest burst time (first process in the ready queue since ready queue is sorted in increasing order by burst time)
 			struct Process p;
 			strcpy(p.process, ready_queue[0].process);
 			p.art = ready_queue[0].art;
 			p.brt = ready_queue[0].brt;

 			// calculate the waiting time for the process
 			int waiting = time - p.art;

 			int turnaround = 0;

 			// output process scheduling details
 			for (int i = 0; i <= p.brt; i++) {
 				printf("CPU Time %d: ", time);

 				// print any processes that have arrived at the time
 				arrived(time, processes, &total_arrived);

 				// print the process scheduling details for the process
 				printf("%c%s [%d/%d]\n", toupper(p.process[0]), p.process + 1, i, p.brt);

 				// if it is the first time printing scheduling details for this process, remove the process from the ready queue
 				if (i == 0) {
 					remove_from_ready_queue(ready_queue, &num_ready_queue);
 				}

 				// if it is not the last time printing scheduling details for this process, increase the time and add any processes that are ready at the time to the ready queue
 				if (i != p.brt) {
 					time++;
 					add_to_ready_queue(time, processes, ready_queue, &num_ready_queue, &total_ready_queue);
 				}

 				// if it is the last time printing scheduling details for this process and the ready queue is empty, calculate the turnaround time, increase the time, and add any processes that are ready at the time to the ready queue
 				if (i == p.brt && num_ready_queue == 0) {
 					// calculate the turnaround time for the process
 					turnaround = time - p.art;

 					time++;
 					add_to_ready_queue(time, processes, ready_queue, &num_ready_queue, &total_ready_queue);
 				}

 				// if it is the last time printing scheduling details for this process and the ready queue is not empty, calculate the turnaround time
 				if (i == p.brt && num_ready_queue != 0) {
 					// calculate the turnaround time for the process
 					turnaround = time - p.art;
 				}
 			}

 			printf("Process %c%s completed with Turnaround Time: %d, Waiting Time: %d\n", toupper(p.process[0]), p.process + 1, turnaround, waiting);

 			// add the process waiting time to the waiting time of all the processes, and add the process turnaround time to the turnaround time of all the processes
 			waiting_time = waiting_time + waiting;
 			turnaround_time = turnaround_time + turnaround;

 			// increase the number of processes that have been completed
 			processes_completed++;
 		}
 	}
 	printf("Process scheduling completed with Avg Turnaround Time: %.2f, Avg Waiting Time: %.2f\n\n", turnaround_time / num_processes, waiting_time / num_processes);
}


int main() {

	// define file pointer
	FILE *fp;

	// open file
	fp = fopen("sjf_input.txt", "r");

	// opening file unsuccessful
	if (fp == NULL) {
		printf("Error opening file");
		exit(1);
	}

	char line[MAX_LINE_LENGTH];
	int test_case = 1;

	// do shortest job first CPU scheduling algorithm for each test case (each line in the file)
	while (fgets(line, sizeof(line), fp) != NULL) {
		printf("Test case #%d: %s", test_case, line);
		
		// create an array of struct Process to hold all the processes for the test case
		struct Process processes[MAX_NUM_PROCESSES];

		char *ptr = strtok(line, " ");

		// keep track of number of processes
		int num_processes = 0;

		// parse the file and populate it into an array of struct Process
		while (ptr != NULL) {
			strcpy(processes[num_processes].process, ptr);
			ptr = strtok(NULL, " ");
			processes[num_processes].art = atoi(ptr);
			ptr = strtok(NULL, " ");
			processes[num_processes].brt = atoi(ptr);
			ptr = strtok(NULL, " ");
			num_processes++;
		}
		printf("Number of Processes: %d\n", num_processes);

		// call shortest job first function to perform shortest job first CPU scheduling algorithm and to output the process scheduling details
		shortest_job_first(processes, num_processes);

		test_case++;
	}

	fclose(fp);
	return 0;
}
