/***************************************************************************************
File 			: utility.c
Author 			: Amit Pratap Singh
Date			: 
Email			: amitprat@buffalo.edu
Organization 	: University at Buffalo
				  Operating System - 521 
				  Multi threaded Web Server(Project 1)
Description		: This file contains supporting functions for all operations such as queuing the request, processing request,
					reading from file, sending data to client, logging details to log file, taking decision based on argument
					value set etc.
				  
 ****************************************************************************************/

#include "gprot.h"

 /*********************************Global Variables***************************/
pthread_mutex_t    		 log_res = PTHREAD_MUTEX_INITIALIZER;
ready_queue 			*ready_first_req  = NULL;
ready_queue 			*ready_cur_req = NULL;
priority_ready_queue 	*p_ready_first_req  = NULL;
priority_ready_queue 	*p_ready_cur_req = NULL;

sched_policy 			default_policy;
bool 					Sleep;
char 					logging_file[MIN_STR_SIZE];
					
char 					*defualt_mapping_dir = NULL;
char 					default_logging_file[MIN_STR_SIZE]= "myhttpd.log";
char 					*default_port = "8080";

/***************************************************************************************
Function 	: identify_sched_policy
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
sched_policy identify_sched_policy()
{
	if(default_policy == DEFAULT_SCHEDULE_POLICY)
	{
		return FCFS;
	}
	else if(default_policy == SJF)
	{
		return SJF;
	}
}


/***************************************************************************************
Function 	: dequeue_request
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void dequeue_request()
{
	time_t now_gmt;
	now_gmt = time(NULL);
	const char* gmt_now_str = asctime( gmtime( &now_gmt ) );

	sched_policy policy;
	if( (policy = identify_sched_policy()) == FCFS)
	{
		ready_queue *temp = ready_first_req;
		ready_queue *del;
		servent_req_info.socket = temp->socket;
		servent_req_info.req_type= temp->req_type;
		strcpy(servent_req_info.req_file , temp->req_file );
		strcpy(servent_req_info.req_line , temp->req_line );
		strcpy(servent_req_info.req_time , temp->req_time);
		strcpy(servent_req_info.req_served_time,gmt_now_str);
		del = temp;
		temp = temp->next;
		ready_first_req = temp;
		free(del);
		num_of_waiting_req--;
	}
	else if(policy == SJF)
	{
		priority_ready_queue *temp = p_ready_first_req;
		priority_ready_queue *del;
		servent_req_info.socket = temp->socket;
		servent_req_info.req_type = temp->req_type;
		strcpy(servent_req_info.req_file , temp->req_file );
		strcpy(servent_req_info.req_line , temp->req_line );
		strcpy(servent_req_info.req_time , temp->req_time) ;
		strcpy(servent_req_info.req_served_time,gmt_now_str);
		del = temp;
		temp = temp->next;
		p_ready_first_req = temp;
		free(del);
		num_of_waiting_req--;
	}
}


/***************************************************************************************
Function 	: enqueue_request
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void enqueue_request()
{
	sched_policy policy;
	if( (policy = identify_sched_policy()) == FCFS)
	{
		ready_queue *new_req;

		new_req = (ready_queue *)malloc(sizeof(ready_queue));
		new_req->socket = sock;
		strcpy(new_req->req_file,req_info.req_file);
		new_req->req_type = req_info.req_type;
		strcpy(new_req->req_line,req_info.req_line);
		strcpy(new_req->req_time,req_info.req_time);
		new_req->next = NULL;
		if(ready_first_req == NULL)
		{
			ready_first_req = new_req;
			ready_cur_req = new_req;
		}
		else
		{
			ready_cur_req->next = new_req;
			ready_cur_req = ready_cur_req->next;
		}
		num_of_waiting_req++;
	}
	else if(policy == SJF)
	{
		long size = filesize(req_info.req_file);
		priority_ready_queue *new_req;
		new_req = (priority_ready_queue *)malloc(sizeof(priority_ready_queue));
		strcpy(new_req->req_file,req_info.req_file);
		new_req->req_type = req_info.req_type;
		strcpy(new_req->req_line,req_info.req_line);
		strcpy(new_req->req_time,req_info.req_time);
		new_req->job_size = size;
		new_req->next = NULL;
		if(!p_ready_first_req)
		{
			p_ready_first_req = new_req;
			p_ready_cur_req = new_req;
		}
		else
		{
			priority_ready_queue *temp = p_ready_first_req;
			if(size <= temp->job_size)
			{
				new_req->next = p_ready_first_req;
				p_ready_first_req = new_req;
				return;
			}
			while(temp->next)
			{
				if(size <= temp->next->job_size)
				{
					break;
				}
				else
					temp = temp->next;
			}
			if((temp == p_ready_first_req) && (temp->next) )
			{
				p_ready_cur_req->next = new_req;
				p_ready_cur_req = p_ready_cur_req->next;
			}
			else if(!(temp->next))
			{
				temp->next = new_req;
				p_ready_cur_req = new_req;
			}
			else
			{
				priority_ready_queue *temp1;
				temp1 = temp->next;
				temp->next = new_req;
				new_req->next = temp1;
			}
		}
	}
	else
	{
		printf("\nEither policy is invalid or not supported!\n");
		exit(0);
	}
}


/***************************************************************************************
Function 	: parse_request_line
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
int parse_request_line()
{
	char *temp,*temp1;
	time_t now_gmt;
	int req_type_len=0;
	temp = temp1 = req_recv_buf;
	while( (*temp) != '\n')
		temp++;
	*(temp - 1) = '\0';
	temp = temp1;
	strcpy(req_info.req_line,temp);

	temp = temp1= req_recv_buf;
	while( (*temp) != ' ')
		temp++;
	(*temp) = '\0';
	temp = temp1;
	if(strcmp(temp,"GET") == 0)
	{	
		req_info.req_type = GET;
		req_type_len = 3;
	}
	else if(strcmp(temp,"HEAD") == 0)
	{
		req_info.req_type = HEAD;
		req_type_len = 4;
	}

	temp = temp1 = req_recv_buf;
	temp = temp + req_type_len	+2;
	temp1 = temp;
	while( (*temp) != ' ')
		temp++;
	(*temp) = '\0';
	temp = temp1;
	strcpy(req_info.req_file,temp);

	now_gmt = time(NULL);
	const char* gmt_now_str = asctime( gmtime( &now_gmt ) );
	memset(&req_info.req_time,0,MAX_STR_SIZE);
	strcpy(req_info.req_time,gmt_now_str);
	return 1;
}


/***************************************************************************************
Function 	: usage
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void usage()
{
	fprintf(stderr,"usage format\n");
	fprintf(stderr, "usage: %s -d -l[log_file] -p[port] -r[root_dir] -t[queuing_time] -n[num_of_threads] -s[scheduling_policy]\n", progname);
	fprintf(stderr,"\n options : usage\n");
	fprintf(stderr, "usage: %s -d[Enable this option to enter debugging mode]\n", progname);
	fprintf(stderr, "usage: %s -l[log file name]\n", progname);
	fprintf(stderr, "usage: %s -p[Set port number otherwise 8080 as default]\n", progname);
	fprintf(stderr, "usage: %s -r[Set root directory, DEFAULT : PWD]\n", progname);
	fprintf(stderr, "usage: %s -t[Set queuing time, DEFAULT : 60 Sec]\n", progname);
	fprintf(stderr, "usage: %s -n[Set number of user threads , DEFAULT:4]\n", progname);
	fprintf(stderr, "usage: %s -s[Set scheduling policy[DEFAULT:FCSE, SJF]\n", progname);
	exit(1);
}

/***************************************************************************************
Function 	: output_http_header
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void output_http_header(int sock,int status,long fl_size,char req_file[],request_type req_type)
{
	int i=0;
	int num_header = 0;
	time_t now_gmt;
	struct stat st;
	char *filetype;
	char status_string[MIN_STR_SIZE];
	memset(&status_string,0,MIN_STR_SIZE);
	char file_type[MAX_STR_SIZE];
	char header_fields[6][MAX_STR_SIZE];
	char err_string[MAX_STR_SIZE] = {'\0'};
	
	filetype = malloc(MAX_STR_SIZE);
	for(i=0;i<6;i++)
		memset(&header_fields,0,MAX_STR_SIZE);

	now_gmt = time(NULL);
	const char* mod_time;
	const char* gmt_now_str = asctime( gmtime( &now_gmt ) );
	

	if(status == 200)
	{
		strcpy(status_string,"OK");
	}
	else if(status == 401)
	{
		strcpy(status_string,"Unauthorized");
		strcpy(err_string,"User is unauthorized to access the resource!");
	}
	else if(status == 404)
	{
		strcpy(status_string,"Not Found");
		strcpy(err_string,"Requested Resource is not available!");
	}
	else if(status ==500)
	{
		strcpy(status_string,"Internal Server Error");
		strcpy(err_string,"Internal Server Error, please check back later!");
	}

	print_file_type(req_file,filetype);
	sprintf(header_fields[0],"HTTP/1.0 %d %s\n",status,status_string);
	sprintf(header_fields[1],"Date: %s GMT\n",gmt_now_str);
	sprintf(header_fields[2],"Server: MYHTTPD\n");

	if(status == 200)
	{
		if (stat(req_file, &st))
		{
			perror(req_file);
		} 
		else 
		{
			mod_time = asctime( gmtime(&(st.st_mtime)) );
		}
		sprintf(header_fields[3],"Last-Modified: %s GMT\n",mod_time);
		sprintf(header_fields[4],"Content-Length: %lu\n",fl_size);
		sprintf(header_fields[5],"Content-Type: %s\n\n",filetype);
		num_header = 6;
	}
	else
	{
		num_header = 4;
		sprintf(header_fields[3],"Content-Length: %lu\n\n",strlen(err_string));
	}


	for(i=0;i<num_header;i++)
	{
		#ifdef TRACE_DEBUG_ON
		printf("\nheader field:%s",header_fields[i]);
		#endif
		write(sock,header_fields[i],strlen(header_fields[i]));
	}
	if(err_string && (req_type == GET) && status != 200  )
		write(sock,err_string,strlen(err_string));
	
	
	free(filetype);
}


/***************************************************************************************
Function 	: read_n_send_file_to_client
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void read_n_send_file_to_client(client_req servent_cl_req)
{
	int fd;
	int nbytes;
	int status;
	int sock = servent_cl_req.socket;
	char buffer[BUF_LEN];
	
	char usr_dir[MAX_STR_SIZE];
	char *final_req;
	memset(&final_req,0,MAX_STR_SIZE);
	final_req = malloc(MAX_STR_SIZE);
	char *req_file;
	memset(&req_file,0,MAX_STR_SIZE);
	req_file = malloc(MAX_STR_SIZE);
	request_type req_type;
	req_type = servent_cl_req.req_type;
	strcpy(req_file,servent_cl_req.req_file);
	struct stat statbuf; 
	
	memset(&buffer,0,BUF_LEN);
	/*Handle user dir specific requests*/
	if(req_file[0] == '~')
	{
		req_file = req_file+1;
		strcpy(final_req,"/home/");
		strcat(final_req,req_file);
		strcat(final_req,"/myhttpd");
		printf("\n%s",final_req);
		char *default_read_file = "index.html";
		bool default_read_file_exists = FALSE;
		char *dir_listing_buffer;
		dir_listing_buffer = malloc(BUF_LEN);
		list_files_in_dir(final_req,default_read_file,dir_listing_buffer,&default_read_file_exists);
		if(default_read_file_exists)
		{
			strcat(final_req,"/");
			strcat(final_req,default_read_file);
			free(dir_listing_buffer);
			goto read;
		}
		else
		{
			long offset =0;
			status = 200;
			if(req_type == HEAD)
			{
				output_http_header(sock,status,0,final_req,req_type);
				return;
			}
			long dir_listing_buf_size = strlen(dir_listing_buffer);
			output_http_header(sock,status,dir_listing_buf_size,final_req,req_type);
			while(offset < dir_listing_buf_size )
			{
				char temp_buffer[BUF_LEN];
				
				memcpy(&temp_buffer,dir_listing_buffer,offset + BUF_LEN-1);
				offset = offset + BUF_LEN-1;
				send(sock,temp_buffer,strlen(temp_buffer),0);
			}
			free(dir_listing_buffer);
			
			pthread_mutex_lock(&log_res);
			log_http_request_details(status,servent_cl_req);
			pthread_mutex_unlock(&log_res);
			
			return;
		}
	}
	/*request validation and formation*/
	if(map_dir != NULL)
	{
		sprintf(final_req,"%s/%s",map_dir,req_file);
	}
	if ( (strcmp(req_file,".") == 0) || 
			(strcmp(req_file,"..") == 0) )
	{
		printf("\n Permission denied");
		status = 401;
		goto report_err;
	}

	if (stat(final_req, &statbuf) < 0) /* stat error */ 
	{
		if(strlen(req_file) == 0)
		{
			strcpy(req_file,"index.html");
		}
		else
		{
			status = 404;
			goto report_err;
		}
	}
	
	if( S_ISDIR(statbuf.st_mode))
	{
		char *default_read_file = "index.html";
		bool default_read_file_exists = FALSE;
		char *dir_listing_buffer;
		dir_listing_buffer = malloc(BUF_LEN);
		list_files_in_dir(final_req,default_read_file,dir_listing_buffer,&default_read_file_exists);
		if(default_read_file_exists)
		{
			free(dir_listing_buffer);
			strcat(final_req,"/");
			strcat(final_req,default_read_file);
			goto read;
		}
		else
		{
			long offset =0;
			status = 200;
			if(req_type == HEAD)
			{
				output_http_header(sock,status,0,final_req,req_type);
				return;
			}
			long dir_listing_buf_size = strlen(dir_listing_buffer);
			output_http_header(sock,status,dir_listing_buf_size,req_file,req_type);
			while(offset < dir_listing_buf_size )
			{
				char temp_buffer[BUF_LEN];
				
				memcpy(&temp_buffer,dir_listing_buffer,offset + BUF_LEN-1);
				offset = offset + BUF_LEN-1;
				send(sock,temp_buffer,strlen(temp_buffer),0);
			}
			free(dir_listing_buffer);
			
			pthread_mutex_lock(&log_res);
			log_http_request_details(status,servent_cl_req);
			pthread_mutex_unlock(&log_res);
			
			return;
		}
	}

read:
	if(req_type == HEAD)
	{
		status = 200;
		output_http_header(sock,status,0,final_req,req_type);
		log_http_request_details(status,servent_cl_req);
		return;
	}
	fd = open(final_req,O_RDONLY);
	if(fd < 0)
	{
		printf("\nCouldn't open file for reading!\n");
		status = get_http_status_info();
		goto report_err;
	}
	else
	{
		status = 200;
		output_http_header(sock,status,filesize(final_req),final_req,req_type);
		while((nbytes = read(fd,buffer,BUF_LEN)) > 0)
		{
			send(sock,buffer,nbytes,0);
		}
		
		pthread_mutex_lock(&log_res);
		log_http_request_details(status,servent_cl_req);
		pthread_mutex_unlock(&log_res);
		return;
	}

report_err:
	output_http_header(sock,status,0,final_req,req_type);
		pthread_mutex_lock(&log_res);
	log_http_request_details(status,servent_cl_req);
		pthread_mutex_unlock(&log_res);
}


/***************************************************************************************
Function 	: filesize
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
long filesize(char *file_name)
{
	if(!file_name)
		return 0;
	FILE *fp;
	long size = 0;
	fp = fopen(file_name,"rb");
	if(fp == NULL )
	{
		return 0;
	}
	fseek(fp,0L,SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
}


/***************************************************************************************
Function 	: clear_already_existing_logging_details
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void clear_already_existing_logging_details()
{
	int fd;
	int logging_file_hdlr;
	logging_file_hdlr = open(logging_file,O_WRONLY|O_TRUNC);
	if(logging_file_hdlr != 0)
	{
		printf("\n File Truncated to Zero");
		close(logging_file_hdlr);
	}
	else
	{
		printf("\n File Opening Error!");
	}
	if(strcmp (logging_file, default_logging_file) != 0)
	{
		fd = open(default_logging_file,O_WRONLY|O_TRUNC);
		if(fd != 0)
		{
			printf("\n File Truncated to Zero");
			close(default_logging_file);
		}
		else
		{
			printf("\n File Opening Error!");
		}
	}
}


/***************************************************************************************
Function 	: log_http_request_details
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void log_http_request_details(int status,client_req servent_req_info)
{
	struct sockaddr_in remote;
	struct sockaddr_in remote1;
	memset(&remote,0,sizeof(remote) );
	int len = sizeof(remote);
	long size = 0;
	if(servent_req_info.req_file)
		size = filesize(servent_req_info.req_file);
	if (getpeername(sock, (struct sockaddr *) &remote, &len) < 0) {
		perror("getpeername");
		exit(1);
	}
	if( (debug_mode == TRUE) ||
		(logging_enabled == TRUE)
	  )
	{
		fill_logging_details(inet_ntoa(remote.sin_addr),
				servent_req_info.req_time,
				servent_req_info.req_served_time,
				servent_req_info.req_line,
				status,
				size);//filesize(servent_req_info.req_file));
	
		write_logging_details_to_log_file();
	}
}

/***************************************************************************************
Function 	: fill_logging_details
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void fill_logging_details(char ipaddr[],char req_time[],char req_served_time[],char req_line[],int status,long content_len)
{
	memset(&cur_logging_detail,0,sizeof(logging));
	if(ipaddr != NULL)
	{
		strcpy(cur_logging_detail.remote_ip_addr,ipaddr);
	}
	if(req_time != NULL)
	{
		req_time[strlen(req_time) -1] = '\0';
		strcpy(cur_logging_detail.req_recvd_time,req_time);
	}
	if(req_served_time != NULL)
	{
		req_served_time[strlen(req_served_time) -1] = '\0';
		strcpy(cur_logging_detail.req_served_time,req_served_time);
	}
	if(req_line != NULL)
	{
		strcpy(cur_logging_detail.req_header,req_line);
	}
	if(status != -1)
	{
		cur_logging_detail.status = status;
	}
	if(content_len != 0)
	{
		cur_logging_detail.content_length = content_len;
	}
	#ifdef TRACE_DEBUG_ON
	printf("\n All Logging details are set");
	#endif
}


/***************************************************************************************
Function 	: write_logging_details_to_log_file
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void write_logging_details_to_log_file()
{
	if(debug_mode)
	{
		fprintf (stdout, "%s - [%s] [%s] \"%s\" %d %lu \n",
			cur_logging_detail.remote_ip_addr,
			cur_logging_detail.req_recvd_time,
			cur_logging_detail.req_served_time,
			cur_logging_detail.req_header,
			cur_logging_detail.status,
			cur_logging_detail.content_length
		);
		
		return;
	}
	
	FILE *fp = fopen(logging_file,"a+");
	if(!fp)
	{
		printf("\n File Opening Error!");
		return;
	}
	fprintf (fp, "%s - [%s] [%s] \"%s\" %d %lu \n",
			cur_logging_detail.remote_ip_addr,
			cur_logging_detail.req_recvd_time,
			cur_logging_detail.req_served_time,
			cur_logging_detail.req_header,
			cur_logging_detail.status,
			cur_logging_detail.content_length
		);
	fclose(fp);
	#ifdef TRACE_DEBUG_ON
	printf("\n All Logging details are written to log file");
	#endif
}


/***************************************************************************************
Function 	: print_file_type
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void print_file_type(char *filename,char *filetype)
{
	int i=0;
	int len = strlen(filename);
	for(i=0;i<len;i++)
	{
		if( (*filename) != '.')
			filename++;
		else
			break;
	}
	if( !(*filename))
	{
		strcpy(filetype,"text/plain");
		
	}
	filename++;
	if( (strcmp(filename,"txt") == 0) ||
			(strcmp(filename,"htm") == 0) ||
			(strcmp(filename,"html") == 0) ||
			(strcmp(filename,"log") == 0)
	  )
	{
		strcpy(filetype,"text/html");
	}

	if( (strcmp(filename,"jpg") == 0) ||
			(strcmp(filename,"jpeg") == 0) ||
			(strcmp(filename,"bmp") == 0) ||
			(strcmp(filename,"gif") == 0) ||
			(strcmp(filename,"png") == 0) ||
			(strcmp(filename,"ico") == 0)
	  )
	{
		strcpy(filetype,"text/gif");
	}
}


/***************************************************************************************
Function 	: get_http_status_info
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
int get_http_status_info()
{
	int status;
	if(errno == EACCES)
		status = 401;
	else
		status = 404;
	return status;
}


/***************************************************************************************
Function 	: list_files_in_dir
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void list_files_in_dir(char *filename,char *default_read_file , char *dir_listing_buffer,bool *default_read_file_exists) 
{ 
	struct stat statbuf; 
	struct dirent *dirp; 
	DIR *dp; 

	if ((dp = opendir(filename)) == NULL) /* can't read directory */ 
		return;

	while ((dirp = readdir(dp)) != NULL) 
	{ 
		if( strcmp(dirp->d_name, ".") == 0 || 
				strcmp(dirp->d_name, "..") == 0 ) 
			continue; /* ignore dot and dot-dot */ 

		strcat(dir_listing_buffer,dirp->d_name);
		strcat(dir_listing_buffer,"\n");
		if( strcmp(default_read_file,dirp->d_name) == 0)
		{
			*default_read_file_exists = TRUE;
		}
	}
	if (closedir(dp) < 0) 
		printf("can't close directory %s", filename); 
}


/***************************************************************************************
Function 	: init_utility_setup_vars
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void	init_utility_setup_vars()
{
	Sleep = TRUE;
	defualt_mapping_dir = malloc(MAX_STR_SIZE);

	strcpy(default_logging_file, "myhttpd.log");
	queuing_status = DISABLED;
	//clear_already_existing_logging_details(); //to truncate the already existing entries from log file before starting the server again.
	if(usr_logging_file == NULL)
		strcpy(logging_file,default_logging_file);
	else
		strcpy(logging_file,usr_logging_file);
	req_queuing_time = (queuing_time == NULL) ? DEFAULT_QUEUING_TIME : atoi(queuing_time);
	if(set_sched_policy == NULL)
		default_policy = DEFAULT_SCHEDULE_POLICY;
	else
		default_policy = atoi(set_sched_policy);
	char *cwd;
	cwd = malloc(MAX_STR_SIZE);
	if( (getcwd(cwd, MAX_STR_SIZE)) != 0)
		strcpy(defualt_mapping_dir,cwd);
	free(cwd);
	if(root_dir)
	{
		if(*root_dir == '/')
			map_dir = root_dir;
		else
		{
			char *new_dir = malloc(256);
			sprintf(new_dir,"%s/%s",defualt_mapping_dir,root_dir);
			map_dir = new_dir;
		}
	}
	else
	{
		map_dir = defualt_mapping_dir;
	}
	if(thread_num != NULL)
		avail_num_of_thread = atoi(thread_num);
	else
		avail_num_of_thread = DEFAULT_NUM_OF_THREADS;
	if(debug_mode)
		avail_num_of_thread = 1;
		
	if (port == NULL)
		port = default_port;
}
int compare_char (const void *a, const void * b)
{  
	return ( *(char *)a - *(char *)b ); 
}
