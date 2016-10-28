
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct PCB 			
{
   int pid;
   int arrivalTime;
   int executionTime;
   int priority;
   int turnaroundTime;
   int waitingTime;
   bool wait;
};


struct PCB *input, tmp; 	
int numProcesses;
void printStruct();
void sortFCFS();
void setWaitToFalse();
void sortPriority();
int scheduler();
void promptUser();
void countProcesses();
void parseInputFile();
void metrics(int totalTurnaround);


int main(int argc, char* argv[])						
{									
	
	setWaitToFalse();
	
	countProcesses();
	parseInputFile();
	
	promptUser();
	
	return 0;
}


void countProcesses()
{
	numProcesses = 0;
	FILE* file = fopen("pinput.txt", "r"); 	
	char line[256];	
     while (fgets(line, sizeof(line), file)) {
		 numProcesses++;
	 }
    input = (PCB *)malloc(numProcesses);
    
    fclose(file);
}

void parseInputFile()
{
	char *token;
	char *data;
	int counter = 0;
	int i = 0; 
	const char delimeter[2] = " ";			
	
	FILE* file = fopen("pinput.txt", "r"); 		
    char line[256];

    while (fgets(line, sizeof(line), file)) {
		data = line;
		token = strtok(data, delimeter);

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
		counter = 0;
		i++;
    } 
    fclose(file);
}

void promptUser()
{
	int in, repeat;
	int totalTurnaround;
	while(true) { 
		printStruct();   
		printf("\nPress 1 to execute with FCFS, press 2 to execute by priority\n");
		scanf("%i", &in);
		if(in == 1) {
			sortFCFS();
		
		}
		else if(in == 2) {
			sortPriority();
		}
		else {
			printf("\nInvalid command");
		}
		totalTurnaround = scheduler();
		metrics(totalTurnaround);
		
		printf("\nPress 1 to sort by another algorithm, press 2 to quit\n");
		scanf("%i", &repeat);
		
		if(repeat == 2) {
			exit(0);
		}
	}
	
}
void printStruct()
{
	printf("\nProcess\t   Arrival Time\t   Execution Time");
	for(int i=0; i < 5; i++)
    {
		printf("\n  %i\t\t%i\t\t%i", input[i].pid, input[i].arrivalTime, input[i].executionTime);		
	}
}

void sortFCFS()
{
	 //Create the ready queue and update every time a new process is scheduled
	 for(int k=0;k<numProcesses-1;k++)
	 {
		for(int m=0;m<numProcesses-1-k;m++)    
		{
			if(input[m].arrivalTime>input[m+1].arrivalTime)
			{
				tmp=input[m];
				input[m]=input[m+1];
				input[m+1]=tmp;
			}
		}
	 }
}

void sortPriority() 
{
	 //Create the ready queue and update every time a new process is scheduled
	 for(int k=0;k<numProcesses-1;k++)
	 {
		for(int m=0;m<numProcesses-1-k;m++)    
		{
			if(input[m].priority>input[m+1].priority)
			{
				tmp=input[m];
				input[m]=input[m+1];
				input[m+1]=tmp;
			}
		}
	 }
}

void setWaitToFalse() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
	}
}


int scheduler()
{
	
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Time of Transition\tPid\tOld State\tNew State";
	fprintf(file, "%s\n", text);	
	
	printf("\n\nOrder processes will execute in:");
	printStruct(); 
	
	int totalTurnaround = 0;
	int processes = numProcesses; 
	
	for(int i = 0; i < numProcesses; i++) {
		if(i != 0){ 
			fprintf(file, "\t%i\t\t", totalTurnaround);			
			fprintf(file, "%i\t", input[i].pid);				
			const char *text2 = "waiting\t\tready";
			fprintf(file, "%s\n", text2);
		}
		
		fprintf(file, "\t%i\t\t", totalTurnaround);
		fprintf(file, "%i\t", input[i].pid);
		const char *text3 = "ready\t\trunning";
		fprintf(file, "%s\n", text3);
				
		totalTurnaround += input[i].executionTime;
		input[i].turnaroundTime +=totalTurnaround;

		int j;
		for(j= i+1; j < processes; j++) {
			if(input[j].wait == false) {
				fprintf(file, "\t%i\t\t", input[j].arrivalTime);
				fprintf(file, "%i\t", input[j].pid);
				const char *text4 = "ready\t\twaiting";
				fprintf(file, "%s\n", text4);
			
				input[j].waitingTime = totalTurnaround - input[j].arrivalTime;
			}
			input[j].wait = true;
		}
	}
	
	fclose(file);
	return totalTurnaround;
	
}

void metrics(int totalTurnaround)
{	
	float averageTurnaround;
	float totalTime = 0;
	float throughput = 0;
	float totalWaitingTime = 0;
	float avgWaitingTime = 0;
	printf("\nAverage Turnaround = %i", totalTurnaround);
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
		totalWaitingTime += input[i].executionTime;
	}
	avgWaitingTime = totalWaitingTime/numProcesses;
	printf("\nAverage Waiting Time = %f", avgWaitingTime);
	
}

	
