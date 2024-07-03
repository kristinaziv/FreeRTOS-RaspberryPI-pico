/*******************************************************************************
 * Copyright (c) 2011 protos software gmbh (http://www.protos.de).
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * CONTRIBUTORS:
 * 		Thomas Schuetz (initial contribution)
 *
 *******************************************************************************/

#ifndef _ETQUEUE_H_
#define _ETQUEUE_H_

#include <stddef.h>
#include "etDatatypes.h"

ET_EXTERN_C_BEGIN

typedef struct etQueueObj {
	struct etQueueObj* next;
} etQueueObj;

typedef struct etQueue {
	etQueueObj* first;
	etQueueObj* last;
	size_t size;
} etQueue;

void etQueue_init(etQueue* self);

void etQueue_push(etQueue* self, etQueueObj* obj);

etQueueObj* etQueue_pop(etQueue* self);

etQueueObj* etQueue_getFirst(etQueue* self);

etQueueObj* etQueue_getLast(etQueue* self);

etBool etQueue_isNotEmpty(etQueue* self);

size_t etQueue_getSize(etQueue* self);

ET_EXTERN_C_END

#endif /* _ETQUEUE_H_ */
