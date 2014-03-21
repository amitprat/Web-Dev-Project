/***************************************************************************************
File 			: amitprat_proj1.c
Author 			: Amit Pratap Singh
Organization 	: University at Buffalo
Description		: This file contains the main functions initiating program execution 
					and calling other transferring calls to other subroutines for 
					further execution. It contains the loading  code for both client
					and server.
 ****************************************************************************************/
#include"gprot.h"
/***************************************************************************************
Function 	: main
Description	: It takes input arguments from user and identifies the argument correctness
				and initiates the corresponding server/client procedure based on arguments.
****************************************************************************************/
int main(int argc , char *argv[])
{

    if(argc != 3)
    {
        printf("Invalid Command Format!\n");
        printf("Command type(c - client or s-server) port(such as 9669)\n");
        exit(0);
    }

    mode = (strcmp(argv[1],"c") == 0)? 1 : 0 ;
    port  = atoi(argv[2]);
    if(mode ==  SERVER)
	{
		run_server_part();
	}
    else if(mode == CLIENT)
	{
		run_client_part();
	}
    return 0;
}

/***************************************************************************************
Function 		: run_server_part
Description		: It initiates the server code execution procedure. It provides interface
					for different sockets and IO interrupts.
****************************************************************************************/
void run_server_part()
{
    int index;
	int i=0;
    server_sock_id = socket(SOCK_FAMILY,SOCK_TYPE,SOCK_FLAG);
    if(server_sock_id < 0)
    {
        perror("Socket Creation Error\n");
        exit(0);
    }
	memset(&server_addr,0,sizeof(server_addr));
	FD_ZERO(&master);
	FD_ZERO(&server_fds);
	FD_SET(STDIN,&master);
	FD_SET(server_sock_id,&master);
    server_fd_max = server_sock_id;
	server_setup();
	#ifdef TRACE_DEBUG
	printf("Server Setup Done\n");
	printf("Cmd line iptr\n");
	#endif
    for(;;)
    {
		if(quit == TRUE)
		{
		  printf("Bye__\n");
		  return;
 		}
		printf("\nproj1$_>");
		fflush(stdout);
        server_fds = master;
		if(select(server_fd_max+1,&server_fds,NULL,NULL,NULL) == 0)
		{
			perror("Error\n");
			exit(4);
		}
		if(FD_ISSET(STDIN,&server_fds))
		{
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
			char ipstr[HOST_SIZE];
			char lo_port[10];
			int conn_id;
			int len;
			int i = 0;
			#ifdef TRACE_DEBUG
			printf("Server Accept Wait\n");
			#endif
               conn_id = accept(server_sock_id,(struct sockaddr *)NULL,NULL);
               if(conn_id == -1)
               {

                   perror("Accept failed\n");
                   exit(0);

               }
			#ifdef TRACE_DEBUG
			printf("Connection accepted\n");
			#endif
			FD_SET(conn_id,&master);
			if(conn_id > server_fd_max)
				server_fd_max = conn_id;

			server_conn_id[cur_server_conn_id] = conn_id;
			cur_server_conn_id++;
        }
		for(i=0;i<cur_server_conn_id;i++)
		{
			if(FD_ISSET(server_conn_id[i],&server_fds))
			{
				int status;
				status = recv(server_conn_id[i],recv_buf,MAX_SIZE,0);
				#ifdef TRACE_DEBUG
				printf("server_kil_status:%d\n",status);
				#endif
				if(status > 0)
				{
					update_server_ip_list(recv_buf,server_conn_id[i],TRUE);
					break;
				}
				else if(status <= 0)
				{
					int j=0;
					int conn_id = server_conn_id[i];
					#ifdef TRACE_DEBUG
					printf("server_kil_status1:%d\n",status);
					#endif
					for(j=i;j<cur_server_conn_id;j++)
					{
						if(j == cur_server_conn_id-1)
							server_conn_id[j] = 0;
						else
							server_conn_id[j] = server_conn_id[j+1];
					}
					cur_server_conn_id--;
					update_server_ip_list(NULL,conn_id,FALSE);
					FD_CLR(conn_id,&master);
					break;
				}
			}
		}
    }

}
/***************************************************************************************
Function 	: update_server_ip_list
Description	: It updates the server list for registered clients. It keeps the IP address,
				port number and other details of registered clients. It intiates the call
				for removal/update of list.
 ****************************************************************************************/
void update_server_ip_list(char recv_buf[],int sock_id,bool add_dev)
{
	int status=0;
	#ifdef TRACE_DEBUG
	printf("update_server_ip_list,add_dev:%d\n",add_dev);
	#endif
	if(add_dev == 1)
	{
		set_server_ip_list(recv_buf,sock_id);
	}
	else
	{
		status = remove_server_ip_list(sock_id);
	}
	#ifdef TRACE_DEBUG
	if(status == -1)
		printf("Device removal Error!\n");
	else
		printf("Device removed, list updated\n");
    #endif
	print_server_ip_list();
}
/***************************************************************************************
Function 	: print_server_ip_list
Description	: It sends the updated server IP list data to all other registered clients.
				It also sends the update to other clients if some client enters or 
				leaves in between.
****************************************************************************************/
void print_server_ip_list()
{
	int i = 0;
	char lo_port[10];
	char init_ind[] = "begin";
	char end_ind[] = "end";
	ip_list_struct *temp = NULL;
	
	
	for(i=0;i<cur_server_conn_id;i++)
	{
		send(server_conn_id[i],init_ind,MAX_SIZE,0);
	}
	usleep(100);
	for(i=0;i<cur_server_conn_id;i++)
	{
		temp = ip_list_first;
		while((temp!=  NULL))
		{
			memset(send_buf,0,sizeof(send_buf));
			strcpy(send_buf,temp->ip_addr);
			sprintf(lo_port,"%d",temp->port);
			strcat(send_buf,":");
			strcat(send_buf,lo_port);
			#ifdef TRACE_DEBUG
			printf("send_buf:%s::%d\n",send_buf,server_conn_id[i]);
			#endif
			send(server_conn_id[i],send_buf,MAX_SIZE,0);
			temp = temp->next;
		}
	}
	usleep(100);
	for(i=0;i<cur_server_conn_id;i++)
	{
		send(server_conn_id[i],end_ind,MAX_SIZE,0);
	}
}
/***************************************************************************************
Function 	: remove_server_ip_list
Description	: It removes the details stored of a client if it closes connection with server.
 ****************************************************************************************/
int remove_server_ip_list(int sock_id)
{
	int status = -1;
	#ifdef TRACE_DEBUG
	printf("Remove_server_ip_list!\n");
	#endif
	ip_list_struct *temp=ip_list_first;
	ip_list_struct *del;
	if(!temp)
	{
		printf("Device not registered!\n");
		return status;
	}
	else if(!temp->next)
	{
	    #ifdef TRACE_DEBUG
		printf("Remove_server_ip_list1!\n");
		#endif
		if(temp->sock_id == sock_id)
	{
		free(temp);
			ip_list_first = NULL;
		return 1;
	}
		else
			return status;
	}
	if(temp && temp->next)
	{
        #ifdef TRACE_DEBUG
			printf("Remove_server_ip_list,sock_id:%d,temp->next->sock_id:%d\n",sock_id,temp->next->sock_id);
		#endif
        if(temp->sock_id == sock_id)
        {
           del = temp;
           temp = temp->next;
           free(del);
           return 1;
        }
		while( temp->next && temp->next->sock_id != sock_id)
		{
			temp = temp->next;
 		}
        if(temp->next == NULL)
            return status;
		#ifdef TRACE_DEBUG
		printf("Remove_server_ip_list3!\n");
		#endif
		del = temp->next;
		temp->next = temp->next->next;
		free(del);
		return 1;
	}
    #ifdef TRACE_DEBUG
	printf("Remove_server_ip_list4!\n");
    #endif
	return status;
}
/***************************************************************************************
Function 	: set_server_ip_list
Desc		: It stores the client information with server i.e IP address/port number and
				connection ids. These information can be accessed by other clients to 
				authenticate the other clients.
****************************************************************************************/
int set_server_ip_list(char recv_buf[],int id)
{
	int status;
	socklen_t length;
	ip_list_struct *new_node=NULL;
	int index = 0;
	char ip_str[2][HOST_SIZE];
    char *str_tok;
	index = 0;
	
    str_tok = strtok (recv_buf,":");
    while (str_tok != NULL)
    {
        strcpy(ip_str[index],str_tok);
        str_tok = strtok (NULL, ":");
        index++;
    }
	new_node = malloc(sizeof(new_node));
	memset(new_node,0,sizeof(new_node));
	strcpy(new_node->ip_addr,ip_str[0]);

	new_node->port = atoi(ip_str[1]);
	new_node->sock_id = id;
	new_node->next = NULL;

	if(!ip_list_current && !ip_list_first)
	{
		#ifdef TRACE_DEBUG
		printf("set server ip list:null\n");
		#endif
		ip_list_current = new_node;
		ip_list_first = new_node;
		//ip_list_first->next = NULL;
		//ip_list_first->next = NULL;
	}
	else
	{
		#ifdef TRACE_DEBUG
		printf("set server ip list:!NULL\n");
		#endif
		//ip_list_current = ip_list_first;
		//while(ip_list_current->next != NULL)
		//	ip_list_current = ip_list_current->next;
		ip_list_current->next = new_node;
		ip_list_current = ip_list_current->next;
		if(ip_list_first->next == NULL)
			ip_list_first = ip_list_current;
	}
	return 1;
}

/***************************************************************************************
Function 	: run_client_part
Description	: It starts the client part for handling IO and socket interrupts from other peers
				as well as other sockets.
****************************************************************************************/
void run_client_part()
{
    int index;
    client_sock_id = socket(SOCK_FAMILY,SOCK_TYPE,SOCK_FLAG);
    if(client_sock_id < 0)
    {
        perror("Socket Creation Error\n");
        exit(0);
    }
    memset(&client_addr,0,sizeof(client_addr));
    FD_ZERO(&master);
    FD_ZERO(&client_fds);
    FD_SET(STDIN,&master);
    FD_SET(client_sock_id,&master);
    client_fd_max = client_sock_id;
    client_addr.sin_family = SOCK_FAMILY;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(port);
    if(bind(client_sock_id,(struct sockaddr *)&client_addr,sizeof(client_addr)))
    {
        perror("Bind failed");
        exit(1);
    }
    listen(client_sock_id,MAX_CONN);
    #ifdef TRACE_DEBUG
    printf("Client Setup Done\n");
    printf("Cmd line iptr\n");
    #endif
    for(;;)
    {
	if(quit == TRUE)
	{
	  printf("Bye__\n");
	  return;
 	}
	if(down_begin == FALSE)
	{
	  printf("\nproj1$_>");
	  fflush(stdout);
	}
    client_fds = master;
	if(select(client_fd_max+1,&client_fds,NULL,NULL,NULL) == 0)
	{
	  perror("Error\n");
	  exit(4);
	}

	for(index=0;index<=client_fd_max;index++)
	   {

		if(FD_ISSET(index,&client_fds))
		{
			int j=0;
			/*this socket is used by client to connect to server so any register
			command will cause input on this socket written by server	*/
			if((cl_server_sock_id != 0) && (index == cl_server_sock_id))
			{
			  int status;
			  if( (status = recv(cl_server_sock_id,recv_buf,MAX_SIZE,0)) <= 0)
			  {
			    reg_host_ip_list(recv_buf,0);
			    FD_CLR(index,&master);
			    close(index);
			  }
			  else if(status > 0)
			  {
				if(strcmp(recv_buf,"begin") == 0)
				{
							printf("\n******Registered Clients with this server****************\n");
							printf("HOST-Name\tIP_ADDRESS\tPORT\n");
							down_begin = TRUE;
							reg_host_ip_list(recv_buf,1);
				}
				else if(strcmp(recv_buf,"end") == 0)
				{
					print_reg_ip_list();
					printf("********************************************************\n");
					down_begin = FALSE;
				}
			    else
			    {
			      reg_host_ip_list(recv_buf,1);
			    }
			  }
			  //printf("RECD_data:%s\n",recv_buf);
			}

			/*this is the socket ids on which client initiate connection to other peers*/
			for(j=0;j<cur_peer_sock_id;j++)
			{
				if((peer_sock_id[j] != 0) && (index == peer_sock_id[j]) )
				{

				   #ifdef TRACE_DEBUG
		    		   printf("peer sent some data:%d\n",index);
				   #endif
				   if(recv(index,recv_buf,MAX_READ_SIZE,0) <= 0)
				   {
						update_conn_dev_list(NULL,0,index,FALSE);
						FD_CLR(index,&master);
				   }
				   else
				   {
			       	    handle_peer_data(index,recv_buf);
				   }
				   break;

				}
			 }
			/*these are the conn ids on which client listens of incoming data.
			These are peer connections initiated by other client*/
			for(j=0;j<cur_peer_conn_id;j++)
			{
				if( (peer_conn_id[j]!=0) && (index == peer_conn_id[j]) )
				{

				   #ifdef TRACE_DEBUG
		    		   printf("peer req some data:%d\n",index);
				   #endif
				   if(recv(index,recv_buf,MAX_READ_SIZE,0) <= 0)
				   {
					update_accepted_conn_list(NULL,0,index,FALSE);
					FD_CLR(index,&master);
				   }
			       	   else
				  {
					handle_download_req(index,recv_buf);
				  }
				   break;

				}
			 }

		}//end for if(FD_ISSET(*(*))

	 }//end for(index=0;index<=client_fd_max;index++)
		/*Listen for input on STDIN*/
		if(FD_ISSET(STDIN,&client_fds))
		{
    		char input[MAX_CMD_SIZE];
			memset(input,0,sizeof(input));
			fgets(input,MAX_CMD_SIZE,stdin);
			input[strlen(input)] = '\0';
    		if(iswhitespc(input))
    		{
				fflush(stdout);
    		}
    		else
	           	handle_cmd_ln_intr(input);
	    }

		/*Listen for incoming peer connections on this socket.
		Add conn id for further listening peer data*/
	    	if(FD_ISSET(client_sock_id,&client_fds))
	    	{
			char ipstr[HOST_SIZE];
			int conn_id;
			int len;
			int i = 0;
			#ifdef TRACE_DEBUG
			printf("Server Accept Wait\n");
			#endif
        	conn_id = accept(client_sock_id,(struct sockaddr *)NULL,NULL);
        	if(conn_id == -1)
        	{

        	    perror("Accept failed\n");
        	    exit(0);

        	}
			printf("Server Accepted Connection\n");
			peer_conn_id[cur_peer_conn_id] = conn_id;
			FD_SET(conn_id,&master);
			client_fd_max = (conn_id > client_fd_max)?conn_id:client_fd_max;
			cur_peer_conn_id++;
			update_accepted_conn_list(NULL,0,conn_id,TRUE);
           	}

    }//end for(;;)

} //end run_client_part

/***************************************************************************************
Function 	: handle_download_req
Desc		: It handles the download request of client. It analyses the request data size 
				and offset to read and sends the read data to the other peer.
****************************************************************************************/
void handle_download_req(int conn_id,char recv_buf[])
{
	int fd;
	int status;
	int chunk_size = 0;
	int offset =0;
	char data[3][32];
	char f_name[32];
	char *str_tok;
	int n;
	memset(send_buf,0,sizeof(send_buf));
	str_tok = strtok (recv_buf,":");
    int index = 0;
    while (str_tok != NULL)
    {
        strcpy(data[index],str_tok);
        str_tok = strtok(NULL, ":");
        index++;
    }

	strcpy(fl_name,data[0]);
	chunk_size = atoi(data[1]);
	offset = atoi(data[2]);
	if(strcmp(data[1],"size?") == 0)
	{
		char temp[10];
		FILE *fp;
		long sz;
		start = clock();
		fp = fopen(fl_name,"r");
		fseek(fp,0L,SEEK_END);
		sz = ftell(fp);
		cur_download_size = sz;
		strcpy(send_buf,fl_name);
		strcat(send_buf,":size=:");
		sprintf(temp,"%lu",sz);
		strcat(send_buf,temp);
		#ifdef TRACE_DEBUG
		printf("sent data from size:%s\n",send_buf);
		#endif
		send(conn_id,send_buf,sizeof(send_buf),0);
		fclose(fp);
		return;
	}
	if(cur_download_size >= req_download_size)
	{
		#ifdef TRACE_DEBUG
		down_begin = FALSE;
		//stop = clock();
		//used_time = (double)(stop -start);
		//printf("Transfer Time:%f\n",used_time);
		#endif
	}
	fd = open(fl_name,O_RDONLY);
	lseek(fd,offset,SEEK_CUR);
	memset(&send_buf,0,sizeof(send_buf));
	status = read(fd,send_buf,chunk_size);
	#ifdef TRACE_DEBUG
	if(status != down_size)
		printf("Read error!\n");
	#endif
	send(conn_id,send_buf,sizeof(send_buf),0);
	close(fd);
}

/***************************************************************************************
Function 	: handle_peer_data
Description	: It is the data handling function on downloaded part. It parses the data, stores the
				data into file and requests for further data from available clients if they are
				ready for data transfer.
****************************************************************************************/
void handle_peer_data(int conn_id,char recv_buf[])
{
	int status;
	int fd;
	int index = 0;
	int n1;
	int n;
	char *final;
	char *str_tok;
	FILE *tmp;
	char out_fl_name[FILE_SIZE];
	char data[3][MAX_SIZE];
	
	memset(&data[0],0,sizeof(data[0]));
	memset(&data[1],0,sizeof(data[1]));
	memset(&data[2],0,sizeof(data[2]));
	if(!custom_check)
	{
	str_tok = strtok (recv_buf,":");
	if(strcmp(recv_buf,":")!=0)
	{
		while (str_tok != NULL)
		{
			strcpy(data[index],str_tok);
			str_tok = strtok(NULL, ":");
			index++;
		}
	if(strcmp(data[1],"size=") == 0)
	{
		req_download_size = atoi(data[2]);
		#ifdef TRACE_DEBUG
		printf("requested download size:%lu\n",req_download_size);
		#endif
		subs_download_req(req_download_size);
		custom_check = TRUE;
		return;
	}
	}
	}
	sprintf(out_fl_name,"out_%s",fl_name);
	#ifdef TRACE_DEBUG
	printf("handle_peer_data\n");
	#endif
	fd = open(out_fl_name,O_CREAT|O_WRONLY|O_APPEND);
	if(fd < 0)
	{
		#ifdef TRACE_DEBUG
		printf("Can't write to file\n");
		#endif
		exit(0);
	}

	status = write(fd,recv_buf,down_size);	
	#ifdef TRACE_DEBUG
	if(status != strlen(recv_buf))
		printf("write error!\n");
	#endif
	close(fd);
	#ifdef TRACE_DEBUG
	printf("peer_conn_id:%d,conn_id:%d\n",peer_sock_id[0],conn_id);
	#endif
	if(conn_id == peer_sock_id[0])
	{
		peer1_data_chunk->fin = FALSE;
	}
	if(conn_id == peer_sock_id[1])
	{
		peer2_data_chunk->fin = FALSE;
	}
	if(conn_id == peer_sock_id[2])
	{
		peer3_data_chunk->fin = FALSE;
	}
	if(conn_id == peer_sock_id[3])
	{
		peer4_data_chunk->fin = FALSE;
	}

	status = subs_download_req(req_download_size);
	if(status == -1)
	{
		down_begin = FALSE;
		custom_check = FALSE;
		printf("\n***********************Download complete************************\n");
		#ifdef TRACE_DEBUG
		stop = clock();
		used_time = (double)(stop-start);	
		printf("Transfer Time:%f\n",used_time);
		#endif
		printf("\n<CHART> -cmd to display downloaded data chart");
		#ifdef TRACE_DEBUG
		//display_downloaded_data();
		#endif
	}

}
