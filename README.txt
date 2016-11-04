The FCFS Schedulers without IO and with IO, SYSC 4001, A2

1) Open terminal and go to the directory where the files scheduler.cpp and schedulerIO.cpp are saved. Make sure you are running the programs on Linux as there are system calls. 

2) Compile the program(s):

	Scheduler without IO:
		gcc scheduler.cpp -o scheduler

	Scheduler with IO:
		gcc schedulerIO.cpp -o schedulerIO

3) Run the program(s):
	
	Scheduler without IO:
		./scheduler

	Scheduler with IO:
		./schedulerIO

4) Input Files
   The input file for scheduler without IO has the following format: 
	Pid  	Arrival Time 	Total CPU Time	Priority

   List your pid, arrival time, execution time and priority for each process in a single line with a space between each. The input file has been provided with an example of this format for scheduler.cpp. It is called pinput.txt.

   The input file for scheduler with IO has the following format: 
	Pid  	Arrival Time 	Total CPU Time	   Priority	IO Frequency	IODuration

   List your pid, arrival time, execution time, priority, IO frequency and IO duration for each process in a single line with a space between each. The input file has been provided with an example of this format for schedulerIO.cpp. It is called input.txt.

   All other input files for simulations tested are in the report which is attached to Part 1 of this assignment. 

5) Output File
   We have included a file called output.txt where the state changes and time of transition for each process are displayed. This file gets overwritten every time you run either scheduler.cpp and schedulerIO.cpp (they both use the same output file). All other output files for simulations tested are included in the report. 
