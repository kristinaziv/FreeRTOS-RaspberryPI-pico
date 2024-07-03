/**
 * @author generated by eTrice
 *
 * Utils File of ActorClass ATimerService
 *
 */

#ifndef _ETRICE_API_TIMER_ATIMERSERVICE_UTILS_H_
#define _ETRICE_API_TIMER_ATIMERSERVICE_UTILS_H_

#include "etrice/api/timer/ATimerService.h"

/*
 * access macros for ports, operations and attributes
*/

/* simple event ports */

/* data receive ports */

/* data send ports */

/* saps */

/* replicated event ports */

/* services */
#define timer_timeout_broadcast() PTimerReplPort_timeout_broadcast(&self->constData->timer)
#define timer_timeout(idx) PTimerReplPort_timeout(&self->constData->timer, idx)
#define timer__timeout_broadcast(data) PTimerReplPort__timeout_broadcast(&self->constData->timer, data)
#define timer__timeout(idx, data) PTimerReplPort__timeout(&self->constData->timer, idx, data)
#define supervisionControl_timerDeviationHighWaterMark_broadcast(data) PTimerSupervisionControlReplPort_timerDeviationHighWaterMark_broadcast(&self->constData->supervisionControl, data)
#define supervisionControl_timerDeviationHighWaterMark(idx, data) PTimerSupervisionControlReplPort_timerDeviationHighWaterMark(&self->constData->supervisionControl, idx, data)

/* operations */
#define putTcbToUsedList(block) ATimerService_putTcbToUsedList(self, block)
#define queueTcb(ifitem, startTimer, periodic) ATimerService_queueTcb(self, ifitem, startTimer, periodic)
#define getTime(t) ATimerService_getTime(self, t)
#define removeTcbFromUsedList(idx) ATimerService_removeTcbFromUsedList(self, idx)

/* attributes */
#define usedTcbsRoot (self->usedTcbsRoot)
#define timerDeviationHighWaterMark (self->timerDeviationHighWaterMark)

/* port operations */

#endif /* _ETRICE_API_TIMER_ATIMERSERVICE_UTILS_H_ */

