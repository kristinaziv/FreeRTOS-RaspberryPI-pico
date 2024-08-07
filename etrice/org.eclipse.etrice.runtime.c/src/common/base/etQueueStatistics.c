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
 * 		Henrik Rentz-Reichert (initial contribution)
 *
 *******************************************************************************/

#include "etQueueStatistics.h"

void etQueueStatistics_init(etQueueStatistics* self) {
	self->highWaterMark = 0;
	self->lowWaterMark = 0;
	self->nFailingRequests = 0;
}

void etQueueStatistics_reset(etQueueStatistics* self, size_t lowWaterMark) {
	self->highWaterMark = 0;
	self->lowWaterMark = lowWaterMark;
	self->nFailingRequests = 0;
}
