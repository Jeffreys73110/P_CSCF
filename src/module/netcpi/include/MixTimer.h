#ifndef __MIXTIMER_H__
#define __MIXTIMER_H__

#include "../../wincmpt/include/Threading.h"

// Mix logic timer
typedef struct _MIX_TIMER_STRUCT
{
	int				initFlag;		// if(initFlag==20151005)=true
	unsigned char	Kill;			// it is a killed timer or not
	unsigned char	callbackFlag;	// for callbacking or not
	unsigned int	nID;			// timer ID
	int				Timeout;		// timeout (m sec)
	int				Cnt;			// microsecond count
	ymutex			KeyLock;		// for thread safe
	void*			pObject;		// a pointer to a variable to be passed to the callback
	void*			callback;		// a pointer for callback function
	unsigned int	timeoutIndex;	// check timer effective in msg_queue
	void*			preTimer;		// for Timer Link -Previous
	void*			nextTimer;		// for Timer Link -Next
}
MIXTIMER, *LPMIXTIMER;

// Callback
typedef void (*MIXT_CALLBACK)(MIXTIMER *wParam, void* lpObj);

#endif
