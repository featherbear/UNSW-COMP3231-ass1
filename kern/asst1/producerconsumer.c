/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include "producerconsumer_driver.h"

/* Declare any variables you need here to keep track of and
   synchronise your bounded. A sample declaration of a buffer is shown
   below. It is an array of pointers to items.

   You can change this if you choose another implementation.
   However, you should not have a buffer bigger than BUFFER_SIZE
*/

data_item_t * item_buffer[BUFFER_SIZE];
int head;
int tail;

struct semaphore *producer_ready;
struct semaphore *consumer_ready;
struct lock *buffer_lock;

/* consumer_receive() is called by a consumer to request more data. It
   should block on a sync primitive if no data is available in your
   buffer. It should not busy wait! */

data_item_t * consumer_receive(void)
{
   data_item_t * item;

   P(consumer_ready);

   lock_acquire(buffer_lock);
   item = item_buffer[head];
   head = (head + 1) % BUFFER_SIZE;
   lock_release(buffer_lock);

   V(producer_ready);

   return item;
}

/* procucer_send() is called by a producer to store data in your
   bounded buffer.  It should block on a sync primitive if no space is
   available in your buffer. It should not busy wait!*/

void producer_send(data_item_t *item)
{
   P(producer_ready);

   lock_acquire(buffer_lock);
   item_buffer[tail] = item;
   tail = (tail + 1) % BUFFER_SIZE;
   lock_release(buffer_lock);

   V(consumer_ready);
}




/* Perform any initialisation (e.g. of global data) you need
   here. Note: You can panic if any allocation fails during setup */

void producerconsumer_startup(void)
{
   head = 0;
   tail = 0;
   producer_ready = sem_create("producer_ready", BUFFER_SIZE);
   consumer_ready = sem_create("consumer_ready", 0);
   buffer_lock = lock_create("buffer_lock");
}

/* Perform any clean-up you need here */
void producerconsumer_shutdown(void)
{
   sem_destroy(producer_ready);
   sem_destroy(consumer_ready);
   lock_destroy(buffer_lock);
}
