#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdbool.h>
#include "threads/synch.h"
#include "userprog/process.h"
#include "vm/page.h"
/* A physical frame. */
typedef struct page  suppl_page_table;
typedef struct _frame_struct
{
    void *base;                 /* Kernel virtual base address. */
	suppl_page_table *page;          /* Mapped process page, if any. */
	struct hash_elem hash_elem; /* Hash table element. */
}frame_struct;
frame_struct *frame;  
struct hash frames; 
void frame_init (void);
uint8_t *frame_alloc_and_lock (suppl_page_table *page);
void frame_free (frame_struct *f);
frame_struct  *add_new_frame(suppl_page_table *page,void *addr);
unsigned frame_hash(const struct hash_elem *p_, void *aux UNUSED);
bool frame_less(const struct hash_elem *a_, const struct hash_elem *b_,void *aux UNUSED);
#endif /* vm/frame.h */
