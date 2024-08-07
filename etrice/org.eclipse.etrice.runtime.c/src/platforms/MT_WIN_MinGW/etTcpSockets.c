/*******************************************************************************
 * Copyright (c) 2013 protos software gmbh (http://www.protos.de).
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * CONTRIBUTORS:
 * 		Henrik Rentz-Reichert (initial contribution)
 *
 *******************************************************************************/

#include <winsock2.h>
#include <ctype.h>
#include <osal/etTcpSockets.h>
#include "osal/etThread.h"

#define STACK_SIZE		0						/* let system select default size */
#define PRIO			THREAD_PRIORITY_NORMAL
#define LOCAL_HOST		"127.0.0.1"

#define PRINT_DEBUG(x)	{ printf(x); fflush(stdout); }

/* implementation versions of data */

typedef struct etSocketConnectionDataImpl {
	/* public part */
	etSocketConnectionData data;

	/* implementation specific */
	SOCKET socket;
	int channel;
	struct sockaddr_in address;
	etThread readThread;
}
etSocketConnectionDataImpl;

typedef struct etSocketServerDataImpl {
	/* public part */
	etSocketServerData data;

	/* implementation specific */
	SOCKET socket;
	etThread listenerThread;
	int nConnections;
	etSocketConnectionDataImpl connections[MAX_CONNECTIONS];
}
etSocketServerDataImpl;

/* thread function reading from the socket */
static void readThreadFunc(void* threadData) {
	etSocketConnectionDataImpl* self = (etSocketConnectionDataImpl*) threadData;
	size_t len;
	ssize_t retval;
	int8* buffer = (self->data.bufferProvider)(self->data.userData, &len, self->channel);

	while (ET_TRUE) {
		retval = recv(self->socket, (char*)buffer, (int)len, 0);
		if (retval<=0) {
			/* TODO: call  WSAGetLastError and do error handling
			int err = WSAGetLastError();
			*/
			PRINT_DEBUG("connection thread: socket lost, exiting\n")
			self->socket = INVALID_SOCKET;
			return;
		}

		(self->data.receiver)(self->data.userData, self->channel, (size_t)retval, buffer);
	}
}

/* thread function listening to the socket and creating new listener threads for accepted connections */
static void listenerThreadFunc(void* threadData) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) threadData;

	PRINT_DEBUG("server: listening\n")
	if (listen(self->socket, self->data.maxConnections) == SOCKET_ERROR) {
		PRINT_DEBUG("server: error\n")
		return;
	}

	while (self->data.maxConnections > self->nConnections) {
		int slot;
		int len;

		/* find next free slot */
		for (slot=0; slot<MAX_CONNECTIONS; ++slot)
			if (self->connections[slot].socket==INVALID_SOCKET)
				break;

		PRINT_DEBUG("server: accepting\n")
		len = sizeof(self->connections[slot].address);
		self->connections[slot].socket = accept(
				self->socket,
				(struct sockaddr*) &self->connections[slot].address,
				&len);

		if (self->connections[slot].socket == INVALID_SOCKET) {
			/* TODO: error handling */
			PRINT_DEBUG("server: accept interrupted, exiting\n")
			return;
		}

		PRINT_DEBUG("server: accepted new client, starting read thread\n")
		self->connections[slot].channel = slot;

		etThread_construct(
				&self->connections[slot].readThread,
				STACK_SIZE,
				PRIO,
				"etSocketServer",
				readThreadFunc,
				&self->connections[slot]);
		etThread_start(&self->connections[slot].readThread);
	}

	/* TODO: if maxConnections is reached this thread terminates.
	 * Should we wait until a connection is closed and accept again?
	 */

	PRINT_DEBUG("server: exiting listener thread\n")

	/* exiting: see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682659%28v=vs.85%29.aspx */
}

etSocketError etInitSockets(void) {
	WSADATA wsaData;

	PRINT_DEBUG("sockets: init\n")
	if (( WSAStartup(0x202, &wsaData)) != 0) {
		WSACleanup();
		return ETSOCKET_ERROR;
	}
	return ETSOCKET_OK;
}

etSocketError etCleanupSockets(void) {
	PRINT_DEBUG("sockets: clean-up\n")
	WSACleanup();
	return ETSOCKET_OK;
}

etSocketServerData* etCreateSocketServerData(void) {
	etSocketServerDataImpl* data = malloc(sizeof(etSocketServerDataImpl));
	memset(data, 0, sizeof(etSocketServerDataImpl));
	data->data.maxConnections = MAX_CONNECTIONS;
	return &data->data;
}

void etFreeSocketServerData(etSocketServerData* data) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) data;
	free(self);
}

etSocketError etStartListening(etSocketServerData* data, unsigned short port) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) data;
	struct sockaddr_in local;
	int i;

	if (self==NULL)
		return ETSOCKET_ERROR;

	if (self->data.maxConnections>MAX_CONNECTIONS)
		return ETSOCKET_ERROR;

	/* mark all connections unused and set receiver and buffer provider */
	for (i=0; i<MAX_CONNECTIONS; ++i) {
		self->connections[i].socket = INVALID_SOCKET;
		self->connections[i].data.receiver = self->data.receiver;
		self->connections[i].data.bufferProvider = self->data.bufferProvider;
		self->connections[i].data.userData = self->data.userData;
	}
	self->nConnections = 0;

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;

	local.sin_port = htons(port);

	self->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (self->socket == INVALID_SOCKET)
		return ETSOCKET_ERROR;

	if (bind(self->socket, (struct sockaddr*) &local, sizeof(local)) == SOCKET_ERROR)
		return ETSOCKET_ERROR;

	PRINT_DEBUG("server: starting listener thread\n")
	etThread_construct(
			&self->listenerThread,
			STACK_SIZE,
			PRIO,
			"etSocketServer",
			listenerThreadFunc,
			self);
	etThread_start(&self->listenerThread);

	return ETSOCKET_OK;
}

etSocketError etStopSocketServer(etSocketServerData* data) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) data;
	PRINT_DEBUG("server: stop\n")
	closesocket(self->socket);
	return ETSOCKET_OK;
}

etSocketError etWriteServerSocket(etSocketServerData* dat, int connection, size_t size, const int8* data) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) dat;
	size_t offset = 0;

	if (connection<0 || connection>MAX_CONNECTIONS || self->connections[connection].socket==INVALID_SOCKET) {
		PRINT_DEBUG("connection error\n");
		return ETSOCKET_ERROR;
	}

	/* Note: loop required because:
	 * If no error occurs, send returns the total number of bytes sent, which can be less than the number
	 * requested to be sent in the len parameter.
	 * http://msdn.microsoft.com/en-us/library/windows/desktop/ms740149%28v=vs.85%29.aspx
	 */

	while (size>0) {
		int sent = send(self->connections[connection].socket, (char*)(((int8*)data)+offset), (int)size, 0);

		if (sent<=0)
			return ETSOCKET_ERROR;

		offset += (size_t)sent;
		size -= (size_t)sent;
	}

	return ETSOCKET_OK;
}

etSocketError etCloseServerSocket(etSocketServerData* data, int connection) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) data;

	if (self->connections[connection].socket!=INVALID_SOCKET) {
		PRINT_DEBUG("server: close connection\n")
		closesocket(self->connections[connection].socket);
		self->connections[connection].socket = INVALID_SOCKET;
	}

	return ETSOCKET_OK;
}

etSocketError etCloseAllServerSockets(etSocketServerData* data) {
	etSocketServerDataImpl* self = (etSocketServerDataImpl*) data;
	int i;

	PRINT_DEBUG("server: close all connections\n")
	for (i=0; i<MAX_CONNECTIONS; ++i) {
		if (self->connections[i].socket!=INVALID_SOCKET) {
			closesocket(self->connections[i].socket);
			self->connections[i].socket = INVALID_SOCKET;
		}
	}

	return ETSOCKET_OK;
}

etSocketConnectionData* etCreateSocketConnectionData(void) {
	etSocketConnectionDataImpl* data = malloc(sizeof(etSocketConnectionDataImpl));
	memset(data, 0, sizeof(etSocketConnectionDataImpl));
	return &data->data;
}

void etFreeSocketConnectionData(etSocketConnectionData* data) {
	etSocketConnectionDataImpl* self = (etSocketConnectionDataImpl*) data;
	free(self);
}

etSocketError etConnectServer(etSocketConnectionData* data, const char* addr, unsigned short port) {
	etSocketConnectionDataImpl* self = (etSocketConnectionDataImpl*) data;
	struct hostent *host;

	if (addr==NULL)
		addr = LOCAL_HOST;

	if (isalpha(addr[0])) {
		host = gethostbyname(addr);
	}
	else {
		unsigned long a = inet_addr(addr);
		host = gethostbyaddr((char *)&a, 4, AF_INET);
	}

	if (host == NULL )
		return ETSOCKET_ERROR;

	memset(&self->address, 0, sizeof(self->address));
	memcpy(&(self->address.sin_addr), host->h_addr, (size_t)host->h_length);
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsign-conversion"
		self->address.sin_family = host->h_addrtype;
	#pragma GCC diagnostic pop
	
	self->address.sin_port = htons(port);

	self->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (self->socket==INVALID_SOCKET)
		return ETSOCKET_ERROR;

	PRINT_DEBUG("client: connecting\n")
	if (connect(self->socket, (struct sockaddr*)&(self->address), sizeof(self->address)) == SOCKET_ERROR)
		return ETSOCKET_ERROR;

	PRINT_DEBUG("client: connected\n")
	PRINT_DEBUG("client: starting read thread\n")
	etThread_construct(
			&self->readThread,
			STACK_SIZE,
			PRIO,
			"etSocketConnection",
			readThreadFunc,
			self);
	etThread_start(&self->readThread);

	return ETSOCKET_OK;
}

etSocketError etWriteSocket(etSocketConnectionData* dat, size_t size, const int8* data) {
	etSocketConnectionDataImpl* self = (etSocketConnectionDataImpl*) dat;
	size_t offset = 0;

	while (size>0) {
		int sent = send(self->socket, (char*)(((int8*)data)+offset), (int)size, 0);

		if (sent<=0)
			return ETSOCKET_ERROR;

		offset += (size_t)sent;
		size -= (size_t)sent;
	}

	return ETSOCKET_OK;
}

etSocketError etCloseSocket(etSocketConnectionData* data) {
	etSocketConnectionDataImpl* self = (etSocketConnectionDataImpl*) data;

	closesocket(self->socket);

	return ETSOCKET_OK;
}

