Read Me File:

1) If some problem happens with Makefile for program execution,
please follow the following steps:

gcc -c -g amitprat_proj1.c
gcc -c -g utility.c
gcc -c -g cmd_iptr.c
gcc amitprat_proj1.o utility.o cmd_iptr.o -o proj1

Note - Run the 'proj1' as main execution file.
./proj1 c 23244
./proj1 s 34358
2)Command Set:
	HELP : Help content for available command interface
	CREATOR : Displays the creator name,email ids
	MYIP : Displays its own IP address
	MYPORT : Displays its own port address
	QUIT/EXIT : Back to main terminal window(terminate all connections)
	TERMINATE : Terminate a single connection
	REGISTER : Register Client with Server
	CONNECT : Connect Client with other peers
	LIST : List all connected devices
	DOWNLOAD : File Download from peers
	CHART : Display chunk downloaded details from multiple hosts.
	CLEAR/CLS - to clear the sub shell data.
3)The output file after download is names as 
	out_"source file name"

4)TRACE_DEBUG ON/OFF can be used to on/off the extra traces for program.
