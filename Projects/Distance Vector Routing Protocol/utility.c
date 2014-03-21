/***************************************************************************************
File 	: utility.c
Desc	: This file implements the supporting the functions for both main file and algorithm 
			file also. It doesn't have actual functional implementation but it has supporting 
			functions such as topology file reading, printing routing table to STDOUT , execting
			user command, verify user input.
 ****************************************************************************************/

#include"gprot.h"

/***************************************************************************************
Function 	: read_topology_file
Desc		: It reads the data from topology file and fills the topology structure for further
				reference. It opens file in read only mode so no modifications and done to 
				existing topology file.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void read_topology_file()
{
	int status;
	int u,v;
	char *read_buffer;
	read_buffer = (char *)malloc(MAX_BUF_SIZE);
	FILE *fp;
	fp = fopen(topology_file_name,"r");
	fscanf(fp,"%hu\n%hu\n",&topology.num_servers,&topology.num_peers);
	for(u=0;u<topology.num_servers;u++)
	{
		int id,port;
		char ipstr[32];
		fscanf(fp,"\n%d %s %d",&id,ipstr,&port);
		if(u == 0)
		{
			topology.server_id	= id;
			strcpy(topology.server_addr,ipstr);
			topology.server_port = port;
		}
		topology.peer_detail[u].peer_id = id;
		topology.peer_detail[u].port = port;
		topology.peer_detail[u].cost = MY_MAX;
		strcpy(topology.peer_detail[u].peer_addr,ipstr);
	}
	topology.peer_detail[0].cost = 0;
	for(v=1;v<=topology.num_peers;v++)
	{
		int from_node;
		int to_node,cost;
		fscanf(fp,"\n%d %d %d",&from_node,&to_node,&cost);
		
		int k =1;
		for(k;k<topology.num_servers;k++)
		{
			if(topology.peer_detail[k].peer_id == to_node)
				break;
		}
		
		topology.peer_detail[k].cost = cost;
	}
	fclose(fp);
}


/***************************************************************************************
Function 	: make_send_packet
Desc		: It fills the packet from topology structure and link cost and the it packs
				the structure to byte format and sends to all neighbour nodes.
Parameters	: void
Return Val	: void
Comments	: 
Reference : 3.2.4	Structure Name : SENDER_PACKET_STRUCT (Under Section Data Structures )
 ****************************************************************************************/
void make_send_packet()
{
	int i=0;
	int j = i+1;
	memset(&sender_packet,0,sizeof(sender_packet));
	sender_packet.num_update_fields = topology.num_peers;
	sender_packet.server_port = topology.server_port;
	memcpy(sender_packet.server_ip ,topology.server_addr,HOST_SIZE);
	for(i=0;i<topology.num_servers;i++)
	{
		j = i+1;
		if(topology.peer_detail[j].cost != MY_MAX)
		{
			sender_packet.peer_detail[i].peer_id = topology.peer_detail[j].peer_id;
			sender_packet.peer_detail[i].port = topology.peer_detail[j].port;
			if(link_cost[j].cost != MY_MAX)
				sender_packet.peer_detail[i].cost = link_cost[j].cost;
			else
				sender_packet.peer_detail[i].cost = topology.peer_detail[j].cost;
			memcpy(sender_packet.peer_detail[i].peer_addr , topology.peer_detail[j].peer_addr,HOST_SIZE);
		}
	}
}


/***************************************************************************************
Function 	: print_updated_link_cost
Desc		: It just prints the link cost after receiving update from each neighbour.
Parameters	: integer  - server_id
Return Val	: void
Comments	: 
 ****************************************************************************************/
void print_updated_link_cost(int server_id)
{
	int i = 0;
	if(server_id == -1)
		return;
	printf("\n Updates Received from Server :%d",server_id);
	link_cost_struct link_cost_dup[num_servers];
	for(i=0;i<num_servers;i++)
		memset(&link_cost_dup[i],0,sizeof(link_cost_struct));
	size_t size = sizeof(link_cost_struct);
	for(i=0;i<num_servers;i++)
		memcpy(&link_cost_dup[i],&link_cost[i],size);
	qsort (link_cost_dup, num_servers, sizeof(link_cost_struct), cmp);
	for(i=1;i<num_servers;i++)
	{
		printf("\n %hu -(%hu)-> %hu: %hu" ,link_cost_dup[i].from_node,link_cost_dup[i].via_node,link_cost_dup[i].to_node ,link_cost_dup[i].cost);
	}
}


/***************************************************************************************
Function 	: update_peer_count_missing_update
Desc		: It is supporting function to handle the missing peer update from neighbour
				if it goes down.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void update_peer_count_missing_update()
{
	int i=0;
	for(i=0;i<num_servers;i++)
	{
		count_peer_missing_update[topology.peer_detail[i].peer_id]++;
	}
}


/***************************************************************************************
Function 	: verify_peer_count_missing_update
Desc		: It actually performs the checks if the missing count has gone more than 3 and 
			then stops taking any input or sending any data to this host. Also the link cost 
			for this host is set to INFINITY(999).
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void verify_peer_count_missing_update()
{
	int i=0;
	for(i=0;i<num_servers;i++)
	{
		if( (count_peer_missing_update[topology.peer_detail[i].peer_id] >= 3) && 
			(!identify_missing_update_node[topology.peer_detail[i].peer_id]) &&
			(!is_server_link_disabled(topology.peer_detail[i].peer_id)) &&
			(
				(topology.peer_detail[i].cost != MY_MAX) &&
				(topology.peer_detail[i].cost != 0 ) 
			)
		)
		{
			printf("\nMissing 3 consecutive updates from peer : %d",topology.peer_detail[i].peer_id);
			printf("\nSetting the link cost to INF");
			//printf("\n Peer : %d  is added to disabled list",topology.peer_detail[i].peer_id);
			identify_missing_update_node[topology.peer_detail[i].peer_id] = TRUE;
			disable_server_link(topology.peer_detail[i].peer_id,FALSE);
		}
	}
}

/***************************************************************************************
Function 	: validate_user_cmd_input
Desc		: It validates the user command for its syntax and return TRUE/FALSE.
Parameters	: character pointer array(input)
Return Val	: boolean
Comments	: 
 ****************************************************************************************/
bool validate_user_cmd_input(char *input[])
{
	if(!strcmp(cmd_arg_var[0],"UPDATE"))
	{
		if(cmd_arg_var[1] && cmd_arg_var[2] && cmd_arg_var[3])
			return TRUE;
		else
		{
			printf("\nUPDATE command format error!");
			return FALSE;
		}
	}
	else if(!strcmp(cmd_arg_var[0],"STEP"))
		return TRUE;
	else if(!strcmp(cmd_arg_var[0],"PACKETS"))
		return TRUE;
	else if(!strcmp(cmd_arg_var[0],"DISPLAY"))
		return TRUE;
	else if(!strcmp(cmd_arg_var[0],"DISABLE"))
	{
		if(cmd_arg_var[1])
			return TRUE;
		else
		{
			printf("\nUPDATE command format error!");
			return FALSE;
		}
	}
	else if(!strcmp(cmd_arg_var[0],"CRASH"))
		return TRUE;
	else
	{
		printf("Not a valid TCP Command! :%s\n",cmd_arg_var[0]);
		return FALSE;
	}
}

/***************************************************************************************
Function 	: execute_user_cmd
Desc		: It calls the subroutine for command execution based on command string comparison.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void execute_user_cmd()
{
	if(!strcmp(cmd_arg_var[0],"UPDATE"))
	{
		int cost;
		bool status;
		if( (strcmp("INF",cmd_arg_var[3]) ==0 ) ||
			((strcmp("INFINITY",cmd_arg_var[3]) ==0 )) )
			cost = 999;
		else
			cost = atoi(cmd_arg_var[3]);
		status = update_link_cost(atoi(cmd_arg_var[1]),atoi(cmd_arg_var[2]),cost);
		status = update_link_cost(atoi(cmd_arg_var[2]),atoi(cmd_arg_var[1]),cost);
		remove_disabled_node(atoi(cmd_arg_var[2]));
		if(status == TRUE)
			printf("\n Update SUCESS");
		
	}
	else if(!strcmp(cmd_arg_var[0],"STEP"))
	{
		bool status;
		tv.tv_sec = periodic_update_time;
		status =send_routing_update();
		if(status == TRUE)
			printf("\nStep SUCESS");
	}
	else if(!strcmp(cmd_arg_var[0],"PACKETS"))
	{
		display_packets_count();
	}
	else if(!strcmp(cmd_arg_var[0],"DISABLE"))
	{
		bool status;
		status = disable_server_link(atoi(cmd_arg_var[1]),TRUE);
		if(status == TRUE)
			printf("\nDisable SUCESS");
	}
	else if(!strcmp(cmd_arg_var[0],"CRASH"))
	{
		simulate_server_crash();
	}
	else if(!strcmp(cmd_arg_var[0],"DISPLAY"))
	{
		display_routing_table();
	}
}

/***************************************************************************************
Function 	: remove_disabled_node
Desc		: It removes the disabled node from disabled  list.
Parameters	: Integer - to_node
Return Val	: void
Comments	: 
 ****************************************************************************************/
void remove_disabled_node(int to_node)
{
	int i=0;
	if(is_server_link_disabled(to_node) == TRUE)
	{
		while(disabled_server_ids[i] != to_node && i<num_servers)
			i++;
		disabled_server_ids[i] = 0;
		while(i+1<num_servers)
		{
			disabled_server_ids[i] = disabled_server_ids[i+1];
			i++;
		}
	}
}

/***************************************************************************************
Function 	: is_server_link_disabled
Desc		: It is the utility function to check if server is added to disabled list or not.
Parameters	: integer - Server_id
Return Val	: bool
Comments	: 
 ****************************************************************************************/
bool is_server_link_disabled(int server_id)
{
	int i=0;
	while(disabled_server_ids[i] && i<num_servers)
	{
		if( (disabled_server_ids[i] == server_id)/*  &&
			(routing_table[i][topology.server_id] == MY_MAX)*/
			)
			return TRUE;
		i++;
	}
	return FALSE;
}


/***************************************************************************************
Function 	: freeze_system
Desc		: It handles the signals and closes the input port. 
			Note - i didn't handle SIGTERM and SIGKILL command because it makes process
			difficult to kill.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void freeze_system()
{
	int i=0;
	int fd_1,fd_2,fd_3;
	struct rlimit limit;
	struct sigaction sig_hdlr;
	umask(0);
	if(getrlimit(RLIMIT_NOFILE,&limit) < 0)
		printf("\n Can't get file limit!");
	sig_hdlr.sa_handler = SIG_IGN;
	sigemptyset(&sig_hdlr.sa_mask);
	sig_hdlr.sa_flags = 0;
	if(sigaction(SIGHUP,&sig_hdlr,NULL) < 0 )
		printf("\n Error ignoring SIGHUP!");
	FD_CLR(STDIN,&master);
	periodic_update_time = MY_MAX;
}


/***************************************************************************************
Function 	: cmp
Desc		: It is  utility function for quick sort to sort elements.
Parameters	: void pointer p1 & p2
Return Val	: int
Comments	: 
 ****************************************************************************************/
int cmp(const void *p1, const void *p2)
{
        int y1 = (( link_cost_struct *)p1)->to_node;
        int y2 = ((link_cost_struct *)p2)->to_node;

        if (y1 < y2)
            return -1;
        else if (y1 > y2)
            return 1;
}


/***************************************************************************************
Function 	: min
Desc		: It gives the minimum cost for a 'to node' via all possible 'via nodes'.
				It simply compares and gives the minimum.
Parameters	: integer - next hop id
			  cost table (row of routing table)
Return Val	: unsigned short
Comments	: 
 ****************************************************************************************/
unsigned short min(int *next_hop,int cost[num_servers])
{
	int i=0;
	unsigned short min = cost[0];
	for(i=1;i<num_servers;i++)
		if(cost[i] < min)
		{
			min = cost[i];
			*next_hop = i;
		}
			
	return min;
}


/***************************************************************************************
Function 	: iswhitespc
Desc		: It simply skips the white spaces in input.
Parameters	: character array
Return Val	: bool
Comments	: 
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

void print_routing_table()
{
	int l,m;
	printf("\nRouting table\n");
	for(l=1;l<num_servers;l++)
	{
		for(m=1;m<num_servers;m++)
			printf("\t%d",routing_table[l][m]);
		printf("\n");
	}
}
