/**
	FCFS and Priority Scheduler without IO. 
	Takes input file pinput.txt
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct PCB 	// struct to represent process control block for each process		
{
   int pid;
   int arrivalTime;
   int executionTime;
   int priority;
   int turnaroundTime;
   int waitingTime;
   
   bool wait;
   bool arrived1;
   bool arrived2; 
   bool ran;
};

struct PCB *input, tmp;	// input is a collection of all processes PCB's, tmp is later used for swapping
int numProcesses, count;

void promptUser();
void countProcesses();
void parseInputFile();

// formatting methods
void printStruct();
void setWaitToFalse();
void resetAllPCB();

// sorting and schedulers
void sortFCFS();
void comparePriority();
int schedulerFCFS();
int schedulerPriority();
void metrics(int totalTurnaround);

int main(int argc, char* argv[])						
{									
	countProcesses();
	parseInputFile();
	promptUser();
	
	return 0;
}

/**
	Opens the input files and counts the number of lines. 
	Sets it to the global variable numProcesses 
*/
void countProcesses()
{
	numProcesses = 0;
	FILE* file = fopen("pinput.txt", "r"); 	
	char line[256];	
     while (fgets(line, sizeof(line), file)) {
		 numProcesses++;
	 }
    input = (PCB* )malloc(sizeof(PCB)*numProcesses);
    
    fclose(file);
}

/**
	Opens the input file "pinput.txt" for reading and enters the information
	for each processes' PCB
	
*/
void parseInputFile()
{
	char *token;
	char *data;
	int counter = 0;
	int i = 0; 
	const char delimeter[2] = " ";			
	
	FILE* file = fopen("pinput.txt", "r"); 		// open file for reading
    char line[256];

    while (fgets(line, sizeof(line), file)) {  	// iterate through lines in file
		data = line;
		token = strtok(data, delimeter);		// iterate through each "token" (delimeter is a space)

		while( token != NULL ) 
		{
			int x = atoi(token);
		    
		    if(counter == 0)
		    {
				input[i].pid = x;
			}
			else if(counter == 1) 
			{
				input[i].arrivalTime= x;
			}
			else if(counter == 2)
			{
				input[i].executionTime = x;
			}
			else if(counter == 3) 
			{
				input[i].priority = x;
			}
			
			token = strtok(NULL, delimeter);
			counter++;
		}
		counter = 0;	// reset the counter
		i++;
    } 
    fclose(file);		// close the file
}

/**
	Prompts the user on which scheduling algorithm they would like to use. 
	1 - FCFS
	2 - Priority
	
	1 - another algorithm
	2 - quit
*/
void promptUser()
{
	int in, repeat;
	int totalTurnaround, tt2;
	while(true) { 
		printStruct();   
		printf("\nPress 1 to execute with FCFS, press 2 to execute by priority\n");
		sortFCFS();
		
		scanf("%i", &in);	// read user inputted value
		if(in == 1) {
			totalTurnaround = schedulerFCFS();
			metrics(totalTurnaround);
			resetAllPCB(); 
		}
		else if(in == 2) {
			tt2 = schedulerPriority();
			metrics(tt2);
			resetAllPCB();
		}
		else {
			printf("\nInvalid command");
		}
		setWaitToFalse();

		printf("\nPress 1 to sort by another algorithm, press 2 to quit\n");
		scanf("%i", &repeat);
		
		if(repeat == 2) {
			exit(0);
		}
	}
	
}

/**
	Print out all the processes and their required PCB information
*/
void printStruct()
{
	printf("\nProcess\t   Arrival Time\t   Execution Time   Priority");
	for(int i=0; i < numProcesses; i++)
    {
		printf("\n  %i\t\t%i\t\t%i\t\t%i", input[i].pid, input[i].arrivalTime, input[i].executionTime, input[i].priority);		
	}
}

/**
	Sort processes by arrival time
*/
void sortFCFS()
{
	 //Create the ready queue and update every time a new process is scheduled
	 for(int k=0;k<numProcesses-1;k++)
	 {
		for(int m=0;m<numProcesses-1-k;m++)    
		{
			if(input[m].arrivalTime>input[m+1].arrivalTime)
			{
				tmp=input[m];					// tmp variable used to not override variable
				input[m]=input[m+1];
				input[m+1]=tmp;
			}
		}
	 }
}

/**
	Takes in an array of struct PCB
	Sorts by priority
*/
void comparePriority(struct PCB *waiting) {
	for(int k=0;k<count-1;k++)
	{
		for(int m=0;m<count-1-k;m++)    
		{
			if(waiting[m].priority > waiting[m+1].priority)
			{
				tmp=waiting[m];
				waiting[m]=waiting[m+1];
				waiting[m+1]=tmp;
			}
		}
	 }
}
/**
	Sets boolean wait in all PCB's to false
*/
void setWaitToFalse() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
	}
}

/**
	Resets all temporary process values
	(Temporary as in they change every time a different scheduler is executed)
	
*/
void resetAllPCB() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
		input[i].turnaroundTime = 0;
		input[i].waitingTime = 0;
		input[i].arrived1 = false; 
		input[i].arrived2 = false; 
		input[i].ran = false; 
	}
}

/**
	First Come First Serve Scheduler
	
	Returns the total turnaround, i.e. the total time taken for all processes to execute. 
*/
int schedulerFCFS()
{
	FILE* file = fopen("output.txt", "w");	// open file to write to, called output.txt
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Time of Transition\tPid\tOld State\tNew State";		// output table headers
	fprintf(file, "%s\n", text);	
	
	printf("\n\nOrder processes will execute in:");
	printStruct(); 
	
	int totalTurnaround = input[0].arrivalTime;		// total time passed so far is when the first process arrives
	int processes = numProcesses; 
	
	for(int i = 0; i < numProcesses; i++) {
		if(i != 0){									// process 1 will never be waiting 			
			fprintf(file, "\t%i\t\t", totalTurnaround);			
			fprintf(file, "%i\t", input[i].pid);				
			const char *text2 = "waiting\t\tready";
			fprintf(file, "%s\n", text2);
		}
		
		fprintf(file, "\t%i\t\t", totalTurnaround);	// process starts running
		fprintf(file, "%i\t", input[i].pid);
		const char *text3 = "ready\t\trunning";
		fprintf(file, "%s\n", text3);
				
		totalTurnaround += input[i].executionTime;	// total time passed is how long each processes burst time
		input[i].turnaroundTime +=totalTurnaround;

		int j;
		for(j= i+1; j < processes; j++) {			// iterate through the remaining processes 
			if(input[j].wait == false) {
				fprintf(file, "\t%i\t\t", input[j].arrivalTime);
				fprintf(file, "%i\t", input[j].pid);
				const char *text4 = "ready\t\twaiting";
				fprintf(file, "%s\n", text4);

			}
			input[j].waitingTime = totalTurnaround - input[j].arrivalTime;	// add to the remaining processes waiting time
			input[j].wait = true;
		}
	}
	
	fclose(file);
	
	return totalTurnaround;
	
}
/**
	Priority Scheduler
	
	Returns the total turnaround, i.e. the total time taken for all processes to execute. 
*/
int schedulerPriority() 
{	
	int processes = numProcesses; 
	struct PCB *waiting;
	bool finished = false;
	int waitingIndex = 0;
	FILE* file = fopen("output.txt", "w");	// open file to write to, called output.txt
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Time of Transition\tPid\tOld State\tNew State"; // output table headers
	fprintf(file, "%s\n", text);

	int totalTurnaround = input[0].arrivalTime; // total time passed so far is when the first process arrives
	int index = 0;
	
	while(!finished) {
		
			if(index==0){
				fprintf(file, "\t%i\t\t", totalTurnaround);
				fprintf(file, "%i\t", input[index].pid);
				const char *text3 = "ready\t\trunning";		// process is running
				fprintf(file, "%s\n", text3);
			
				totalTurnaround += input[index].executionTime;	
				input[index].turnaroundTime +=totalTurnaround;
				input[index].ran = true;
				processes--;	// process counts as arrived
			}
			
			for(int j = index+1; j < numProcesses; j++) {
				if(input[j].arrivalTime < totalTurnaround && input[j].ran == false) 
				{	
					if(input[j].arrived1 == false)
					{
						count++;		// number of processes to go in the waiting (i.e. the ready) queue
						processes--;	// process counts as arrived
						input[j].arrived1 = true;
					}
				}
			}
			waiting = (PCB* )malloc(sizeof(PCB)*count);		// allocate enough memory in waiting for processes that have arrived

			for(int k = 1; k < numProcesses; k++)	
			{
				if(input[k].arrivalTime < totalTurnaround && input[k].ran == false)
				{ 
					if(input[k].arrived2 == false)
					{		
						waiting[k-1] = input[k];
						if(input[k].wait == false) 	// if hasn't been set to wait, it hasn't started waiting yet
						{
							fprintf(file, "\t%i\t\t", waiting[k-1].arrivalTime);
							fprintf(file, "%i\t", waiting[k-1].pid);
							const char *text4 = "ready\t\twaiting";
							fprintf(file, "%s\n", text4);	
			
							input[k].wait = true; 	
						}
					}	
				}
			}
			comparePriority(waiting);
			
			for(int i = waitingIndex; i<count; i++)
			{
				if(waiting[i].ran == false) {	// process has not yet run, then run it
					
					fprintf(file, "\t%i\t\t", totalTurnaround);			
					fprintf(file, "%i\t", waiting[i].pid);				
					const char *text2 = "waiting\t\tready";
					fprintf(file, "%s\n", text2);
				
					fprintf(file, "\t%i\t\t", totalTurnaround);
					fprintf(file, "%i\t", waiting[i].pid);
					const char *text3 = "ready\t\trunning";
					fprintf(file, "%s\n", text3);
					
					totalTurnaround += waiting[i].executionTime;
					waiting[i].turnaroundTime +=totalTurnaround;
					
					waiting[i].ran = true; 
					
					for(int j = 0; j < numProcesses; j++) {
						if(waiting[i].pid == input[j].pid) 		// set values in input to values in waiting in order to calculate metrics properly
						{
							input[j].ran = true;
							input[j].turnaroundTime = waiting[i].turnaroundTime;
							input[j].waitingTime = totalTurnaround - waiting[i].arrivalTime;
						}
					}
					if(processes != 0) // if all processes have not arrived yet, loop through again. Otherwise, they have all arrived so keep running the rest
					{
						break;
					}
				}
			}
			
			waitingIndex++;
				
			for(int i = 0; i < numProcesses; i++) {
				finished = input[i].ran;	// If all have been set to true, this means they have all executed. We are now done. 
			}
			
		index++;
	}
	
	fclose(file);
	
	
	printf("\n\nOrder processes will execute in:");
	printf("\nProcess\t   Arrival Time\t   Execution Time    Priority");
	printf("\n  %i\t\t%i\t\t%i \t\t%i", input[0].pid, input[0].arrivalTime, input[0].executionTime, input[0].priority);
	
	for(int i = 0; i < count; i++) {
		printf("\n  %i\t\t%i\t\t%i\t\t%i", waiting[i].pid, waiting[i].arrivalTime, waiting[i].executionTime, waiting[i].priority);
	}
		
	
	return totalTurnaround; 
	
}

/**
	Metrics to calculate and display on the console
	
	Total Turnaround Time
	Average Turnaround Time
	Throughput
	Average Waiting Time
	
	Takes in totalTurnaroundTime from the schedulers
 
 
*/
void metrics(int totalTurnaround)
{	
	float averageTurnaround;
	float totalTime = 0;
	float throughput = 0;
	float totalWaitingTime = 0;
	float avgWaitingTime = 0;
	printf("\nTotal Turnaround = %i", totalTurnaround);
	for(int k = 0; k < numProcesses; k++) {										
																		
		averageTurnaround += input[k].turnaroundTime;
	}
	averageTurnaround /= numProcesses;
	printf("\nAverage Turnaround = %f", averageTurnaround);
	
	for(int i = 0; i < numProcesses; i++) 
	{
		totalTime += input[i].executionTime;
	}
	throughput = numProcesses/totalTime;
	printf("\nThroughput = %f", throughput);

	for(int i = 0; i < numProcesses; i++) 
	{
		totalWaitingTime += input[i].waitingTime;
	}
	avgWaitingTime = totalWaitingTime/numProcesses;
	printf("\nAverage Waiting Time = %f", avgWaitingTime);

	
}


	
