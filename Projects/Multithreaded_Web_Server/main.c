/***************************************************************************************
File 			: main.c
Author 			: Amit Pratap Singh
Date			: 
Email			: amitprat@buffalo.edu
Organization 	: University at Buffalo
				  Operating System - 521 
				  Multi threaded Web Server(Project 1)
Description		: This file contains the data structures and function declaration for all three
				  files( one main file, one thread file and a supporting file for both(utility.c))
				  
 ****************************************************************************************/

#include "gprot.h"

/***************************************************************************************
Function 	: main
Description	: This functions is the main function for starting execution of the program.
			  It processes the input arguments, set the programs vars and exectuion path
			  according to them.
Parameters	: argc  - input argument count
			  argv  - Array of pointers to input argumerns
Return Val	: Integer
Comments	: 
****************************************************************************************/
int main(int argc,char *argv[])
{
	/*Input programme arguments*/
	if ((progname = rindex(argv[0], '/')) == NULL)
		progname = argv[0];
	else
		progname++;
	get_arguments(argc,argv);
	if(debug_mode == TRUE)
	{
		start_myhttp();
	}
	else
	{
		demonize_myhttpd_web_server();
	}
	return 1;
}

/***************************************************************************************
Function 	: demonize_myhttpd_web_server
Description	: 
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void demonize_myhttpd_web_server()
{
	int i=0;
	int fd_1,fd_2,fd_3;
	pid_t pid;
	struct rlimit limit;
	struct sigaction sig_hdlr;
	umask(0);
	if(getrlimit(RLIMIT_NOFILE,&limit) < 0)
		printf("\n Can't get file limit!");

	if( (pid = fork() ) < 0)
	{
		printf("\n Error whle forking!");
	}
	else if(pid != 0)
	{
		exit(0);
	}
	else
	{
		setsid();
	}

	sig_hdlr.sa_handler = SIG_IGN;
	sigemptyset(&sig_hdlr.sa_mask);
	sig_hdlr.sa_flags = 0;

	if(sigaction(SIGHUP,&sig_hdlr,NULL) < 0 )
		printf("\n Error ignoring SIGHUP!");
	if ((pid = fork()) < 0) 
		printf("\nError while forking!"); 
	else if (pid != 0)
		exit(0); 

	if (limit.rlim_max == RLIM_INFINITY) 
		limit.rlim_max = MAX_RLIMIT; 
	for (i = 0; i < limit.rlim_max; i++) 
		close(i); 

	fd_1 = open("/dev/null", O_RDWR); 
	fd_2 = dup(0); 
	fd_3 = dup(0); 

	if (fd_1 != 0 || fd_2 != 1 || fd_3 != 2)
	{
		printf("\n Unexpected file descriptor!");
		exit(1);
	}
	start_myhttp();
}


/***************************************************************************************
Function 	: demonize_myhttpd_web_server
Description	: This function starts the main http web server.
Parameters	: void
Return Val	: integer
Comments	: 
****************************************************************************************/
int start_myhttp()
{
	int i =0;
	/*Create socket on local host.*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("Socket Creation Error!");
		exit(1);
	}
	
	init_utility_setup_vars();
	
	sock = setup_server();
	
	init_thread_setup_vars();
	return(0);
}

/***************************************************************************************
Function 	: setup_server
Description	: This function starts the main http web server.
Parameters	: void
Return Val	: integer
Comments	: 
****************************************************************************************/
int setup_server() 
{
	struct sockaddr_in serv, remote;
	struct servent *se;
	int newsock, len;

	len = sizeof(remote);
	memset((void *)&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	if (port == NULL)
		serv.sin_port = htons(0);
	else if (isdigit(*port))
		serv.sin_port = htons(atoi(port));
	else {
		if ((se = getservbyname(port, (char *)NULL)) < (struct servent *) 0) {
			perror(port);
			exit(1);
		}
		serv.sin_port = se->s_port;
	}
	if (bind(s, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
		perror("bind");
		exit(1);
	}
	if (getsockname(s, (struct sockaddr *) &remote, &len) < 0) {
		perror("getsockname error!");
		exit(1);
	}
	#ifdef TRACE_DEBUG_ON
	fprintf(stderr, "Port number is %d\n", ntohs(remote.sin_port));
	#endif
	listen(s, 5);
	newsock = s;
	return(newsock);
}

/***************************************************************************************
Function 	: get_arguments
Description	: This function starts the main http web server.
Parameters	: void
Return Val	: integer
Comments	: 
****************************************************************************************/
void get_arguments(int argc,char *argv[])
{
	char ch;
	while ((ch = getopt(argc, argv, "dhl:p:r:t:n:s:")) != -1)
		switch(ch) {
			case 'd':
				debug_mode = 1;
				break;
			case 'h':
				help = 1;
				usage();
				break;
			case 'l':
				logging_enabled = TRUE;
				usr_logging_file = optarg;
				break;
			case 'p':
				port = optarg;
				break;
			case 'r':
				root_dir = optarg;
				break;
			case 't':
				queuing_time = optarg;
				break;
			case 'n':
				thread_num = optarg;
				break;
			case 's':
				set_sched_policy = optarg;
				break;
			case '?':
				break;
			default:
				break;
		}
	argc -= optind;
}
