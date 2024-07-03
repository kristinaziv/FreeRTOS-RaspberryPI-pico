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
 * 		Thomas Schuetz, Thomas Jung (initial contribution)
 *
 *******************************************************************************/

/**
 *
 * etSema.c MinGW implementation of etSema
 *
 */

#include "osal/etSema.h"
#include "etDatatypes.h"

#include "debugging/etMSCLogger.h"


void etSema_construct(etSema* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etSema", "construct")
	self->osData = CreateEvent( NULL, ET_FALSE, ET_FALSE, NULL );
	ET_MSC_LOGGER_SYNC_EXIT
}
void etSema_destruct(etSema* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etSema", "destruct")
	CloseHandle(self->osData);
	ET_MSC_LOGGER_SYNC_EXIT
}

void etSema_wakeupFromISR(etSema* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etSema", "wakeupFromISR")
	ET_TOUCH(self);
	ET_MSC_LOGGER_SYNC_EXIT
}

void etSema_wakeup(etSema* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etSema", "wakeup")
	SetEvent( self->osData );
	ET_MSC_LOGGER_SYNC_EXIT
}

void etSema_waitForWakeup(etSema* self){
	ET_MSC_LOGGER_SYNC_ENTRY("etSema", "waitForWakeup")
	WaitForSingleObject( self->osData, INFINITE );
	ET_MSC_LOGGER_SYNC_EXIT
}
