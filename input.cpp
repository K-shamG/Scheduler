
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
   bool arrived1;
   bool arrived2; 
   bool ran;
};

struct PCB *input, tmp;
 	
int numProcesses, count;
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
int schedulerPriority();

int main(int argc, char* argv[])						
{									
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
	int totalTurnaround, tt2;
	while(true) { 
		printStruct();   
		printf("\nPress 1 to execute with FCFS, press 2 to execute by priority\n");
		sortFCFS();
		
		scanf("%i", &in);
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

void setWaitToFalse() {
	for(int i = 0; i < numProcesses; i++) {
		input[i].wait = false;
	}
}

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

			}
			input[j].waitingTime = totalTurnaround - input[j].arrivalTime;
			input[j].wait = true;
		}
	}
	
	fclose(file);
	
	return totalTurnaround;
	
}

int schedulerPriority() 
{	
	int processes = numProcesses; 
	struct PCB *waiting;
	bool finished = false;
	int waitingIndex = 0;
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Time of Transition\tPid\tOld State\tNew State";
	fprintf(file, "%s\n", text);

	int totalTurnaround = input[0].arrivalTime; 
	int index = 0;
	
	while(!finished) {
		
			if(index==0){
				fprintf(file, "\t%i\t\t", totalTurnaround);
				fprintf(file, "%i\t", input[index].pid);
				const char *text3 = "ready\t\trunning";
				fprintf(file, "%s\n", text3);
			
				totalTurnaround += input[index].executionTime;
				input[index].turnaroundTime +=totalTurnaround;
				input[index].ran = true;
				processes--;
			}
			
			for(int j = index+1; j < numProcesses; j++) {
				if(input[j].arrivalTime < totalTurnaround && input[j].ran == false) 
				{	
					if(input[j].arrived1 == false)
					{
						count++;
						processes--;
						input[j].arrived1 = true;
					}
				}
			}
			waiting = (PCB* )malloc(sizeof(PCB)*count);

			for(int k = 1; k < numProcesses; k++)
			{
				if(input[k].arrivalTime < totalTurnaround && input[k].ran == false)
				{ 
					if(input[k].arrived2 == false)
					{		
						waiting[k-1] = input[k];
						if(input[k].wait == false) 
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
				if(waiting[i].ran == false) {
					
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
						if(waiting[i].pid == input[j].pid) 
						{
							input[j].ran = true;
							input[j].turnaroundTime = waiting[i].turnaroundTime;
							input[j].waitingTime = totalTurnaround - waiting[i].arrivalTime;
						}
					}
					if(processes != 0)
					{
						break;
					}
				}
			}
			
			waitingIndex++;
				
			for(int i = 0; i < numProcesses; i++) {
				finished = input[i].ran;
			}
			
		index++;
	}
	
	fclose(file);
	
	
	printf("\n\nOrder processes will execute in:");
	printf("\nProcess\t   Arrival Time\t   Execution Time    Priority");
	printf("\n  %i\t\t%i\t\t%i, \t\t%i", input[0].pid, input[0].arrivalTime, input[0].executionTime, input[0].priority);
	
	for(int i = 0; i < count; i++) {
		printf("\n  %i\t\t%i\t\t%i\t\t%i", waiting[i].pid, waiting[i].arrivalTime, waiting[i].executionTime, waiting[i].priority);
	}
		
	
	return totalTurnaround; 
	
}

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


	
