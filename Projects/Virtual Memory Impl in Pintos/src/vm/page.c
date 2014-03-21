#include "vm/page.h"
#include <stdio.h>
#include <string.h>
#include "vm/frame.h"
#include "vm/swap.h"
#include "filesys/file.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"

suppl_page_table *victim_page = NULL;
/* Maximum size of process stack, in bytes. */
#define STACK_MAX (8*1024 * 1024)
#define MAX_PAGES 383
#define USER_VADDR_BOTTOM ((void *) 0x08048000)
#define STACK_HEURISTIC 32
bool page_init_done = false;
static struct lock page_lk;
void page_init (void) 
{
	lock_init(&page_lk);
	hash_initialize();
}
/* Tries to lock the page containing ADDR into physical memory.
   If WILL_WRITE is true, the page must be writeable;
   otherwise it may be read-only.
   Returns true if successful, false on failure. */
bool page_lock (suppl_page_table *p)
{
	lock_acquire(&page_lk);
	p->lock = true;
	lock_release(&page_lk);
}

/* Unlocks a page locked with page_lock(). */
void page_unlock (suppl_page_table *p) 
{
	lock_acquire(&page_lk);
	p->lock = false;
	lock_release(&page_lk);
}

suppl_page_table *update_suppl_page_table(struct file *file, uint32_t ofs, void *upage,
											uint32_t read_bytes, uint32_t zero_bytes, bool writable)
{

	#if TRACE_DEBUG
	//printf("\n update_suppl_page_table");
	#endif
	if(!page_init_done)
	{
		page_init();
		page_init_done = true;
	}
	struct thread *t = thread_current ();
	uint32_t *pd = t->pagedir;
	
	//1)check if page is already available, it exists then handle and return
	suppl_page_table *page = page_hash_look_up(upage);
	if(page)
	{
		return page;
	}
	//2)ELSE
	lock_acquire(&page_lk);
	suppl_page_table *new_entry = malloc(sizeof(suppl_page_table));
	if(!new_entry)
		printf("out of memory");
	memset(new_entry,0,sizeof(suppl_page_table));
	new_entry->file = file;
	new_entry->ofs = ofs;
	new_entry->addr = upage;
	new_entry->read_bytes = read_bytes;
	new_entry->zero_bytes = zero_bytes;
	new_entry->writable = writable;
	new_entry->pd = pd;
	lock_release(&page_lk);
	page_hash_insert(new_entry);
	return new_entry;
}

result_enum stack_expand_init(uint32_t *pd,void *upage,uint32_t *total_pages,uint8_t *cur_esp)
{
	uint8_t *esp=NULL,*user_addr=NULL;
	result_enum result;
	int i=0;
	size_t size = page_hash_size();
	suppl_page_table *page = page_hash_first();
	while(page && i<size)
	{
		if(page->esp)
		{
			esp = page->esp;
			break;
		}
		i++;
		page = page_hash_next();
	}
	page_lock(page);
	if(((uint8_t *)upage - cur_esp) >=-32)
	{
		if( ((uint8_t *)upage - cur_esp) <=STACK_HEURISTIC)
		{
			(*total_pages) = (page->stack_pgs) +1;
			(page->stack_pgs)++;
			if( (*total_pages)*PGSIZE < STACK_MAX)
			{
				result = OK;
			}
			else
				result = REACHED_MAX;
		}
		else
		{
			#if TRACE_DEBUG
			printf("illegal access");
			#endif
			result = ILLEGAL_ACCESS;
		}
	}
	else
		result = NOT_REQ_OP;
	page_unlock(page);
	#if TRACE_DEBUG
	printf("result : %d",result);
	#endif
	return result;
}

/*will be called from exception.c : page_fault*/
bool page_fault_hdlr(void *upage,bool not_present,bool write,bool user,void *esp)
{
	bool status =false;
	suppl_page_table *page;
	//check if kernel virtual address access
	if(is_kernel_vaddr(upage))
	{
		#if TRACE_DEBUG
		printf("\n invalid user access!");
		#endif
		return false;
	}
	//1)section 1 - MMAP check
	//identify if it is MMAP read
	if( (page= page_hash_look_up(upage)) && page->map_id )
	{
		page_lock(page);
		#if TRACE_DEBUG
			printf("\n page fault_hdlr - mmap");
		#endif
		status = sys_load_page_runtime(page);
		page_unlock(page);
		if(!status)
			return false;
		else
		{
			process_activate();
			return true;
		}
	}
	
	//2)Section 2 /*look for page replacement*/
	#if TRACE_DEBUG
	printf("\n page fault_hdlr - sec 2");
	#endif
	uint32_t ofs = pg_ofs(upage),total_pages=0;
	void *f_addr =upage - ofs; 
	suppl_page_table *spte = page_hash_look_up(f_addr);
	if(spte && !spte->lock)
	{
		page_lock(spte);
		if(spte->in_swap)
		{
			bool result = swap_in(spte);
			printf("swap_in");
			if(result)
			{
				frame_struct *frame = spte->frame;
				spte->avl = true;
				spte->in_swap = false;
				page_unlock(spte);
				pagedir_set_page (spte->pd, spte->addr, frame->base, spte->writable);
				process_activate();
			}				
			else
				return false;
		}	
		else
		{
			bool result = load_page_data(spte);
			if(result)
			{
				process_activate();
			}
			else
			{
				printf("failed");
				return false;
			}
		}
		page_unlock(spte);
		return true;
	}
	//3) Check if stack growth needed
	#if TRACE_DEBUG
	printf("\n page fault_hdlr - sec 3");
	#endif
	if(!write)
	{
		return false; //no need
	}
	 #if TRACE_DEBUG
	printf("May be stack growth needed");
	#endif
	struct thread *t = thread_current ();
	uint32_t *pd = t->pagedir;
	result_enum result = stack_expand_init(pd,upage,&total_pages,esp);
	if(result == OK)
	{
		bool success = extend_stack(pd,total_pages);
		if(success)
			return true;
		else
			return false;
	}
	else if(result == REACHED_MAX || result == ILLEGAL_ACCESS)
	{
		if(result == ILLEGAL_ACCESS)
		{
			#if TRACE_DEBUG
				printf("Invalid write");
			#endif
		}
		return false;
	}
	
}
/*it will handle freeing all allocate resources for particular upgae - 2 req for suppl page table*/
void free_all_resources(uint32_t *pd)
{
	 #if TRACE_DEBUG
	printf("\n free_all_resources");
	#endif
	void *marked_pages[MAX_PAGES];
	int j=-1,i=0;
	size_t size = page_hash_size();
	suppl_page_table *page = page_hash_first();
	while(page && i<size)
	{
		marked_pages[++j] = page;
		page = page_hash_next();
		i++;
	}
	while(j>=0)
	{
		suppl_page_table *page = marked_pages[i];
		frame_struct *frame = page->frame;
		frame_free(frame->base);
		page_hash_del(page);
		free(page);
		j--;
	}
}
void update_reference_bit(void *vaddr)
{
	#if TRACE_DEBUG
	printf("\n page accessed");
	#endif
	frame_struct *frame = frame_lookup(vaddr);
	suppl_page_table *page = frame->page;
	page->reference = 1;
}

void free_mapped_resource(int map_id)
{
	int i=-1,j=0;
	size_t size = page_hash_size();
	void *mapped_pages[MAX_PAGES];
	suppl_page_table *page = page_hash_first();
	while(page && j<size)
	{
		if(page->map_id == map_id)
		{
			mapped_pages[++i] = page;
		}
		j++;
		page = page_hash_next();
	}
	while(i>=0)
	{
		suppl_page_table *page = mapped_pages[i];
		frame_struct *frame = page->frame;
		page_hash_del(page);
		free(page);
		i--;
	}
}
bool page_replacement_alg()
{
	#if TRACE_DEBUG
	printf("\n page_replacement_alg");
	#endif
	int i = -1;
	bool replace = false;
	start:
	if(!victim_page)
		victim_page = page_hash_first();
		
	while(victim_page)
	{
		if(!(victim_page->reference) && (victim_page->avl) && (victim_page->frame) && !(victim_page->lock)) //mod
		{
			replace=true;
			page_lock(victim_page);
			frame_struct *frame = victim_page->frame;
			if (pagedir_is_dirty (victim_page->pd, victim_page->addr) || pagedir_is_dirty (victim_page->pd, frame->base))
			//if(pagedir_is_dirty(victim_page->pd , frame->base))
			{
				bool result=false;
				if(victim_page->writable)
				{
					if(victim_page->map_id)
					{
						result = swap_out(victim_page);
						victim_page->in_swap=true;
					}
					else
					{
						result = writeback_frame_data_to_file(victim_page,frame->base);
					}
				}
			}
			victim_page->avl = false;
			victim_page->reference = 0;
			frame_free(victim_page->frame);
			victim_page->frame = NULL;
			pagedir_clear_page(victim_page->pd,victim_page->addr);
			page_unlock(victim_page);
			break;
		}
		victim_page->reference = 0;
		victim_page = page_hash_next();
	}
	if(!replace)
	{
		goto start;
	}
	victim_page = page_hash_next();
	return true;
}
void trace_list()
{
	int i=0;
	size_t size = page_hash_size();
	printf("size :%d",size);
	suppl_page_table *page = page_hash_first();
	if(!page)
		return;
	while(page && i< size)
	{
		printf("\t %d",page->in_swap);
		i++;
		page = page_hash_next();
	}
}