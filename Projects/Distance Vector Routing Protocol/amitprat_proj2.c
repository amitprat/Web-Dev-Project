/***************************************************************************************
File 	: amitprat_proj1.c
Desc	: This is the main program file which starts the program execution. The main function kicks the server part.
			This file implements the basic working of client server connection set-up, argument initialization.
			It runs the server part in while loop forever and wait in select condition for either any input on
			its sockets or command time-out. It invokes the DV algorithm further for command handling and cost
			updation.
 ****************************************************************************************/

#include"gprot.h"


/***************************************************************************************
Function 	: main
Desc		: It starts the program execution. It first takes the program arguments and
				the read topology file to fill topology structure. After that it initializes
				the link structure to access the link cost of its neighbours and then it
				run the server which waits for STDIN or Input/Output on socket in while loop.
Parameters	: argc  - input argument count
			  argv  - Array of pointers to input argumerns
Return Val	: Integer
Comments	: 
 ****************************************************************************************/
int main(int argc , char *argv[])
{
	get_arguments(argc,argv);
	memset(&disabled_server_ids,0,num_servers);
	read_topology_file();
	num_servers = topology.num_servers +1;
	init_cost_table();
	port = topology.server_port;
	printf("\n My Port :%d",port);
	run_server_part();
	return 0;
}

/***************************************************************************************
Function 	: run_server_part
Desc		: It sends the periodic after to every neighbour after the time expiry that runs
			  in select wait. When select time expires, it call the subroutine to trigger
			  update. Simultaneously it listens for All incoming sockets(including STDIN) for
			  any data and process the user command in parallel.
Parameters	: 
Return Val	: 
Comments	: 
 ****************************************************************************************/
void run_server_part()
{
	tv.tv_sec = periodic_update_time;
	tv.tv_usec = 0;
	if(server_sock_id < 0)
	{
		perror("Socket Creation Error\n");
		exit(0);
	}
	server_setup();
	for(;;)
	{
		printf("\nproj2$_>");
		fflush(stdout);
		server_fds = master;
		if(select(server_fd_max+1,&server_fds,NULL,NULL,&tv) == 0)
		{
			send_periodic_update();
		}
		if(FD_ISSET(STDIN,&server_fds))
		{
			prev_time_out_val = tv.tv_sec;
			char input[MAX_CMD_SIZE];
			fgets(input,MAX_CMD_SIZE,stdin);
			input[strlen(input)] = '\0';
			if(iswhitespc(input))
			{
				fflush(stdout);
			}
			else
			{
				handle_cmd_ln_intr(input);
			}

		}
		if(FD_ISSET(server_sock_id,&server_fds))
		{
			prev_time_out_val = tv.tv_sec;
			int len = 0;
			long numbytes;
			if ((numbytes = recvfrom(server_sock_id, buffer_size, MAX_BUF_SIZE-1 , 0,
					(struct sockaddr *)NULL, &len)) == -1) 
			{
				perror("recvfrom");
				exit(1);
			}
			int server_id;
			server_id = update_topology_link_cost(buffer_size);
			print_updated_link_cost(server_id);
		}
	}
}

/***************************************************************************************
Function 	: handle_cmd_ln_intr
Desc		: It takes the command line input for user command and the it parses, verifies 
				command format and prints the proper error message if command format is wrong.
				It further passes the command variables to execution routine for further execution
				of command.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void handle_cmd_ln_intr(char input[])
{
	bool quit = 0;
	bool status;
	int index=0;
	char input1[MAX_CMD_SIZE];
	char *str_tok;
	{

		input[strlen(input)-1]='\0';
		for(index=0;index<MAX_CMD_LN_VAR;index++)
		{
			cmd_arg_var[index] = malloc(MAX_CMD_SIZE);
			memset(cmd_arg_var[index],0,MAX_CMD_SIZE);
		}
		str_tok = strtok (input," ");
		index = 0;

		while (str_tok != NULL)
		{
			strcpy(cmd_arg_var[index],str_tok);
			str_tok = strtok (NULL, " ");
			index++;
		}
		index=0;
		while(cmd_arg_var[0][index])
		{
			cmd_arg_var[0][index] = toupper(cmd_arg_var[0][index]);
			index++;
		}
		status = validate_user_cmd_input(cmd_arg_var);
		if(status == TRUE)
			execute_user_cmd();
	}
}

/***************************************************************************************
Function 	: client_setup
Desc		: It handle the socket creation, server address filling on behalf of client.
Parameters	: Input address
			  Port Number
Return Val	: void
Comments	: 
 ****************************************************************************************/
void client_setup(char ipstr[],int port)
{
	sock_id = socket(SOCK_FAMILY,SOCK_TYPE,SOCK_FLAG);
	memset(&client_addr,0,sizeof(client_addr));
	client_addr.sin_family = SOCK_FAMILY;
	client_addr.sin_addr.s_addr =inet_addr(ipstr);
	client_addr.sin_port = htons(port);
}
/***************************************************************************************
Function 	: server_setup
Desc		: It handle the socket creation, server address filling on behalf of server
				to start.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void server_setup()
{
	server_sock_id = socket(SOCK_FAMILY,SOCK_TYPE,SOCK_FLAG);
	memset(&server_addr,0,sizeof(server_addr));
	FD_ZERO(&master);
	FD_ZERO(&server_fds);
	FD_SET(STDIN,&master);
	FD_SET(server_sock_id,&master);
	server_fd_max = server_sock_id;
	server_addr.sin_family = SOCK_FAMILY;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	if(bind(server_sock_id,(struct sockaddr *)&server_addr,sizeof(server_addr)))
	{
		perror("Bind failed\n");
		exit(1);
	}
	printf("\nServer Set-up Done\n");
}

/***************************************************************************************
Function 	: get_arguments
Description	: This function takes the input and sets them to global variable.
Parameters	: integer argument count
				character argument pointer array
Return Val	: integer
Comments	: 
****************************************************************************************/
void get_arguments(int argc,char *argv[])
{
	char ch;
	topology_file_name = NULL;
	periodic_update_time = -1;
	while ((ch = getopt(argc, argv, "t:i:")) != -1)
		switch(ch) 
		{
			case 't':
				topology_file_name = optarg;
				break;
			case 'i':
				periodic_update_time = atoi(optarg);
				break;
			default:
				break;
		}
	if(topology_file_name == NULL &&
		periodic_update_time == -1)
	{
		printf("Invalid Command Format!\n");
		printf("Command -i <filename> -t <time_interval>\n");
		exit(0);
	}
	argc -= optind;
}

