#include <stdio.h>
#include "event.h"

#pragma comment(lib, "Ws2_32.lib")

struct event   ev;
struct timeval tv;

void time_cb(int fd, short event, void *arg)
{
	printf("***********timer wakeup*********** \n");
	event_add(&ev, &tv);
}

int main()
{
	struct event_base *base = event_init();
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	evtimer_set(&ev, time_cb, NULL);
	event_add(&ev, &tv);
	event_base_dispatch(base);
}