/***************************************************************************************
File 	: algorithm.c
Desc	: This file implements the working distance vector algorithm.
			It supports the cost updation, routing table display, handling the broken link etc.
			For this, it maintains the routing table matrix of n*v to store the details about 
			cost by 'via node' like cost 1-->2 via 1 or via 3. It takes the row minimum of DV 
			matrix to calculate the minimum cost for a to node by some via node.
 ****************************************************************************************/

#include"gprot.h"

/***************************************************************************************
Function 	: update_topology_link_cost
Desc		: It takes the input from neighbour server and then it adds the cost of the
			  possible nodes the via that node to routing table. Then its fills the link_cost
			  structure which holds the final minimum cost link.
Parameters	: character pointer  - Receive buffer(buff)
Return Val	: integer
Comments	:Reference : 3.2.4	Structure Name : SENDER_PACKET_STRUCT (Under Section Data Structures )
	         Reference : 3.2.1	Structure Name : LINK_COST_STRUCT (flat link cost list for maintaining cost to all servers) ((Under Section Data Structures ))
		  Reference : 3.2	 Arrays - Routing_table[MAX_NUM][MAX_NUM]((Under Section Data Structures )) 
 ****************************************************************************************/
int update_topology_link_cost(char *buff)
{
	int i=0;
	int update_node = 0;
	int prev_via_node = 0;
	int curr_cost=0,prev_cost=0,via_node_cost = 0;
	int source_node = 1;
	int to_node , via_node;
	int num_update_fields;
	sender_packet_struct recv_data;
	memset(&recv_data,0,sizeof(recv_data));
	memcpy(&recv_data,buff,sizeof(sender_packet_struct));
	total_num_of_diaplay_packets_recieved++; //packets command
	num_update_fields = recv_data.num_update_fields;
	for(i=0;i<num_servers;i++)
	{
		if( (strcmp(topology.peer_detail[i].peer_addr , recv_data.server_ip) ==0 ) && 
			(topology.peer_detail[i].port==recv_data.server_port) )
		{
			via_node = topology.peer_detail[i].peer_id;
			break;
		}
	}
	if(is_server_link_disabled(via_node))
			return -1;

	if(count_peer_missing_update[via_node] > 0)
	{
		identify_missing_update_node[via_node] = FALSE;
		count_peer_missing_update[via_node] = 0;
	}
	int j=0;
	for(i=0;i<=num_update_fields;i++)
	{
		int next_hop=via_node;
		update_node = 0;
		to_node = recv_data.peer_detail[i].peer_id;
		curr_cost = recv_data.peer_detail[i].cost;
		prev_cost = via_node_cost = 0;
		
		//if(is_server_link_disabled(to_node))
			//continue;
			
		for(j=1;j<num_servers;j++)
		{
			if(link_cost[j].to_node == to_node)
			{
				prev_cost = link_cost[j].cost;
				update_node = j;
				prev_via_node = link_cost[j].from_node;
			}
			if(link_cost[j].to_node == via_node)
			{
				via_node_cost = link_cost[j].cost;
			}
			if(prev_cost && via_node_cost)
				break;
		}
		routing_table[to_node][via_node] = curr_cost + via_node_cost;
		link_cost[update_node].cost = min(&next_hop,routing_table[to_node]);
		link_cost[update_node].via_node = next_hop;
	}
	return via_node;
}

/***************************************************************************************
Function 	: update_link_cost
Desc		: It updates the link cost when update command is executed the user.It takes the
			  from node, to node and cost for cost updation.
Parameters	: Integer - Input1(to_node)
			  Integer - Input2(from_node)
			  Integer - Input3(Cost)
Return Val	: boolean
Comments	: 
 ****************************************************************************************/
bool update_link_cost(int input1,int input2,int input3)
{
	int i = 0;
	unsigned short from_node = (unsigned short)input1;
	unsigned short to_node = (unsigned short)input2;
	unsigned short cost = (unsigned short)input3;
	
	for(i=0;i<num_servers;i++)
	{
		if( (link_cost[i].to_node == to_node) &&
			(link_cost[i].from_node == from_node) )
		{
			link_cost[i].cost = cost;
			break;
		}
	}
	//routing_table[to_node][from_node] = cost;
	routing_table[from_node][to_node] = cost;
	return TRUE;
}

/***************************************************************************************
Function 	: init_cost_table
Desc		: It initializes the link_cost structure and routing table(matrix) for the
				initial data set read from topology file.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void init_cost_table()
{
	int i=0;
	int l,m;
	unsigned short source_node = topology.server_id;
	link_cost[0].via_node = source_node;
	for(l=0;l<num_servers;l++)
	{
		for(m=0;m<num_servers;m++)
			routing_table[l][m] = MY_MAX;
	}
	int k=0;
	int j=0;
	for(i=0;i<num_servers;i++)
	{
		link_cost[i].to_node = topology.peer_detail[i].peer_id;
		link_cost[i].from_node = source_node;
		link_cost[i].via_node = source_node;
		if(topology.peer_detail[i].cost != MY_MAX)
			link_cost[i].cost = topology.peer_detail[i].cost;
		else
			link_cost[i].cost = MY_MAX;
		routing_table[link_cost[i].to_node][link_cost[i].from_node] = link_cost[i].cost;
	}
}

/***************************************************************************************
Function 	: disable_server_link
Desc		: It is the subroutine for "DISABLE' user command. It adds the disabled 
				server id to disabled list and then further checks if a server_id is disabled
				or not while sending and receiving data. It also sets its cost to infinity.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
bool disable_server_link(int link_id,bool disable)
{
	int i=-1;
	bool status;
	if(disable == TRUE)
	{
		while(disabled_server_ids[++i]);
		disabled_server_ids[i] = link_id;
	}
	status = update_link_cost(topology.server_id,link_id,MY_MAX);
	status = update_link_cost(link_id,topology.server_id,MY_MAX);
	
	routing_table[topology.server_id][link_id] = MY_MAX;
	for(i=1;i<num_servers;i++)
	{
		routing_table[link_id][i] = MY_MAX;
		
	}
	for(i=1;i<num_servers;i++)
	{
		routing_table[i][link_id] = MY_MAX;
		
	}
	int j=0;
	i=1;
	for(j=0;j<num_servers;j++)
	{
		int next_hop=link_cost[j].via_node;
		for(i=0;j<num_servers;i++)
		{
			if(link_cost[j].to_node == i)
			{
				link_cost[j].cost = min(&next_hop , routing_table[i]);
				link_cost[j].via_node = next_hop;
				//printf("\n%d",l);
				//printf("\n%d:(%d):%d:%d",link_cost[j].from_node,link_cost[j].via_node,link_cost[j].to_node,link_cost[j].cost);
				break;
			}
		}
	}
	//print_routing_table();
	/*
	for(i=1;i<MY_MAX;i++)
	{
		//link_cost[i]
	}
	*/
	return TRUE;
}

/***************************************************************************************
Function 	: disable_server_link
Desc		: It is the subroutine for "DISPLAY' user command. It prints the current status
				of routing table to STDOUT.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void display_routing_table()
{
	int i = 0;
	link_cost_struct link_cost_dup[MAX_NUM];
	for(i=0;i<MAX_NUM;i++)
		memset(&link_cost_dup[i],0,sizeof(link_cost_struct));
	size_t size = sizeof(link_cost_struct);
	for(i=0;i<num_servers;i++)
		memcpy(&link_cost_dup[i],&link_cost[i],size);
	printf("\n Current Routing Table Status:");
	qsort (link_cost_dup, num_servers, sizeof(link_cost_struct), cmp);
	for(i=1;i<num_servers;i++)
	{
		int j = i;
		printf("\n %hu  %hu : " ,link_cost_dup[j].to_node,link_cost_dup[j].via_node);
		if(link_cost_dup[j].cost == MY_MAX)
			printf ("%s", "INF");
		else if(link_cost_dup[j].cost == 0)
			printf ("%s","NIL");
		else
			printf ("%hu",link_cost_dup[j].cost);
	}
	printf("\nDisplay SUCESS");
}


/***************************************************************************************
Function 	: simulate_server_crash
Desc		: It is the subroutine for "CRASH' user command. It closes the STDIN, disables
			  all connections.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void simulate_server_crash()
{
	int i=0;
	for(i=0;i<num_servers;i++)
	{
		disable_server_link(i,TRUE);
	}
	freeze_system();
	printf("\nCrash Simulate SUCESS");
}


/***************************************************************************************
Function 	: display_packets_count
Desc		: It is the subroutine for "PACKETS' user command. It prints the received packet
				count from last query.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void display_packets_count()
{
	printf("\n Number of distance vector packets received :%d",total_num_of_diaplay_packets_recieved - prev_tap_on_display_packets_recieved);
	prev_tap_on_display_packets_recieved = total_num_of_diaplay_packets_recieved;
	printf("\nPackets SUCESS");
}


/***************************************************************************************
Function 	: send_routing_update
Desc		: It is utility function to trigger updates.
Parameters	: void
Return Val	: bool
Comments	:Reference : 3.2.4	Structure Name : SENDER_PACKET_STRUCT (Under Section Data Structures )
 ****************************************************************************************/
bool send_routing_update()
{
	int i=0;
	make_send_packet();
	for(i=0;i<topology.num_servers;i++)
	{
		char *temp_send_buf;
		if(!is_server_link_disabled(topology.peer_detail[i].peer_id) && 
			topology.peer_detail[i].cost != MY_MAX && 
			topology.peer_detail[i].peer_id != topology.server_id)
		{
			printf("\n Updates Sent to Server : %s:%hu",topology.peer_detail[i].peer_addr,topology.peer_detail[i].port);
			client_setup(topology.peer_detail[i].peer_addr,
						topology.peer_detail[i].port);
			temp_send_buf = (char*)malloc(sizeof(sender_packet));
			memset(temp_send_buf,0,sizeof(sender_packet));
			memcpy(temp_send_buf,&sender_packet,sizeof(sender_packet));
			if (sendto(sock_id, temp_send_buf, sizeof(sender_packet), 0, (struct sockaddr *)&client_addr, sizeof(client_addr))==-1)
			printf("\n error!");
		}
	}
	return TRUE;
}

/***************************************************************************************
Function 	: send_periodic_update
Desc		: It sends the routing updates to its neighbour on timer expiry. It also verifies
				for missing updates.
Parameters	: void
Return Val	: void
Comments	: 
 ****************************************************************************************/
void send_periodic_update()
{
	tv.tv_sec = periodic_update_time;
	verify_peer_count_missing_update();
	update_peer_count_missing_update();
	send_routing_update();
}
