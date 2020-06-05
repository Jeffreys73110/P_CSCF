#ifndef __CQUEUE_EVENT_H__
#define __CQUEUE_EVENT_H__

#include "Threading.h"

#define CQE_DROPPING_EVENT		0xFFFFFFFE
#define CQE_BLOCKING_TASK_EVENT 0xFFFFFFFF
typedef void (*CQLIST_EVENT)(unsigned int event, void* param);
typedef struct 
{
	unsigned int event;
	unsigned int param_size;
	CQLIST_EVENT callback;
}
CQListHeader;
typedef struct 
{
	unsigned char* queue; // [num][param_size]
	unsigned int max_num;
	unsigned int max_size;
	unsigned int read_cur;
	unsigned int write_cur;
	unsigned int blocking_cnt;
	ymutex vlock;
	//
	unsigned char is_allocate;
	unsigned char is_flag0;
	unsigned char is_flag1;
	unsigned char is_flag2;
}
CQList;

// core object
typedef struct 
{
	CQList		vlist;
	ypthread_t	vthread;
	ysemaphore	work_sem;
	ysemaphore	post_sem;
}
CQueueEvent;

#ifdef __cplusplus
extern "C" {
#endif

int CQEventInit(CQueueEvent* obj, void* queue, unsigned int queue_num, unsigned int max_param_size, unsigned int stack_ksize); // null pointer will auto allocate
int CQEventPost(CQueueEvent* obj, CQLIST_EVENT callback, unsigned int event, void* param, unsigned int msize);
int CQEventDrop(CQueueEvent* obj, CQLIST_EVENT callback, unsigned int event, void* param, unsigned int msize);
unsigned int CQEventRemainNum(CQueueEvent* obj);
int CQEventFree(CQueueEvent* obj);

#ifdef __cplusplus
}
#endif

#endif
