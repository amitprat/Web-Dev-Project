/***************************************************************************************
File 	: gprot.h
Desc	: This file includes the necessary library header files, macro definitions, function declarations 
			and data structures declaration.
 ****************************************************************************************/

#ifndef GPROT_H
#define GPROT_H
/*Include headers*/
#include	<sys/types.h>
#include	<arpa/inet.h>
#include	<unistd.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<ctype.h>
#include	<fcntl.h>
#include	<limits.h>
#include	<syslog.h> 
#include	<sys/resource.h> 
#include	<signal.h>

/*Macro definitions*/
#define TRACE_DEBUG 1
#define NUM_SERVERS	5
#define MY_MAX	999
#define MAX_RLIMIT 1024
#define TRUE 1
#define FALSE  0
#define STDIN	0
#define MAX_NUM	NUM_SERVERS+1
#define SOCK_FAMILY AF_INET
#define SOCK_TYPE SOCK_DGRAM
#define SOCK_FLAG	FALSE
#define MAX_CMD_SIZE 256 
#define HOST_SIZE 32
#define MAX_CMD_LN_VAR 4
#define MAX_BUF_SIZE 1024

typedef unsigned char bool;

/* It stores the link cost details of all connected servers
	Its cost to itself is 0 and other valid nodes have valid costs(0<cost<999).
	All non neighbours have Infinity(999) cost in beginning which is updated further
	by gathering updates from other nodes.*/
/*Reference : 3.2	 Structures/Unions:
3.2.1	Structure Name : LINK_COST_STRUCT
i*/
typedef struct link_cost_struct
{
	unsigned short from_node;
	unsigned short to_node;
	unsigned short via_node;
	unsigned short cost;
}link_cost_struct;
link_cost_struct 	link_cost[MAX_NUM];

/* It stores the details of all peers including the server itself.*/
/*Reference : 3.2	 Structures/Unions:
3.2.2	Structure Name : PEER_STRUCT
i*/
typedef struct _peer_struct
{
	unsigned short peer_id;
	unsigned short port;
	unsigned short cost;
	unsigned short reserve;
	char peer_addr[HOST_SIZE];
}peer_struct;

/*This is the sender packet details. Send packets are prepared in this format and then byte packet further 
to be send over UDP link to other neighbours */

/*Reference : 3.2	 Structures/Unions:
3.2.4	Structure Name : SENDER_PACKET_STRUCT
i*/
typedef struct sender_packet_struct
{
	unsigned short num_update_fields;
	unsigned short server_port;
	char server_ip[HOST_SIZE];
	peer_struct peer_detail[MAX_NUM];
}sender_packet_struct;
sender_packet_struct	sender_packet;

/*This is the topology file information.It is read in beginning and further reference
for any detail will be directly served from this structure rather than accessing file.*/
/*Reference : 3.2	 Structures/Unions:
3.2.3	Structure Name : TOPOLOGY_STRUCT
i*/
typedef struct _topology_struct
{
	unsigned short num_servers;
	unsigned short num_peers;
	unsigned short server_id;
	unsigned short server_port;
	char server_addr[HOST_SIZE];
	peer_struct	peer_detail[MAX_NUM];
}topology_struct;
topology_struct	topology;

/*This the routing table to maintain the cost details in a matrix.
It stores the distance of all  peers via all possible paths from source*/
int routing_table[MAX_NUM][MAX_NUM];//routing_table[to_node][via_node];
bool identify_missing_update_node[MAX_NUM];

/*Global Variable Section*/
int 	 port;
int 	 client_fd_max;
int		 server_fd_max;
int 	 sock_id;
int		 server_sock_id; //for litneing connections when run as 's'
int		 client_sock_id; //for listerning connections wehn run as 'c'
int		 periodic_update_time;
int		 prev_time_out_val;
int 	 num_servers;
int 	count_peer_missing_update[MAX_NUM];
int 	total_num_of_diaplay_packets_recieved;
int 	prev_tap_on_display_packets_recieved;
int 	disabled_server_ids[MAX_NUM];
char 	*topology_file_name;
char 	*cmd_arg_var[MAX_CMD_LN_VAR];
char 	buffer_size[MAX_BUF_SIZE];
struct 	timeval tv;

fd_set master;
fd_set client_fds;
fd_set server_fds;

/*client_addr - socket address on client side to listen for incoming connections*/
struct sockaddr_in client_addr;

/*server_addr - socket address on server side for listening incoming connections*/
struct sockaddr_in server_addr;

/*Function Declarations*/
void handle_cmd_ln_intr(char input[]);
void run_server_part();
void client_setup(char ipstr[],int port);
void server_setup();
bool iswhitespc(char input[]);
void send_periodic_update();
void read_topology_file();
void make_send_packet();
bool send_routing_update();
void init_cost_table();
int update_topology_link_cost(char *buff);
void print_updated_link_cost(int server_id);
void execute_user_cmd();
bool validate_user_cmd_input(char *input[]);
void trigger_update();
void print_routing_table();
unsigned short min(int *next_hop,int cost[]);
void display_routing_table();
void simulate_server_crash();
void display_packets_count();
void print_disbaled_array();
bool disable_server_link(int link_id,bool disable);
bool is_server_link_disabled(int server_id);
void get_arguments(int argc,char *argv[]);
void freeze_system();
bool update_link_cost(int input1,int input2,int input3);
int cmp(const void *p1, const void *p2);
void remove_disabled_node(int to_node);
#endif
