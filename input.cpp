
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
   int IOFrequency;
   int IODuration; 
   
   bool wait;
   bool arrived;
   bool checked; 
 //  bool ran;
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
	
	resetAllPCB();
	
	//promptUser();
	
	sortFCFS(); 
	schedulerPriority(); 
	
	return 0;
}


void countProcesses()
{
	numProcesses = 0;
	FILE* file = fopen("input.txt", "r"); 	
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
	
	FILE* file = fopen("input.txt", "r"); 		
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
			else if(counter == 4)
			{
				input[i].IOFrequency = x;
			}
			else if(counter == 5) 
			{		
				input[i].IODuration = x; 
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
	printf("\nProcess\t   Arrival Time\t   Execution Time  Priority  IO Frequency IO Duration");
	for(int i=0; i < 5; i++)
    {
		printf("\n  %i\t\t%i\t\t%i\t     %i\t\t%i\t\t%i", input[i].pid, input[i].arrivalTime, input[i].executionTime, input[i].priority, input[i].IOFrequency, input[i].IODuration);		
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
		input[i].arrived = false; 
		input[i].checked = false; 
	//	input[i].ran = false; 
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
		
		int k = 0;  
		while(k < input[i].executionTime - 1) {
			if(k % input[i].IOFrequency == 0 || k == 0) {	
				totalTurnaround += input[i].IOFrequency;
				int j;
				for(j= i+1; j < processes; j++) {
					if(input[j].wait == false && input[j].arrivalTime < totalTurnaround) {
						fprintf(file, "\t%i\t\t", input[j].arrivalTime);
						fprintf(file, "%i\t", input[j].pid);
						const char *text4 = "ready\t\twaiting";
						fprintf(file, "%s\n", text4);
						
						input[j].wait = true;
					}
				}
				
				fprintf(file, "\t%i\t\t", totalTurnaround);
				fprintf(file, "%i\t", input[i].pid);
				const char *text4 = "running\t\twaiting";
				fprintf(file, "%s\n", text4);
				
				
				totalTurnaround += input[i].IODuration;
				for(j= i+1; j < processes; j++) {
					if(input[j].wait == false  && input[j].arrivalTime < totalTurnaround) {
						fprintf(file, "\t%i\t\t", input[j].arrivalTime);
						fprintf(file, "%i\t", input[j].pid);
						const char *text4 = "ready\t\twaiting";
						fprintf(file, "%s\n", text4);
					
						input[j].wait = true;
					}
					
				}
				
				fprintf(file, "\t%i\t\t", totalTurnaround);
				fprintf(file, "%i\t", input[i].pid);				
				const char *text2 = "waiting\t\tready";
				fprintf(file, "%s\n", text2);
				
				fprintf(file, "\t%i\t\t", totalTurnaround);
				fprintf(file, "%i\t", input[i].pid);
				const char *text3 = "ready\t\trunning";
				fprintf(file, "%s\n", text3);
				
				
				
				k+= input[i].IODuration;
			}
			else {
				k++;
			}
		}
				
		totalTurnaround += input[i].IOFrequency;
		input[i].turnaroundTime +=totalTurnaround;
		
		for(int j= i+1; j < processes; j++) {
			input[j].waitingTime = totalTurnaround - input[j].arrivalTime;
		}
		
		
		
	}
	
	fclose(file);
	
	return totalTurnaround;
	
}


int schedulerPriority() 
{	
	int index = 0; 
	struct PCB ready[numProcesses];
	
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	const char *text = "Time of Transition\tPid\tOld State\tNew State";
	fprintf(file, "%s\n", text);
	
	int totalTurnaround = input[0].arrivalTime;
	
	for(int i = 0; i < numProcesses; i++) 
	{
		if(input[i].arrivalTime <= totalTurnaround && input[i].checked == false)
		{
			ready[i] = input[i];
		}
		comparePriority(ready);
		
		fprintf(file, "\t%i\t\t", totalTurnaround);
		fprintf(file, "%i\t", ready[index].pid);
		const char *text3 = "ready\t\trunning";
		fprintf(file, "%s\n", text3);
		
		totalTurnaround += ready[index].IOFrequency;
		index++;
		
	}
	
	
	
	
	
	return totalTurnaround; 
	
}

void metrics(int totalTurnaround)
{	
	float averageTurnaround;
	float throughput = 0;
	float totalWaitingTime = 0;
	float avgWaitingTime = 0;
	printf("\nTotal Turnaround = %i", totalTurnaround);
	for(int k = 0; k < numProcesses; k++) {										
																		
		averageTurnaround += input[k].turnaroundTime;
	}
	averageTurnaround /= numProcesses;
	printf("\nAverage Turnaround = %f", averageTurnaround);

	throughput = (float)numProcesses/(float)totalTurnaround;
	printf("\nThroughput = %f", throughput);

	for(int i = 0; i < numProcesses; i++) 
	{
		totalWaitingTime += input[i].waitingTime;
	}
	avgWaitingTime = totalWaitingTime/numProcesses;
	printf("\nAverage Waiting Time = %f", avgWaitingTime);
	
}


	
