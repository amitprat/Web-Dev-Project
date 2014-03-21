#include "vm/frame.h"
#include <stdio.h>
#include "vm/page.h"
#include "devices/timer.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "threads/interrupt.h"
static struct lock frame_lock1;
bool is_frame_init = false;
/* Initialize the frame manager. */
void frame_init (void) 
{
	hash_init(&frames,frame_hash,frame_less,NULL);
	lock_init(&frame_lock1);
}

bool first = false;
/* Tries to allocate and lock a frame for PAGE. Returns the frame if successful, false on failure. */
uint8_t *frame_alloc_and_lock (suppl_page_table *page) 
{
	#if TRACE_DEBUG
	printf("\n frame_allocator");
	#endif
	uint8_t *addr = NULL;
	if(!is_frame_init)
	{
		frame_init();
		is_frame_init = true;
	}
	
	if (page->read_bytes == 0)
	addr = palloc_get_page(PAL_USER|PAL_ZERO);
	 else
		addr = palloc_get_page(PAL_USER);
	if(!addr)
	{
		bool replaced = page_replacement_alg();	
		if(replaced == true)
		{
			addr = palloc_get_page(PAL_USER|PAL_ZERO);
			if(!addr)
				printf("error!");
		}
	}
	if(addr)
	{
		frame_struct *frame = add_new_frame(page,addr); //true means frame allocated
		page->frame = frame;
		page->avl = true;
		page->reference = 1;
		frame_hash_insert(frame);
	}
	return addr;
}

/* Releases frame F for use by another page.
   F must be locked for use by the current process.
   Any data in F is lost. */
void frame_free (frame_struct *f)
{
	if(!f)
		return;
	lock_acquire(&frame_lock1);
	suppl_page_table *page = f->page;
	page->avl = false;
	page->reference = 0;
	page->frame = NULL;
	if(f->base)
	{
		palloc_free_page(f->base);
		frame_hash_del(f);
		free(f);
	}
	lock_release(&frame_lock1);
}
/*add new frame - utility function*/
frame_struct  *add_new_frame(suppl_page_table *page,void *addr)
{
    frame_struct *new_frame = malloc(sizeof(frame_struct));
    new_frame->base = (void *)addr;
	new_frame->page = page;
    return new_frame;
}