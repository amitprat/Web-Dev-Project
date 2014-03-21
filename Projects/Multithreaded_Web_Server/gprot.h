/***************************************************************************************
File 			: gprot.h
Author 			: Amit Pratap Singh
Date			: 
Email			: amitprat@buffalo.edu
Organization 	: University at Buffalo
				  Operating System - 521 
				  Multi threaded Web Server(Project 1)
Description		: This file contains the data structures and function declaration for all three
				  files( one main file, one thread file and a supporting file for both(utility.c))
				  
 ****************************************************************************************/
#ifndef 	GPROT_H

#define 	GPROT_H
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<netinet/in.h>
#include	<inttypes.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<time.h>
#include	<sys/stat.h>
#include	<errno.h>
#include	<dirent.h> 
#include	<arpa/inet.h>
#include	<netinet/in.h>
#include	<syslog.h> 
#include	<sys/resource.h> 
#include	<signal.h>
/***********typedef*******************/

typedef unsigned char bool;

/***********Macro Parameters************/

#define FILE_EXISTS 1
#define STDOUT	1
#define TRUE 1
#define FALSE 0
//#define TRACE_DEBUG_ON	TRUE
#define MAX_NUM_THREAD 50
#define DEFAULT_NUM_OF_THREADS 4

#define DEFAULT_SCHEDULE_POLICY FCFS

#define HEADER_LEN	128
#define IP_ADDR_SIZE 128
#define MAX_RLIMIT 1024

#define MIN_STR_SIZE	32
#define MID_STR_SIZE	64
#define MAX_STR_SIZE	128

#define BUF_LEN 4096 //buf for reading/writing to/from socket

/********************Main g_vars********************************/
extern bool Sleep;

fd_set 	ready;
fd_set 	master;

int 	s, sock, done, bytes;
int 	fd_max;
int 	num_of_waiting_req; //keep track of current waiting requests in ready queue
int 	avail_num_of_thread;

char 	req_recv_buf[BUF_LEN];

/***************************************************************************************
Enumeration 	: request_type
Description		: Client request type to determine response format and data.
****************************************************************************************/
typedef enum _request_type
{
	GET, HEAD
}request_type;

/***************************************************************************************
Structure	 	: client_req
Description		: Client request structure to store the information for further processing
				 the request.
****************************************************************************************/
typedef struct client_req_struct
{
	int socket;
	request_type req_type;
	char req_file[MAX_STR_SIZE];
	char req_time[MAX_STR_SIZE];
	char req_served_time[256];
	char req_line[MAX_STR_SIZE];
}client_req;
client_req req_info,servent_req_info;

/****************Command Line Arguments*******************/
extern char *optarg;
extern int optind;

bool 	debug_mode;
bool 	help;
bool 	logging_enabled;
char 	*usr_logging_file;
char 	*root_dir;
char 	*queuing_time;
char 	*thread_num;
char 	*set_sched_policy;
char 	*port;
char 	*host;
char 	*progname;
char 	*map_dir;
/***************************************************************************************
Structure	 	: ready_queue
Description		: FCFS Queue to process request in order of their arrival.
****************************************************************************************/
typedef struct _ready_queue
{
	int socket;
	request_type req_type;
	char req_file[MAX_STR_SIZE];
	char req_time[MAX_STR_SIZE];
	char req_served_time[MAX_STR_SIZE];
	char req_line[MAX_STR_SIZE];
	struct _ready_queue *next;
}ready_queue;

/***************************************************************************************
Structure	 	: priority_ready_queue
Description		: Priority Queue to process request in respect of their requested file size.
****************************************************************************************/
typedef struct _priority_ready_queue
{
	int socket;
	request_type req_type;
	char req_file[MAX_STR_SIZE];
	char req_time[MAX_STR_SIZE];
	char req_served_time[MAX_STR_SIZE];
	char req_line[MAX_STR_SIZE];
	long job_size;
	struct _priority_ready_queue *next;
}priority_ready_queue;

/*********************Scheduling Policy*******************/
typedef enum 
{
	FCFS,
	SJF
}sched_policy;

/***********************Logging**************************/

/***************************************************************************************
Structure	 	: logging
Description		: It stores the data to be stored in logging file.
****************************************************************************************/
typedef struct _LOGGING
{
	char remote_ip_addr[IP_ADDR_SIZE];
	char req_recvd_time[MAX_STR_SIZE];
	char req_served_time[MAX_STR_SIZE];
	char req_header[MAX_STR_SIZE];
	int status;
	long content_length;
}logging;
logging	cur_logging_detail;

/*********************Queuing****************************/
#define DEFAULT_QUEUING_TIME 0
#define DEFAULT_DUEUING_STATE DISABLED
typedef enum
{
	DISABLED,
	ENABLED
}Queuing;
Queuing queuing_status;
int req_queuing_time;

/*****************Function Declarations**********************/
typedef void 	(*time_call_back_ptr)();
time_call_back_ptr 			timer_cb_ptr;
void 			get_arguments();
void 			usage();
int 			setup_client();
int 			setup_server();
int 			parse_request_line();
void 			read_n_send_file_to_client(client_req servent_cl_req;);
void 			*insert_into_ready_queue();
void 			enqueue_request();
void 			*push_for_scheduling();
void 			*main_func();
void 			*schedular_func();
void 			call_scheduler_thrd();
void 			put_scheduler_thread_on_wait();
void 			*server_client_request();
void 			wake_up_servant_thrd(int j) ;
void 			init_all_servent_thrd_wait(int i);
long 			filesize(char *file_name);
sched_policy 	identify_sched_policy();
int 			transform_to_priority_ready_queue();
void 			dequeue_request();
void 			*start_timer(void *param);
void 			timer_timeout();
void 			fill_logging_details(char ipaddr[],char req_time[],char req_served_time[],char req_line[],int status,long content_len); //string is NULL and int/long = -1 if field not set
void 			write_logging_details_to_log_file();
void 			clear_already_existing_logging_details();
void 			print_file_type(char *filename,char *filetype);
int 			get_http_status_info();
void 			log_http_request_details(int status,client_req servent_cl_req);
void 			list_files_in_dir(char *filename,char *default_read_file , char *dir_listing_buffer,bool *default_read_file_exists);
void 			demonize_myhttpd_web_server();
void			init_thread_setup_vars();
void			init_utility_setup_vars();
int compare_char (const void *a, const void * b);
#endif
