/***************************************************************************************
File 			: gprot.h
Author 			: Amit Pratap Singh
Organization 	: University at Buffalo
Description		: This file includes the common set of header files/defined variables and macros.
					also it declares the global data structure and function calls.
 ****************************************************************************************/

#ifndef GPROT_H
#define GPROT_H
/*************Include headers********************/
#include<sys/types.h>
#include<time.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<ctype.h>
#include<fcntl.h>
#include<stdlib.h>
/*************Enable/Disable Extra Traces**************/
//#define TRACE_DEBUG 1
/*************DEFINED VALUE SET********************/
#define STDIN 0
#define FILE_READ_RATE 20
#define MAX_SIZE 2048 
#define MAX_CMD_SIZE 256
#define HOST_SIZE 128
#define SOCK_FAMILY AF_INET
#define SOCK_TYPE SOCK_STREAM
#define SOCK_FLAG  0
#define CLIENT 1
#define SERVER 0
#define MAX_CONN 5
#define TRUE 1
#define FALSE  0
#define MAX_PEER   10
#define MAX_CMD_LN_VAR 3
#define MAX_CHART_SIZE 10000000
#define MAX_BUF_SIZE 2048+2048
#define FILE_SIZE 256
#define MAX_READ_SIZE 4096
int download_chunk_size;
typedef unsigned char bool;

/*Maintain list of registered devices*/
typedef struct _ip_list_struct
{
	int port;
	int sock_id;
	char ip_addr[MAX_SIZE];
	struct _ip_list_struct *next;
}ip_list_struct;

ip_list_struct *ip_list_first,*ip_list_current; //server side -mainatins list of all reg clients
ip_list_struct host_ip_list[10]; //client side - maintains list of all reg clients - copy of server_ip_list

/*maintain list of all connected devices to a client - to print list of all peers connected of its connected to*/
typedef struct _conn_peer_list_struct
{
	int  port;
	int sock_id;
	char ip_addr[MAX_SIZE];
	struct _conn_peer_list_struct *next;
}conn_peer_list_struct;

conn_peer_list_struct *conn_list_first,*conn_list_current;

/*client_addr - socket address on client side to listen for incoming connections*/
struct sockaddr_in client_addr;

/*cl_ser_conn_addr - this is the server address of the to which the client is connected*/
struct sockaddr_in cl_ser_conn_addr;

/*server_addr - socket address on server side for listening incoming connections*/
struct sockaddr_in server_addr;

/*peer_list_addr - maintains list of all connected peers of client side*/
struct sockaddr_in peer_addr_list[MAX_PEER];

typedef struct _download_data_chunks
{
	int start;
	int end;
	bool fin;
	struct _download_data_chunks *next;
}down_data_chunk;
down_data_chunk *peer1_data_chunk,*peer1_data_chunk_first;
down_data_chunk *peer2_data_chunk,*peer2_data_chunk_first;
down_data_chunk *peer3_data_chunk,*peer3_data_chunk_first;
down_data_chunk *peer4_data_chunk,*peer4_data_chunk_first;

char *cmd_arg_var[MAX_CMD_LN_VAR];
char send_buf[MAX_BUF_SIZE];
char recv_buf[MAX_BUF_SIZE];

long down_size;
int server_sock_id; //for litneing connections when run as 's'
int client_sock_id; //for listerning connections wehn run as 'c'
int cl_server_sock_id;//for connecting client to server
int server_conn_id[10];//all connections that server can accept - assign conn_id
int cur_server_conn_id;//server - curr conn_id set for accpet
int peer_sock_id[4]; // client - "connect" max number of peer socket used for conn
int cur_peer_sock_id;// client - latest registered sock id with peer
int cur_peer_conn_id; // client - latest accepted client conn
int peer_conn_id[4]; // client - "accept" max number of peer socket used for conn

int  port;
fd_set master;
fd_set client_fds;
fd_set server_fds;
int client_fd_max;
int server_fd_max;

bool mode; //client/server
bool quit; //sets this bit if program need to be closed

clock_t start,stop;
double used_time;
/*download file vars*/
bool down_begin;
long count1;
long count2;
long count3;
long count4;
char myip[128];
char fl_name[32];
long req_download_size;
long cur_download_size;
bool custom_check;


/*******Function Declarations*****************/

void handle_cmd_ln_intr();
bool validate_cmd(char *input[]);
void cmd_line_execute(char *input[]);
void display_ip();
void get_reg_list();
void run_server_part();
void run_client_part();
void handle_peer_data(int conn_id,char recv_buf[]);
bool is_dev_registered(char ipstr[]);
void print_server_ip_list();
int set_server_ip_list(char recv_buf[],int sock_id);
void handle_download_req(int conn_id,char recv_buf[]);
int update_conn_dev_list(char ip_addr[],int port,int id,bool flag);
void display_downloaded_data();
void update_server_ip_list(char recv_buf[],int sock_id,bool add_dev);
int remove_server_ip_list(int sock_id);
void calculate_ip();
bool is_dev_already_connected(char ip_addr[]);
int update_accepted_conn_list(char ip_addr[],int lo_port,int id,bool flag);
void free_all_mem();
int peer1_download_chunk();
int peer2_download_chunk();
int peer3_download_chunk();
int peer4_download_chunk();
void free_all_download_mem();
#endif // GPROT_H
