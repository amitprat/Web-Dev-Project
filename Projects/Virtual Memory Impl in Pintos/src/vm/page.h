#ifndef VM_PAGE_H
#define VM_PAGE_H

#include 		<hash.h>
#include 		"devices/block.h"
#include 		"filesys/off_t.h"
#include 		"threads/synch.h"
#include 		"userprog/process.h"
#include 		"userprog/pagedir.h"
#include        "threads/pte.h"
#include        "threads/palloc.h"
#include        "threads/init.h"
#include		"vm/frame.h"
/* Virtual page. */
#define TRACE_DEBUG 0
typedef enum _resut
{
	NOT_REQ_OP,
	OK,
	ILLEGAL_ACCESS,
	NO_PHY_MEM,
	REACHED_MAX
}result_enum;
struct _frame_struct;
typedef struct _frame_struct frame_struct;
typedef struct page 
{
	bool lock;
	bool writable;
	
    void *addr;                 /* User virtual address. */  
	struct _frame_struct *frame;        /* Page frame. */   
	struct file *file;
	uint32_t ofs;
	uint32_t read_bytes;
	uint32_t zero_bytes;
	uint8_t reference;

	//for mmap
	uint8_t map_id;
	struct file *fp;
	struct file_descriptor *fd;
	//for process dir
	uint32_t *pd;
	
	//for stack
	uint8_t *esp;
	uint32_t stack_pgs;
	//to check availability
	bool avl;
	//for swap
	bool in_swap;
	uint32_t swap_idx;
	
	struct hash_elem hash_elem; /* Hash table element. */
}suppl_page_table;
suppl_page_table *p; 
struct hash pages;
struct hash_iterator iter;
bool page_accessed_recently (struct page *);
bool page_lock (suppl_page_table *p);
void page_unlock (suppl_page_table *p); 
suppl_page_table *update_suppl_table_table(struct file *file, uint32_t ofs, void *upage,uint32_t read_bytes, uint32_t zero_bytes, bool writable);
void free_all_resources(uint32_t *pd);
#endif /* vm/page.h */
