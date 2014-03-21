#include "vm/swap.h"
#include <bitmap.h>
#include <debug.h>
#include <stdio.h>
#include "vm/frame.h"
#include "vm/page.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

/* The swap device. */
static struct block *swap_device;

/* Used swap pages. */
static struct bitmap *swap_bitmap;

/* Protects swap_bitmap. */
static struct lock swap_lock;

/* Number of sectors per page. */
#define PAGE_SECTORS (PGSIZE / BLOCK_SECTOR_SIZE)
#define SWAP_START 0

/* Sets up swap. */
void swap_init (void) 
{
	swap_device = block_get_role(BLOCK_SWAP);
	block_sector_t block_sz = block_size(swap_device);
	#if TRACE_DEBUF
	printf("\n size : %d",block_sz*BLOCK_SECTOR_SIZE);
	#endif
	swap_bitmap = bitmap_create(block_sz/PAGE_SECTORS);
	bitmap_set_all(swap_bitmap, 0);
	lock_init(&swap_lock);
}

/* Swaps in page P, which must have a locked frame(and be swapped out). */
bool swap_in (suppl_page_table *p) 
{
   if (!swap_bitmap || !swap_device)
    {
      PANIC("Need swap partition but no swap partition present!");
    }
  size_t page_cnt=1,ofs=0,i=0;
  frame_struct *frame=p->frame;
  uint8_t *kpage = frame_alloc_and_lock(p);
  memset(kpage , 0, PGSIZE);
  uint32_t page_idx = p->swap_idx;
  #if TRACE_DEBUG
  printf("\n swap_in , %d",page_idx);
  #endif
  lock_acquire(&swap_lock);
   for (i = 0; i< PAGE_SECTORS; i++)
   {
		block_read (swap_device, page_idx*BLOCK_SECTOR_SIZE + i, kpage + BLOCK_SECTOR_SIZE * i); ////read block from swap
   }
   bitmap_set(swap_device, page_idx, false);
   lock_release(&swap_lock);
   return true;
}

/* Swaps out page P, which must have a locked frame. */
bool swap_out (suppl_page_table *p) 
{
   int i;
   if(swap_device == NULL)   //init the swap table
		swap_init();
   if (!swap_bitmap || !swap_device) //safe check
   {
      PANIC("Need swap partition but no swap partition present!");
   }
   if (!(p->frame)) //safe check
    return NULL;
	
  size_t page_cnt=1,ofs=0;
  void *start;
  frame_struct *frame=p->frame;
  lock_acquire (&swap_lock);
   uint32_t page_idx = bitmap_scan_and_flip (swap_bitmap, 0, page_cnt, false);
   p->swap_idx = page_idx;
   printf("p->swap_idx  :%d",p->swap_idx );
    for (i = 0; i< PAGE_SECTORS; i++)
    {
		block_write (swap_device, page_idx*BLOCK_SECTOR_SIZE + i, frame->base + BLOCK_SECTOR_SIZE * i);//write block to swap
	}
	lock_release (&swap_lock);
	return true;
}
