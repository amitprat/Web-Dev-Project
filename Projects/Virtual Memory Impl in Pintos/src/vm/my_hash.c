#include 		<hash.h>
#include 		"devices/block.h"
#include 		"filesys/off_t.h"
#include 		"threads/synch.h"
#include        "threads/pte.h"
#include        "threads/palloc.h"
#include        "threads/init.h"
#include		"vm/page.h"
#include		"vm/frame.h"
static struct lock hash_lock;
unsigned page_hash(const struct hash_elem *p_, void *aux UNUSED)
{
  suppl_page_table *p = hash_entry (p_, suppl_page_table, hash_elem);
  return hash_bytes (&p->addr, sizeof p->addr);
}

unsigned frame_hash(const struct hash_elem *p_, void *aux UNUSED)
{
  frame_struct *f= hash_entry (p_, frame_struct, hash_elem);
  return hash_bytes (&f->base, sizeof f->base);
}

/* Returns true if page a precedes page b. */
bool page_less(const struct hash_elem *a_, const struct hash_elem *b_,
           void *aux UNUSED)
{
  suppl_page_table *a = hash_entry (a_, suppl_page_table, hash_elem);
  suppl_page_table *b = hash_entry (b_, suppl_page_table, hash_elem);
  return a->addr < b->addr;
}
/* Returns true if page a precedes page b. */
bool frame_less(const struct hash_elem *a_, const struct hash_elem *b_,
           void *aux UNUSED)
{
  frame_struct *a = hash_entry (a_, frame_struct, hash_elem);
  frame_struct *b = hash_entry (b_, frame_struct, hash_elem);
  return a->base < b->base;
}

void hash_initialize()
{
	lock_init(&hash_lock);
	hash_init(&pages,page_hash,page_less,NULL);
}
	
/* Returns the page containing the given virtual address,
   or a null pointer if no such page exists. */
suppl_page_table *page_lookup (const void *address)
{
  suppl_page_table p;
  struct hash_elem *e;

  p.addr = address;
  //lock_acquire(&hash_lock);
  e = hash_find (&pages, &p.hash_elem);
  suppl_page_table *p1 = (e != NULL ? hash_entry (e, suppl_page_table, hash_elem) : NULL);
  //lock_release(&hash_lock);
  return p1;
}

suppl_page_table *page_hash_insert(suppl_page_table *p)
{
	lock_acquire(&hash_lock);
	suppl_page_table *p1 =hash_insert (&pages, &p->hash_elem);
	lock_release(&hash_lock);
	return p1;
}
suppl_page_table *page_hash_del(suppl_page_table *p)
{
	lock_acquire(&hash_lock);
	suppl_page_table *p1 = hash_delete(&pages, &p->hash_elem);
	lock_release(&hash_lock);
	return p1;
}
suppl_page_table *page_hash_look_up(const void *address)
{
	lock_acquire(&hash_lock);
	suppl_page_table *p = page_lookup(address);
	lock_release(&hash_lock);
	return p;
}
suppl_page_table *page_hash_replace(suppl_page_table *p)
{
	lock_acquire(&hash_lock);
	suppl_page_table *p1 =hash_replace(&pages, &p->hash_elem);
	lock_release(&hash_lock);
	return p1;
}
size_t page_hash_size()
{
	return hash_size(&pages);
}
suppl_page_table *page_hash_first()
{
	struct hash_elem *e;
	lock_acquire(&hash_lock);
	hash_first(&iter,&pages);
	hash_find (&pages, &iter);
	e = iter.elem;
	suppl_page_table *p = (e != NULL ? hash_entry (e, suppl_page_table, hash_elem) : NULL);
    lock_release(&hash_lock);
    return p;
}
suppl_page_table *page_hash_next()
{
	lock_acquire(&hash_lock);
	struct hash_elem *e = hash_next(&iter);
	suppl_page_table *p = (e != NULL ? hash_entry (e, suppl_page_table, hash_elem) : NULL);
    lock_release(&hash_lock);
    return p;
}
suppl_page_table *page_hash_cur()
{
	lock_acquire(&hash_lock);
	struct hash_elem *e;
	hash_cur(&iter);
	e = hash_find (&pages, &iter);
    suppl_page_table *p = (e != NULL ? hash_entry (e, suppl_page_table, hash_elem) : NULL);
    lock_release(&hash_lock);
    return p;
}
/*frame functions*/
frame_struct *frame_lookup (const void *address)
{
  frame_struct f;
  struct hash_elem *e;
  f.base = address;
  e = hash_find (&frames, &f.hash_elem);
  return e != NULL ? hash_entry (e, frame_struct, hash_elem) : NULL;
}
frame_struct *frame_hash_insert(frame_struct *f)
{
	lock_acquire(&hash_lock);
	frame_struct *f1 =hash_insert (&frames, &f->hash_elem);
	lock_release(&hash_lock);
	return f1;
}
frame_struct *frame_hash_del(frame_struct *f)
{
	lock_acquire(&hash_lock);
	frame_struct *f1 = hash_delete(&frames, &f->hash_elem);
	lock_release(&hash_lock);
	return f1;
}
frame_struct *frame_hash_look_up(const void *address)
{
	frame_struct *f = frame_lookup(address);
	return f;
}
frame_struct *frame_hash_replace(frame_struct *f)
{
	frame_struct *f1 =hash_replace(&frames, &f->hash_elem);
	return f1;
}
/*
void testing_utility()
{
	hash_init(&pages,page_hash,page_less,NULL);
	struct page *new_page1,*new_page2;
	new_page1 = malloc(sizeof(struct page));
	new_page2 = malloc(sizeof(struct page));
	struct page *p=abs_hash_look_up(1);
	if(p)
		printf("p exists");
	else
		printf("p not exists");
	new_page1->addr = 1;
	new_page2->addr = 2;
	struct page *p1=abs_hash_insert(new_page1);
	if(p1)
		printf("p1 exists");
	else
		printf("p1 inserted");
	struct page *p2=abs_hash_insert(new_page2);
	if(p2)
		printf("p2 exists");
	else
		printf("p2 inserted");
	struct page *p3=abs_hash_insert(new_page2);
	if(p3)
		printf("p3 exists");
	else
		printf("p3 inserted");
	struct page *p4=abs_hash_look_up(1);
	if(p4)
		printf("p4 found");
	else
		printf("p4 not present");
	struct page *p5=abs_hash_look_up(2);
	if(p5)
		printf("p5 found");
	else
		printf("p5 not present");
	struct page *p6=abs_hash_del(new_page1);
	if(p6)
		printf("p6 deleted");
	else
		printf("p6 not present");
	struct page *p7=abs_hash_del(new_page2);
	if(p7)
		printf("p7 deleted");
	else
		printf("p7 not present");
	struct page *p8=abs_hash_look_up(1);
	if(p8)
		printf("p8 found");
	else
		printf("p8 not found");
}
*/
