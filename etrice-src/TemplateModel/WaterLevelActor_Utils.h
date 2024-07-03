/**
 * @author generated by eTrice
 *
 * Utils File of ActorClass WaterLevelActor
 *
 */

#ifndef _TEMPLATEMODEL_WATERLEVELACTOR_UTILS_H_
#define _TEMPLATEMODEL_WATERLEVELACTOR_UTILS_H_

#include "TemplateModel/WaterLevelActor.h"

/*
 * access macros for ports, operations and attributes
*/

/* simple event ports */
#define sendFluelLevel_value(data) flLvlInfoConjPort_value(&self->constData->sendFluelLevel, data)

/* data receive ports */

/* data send ports */

/* saps */
#define timer_startTimer(data) PTimerConjPort_startTimer(&self->constData->timer, data)
#define timer_startTimeout(data) PTimerConjPort_startTimeout(&self->constData->timer, data)
#define timer_kill() PTimerConjPort_kill(&self->constData->timer)
#define timer__startTimer(data) PTimerConjPort__startTimer(&self->constData->timer, data)
#define timer__startTimeout(data) PTimerConjPort__startTimeout(&self->constData->timer, data)

/* replicated event ports */

/* services */

/* operations */

/* attributes */

/* port operations */

#endif /* _TEMPLATEMODEL_WATERLEVELACTOR_UTILS_H_ */

