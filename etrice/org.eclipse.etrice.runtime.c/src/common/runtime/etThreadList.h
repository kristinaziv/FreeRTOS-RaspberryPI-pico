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
 * 		Thomas Schuetz (initial contribution)
 *
 *******************************************************************************/

/**
 * \file etThreadList.h
 *
 * NOT IMPLEMENTED YET
 *
 * holds a complete list of all threads
 *
 */

#ifndef _etThreadController_H_
#define _etThreadController_H_

#include "osal/etThread.h"

ET_EXTERN_C_BEGIN

typedef struct etThreadController{
	etThread* threadList;	/**< list of all thread data (as array) **/
	int32 size;				/**< size of the list **/
}etThreadController;

int32 etThreadController_getNumberOfThreads(etThreadController* self);
void etThreadController_startAll(etThreadController* self);
void etThreadController_stopAll(etThreadController* self);
void etThreadController_getCurrentThreadId(void);

ET_EXTERN_C_END

#endif /* _etThreadController_H_ */
