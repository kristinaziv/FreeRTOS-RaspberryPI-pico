/*******************************************************************************
 * Copyright (c) 2012 protos software gmbh (http://www.protos.de).
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

/**
 * \file etMemory_FixedSize.h
 *
 * a simple memory management that uses equal sized chunks. The free chunks are maintained in
 * a \ref etQueue
 *
 * \author Thomas Schuetz, Henrik Rentz-Reichert
 */
#ifndef _ETMEMORY_FIXED_SIZE_H_
#define _ETMEMORY_FIXED_SIZE_H_

#include "base/etMemory.h"
#include "base/etQueue.h"

ET_EXTERN_C_BEGIN

/**
 * initializes the heap with a simple block management
 *
 * \param heap pointer to the heap to be managed
 * \param size the size in bytes of the heap
 * \param blockSize the size of the (equal sized) blocks
 *
 * \return the pointer to the initialized etMemory struct
 */
etMemory* etMemory_FixedSize_init(void* heap, size_t size, size_t blockSize);


// alloc and free
void* etMemory_FixedSize_alloc(etMemory* heap, size_t size);
void etMemory_FixedSize_free(etMemory* heap, void* obj);

/**
 * determines and returns the free memory of the heap
 *
 * \param mem pointer to the heap to be managed
 *
 * \return the free memory of the heap
 */
size_t etMemory_FixedSize_getFreeHeapMem(etMemory* mem);

ET_EXTERN_C_END

#endif /* _ETMEMORY_FIXED_SIZE_H_ */
