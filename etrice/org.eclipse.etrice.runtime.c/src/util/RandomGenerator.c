/*******************************************************************************
 * Copyright (c) 2014 protos software gmbh (http://www.protos.de).
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

#include "RandomGenerator.h"

#ifdef ET_FLOAT64

#include "math.h"

void RandomGenerator_init(RandomGenerator* self, etFloat64 seed, etFloat64 min, etFloat64 max){
	self->seed = seed;
	self->min = min;
	self->max = max;
	self->range = max-min;
	self->current = seed;
}


etFloat64 RandomGenerator_getNext(RandomGenerator* self){
	etInt64 resultInt;
	self->seed = 1000. * fabs(log(self->seed)); 	/* shift comma 3 times to get nice values */
	resultInt = (etInt64) self->seed;    						/* get int number left of comma */
	self->seed = self->seed - (etFloat64)resultInt; 			/* cut off everything left of comma */
	self->current = self->seed*self->range + self->min; /* stretch to range */
	self->seed += 0.1; 								/* seed must always be bigger than 0 for log in next iteration */

	return self->current;
}

#endif
