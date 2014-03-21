/***************************************************************************************
File 			: utility.c
Author 			: Amit Pratap Singh
Organization 	: University at Buffalo
Description		: This file contains the functions for both main calling functions mentioned
					in main and command interpretor file. This file implements the register,
					list,download and other additional function calls to support these
					functionalities.
 ****************************************************************************************/
#include "gprot.h"
/***************************************************************************************
Function 	: client_setup
Description	: It initializes the client connection parameters.
****************************************************************************************/
void client_setup(char ipstr[],int port)
{
	client_addr.sin_family = SOCK_FAMILY;
	client_addr.sin_addr.s_addr =inet_addr(ipstr);
	client_addr.sin_port = htons(port);
}
/***************************************************************************************
Function 	: server_setup
Description	: It initializes the server connection parameters.
****************************************************************************************/
void server_setup()
{
	server_addr.sin_family = SOCK_FAMILY;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	if(bind(server_sock_id,(struct sockaddr *)&server_addr,sizeof(server_addr)))
	{
		perror("Bind failed\n");
		exit(1);
	}
	listen(server_sock_id,MAX_CONN);
}
/***************************************************************************************
Function 	: iswhitespc
Description	: It checks for white spaces in the user input to verify the command.
****************************************************************************************/
bool iswhitespc(char input[])
{
	char *str;
	str = input;
	while(!isspace(*str))
		str++;
	if(str == input)
		return 1;
	else
		return 0;
}
/***************************************************************************************
Function 	: display_ip
Description	: It connects to google DNS server to get the own IP and displays it the user.
****************************************************************************************/
void display_ip()
{
	int sid;
	int status;
	struct sockaddr_in addr;
	struct sockaddr_in host_addr;
	memset(&addr,0,sizeof(addr));
	memset(&host_addr,0,sizeof(host_addr));
	socklen_t len = sizeof(host_addr);
	sid = socket(AF_INET,SOCK_STREAM,0);	
	if(sid < 0)
	{
		printf("My IP Command Fail\n");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(53);
	addr.sin_addr.s_addr =inet_addr("8.8.8.8");

	status = connect(sid,(struct sockaddr *)&addr,sizeof(addr));
	if(status < 0)
	{
		printf("My IP Command Fail:Connect\n");
		exit(1);
	}
	getsockname(sid,(struct sockaddr *)&host_addr,&len);
	strcpy(myip,inet_ntoa(host_addr.sin_addr));
	printf("Address:%s\n",inet_ntoa(host_addr.sin_addr));
	printf("Port:%d\n",port);
	close(sid);
}
/***************************************************************************************
Function 	: calculate_ip
Description	: It is helper function for display IP and calculates the IP to validate that
				the initiated connections are not self connections.
****************************************************************************************/
void calculate_ip()
{
	int sid;
	int status;
	struct sockaddr_in addr;
	struct sockaddr_in host_addr;
	memset(&addr,0,sizeof(addr));
	memset(&host_addr,0,sizeof(host_addr));
	socklen_t len = sizeof(host_addr);
	sid = socket(AF_INET,SOCK_STREAM,0);	
	if(sid < 0)
	{
		printf("My IP Command Fail\n");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(53);
	addr.sin_addr.s_addr =inet_addr("8.8.8.8");

	status = connect(sid,(struct sockaddr *)&addr,sizeof(addr));
	if(status < 0)
	{
		printf("My IP Command Fail:Connect\n");
		exit(1);
	}
	getsockname(sid,(struct sockaddr *)&host_addr,&len);
	strcpy(myip,inet_ntoa(host_addr.sin_addr));
	close(sid);
}
/***************************************************************************************
Function 	: reg_host_ip_list
Description	: This functions stores the IP list of registered clients for server.
				It maintains the array of structures to store the registered devices list
				so any number of devices can register wit the server.
****************************************************************************************/
void reg_host_ip_list(char input[],bool flag)
{
	int index = 0;
	char ip_str[2][HOST_SIZE];
	for(index=0;index<2;index++)
	{
		memset(&ip_str[index],0,HOST_SIZE);
	}
	index = 0;
	if(strcmp(input,"begin") == 0)
	{
		while(index<MAX_PEER)
		{
			memset((void *)&host_ip_list[index],0,sizeof(host_ip_list[index]));
			host_ip_list[index].port =  0;
			index++;
		}
		return;
	}
	{
		char *str_tok;
		str_tok = strtok (input,":");
		index = 0;
	
		while (str_tok != NULL)
		{
			strcpy(ip_str[index],str_tok);
			str_tok = strtok (NULL, ":");
			index++;
		}
	}
	index = 0;
	if(flag == 1)
	{
		//store data to structure
		while(index < MAX_PEER)
		{
			if(host_ip_list[index].port == 0)
				break;
			else
				index++;
		}
		strcpy(host_ip_list[index].ip_addr , ip_str[0]);
		host_ip_list[index].port = (int)atoi(ip_str[1]);
	}
}

/***************************************************************************************
Function 	: connect_dev
Description	: This functions is to connect to other peers. This connection can be used for 
			   data transfer.
Parameters	:1)IP Address -  of the device to be connected.
			 2)Port Number -  of the device to be connected.
Return Vals	: status (-1/1) for SUCCESS/FAILURE of connection.
Comments	:
****************************************************************************************/
int connect_dev(char ip_addr[],int port)
{
	struct sockaddr_in temp;
	int sock_id;
	
	memset(&temp,0,sizeof(temp));
	temp.sin_family = SOCK_FAMILY;
	temp.sin_addr.s_addr =inet_addr(ip_addr);
	temp.sin_port = htons(port);
	sock_id = socket(AF_INET,SOCK_STREAM,0);
#ifdef TRACE_DEBUG
	printf("sockid:%d\n",sock_id);
	printf("addr:%s\n",inet_ntoa(temp.sin_addr));
	printf("port:%d\n",(int)ntohs(temp.sin_port));
#endif
	calculate_ip();
	if(strcmp(myip,ip_addr) == 0)
	{
		printf("Self Connections not allowed!\n");
		return -1;
	}
	if(is_dev_registered(ip_addr) == FALSE)
	{
		//perror("Device not in list\n");
		//return -1;
	}
	if(is_dev_already_connected(ip_addr))
	{
		perror("Device is already connected!\n");
		return -1;
	} 
	if(connect(sock_id,(struct sockaddr *)&temp,sizeof(temp)))
	{
		perror("Connect failed\n");
		return -1;
	}
	else
		update_conn_dev_list(ip_addr,ntohs(temp.sin_port),sock_id,TRUE);

	FD_SET(sock_id,&master);
	if(sock_id > client_fd_max)
		client_fd_max = sock_id;
	return sock_id;
}
/***************************************************************************************
Function 	: is_dev_already_connected
Description	: This functions is to identify if device is already connected with the peer
			or not.
Parameters	:1)IP Address -  of the device to be connected.
Return Vals	:TRUE/FALSE
Comments	:
****************************************************************************************/
bool is_dev_already_connected(char ip_addr[])
{
	conn_peer_list_struct *temp = conn_list_first;
	while(temp)
	{
		if(strcmp(temp->ip_addr,ip_addr)==0)
			return TRUE;
	}
	return FALSE;
}

/***************************************************************************************
Function 	: update_conn_dev_list
Description	: This functions updates the peer device connection list.
				It also used the array to structures to store the client connections
				data.This data is used to identify the eligible hosts for downloading.
Parameters	:1)IP Address -  of the device to be connected.
			 2)Port Number -  of the device to be connected.
			 2)Conn Id -  Connection id of the connection to be updated.
			 2)Flag -  To indicate of device is need to be removed or added to list.
Return Val	: status (-1/1) for SUCCESS/FAILURE of connection.
Comments	:
****************************************************************************************/
int update_conn_dev_list(char ip_addr[],int lo_port,int id,bool flag)
{
	#ifdef TRACE_DEBUG
	printf("update_conn_dev_list is called1:\n");
	#endif
	if(flag == TRUE)
	{
		conn_peer_list_struct *new_node;
		struct sockaddr_in host_addr;
		if(ip_addr == NULL)
		{
			memset(&host_addr,0,sizeof(host_addr));
			socklen_t len = sizeof(host_addr);
			getpeername(id,(struct sockaddr *)&host_addr,&len);
			#ifdef TRACE_DEBUG
			printf("-->Address:%s\n",inet_ntoa(host_addr.sin_addr));
			printf("-->Name:%d\n",ntohs(host_addr.sin_port));
			#endif
		}

		new_node = malloc(sizeof(new_node));	
		memset(new_node,0,sizeof(new_node));
		if(ip_addr == NULL)
		{
			strcpy(new_node->ip_addr,inet_ntoa(host_addr.sin_addr) );
			new_node->port = ntohs(host_addr.sin_port);
		}
		else
		{
			strcpy(new_node->ip_addr,ip_addr);
			new_node->port = lo_port;
		}
		new_node->sock_id = id;
		new_node->next = NULL;
		if(!conn_list_current || !conn_list_first)
		{
			conn_list_current = conn_list_first = new_node;
			#ifdef TRACE_DEBUG
			printf("----\n");
			printf("conn_list_first--->port:%d\n",conn_list_first->port);
			printf("new_node--->port:%d\n",new_node->port);
			#endif
		}
		else
		{
			conn_list_current->next = new_node;
			conn_list_current = conn_list_current->next;
			if(conn_list_first->next == NULL)
				conn_list_first->next = conn_list_current;
		}
		peer_sock_id[cur_peer_sock_id] = id;
		cur_peer_sock_id++;
	}
	else
	{
		conn_peer_list_struct *temp = conn_list_first;
		conn_peer_list_struct *del;
		if(temp == NULL)
			return -1;
		if(temp->next == NULL)
		{
			if(temp->sock_id == id)
			{
				free(temp);
				temp = conn_list_first = NULL;
				return 1;
			}
			return -1;
		}
		while( (temp->next!= NULL) && (temp->next->sock_id != id) )
			temp = temp->next;
		if(temp->next == NULL)
		{
			printf("No device with this sock_id!\n");
			return;
		}
		del = temp->next;
		temp->next = temp->next->next;
		free(del);
		peer_sock_id[cur_peer_sock_id] = 0;
		cur_peer_sock_id--;
	}
	#ifdef TRACE_DEBUG
	printf("update_conn_dev_list is called2:\n");
	#endif
	return 1;
}
/***************************************************************************************
Function 	: update_accepted_conn_list
Description	: This functions updates the peer device connection list initiated from other side(peer).
				It also used the array to structures to store the client connections
				data.This data is used to identify the eligible hosts for downloading.
Parameters	:1)IP Address -  of the device to be connected.
			 2)Port Number -  of the device to be connected.
			 2)Conn Id -  Connection id of the connection to be updated.
			 2)Flag -  To indicate of device is need to be removed or added to list.
Return Val	: status (-1/1) for SUCCESS/FAILURE of connection.
Comments	:
****************************************************************************************/
int update_accepted_conn_list(char ip_addr[],int lo_port,int id,bool flag)
{
	#ifdef TRACE_DEBUG
	printf("update_accepted_conn_list is called1:\n");
	#endif
	if(flag == TRUE)
	{
		conn_peer_list_struct *new_node;
		struct sockaddr_in host_addr;
		socklen_t len = sizeof(host_addr);
		if(ip_addr == NULL)
		{
			memset(&host_addr,0,sizeof(host_addr));
			getpeername(id,(struct sockaddr *)&host_addr,&len);
			#ifdef TRACE_DEBUG
			printf("-->Address:%s\n",inet_ntoa(host_addr.sin_addr));
			printf("-->Name:%d\n",ntohs(host_addr.sin_port));
			#endif
		}

		new_node = malloc(sizeof(new_node));	
		memset(new_node,0,sizeof(new_node));
		if(ip_addr == NULL)
		{
			strcpy(new_node->ip_addr,inet_ntoa(host_addr.sin_addr) );
			new_node->port = ntohs(host_addr.sin_port);
		}
		else
		{
			strcpy(new_node->ip_addr,ip_addr);
			new_node->port = lo_port;
		}
		new_node->sock_id = id;
		new_node->next = NULL;
		if(!conn_list_current || !conn_list_first)
		{
			conn_list_current = conn_list_first = new_node;
			conn_list_first->next = NULL;
			#ifdef TRACE_DEBUG
			printf("----\n");
			printf("conn_list_first--->port:%d\n",conn_list_first->port);
			printf("new_node--->port:%d\n",new_node->port);
			#endif
		}
		else
		{
			#ifdef TRACE_DEBUG
			printf("update_conn_dev_accepted_list\n");
			#endif
			conn_list_current->next = new_node;
			conn_list_current = conn_list_current->next;
			if(conn_list_first->next == NULL)
			    conn_list_first->next = conn_list_current;
		}
	} //if flag = TRUE
	#ifdef TRACE_DEBUG
	printf("update_conn_dev_accepted_list is called2:\n");
	#endif
	return 1;
}

/***************************************************************************************
Function 	: connect_dev_to_serv
Description	: This functions is to allow client to register with server.
Parameters	:1)IP Address -  of the device to be connected.
			 2)Port Number -  of the device to be connected.
			 2)Flag -  To indicate of device is need to be removed or added to list.
Return Val	: status (-1/1) for SUCCESS/FAILURE of connection.
Comments	:
****************************************************************************************/
int connect_dev_to_serv(char ip_addr[],int port_no)
{
	char lo_port[10];
	cl_ser_conn_addr.sin_family = SOCK_FAMILY;
	cl_ser_conn_addr.sin_addr.s_addr =inet_addr(ip_addr);
	cl_ser_conn_addr.sin_port = htons(port_no);
	cl_server_sock_id = socket(AF_INET,SOCK_STREAM,0);
#ifdef TRACE_DEBUG
	printf("sockid:%d\n",cl_server_sock_id);
	printf("addr:%s\n",inet_ntoa(cl_ser_conn_addr.sin_addr));
	printf("port:%d\n",(int)ntohs(cl_ser_conn_addr.sin_port));
#endif
	calculate_ip();
	if(strcmp(myip,ip_addr) == 0)
	{
		printf("Self Connections not allowed!\n");
		cl_server_sock_id = 0;
		return -1;
	}
	if(connect(cl_server_sock_id,(struct sockaddr *)&cl_ser_conn_addr,sizeof(cl_ser_conn_addr)))
	{
		perror("Connect failed!\n");
		cl_server_sock_id = 0;
		return -1;
	}
	memset(&send_buf,0,MAX_SIZE);
	strcpy(send_buf,myip);
	sprintf(lo_port,"%d",port);
	strcat(send_buf,":");
	strcat(send_buf,lo_port);
	#ifdef TRACE_DEBUG
	printf("send_buf:%s\n",send_buf);
	#endif
	send(cl_server_sock_id,send_buf,MAX_SIZE,0);
	FD_SET(cl_server_sock_id,&master);
	if(cl_server_sock_id > client_fd_max)
		client_fd_max = cl_server_sock_id;
	return 1;
}
/***************************************************************************************
Function 	: print_reg_ip_list
Description	: This functions is display the registered device list to user.
Parameters	:
Return Val	: 
Comments	:
****************************************************************************************/
void print_reg_ip_list()
{
	int index = 0;
	struct hostent *he;
	struct in_addr ipv4addr;
	while( (index <= 10) && (host_ip_list[index].port != 0) )
	{
		inet_pton(AF_INET,host_ip_list[index].ip_addr, &ipv4addr);
		he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
		
		printf("%s\t", he->h_name);
		printf("%s\t",host_ip_list[index].ip_addr);
		printf("%d\n",host_ip_list[index].port);
		
		fflush(stdout);
		index++;
	}
}

/***************************************************************************************
Function 	: is_dev_registered
Description	: This functions is to identify if the device is already registered with the 
				server or not;
Parameters	:1) IP string of the client device to be verified
Return Val	: TRUE/FALSE
Comments	:
****************************************************************************************/
bool is_dev_registered(char ipstr[])
{
	int index;
	#ifdef TRACE_DEBUG
	for(index=0;index<4;index++)
		printf("ip_addr:%s\n",ipstr,host_ip_list[index].ip_addr);
	#endif
	index=0;
	while( (index<4) && (host_ip_list[index].port != 0))
	{
		#ifdef TRACE_DEBUG
		printf("is_dev_in_reg_host_ip_list\n");
		printf("ipstr:%s;ip_addr:%s\n",ipstr,host_ip_list[index].ip_addr);
		#endif
		if(!strcmp(ipstr,host_ip_list[index].ip_addr))
			return TRUE;
		else
			index++;
	}
	return FALSE;
}
/***************************************************************************************
Function 	: list_peer_devices
Description	: This functions is to display the list the connected peer devices
				with this client. The top item is the details of server  it 
				is connected with.
Parameters	:
Return Val	:
Comments	:
****************************************************************************************/
void list_peer_devices()
{
	int i=0;
	struct hostent *he;
	struct in_addr ipv4addr;
	conn_peer_list_struct *temp = conn_list_first;
	printf("\n******List of all peer devices****************\n");
	printf("*Host\t\t\tHost-ID\t\tHost-IP\t\tPort#\n");
	if(cl_server_sock_id)
	{
		struct sockaddr_in host_addr;
		char ipstr[MAX_SIZE];
		struct in_addr ipaddr;
		memset(&host_addr,0,sizeof(host_addr));
		socklen_t len = sizeof(host_addr);
		getpeername(cl_server_sock_id,(struct sockaddr *)&host_addr,&len);
		strcpy(ipstr,inet_ntoa(host_addr.sin_addr));
		inet_pton(AF_INET,ipstr, &ipaddr);
		he = gethostbyaddr(&ipaddr, sizeof(ipaddr), AF_INET);
		printf("%s\t",he->h_name);
		printf("%d\t",cl_server_sock_id);
		printf("%s\t",inet_ntoa(host_addr.sin_addr));
		printf("%d\n",ntohs(host_addr.sin_port));
	}
	if(temp == NULL)
		printf("No Peer Device currently connected!\n");
	while(temp!= NULL)
	{
		inet_pton(AF_INET,temp->ip_addr, &ipv4addr);
		he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
		printf("%s\t", he->h_name);
		printf("%d\t",temp->sock_id);
		printf("%s\t",temp->ip_addr);
		printf("%d\n",temp->port);
		temp = temp->next;
	}
	printf("*************************************************\n");
}
/***************************************************************************************
Function 	: terminate_conn
Description	: This functions is to terminate the connection with peer devices
Parameters	:1) The connection id of the client device to be removed
Return Val	:
Comments	:
****************************************************************************************/
void terminate_conn(char conn_id[])
{
	int conn = atoi(conn_id);
	#ifdef TRACE_DEBUG
	printf("terminate_conn:%d\n",conn);
	#endif
	update_conn_dev_list(NULL,0,conn,FALSE);
	printf("The device with connection id (%s) is removed!\n",conn_id);
}
/***************************************************************************************
Function 	: quit_all_conn
Description	: It is remove all the client/server connections and closes the program shell
				and comes to main terminal shell.
Parameters	:
Return Val	:
Comments	:
****************************************************************************************/
void quit_all_conn()
{
	int i=0;
	#ifdef TRACE_DEBUG
	printf("quit_all_conn1\n");
	#endif
	if(mode == SERVER)
	{
		for(i=0;i<cur_server_conn_id;i++)
			close(server_conn_id[i]);
		close(server_sock_id);
		printf("The registered Client connections is  broken!\n");
	}
	else if(mode == CLIENT)
	{
		for(i=0;i<cur_peer_conn_id;i++)
		{
			close(peer_conn_id[i]);
		}
		for(i=0;i<cur_peer_sock_id;i++)
		{
			close(peer_sock_id[i]);
		}
		close(client_sock_id);
		printf("The connected with peer devices and server is broken!\n");
	}
	free_all_mem();
}

/***************************************************************************************
Function 	: download_file
Description	: It initiated the file download from the connected peer devices.
				Each download uses the separate TCP connection for downloading.
Parameters	:1)Sock_id - Socket id of the device which initiated the download.
			 2)(input1)File Name - The file name which need to be downloaded.
			 3)(input2)Chunk SIze - The chunk size for packet transfer.
Return Val	:
Comments	:
****************************************************************************************/
void download_file(int sock_id,char input1[],char input2[])
{
	int i=0;
	cur_download_size = 0;
	free_all_download_mem();

	peer1_data_chunk = NULL;
	peer2_data_chunk = NULL;
	peer3_data_chunk = NULL;
	peer4_data_chunk = NULL;
	peer1_data_chunk_first =NULL;
	peer2_data_chunk_first = NULL;
	peer3_data_chunk_first = NULL;
	peer4_data_chunk_first = NULL;
	#ifdef TRACE_DEBUG
	start = clock();
	#endif
	download_chunk_size = atoi(input2);
	memset(fl_name,0,sizeof(fl_name));
	strcpy(fl_name,input1);
	memset(&send_buf,0,sizeof(send_buf));
	strcpy(send_buf,fl_name);
	strcat(send_buf,":size?");
	strcat(send_buf,":0");
	send(sock_id,send_buf,sizeof(send_buf),0);
}

/***************************************************************************************
Function 	: subs_download_req
Description	: It distributes the download request to multiple peer depending upon their
				availability.
Parameters	:1)size - Requested Download Size
Return Val	: status - SUCCESS/FAILURE
Comments	:
****************************************************************************************/
int subs_download_req(long size)
{
    int i=0;
    int n = 0;
	int status=0;
    char d_size[10];
    char offset[10];
	req_download_size = size;
	#ifdef TRACE_DEBUG
	printf("sub_download_req:%lu\n",size);
	#endif
   	if( (peer_sock_id[0]!=0)&& ( !peer1_data_chunk || (peer1_data_chunk->fin == FALSE)) )
	{
		status = peer1_download_chunk();
	}
	//peer2
	if( (peer_sock_id[1]!=0)&& (!peer2_data_chunk || (peer2_data_chunk->fin == FALSE)) )
	{

		status = peer2_download_chunk();
	}
	//peer3
	if( (peer_sock_id[2]!=0)&& (!peer3_data_chunk || (peer3_data_chunk->fin == FALSE) ) )
	{
		status = peer3_download_chunk();
	}
	//peer4
	if( (peer_sock_id[3]!=0)&& (!peer4_data_chunk || (peer4_data_chunk->fin == FALSE)) )
	{
		status = peer4_download_chunk();
	}
	return status;
}
/***************************************************************************************
Function 	: display_downloaded_data
Description	: Display Chunk details of downloaded file.
				It displays which chunk is download from which peer.
Parameters	:
Return Val	:
Comments	:
****************************************************************************************/
void display_downloaded_data()
{
	int i=0,j=0;
	int curr_offset=0;
	i =0;
	j=0;
	int peer_list[4]={0};
	conn_peer_list_struct *temp = conn_list_first;
	down_data_chunk *temp_data = peer1_data_chunk_first;
	printf("*************Downloaded Data Chart****************\n");
	printf("*Host\tHost-Id\tFile Chunks Downloaded\n");
	if(peer_sock_id[0])
	{
		while(temp && (temp->sock_id != peer_sock_id[0]))
			temp = temp->next;
		printf("%s\t",temp->ip_addr);
		printf("%d\t",temp->sock_id);
		while(temp_data)
		{
			printf("%d-",temp_data->start);
			printf("%d;",temp_data->end);
			temp_data = temp_data->next;
		}
	}
	printf("\n");
	temp = conn_list_first;
	temp_data = peer1_data_chunk_first;
	if(peer_sock_id[1])
	{
		while(temp && (temp->sock_id != peer_sock_id[1]))
			temp = temp->next;
		if(temp)
		{
			printf("%s\t",temp->ip_addr);
			printf("%d\t",temp->sock_id);
			while(temp_data)
			{
				printf("%d-",temp_data->start);
				printf("%d;",temp_data->end);
				temp_data = temp_data->next;
			}
		}
	}

	printf("\n");
	temp = conn_list_first;
	temp_data = peer1_data_chunk_first;
	if(peer_list[2])
	{
		while(temp && (temp->sock_id != peer_list[2]))
			temp = temp->next;	
		printf("%s\t",temp->ip_addr);
		printf("%d\t",temp->sock_id);
		while(temp_data)
		{
			printf("%d-",temp_data->start);
			printf("%d;",temp_data->end);
			temp_data = temp_data->next;
		}
	}
	printf("\n");
	temp = conn_list_first;
	temp_data = peer1_data_chunk_first;
	if(peer_list[3])
	{
		while(temp && (temp->sock_id != peer_list[3]))
			temp = temp->next;
		printf("%s\t",temp->ip_addr);
		printf("%d\t",temp->sock_id);
		while(temp_data)
		{
			printf("%d-",temp_data->start);
			printf("%d;",temp_data->end);
			temp_data =temp_data->next;
		}
	}
	printf("****************************************************\n");
}
int peer1_download_chunk()
{
    int i=0;
    int n = 0;
	int status=0;
    char d_size[10];
    char offset[10];
	down_data_chunk *new_node;
	new_node = (down_data_chunk *)malloc(sizeof(down_data_chunk));
	new_node->start = cur_download_size;
	new_node->fin = TRUE;
	if(cur_download_size >= req_download_size)
		return -1;
	if(cur_download_size+download_chunk_size > req_download_size)
		down_size = req_download_size - cur_download_size;
	else
		down_size = download_chunk_size;
	printf("peer1 download req,offset:%lu\n",cur_download_size);
	new_node->end = cur_download_size + down_size;
	sprintf(d_size,"%lu",down_size);
	sprintf(offset,"%lu",cur_download_size);
	cur_download_size =  cur_download_size + down_size;
	memset(send_buf,0,sizeof(send_buf));
	strcpy(send_buf,fl_name);
	strcat(send_buf,":");
	strcat(send_buf,d_size);
	strcat(send_buf,":");
	strcat(send_buf,offset);
	new_node->next = NULL;
	if(!peer1_data_chunk && !peer1_data_chunk_first)
	{
		peer1_data_chunk = peer1_data_chunk_first = new_node;
		peer1_data_chunk->next = NULL;
		peer1_data_chunk_first->next = NULL;
	}
	else if(peer1_data_chunk_first->next == NULL)
	{
		peer1_data_chunk->next = new_node;
		peer1_data_chunk = peer1_data_chunk->next;
		peer1_data_chunk_first->next = peer1_data_chunk;
	}
	else
	{
		peer1_data_chunk->next = new_node;
		peer1_data_chunk = peer1_data_chunk->next;
	}
	n = send(peer_sock_id[0],send_buf,MAX_SIZE,0);
	return 1;
}

int peer2_download_chunk()
{
    int i=0;
    int n = 0;
	int status=0;
    char d_size[10];
    char offset[10];
	down_data_chunk *new_node;
	new_node = (down_data_chunk *)malloc(sizeof(down_data_chunk));
	new_node->start = cur_download_size;
	new_node->fin = TRUE;
	if(cur_download_size >= req_download_size)
			return -1;
	if(cur_download_size+download_chunk_size > req_download_size)
		down_size = req_download_size - cur_download_size;
	else
		down_size = download_chunk_size;
	new_node->end = cur_download_size + down_size;
	sprintf(d_size,"%lu",down_size);
	sprintf(offset,"%lu",cur_download_size);
	printf("peer2 download req,%lu\n",cur_download_size);
	cur_download_size =  cur_download_size + down_size;
	memset(send_buf,0,sizeof(send_buf));
	strcpy(send_buf,fl_name);
	strcat(send_buf,":");
	strcat(send_buf,d_size);
	strcat(send_buf,":");
	strcat(send_buf,offset);
	new_node->next = NULL;
	if(!peer2_data_chunk && !peer2_data_chunk_first)
	{
		peer2_data_chunk = peer2_data_chunk_first = new_node;
		peer2_data_chunk->next = NULL;
		peer2_data_chunk_first->next = NULL;
	}
	else if(peer2_data_chunk_first->next == NULL)
	{
		peer2_data_chunk->next = new_node;
		peer2_data_chunk = peer2_data_chunk->next;
		peer2_data_chunk_first->next = peer2_data_chunk;
	}
	else
	{
		peer2_data_chunk->next = new_node;
		peer2_data_chunk = peer2_data_chunk->next;
	}
	send(peer_sock_id[1],send_buf,MAX_SIZE,0);
	return 1;
}
int peer3_download_chunk()
{
    int i=0;
    int n = 0;
	int status=0;
    char d_size[10];
    char offset[10];
	down_data_chunk *new_node;
	new_node = (down_data_chunk *)malloc(sizeof(down_data_chunk));
	new_node->start = cur_download_size;
	new_node->fin = TRUE;
	if(cur_download_size >= req_download_size)
		return -1;
	if(cur_download_size+download_chunk_size > req_download_size)
		down_size = req_download_size - cur_download_size;
	else
		down_size = download_chunk_size;
	new_node->end = cur_download_size + down_size;
	sprintf(d_size,"%lu",cur_download_size);
	sprintf(offset,"%lu",cur_download_size);
	cur_download_size =  cur_download_size + down_size;
	memset(send_buf,0,sizeof(send_buf));
	strcpy(send_buf,fl_name);
	strcat(send_buf,":");
	strcat(send_buf,d_size);
	strcat(send_buf,":");
	strcat(send_buf,offset);
	new_node->next = NULL;
	if(!peer3_data_chunk && !peer3_data_chunk_first)
	{
		peer3_data_chunk = peer3_data_chunk_first = new_node;
		peer3_data_chunk->next = NULL;
		peer3_data_chunk_first->next = NULL;
	}
	else if(peer3_data_chunk_first->next == NULL)
	{
		peer3_data_chunk->next = new_node;
		peer3_data_chunk = peer3_data_chunk->next;
		peer3_data_chunk_first->next = peer3_data_chunk;
	}
	else
	{
		peer3_data_chunk->next = new_node;
		peer3_data_chunk = peer3_data_chunk->next;
	}
	send(peer_sock_id[2],send_buf,sizeof(send_buf),0);
	return 1;
}

int peer4_download_chunk()
{
    int i=0;
    int n = 0;
	int status=0;
    char d_size[10];
    char offset[10];
	down_data_chunk *new_node;
	new_node = (down_data_chunk *)malloc(sizeof(down_data_chunk));
	new_node->start = cur_download_size;
	new_node->fin = TRUE;
	if(cur_download_size >= req_download_size)
		return -1;
	if(cur_download_size+download_chunk_size > req_download_size)
		down_size = req_download_size - cur_download_size;
	else
		down_size = download_chunk_size;
	peer4_data_chunk[count4].end = cur_download_size + down_size;
	sprintf(d_size,"%lu",cur_download_size);
	sprintf(offset,"%lu",cur_download_size);
	cur_download_size =  cur_download_size + down_size;
	memset(send_buf,0,sizeof(send_buf));
	strcpy(send_buf,fl_name);
	strcat(send_buf,":");
	strcat(send_buf,d_size);
	strcat(send_buf,":");
	strcat(send_buf,offset);
	new_node->next = NULL;
	if(!peer4_data_chunk && !peer4_data_chunk_first)
	{
		peer4_data_chunk = peer4_data_chunk_first = new_node;
		peer4_data_chunk->next = NULL;
		peer4_data_chunk_first->next = NULL;
	}
	else if(peer4_data_chunk_first->next == NULL)
	{
		peer4_data_chunk->next = new_node;
		peer4_data_chunk = peer4_data_chunk->next;
		peer4_data_chunk_first->next = peer4_data_chunk;
	}
	else
	{
		peer4_data_chunk->next = new_node;
		peer4_data_chunk = peer4_data_chunk->next;
	}
	send(peer_sock_id[3],send_buf,sizeof(send_buf),0);
	return 1;
}
void free_all_download_mem()
{
	int i= 0;
	down_data_chunk *tmp = peer1_data_chunk_first;
	down_data_chunk *del = NULL;
	while(tmp)
	{
		del = tmp;
		tmp = tmp->next;
		free(del);
	}
	tmp = peer2_data_chunk_first;
	del = NULL;
	while(tmp)
	{
		del = tmp;
		tmp = tmp->next;
		free(del);
	}
	tmp = peer3_data_chunk_first;
	del = NULL;
	while(tmp)
	{
		del = tmp;
		tmp = tmp->next;
		free(del);
	}
	tmp = peer4_data_chunk_first;
	del = NULL;
	while(tmp)
	{
		del = tmp;
		tmp = tmp->next;
		free(del);
	}
}
void free_all_mem()
{
	int i=0;
	free_all_download_mem();
	//free comd line arg size
	for(i=0;i<MAX_CMD_LN_VAR;i++)
	{
		if(cmd_arg_var[i])
			free(cmd_arg_var[i]);
	}
	//free peer ip list data
	conn_peer_list_struct *temp1 = conn_list_first;
	conn_peer_list_struct *del1 = NULL;
	while(temp1)
	{
		del1 = temp1;
		temp1 = temp1->next;
		free(del1);
	}
	
	//free registered devices ip list
	ip_list_struct *temp2 = ip_list_first;
	ip_list_struct *del2 = NULL;
	while(temp2)
	{
		del2 = temp2;
		temp2 = temp2->next;
		free(del2);
	}
	printf("All allocated memory allocated!\n");
}

/***************************************************************************************
Function 	: display_creator_info
Description	: Display Author Data
Parameters	:
Return Val	:
Comments	:
****************************************************************************************/
void display_creator_info()
{
	printf("/***************************************************************************************\n");
	printf("*Name\t:\tAmit Pratap Singh\n");
	printf("*Email ID\t:\tamitprat@buffalo.edu\n");
	printf("*UBIT Name\t:\tAmit Pratap Singh\n");
	printf("****************************************************************************************/\n");
}
/***************************************************************************************
Function 	: display_help
Description	: Display Command uses guidelines.
Parameters	:
Return Val	:
Comments	:
****************************************************************************************/
void display_help()
{
	printf("/***************************************************************************************\n");
	printf("*cmd : HELP : Help content for available cmd interface\n");
	printf("*cmd : CREATOR : Displays the creator name,emailid\n");
	printf("*cmd : MYIP : Displays its own ip address\n");
	printf("*cmd : MYPORT : Displays its own port address\n");
	printf("*cmd : QUIT : Back to main terminal window(terminate all conn)\n");
	printf("*cmd : TERMINATE : Terminate a single connection\n");
	if(mode == SERVER)
	{
	}
	else
	{
		printf("*cmd : REGISTER : Register Client with Server\n");
		printf("*cmd : CONNECT : Connect Client with other peers\n");
		printf("*cmd : LIST : List all connected devices\n");
		printf("*cmd : DOWNLOAD : File Download from peers\n");
	}
	printf("****************************************************************************************/\n");
}
