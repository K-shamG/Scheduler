#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct inputData
{
   int pid;
   int arrivalTime;
   int executionTime;
   int turnaroundTime;
   int waitingTime;
   bool ready;
   bool running;
   bool waiting;
   char state[]; 
};
//ask TA if going to add more processes to change the '5'
struct inputData input[5], tmp;
void printStruct();
void sortFCFS();
void FCFS();

int main(int argc, char* argv[])
{
	const char s[2] = " ";
	char *token;
	char *data;

	int i = 0;
	
	int counter =0;

    FILE* file = fopen("input.txt", "r"); 
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
			
			token = strtok(NULL, s); 
			counter++;
		}
		counter = 0;
		i++;
		
    } 
    fclose(file);
    printStruct();
    
    FCFS();

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
//comment
void FCFS()
{
	
	FILE* file = fopen("output.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Pid\tOld State\tNew State";
	fprintf(file, "%s\n", text);	
	
	sortFCFS();
	printf("\n\nOrder processes will execute in for FCFS:");
	printStruct(); 
	
	int totalTurnaround = 0;
	int numProcesses = 5;  
	
	for(int i = 0; i < 5; i++) {
		input[i].waiting = false; 
		input[i].ready = true;
		
		fprintf(file, "%i", input[i].pid);
		
		const char *text2 = "\twaiting\t\tready";
		fprintf(file, "%s\n", text2);
		
		//printf("\n%s", s);
		printf("\n%i %d %d" , input[i].pid, input[i].waiting, input[i].ready); 
		printf("\n%i %d %d" , input[i].pid, input[i].waiting, input[i].ready); 
		input[i].ready = false;
		input[i].running = true;
		
		totalTurnaround += input[i].executionTime;
		input[i].turnaroundTime +=totalTurnaround;

		int j;
		
		for(j= i+1; j < numProcesses; j++) {
			input[j].waiting = true;
			input[j].waitingTime = totalTurnaround - input[j].arrivalTime;
		}
	}
	
	for(int k = 0; k < 5; k++) {
		printf("\n%i Turnaround %i Waiting %i ", k+1, input[k].turnaroundTime, input[k].waitingTime);
	}
	
	fclose(file);
}



