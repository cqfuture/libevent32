#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include "event.h"
#include "evhttp.h"

void root_handler(struct evhttp_request *req, void *arg)
{
	struct evbuffer *buf = evbuffer_new();
	if ( ! buf )
	{
		printf("failed to create response buffer\n");
		return;
	}

	evbuffer_add_printf(buf, "hello: %s\n", evhttp_request_uri(req));
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

void generic_handler(struct evhttp_request *req, void *arg )
{
	struct evbuffer *buf = evbuffer_new();
	if ( ! buf )
	{
		printf("failed to create response buffer\n");
		return;
	}
	evbuffer_add_printf(buf, "Requested: %s\n", evhttp_request_uri(req));
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

int main(int argc, char *argv[])
{
	struct evhttp *httpd;

	WSADATA wsaData;
	DWORD ret;

	if ( (ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0 )
	{
		printf("WSAStartup failed with error: %d\n", ret);
		return -1;
	}

	event_init();

	httpd = evhttp_start("127.0.0.1", 8505);
	if ( ! httpd )
	{
		return 1;
	}
	
	evhttp_set_cb(httpd, "/", root_handler, NULL );
	evhttp_set_gencb(httpd, generic_handler, NULL);
	printf("httpd server start OK !\n");
	
	event_dispatch();
	evhttp_free(httpd);

	WSACleanup();

	return 0;
}