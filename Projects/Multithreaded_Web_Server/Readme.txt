

1) Compilation
	code can be compiled simply by typing "make" or "make -f Makefile"
	The corresponding Makefile is included in the package.
	If some issue comes with program compilation by makefile, please
	run  the following command.
	
	gcc main.c thread.c utility.c -lpthread -o project
	
	Note : Make file compiles and links the code to make executable with the name "project".
	
2)  Execution :
	Program can simply be started as 
	./project
	(wihtout any option)
	All the details for option usage can be obtained by typing
	./project -h
	
	as follow:
	
	usage format
	./project -d -l[log_file] -p[port] -r[root_dir] -t[queuing_time] -n[num_of_threads] -s[scheduling_policy]
	./project -d[Enable this option to enter debugging mode]
	./project -l[Provide log file name]
	./project -p[Set port number otherwise it will take 8080 as default]
	./project -r[Set root directory, DEFAULT : current working directory ]
	./project -t[Set queuing time, DEFAULT : 60 Sec]
	./project -n[Set number of user threads , DEFAULT:4]
	./project -s[Set scheduling policy[DEFAULT:FCSE, SJF]
	
3)	All the debug time logging is disabled , but can simply be uncommented with uncommenting
	TRACE_DEBUG_ON