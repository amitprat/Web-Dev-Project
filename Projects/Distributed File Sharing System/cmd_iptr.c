/***************************************************************************************
File 			: cmd_iptr.c
Author 			: Amit Pratap Singh
Organization 	: University at Buffalo
Description		: This file contains command input validation and execution function set.
					It takes the user input, parses data and compares the string set available
					to verify command and if it is valid command then calls the particular
					execution function otherwise displays proper command format to users or 
					invalid command warning.
 ****************************************************************************************/
#include "gprot.h"
/***************************************************************************************
Function 	: handle_cmd_ln_intr
Description	: It starts with user input parsing, analysing and calling the specific execution
				function.
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
		index = 0;
		status = validate_cmd(cmd_arg_var);
		if(status)
			cmd_line_execute(cmd_arg_var);
		else
			printf("Not a valid TCP Command!\n");
	}
}
/***************************************************************************************
Function 	: validate_cmd
Description	: It validates the command and return TRUE/FALSE to caller function. Also it 
				displays proper error message to user.
****************************************************************************************/
bool validate_cmd(char *input[])
{
	bool result = FALSE;
	if(strcmp(input[0],"HELP")==0)
	{
		return TRUE;
	}
	if( (strcmp(input[0],"CLEAR")==0) || (strcmp(input[0],"CLS")==0) )
	{
		return TRUE;
	}
	if(strcmp(input[0],"CREATOR")==0)
	{
		return TRUE;
	}
	if( strcmp(input[0],"MYIP") == 0)
	{
		result = TRUE;
	}
	if( strcmp(input[0],"MYPORT") == 0)
	{
		result = TRUE;
	}
	if(strcmp(input[0],"TERMINATE") == 0)
	{
		if(!input[1])
		{
			perror("TERMINATE <conn_id/sock_id>\n");
			return FALSE;
		}
		return TRUE;
	}
	if(strcmp(input[0],"CHART") == 0)
	{
		return TRUE;
	}
	if( (strcmp(input[0],"QUIT") == 0) || (strcmp(input[0],"EXIT") == 0) )
	{
		if(mode == CLIENT)
			return TRUE;
		else
		{
			printf("Run of Client only!");
			return FALSE;
		}
	}
	if(strcmp(input[0],"DOWNLOAD") == 0)
	{
		if(mode == SERVER)
		{
			printf("This command can be run on client only!\n");
			return FALSE;
		}
		if(!input[1] || !input[2])
		{
			perror("DOWNLOAD <filename> <filesize>\n");
			return FALSE;
		}
		return TRUE;
	}
	if(strcmp(input[0],"LIST") == 0)
	{
		if(mode == SERVER)
		{
			printf("This command can be run on client only!\n");
			return FALSE;
		}
		return TRUE;
	}
	if( !strcmp(input[0],"REGISTER") || !strcmp(input[0],"CONNECT"))
	{
		if(mode == SERVER)
		{
			printf("This command can be run on client only!\n");
			return FALSE;
		}
		if( !input[1] || !input[2])
		{
			printf("%s <ip address/name> <port>",input[0]);
			return FALSE;
		}
		return TRUE;
	}
	return result;
}

/***************************************************************************************
Function 	: validate_cmd
Description	: It validates the command and return TRUE/FALSE to caller function. Also it 
				displays proper error message to user.
****************************************************************************************/
void cmd_line_execute(char *input[])
{
	if(strcmp(input[0],"HELP")==0)
	{
		display_help();
	}
	if( (strcmp(input[0],"CLEAR")==0) || (strcmp(input[0],"CLS")==0) )
	{
		system("clear");
	}
	if(strcmp(input[0],"CREATOR")==0)
	{
		display_creator_info();
	}
	if(strcmp(input[0],"MYIP")==0)
	{
		display_ip();
	}
	if(strcmp(input[0],"MYPORT") == 0)
	{
		printf("My Port : %d\n",port);
	}
	if(strcmp(input[0],"LIST") == 0)
	{
		list_peer_devices();
	}
	if(strcmp(input[0],"TERMINATE") == 0)
	{
		terminate_conn(input[1]);
	}
	if(strcmp(input[0],"CHART") == 0)
	{
		display_downloaded_data();
	}
	if(strcmp(input[0],"DOWNLOAD") == 0)
	{
		int i=0;
		memset(&send_buf,0,sizeof(send_buf));
		strcpy(send_buf,input[1]);
		strcat(send_buf,":size:0");
		if(i<=cur_peer_sock_id)
		{
			printf("FILE DOWNLOAD START--\n");
			if(peer_sock_id[0])
			download_file(peer_sock_id[0],input[1],input[2]);
			else if(peer_conn_id[0])
			download_file(peer_conn_id[0],input[1],input[2]);
			else
			{
				printf("No Connection present!\n");
				return;
			}
			down_begin = TRUE;
			start = clock();
		}
		else
		{
			perror("No device connected!\n");
			return;
		}
	}
	if( (strcmp(input[0],"QUIT") == 0) || (strcmp(input[0],"EXIT") == 0) )
	{
		quit_all_conn();
		quit = TRUE;
	}
	if(strcmp(input[0],"REGISTER") ==0 )
	{
		if(mode == SERVER)
		{
			perror("This Cmd can run of client only\n");
		}
		else
		{
			if(cl_server_sock_id > 0)
			{
				perror("Client already registered with this server!\n");
			}
			else
			{
				int sid = 0;
				int status;
				char ipstr[128];	
				char ipstr1[128];
				struct hostent *host;
				if(isalpha(input[1][0]))
				{
					host = gethostbyname(input[1]);
					#ifdef TRACE_DEBUG
					printf("Hostname: %s\n", host->h_name);
					printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
					#endif
					strcpy(ipstr1,inet_ntoa(*((struct in_addr *)host->h_addr)));
					status = connect_dev_to_serv(ipstr1,atoi(input[2]));
				}
				else
				{
					status = connect_dev_to_serv(input[1],atoi(input[2]));
				}
				if(status < 0)
				{
					printf("Connect fail\n");
				}
			}
		}

	}
	if(strstr(input[0],"CONNECT") !=0 )
	{
		if(mode == SERVER)
		{
			perror("This Cmd can run in Client Mode only\n");
		}
		else
		{
			if(cur_peer_sock_id+cur_peer_conn_id >= 3)
				perror("Maximum(3) number of connection reached:\n");
			else
			{
				int status;
				char ipstr[128];
				char ip_str1[MAX_SIZE];
				struct hostent *host;
				if(isalpha(input[1][0]))
				{
					host = gethostbyname(input[1]);
					#ifdef TRACE_DEBUG
					printf("Hostname: %s\n", host->h_name);
					printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
					#endif
					strcpy(ip_str1,inet_ntoa(*((struct in_addr *)host->h_addr)));
					status = connect_dev(ip_str1,atoi(input[2]));
				}
				else
				{
					status = connect_dev(input[1],atoi(input[2]));
				}
				if(status > 0)
				{
					int file_size;
					char fl_name[10]={'\0'};
					printf("Device Connected\n");
				}
			}
		}

	}
	if(strcmp(input[0],"Exit") == 0)
	{
		printf("Terminate Connection\n");
	}
}
