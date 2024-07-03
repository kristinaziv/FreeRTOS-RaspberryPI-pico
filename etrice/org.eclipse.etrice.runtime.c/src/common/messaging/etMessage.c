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

#include "messaging/etMessage.h"

#include "debugging/etMSCLogger.h"

#include <stddef.h>

void etMessage_init(etMessage* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etMessage", "init")
	self->next = NULL;
	self->address = 0;
	self->evtID = 0;
	ET_MSC_LOGGER_SYNC_EXIT
}
