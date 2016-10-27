#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct inputData
{
   int pid;
   int arrivalTime;
   int executionTime;
   int priority;
   int turnaroundTime;
   int waitingTime;
   bool wait;
   char state[]; 
};
//ask TA if going to add more processes to change the '5'
struct inputData input[5], tmp;
void printStruct();
void sortFCFS();
void setWaitToFalse();
void sortPriority();
void scheduler();

int main(int argc, char* argv[])
{
	const char s[2] = " ";
	char *token;
	char *data;
	int in;

	int i = 0;
	
	int counter =0;
	
	setWaitToFalse();

    FILE* file = fopen("pinput.txt", "r"); 
    char line[256];
	
	//each line
    while (fgets(line, sizeof(line), file)) {
		data = line;
        //printf("%s", line); 
        //splits by delimeter
		token = strtok(data, s);
		//each line each token
		while( token != NULL ) 
		{
			int x = atoi(token);
		    
		    if(counter == 0)
		    {
				input[i].pid = x;
				//printf("Pid %i\n", x);
			}
			else if(counter == 1) 
			{
				input[i].arrivalTime= x;
				//printf("ArrivalTime %i\n", x);
			}
			else if(counter == 2)
			{
				input[i].executionTime = x;
				//printf("*ExecutionTime %i\n", input[i].executionTime);
			}
			else if(counter == 3) 
			{
				input[i].priority = x;
			}
			
			token = strtok(NULL, s); 
			counter++;
		}
		counter = 0;
		i++;
		
    } 
    fclose(file);
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
	scheduler();

    return 0;
}

void printStruct()
{
	int j;
	printf("\nProcess\t   Arrival Time\t   Execution Time");
	for(j=0; j < 5; j++)
    {
		printf("\n  %i\t\t%i\t\t%i", input[j].pid, input[j].arrivalTime, input[j].executionTime);		
	}
}

void sortFCFS()
{
	int k, m;
	 //Create the ready queue and update every time a new process is scheduled
	 for(k=0;k<5-1;k++)
	 {
		for(m=0;m<5-1-k;m++)    
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
	int k, m;
	 //Create the ready queue and update every time a new process is scheduled
	 for(k=0;k<5-1;k++)
	 {
		for(m=0;m<5-1-k;m++)    
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
	for(int i = 0; i < 5; i++) {
		input[i].wait = false;
	}
}

void scheduler()
{
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Time of Transition\tPid\tOld State\tNew State";
	fprintf(file, "%s\n", text);	
	
	//sortPriority();
	printf("\n\nOrder processes will execute in for Priority Scheduling:");
	printStruct(); 
	
	int totalTurnaround = 0;
	int numProcesses = 5;  
	
	for(int i = 0; i < 5; i++) {
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
		
		for(j= i+1; j < numProcesses; j++) {
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
}



