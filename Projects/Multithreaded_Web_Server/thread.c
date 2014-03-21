/***************************************************************************************
File 			: thread.c
Author 			: Amit Pratap Singh
Date			: 
Email			: amitprat@buffalo.edu
Organization 	: University at Buffalo
				  Operating System - 521 
				  Multi threaded Web Server(Project 1)
Description		: This thread contains all the synchronous operations with thread scheduling,waiting etc.
				 This file mainly address three different type of thread - 1)main thread - which always listen to socket 
				for any incoming requests and insert the request to ready queue. 2)scheduler Thread : which simply sniffs every
				servant thread and if it os free, the request is allocated to them. 3)servant thread : these are main working
				threads which reads file, send data and header and logs request.
				  
 ****************************************************************************************/

#include "gprot.h"

 /*********************************Thread Pool***************************/
pthread_t thread_pool[MAX_NUM_THREAD];
pthread_t main_thread;
pthread_t scheduler_thread;
pthread_cond_t      cond_sched  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex_sched = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t      cond[MAX_NUM_THREAD]  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex[MAX_NUM_THREAD] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t     read_req = PTHREAD_MUTEX_INITIALIZER;
int thrd_arr[MAX_NUM_THREAD];
enum
{
	FREE,
	ALLOC,
	BUSY
}avail[MAX_NUM_THREAD];	//keep track of which thread is busy

/***************************************************************************************
Function 	: insert_into_ready_queue
Description	: First thread called from main function to insert request into ready queue
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void *insert_into_ready_queue()
{
	int status = 0,i=0;
	struct sockaddr_in msgfrom;
	int msgsize;
	FD_ZERO(&master);
	FD_SET(s,&master);
	fd_max = s;

	time_t now_gmt;
	now_gmt = time(NULL);
	const char* gmt_now_str = asctime( gmtime( &now_gmt ) );

	while(1)
	{
		FD_ZERO(&ready);
		ready = master;
		#ifdef TRACE_DEBUG_ON
		printf("\n Waiting in Main function:%d-\n",num_of_waiting_req);
		#endif
		if (select(fd_max+1, &ready, 0, 0, 0) < 0)
		{
			perror("select");
			exit(1);
		}
		if(FD_ISSET(s, &ready)) 
		{
			int len = sizeof(struct sockaddr *);
			#ifdef TRACE_DEBUG_ON
			fprintf(stderr, "\nEntering accept() , waiting for connection.\n");
			#endif
			sock = accept(s, (struct sockaddr *)NULL, &len);
			#ifdef TRACE_DEBUG_ON
			printf("\nConnection accepted");
			#endif
			FD_SET(sock,&master);
			if(sock > fd_max)
				fd_max = sock;
		}
		else if (FD_ISSET(sock, &ready)) 
		{
			memset(&req_recv_buf,0,BUF_LEN);	
			if ((bytes = recv(sock, req_recv_buf, BUF_LEN, 0)) > 0)			
			{
				#ifdef TRACE_DEBUG_ON
				printf("\nRequest Received from server");
				#endif
				status = parse_request_line();
				enqueue_request();

				if(Sleep == TRUE)
				{
					Sleep = FALSE;
					#ifdef TRACE_DEBUG_ON
					printf("\nCall_scheduler Thread");
					#endif
					call_scheduler_thrd();
				}
			} 
			else if(bytes <= 0)
			{
				done++;	
				FD_CLR(sock,&master);
			}
		}	
	}
}

/***************************************************************************************
Function 	: push_for_scheduling
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void *push_for_scheduling()
{
	int i=0;
	put_scheduler_thread_on_wait();
	while(num_of_waiting_req)
	{
		#ifdef TRACE_DEBUG_ON
		printf("\nCurrent available number of threads:%d\n",avail_num_of_thread);
		#endif
		if(avail_num_of_thread)
		{
			int j=0;
			for(j=0;j<avail_num_of_thread;j++)
			{
				if(avail[j] == FREE)
				{
					avail_num_of_thread--;
					avail[j] = ALLOC;
					dequeue_request();
					wake_up_servant_thrd(j);
					break;
				}
			}
		}
			Sleep = TRUE;
			put_scheduler_thread_on_wait();
	}
	
}

/***************************************************************************************
Function 	: push_for_exec
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void *client_request_server(void  *param)
{
	int thread_no;
	client_req servent_cl_req;
	thread_no= *(int *)param;
	
	init_all_servent_thrd_wait(thread_no);
	
	//hold mutex while copying request data 
	pthread_mutex_lock(&read_req);
	servent_cl_req.socket = servent_req_info.socket;
	servent_cl_req.req_type = servent_req_info.req_type;
	strcpy(servent_cl_req.req_file,servent_req_info.req_file);
	strcpy(servent_cl_req.req_line,servent_req_info.req_line);
	strcpy(servent_cl_req.req_time,servent_req_info.req_time);
	pthread_mutex_unlock(&read_req); //release lock
	
	strcpy(servent_cl_req.req_served_time,servent_req_info.req_served_time);
	read_n_send_file_to_client(servent_cl_req);

	FD_CLR(servent_cl_req.socket,&master);
	avail_num_of_thread++;
	avail[thread_no] = FREE;
	if(num_of_waiting_req && avail_num_of_thread)
	{
				Sleep = FALSE;
	call_scheduler_thrd();
	}
	client_request_server((void *)&thread_no);
}


/***************************************************************************************
Function 	: wake_up_servant_thrd
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void wake_up_servant_thrd(int j) 
{
	pthread_mutex_lock(&mutex[j]);
	pthread_cond_signal(&cond[j]);
	pthread_mutex_unlock(&mutex[j]);
}

/***************************************************************************************
Function 	: init_all_servent_thrd_wait
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void init_all_servent_thrd_wait(int i) 
{
	pthread_mutex_lock(&mutex[i]);
	while(avail[i] == FREE)
	{
		pthread_cond_wait(&cond[i],&mutex[i]);
	}
	pthread_mutex_unlock(&mutex[i]);
}

/***************************************************************************************
Function 	: put_exec_thrd_on_wait
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void put_scheduler_thread_on_wait() 
{
	pthread_mutex_lock(&mutex_sched);
	while( (queuing_status == DISABLED) ||(Sleep == TRUE) )
	{
		pthread_cond_wait(&cond_sched, &mutex_sched);
	}
	pthread_mutex_unlock(&mutex_sched);
}

/***************************************************************************************
Function 	: call_exec_thrd
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void call_scheduler_thrd() 
{
	pthread_mutex_lock(&mutex_sched);
	pthread_cond_signal(&cond_sched);
	pthread_mutex_unlock(&mutex_sched);
}


/***************************************************************************************
Function 	: timer_timeout
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void timer_timeout()
{
	printf("\n Queuing Timer Expired\n");
	queuing_status = ENABLED;
	call_scheduler_thrd();
}


/***************************************************************************************
Function 	: start_timer
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void *start_timer(void *param)
{
	int time = *(int *)param;
	sleep(time);
	timer_cb_ptr = timer_timeout;
	timer_cb_ptr();
}

/***************************************************************************************
Function 	: init_thread_setup_vars
Description	: Second Thread called for putting request from ready queue to execution
Parameters	: 
Return Val	: void
Comments	: 
****************************************************************************************/
void  init_thread_setup_vars()
{
	int i=0;
	void *tret;
	int err;
	for(i=0;i<avail_num_of_thread;i++)
	{
		thrd_arr[i] = i;
	}

	/*Thread creation and put on waiting*/
	pthread_t timer_thrd;

	err = pthread_create(&timer_thrd,NULL,start_timer,(void *)&req_queuing_time);
	if(err != 0)
	{
		printf("\n Thread creation failed!");
		exit(0);
	}

	err = pthread_create(&main_thread,NULL,insert_into_ready_queue,NULL);
	if(err != 0)
	{
		printf("\nError while creating thread1!\n");
		exit(0);
	}

	err = pthread_create(&scheduler_thread,NULL,push_for_scheduling,NULL);
	if(err != 0)
	{
		printf("\nError while creating thread1!\n");
		exit(0);
	}

	for(i=0;i<avail_num_of_thread;i++)
	{
		err = pthread_create(&thread_pool[i],NULL,client_request_server,(void *)&thrd_arr[i]);
		if(err != 0)
		{
			perror("Error!\n");
			exit(0);
		}
	}
	pthread_join(timer_thrd,NULL);
	pthread_join(main_thread,tret);
}
