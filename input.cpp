
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

struct PCB *input, tmp, *waiting;
 	
int numProcesses;
void printStruct();
void sortFCFS();
void setWaitToFalse();
void sortPriority();
int schedulerFCFS();
void promptUser();
void countProcesses();
void parseInputFile();
void metrics(int totalTurnaround);
void resetAllPCB();
void comparePriority();
void schedulerPriority();
int growArray(PCB **user_array, int currentSize, int numNewElems);

int main(int argc, char* argv[])						
{									
	resetAllPCB();
	
	countProcesses();
	parseInputFile();
	
	//promptUser();
	//sortPriority();
	//int wow = scheduler();
	//metrics(wow);
	
	sortFCFS();
	schedulerPriority();
	//printStruct();
	
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
    input = (PCB* )malloc(sizeof(PCB)*numProcesses);
    
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
		totalTurnaround = schedulerFCFS();
		metrics(totalTurnaround);
		setWaitToFalse();
		
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

void comparePriority() {
	int length = sizeof(waiting[0])/sizeof(waiting);	
	for(int k=0;k<length-1;k++)
	{
		for(int m=0;m<length-k;m++)    
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

void setWaitToFalse() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
	}
}

void resetAllPCB() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
		input[i].turnaroundTime = 0;
		input[i].pid = 0;
		input[i].arrivalTime = 0;
		input[i].executionTime = 0;
		input[i].priority = 0;
		input[i].waitingTime = 0; 
	}
}


int schedulerFCFS()
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
	
	int totalTurnaround = input[0].arrivalTime;
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

void schedulerPriority() 
{
	int count = 0;
	 
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	int totalTurnaround = input[0].arrivalTime; 
	
	for(int i = 0; i < numProcesses; i++) {
		totalTurnaround += input[i].executionTime;
		input[i].turnaroundTime +=totalTurnaround;
		
		for(int j = i+1; j < numProcesses; j++) {
			if(input[j].arrivalTime < totalTurnaround) 
			{	
				count++;		
			}
		}
		waiting = (PCB* )malloc(sizeof(PCB)*count);
		for(int k = i+1; k < numProcesses; k++) {
			if(input[k].arrivalTime < totalTurnaround)
			{ 
				waiting[k] = input[k];
			}
		}
		comparePriority();
		 
	}
	for(int i=0; i < 4; i++)
	{
		printf("\n%i", waiting[i].pid);		
	}
	
	fclose(file);
	
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


	
