/**
 * @author generated by eTrice
 *
 * Utils File of ActorClass NotifyOperatorWLCmd
 *
 */

#ifndef _TEMPLATEMODEL_NOTIFYOPERATORWLCMD_UTILS_H_
#define _TEMPLATEMODEL_NOTIFYOPERATORWLCMD_UTILS_H_

#include "TemplateModel/NotifyOperatorWLCmd.h"

/*
 * access macros for ports, operations and attributes
*/

/* simple event ports */
#define sendCritical_criticalOk() sendInfoCriticalFLConjPort_criticalOk(&self->constData->sendCritical)
#define sendCritical_criticalNOK() sendInfoCriticalFLConjPort_criticalNOK(&self->constData->sendCritical)

/* data receive ports */

/* data send ports */

/* saps */

/* replicated event ports */

/* services */

/* operations */

/* attributes */
#define criticalCount (self->criticalCount)
#define threshold (self->threshold)
#define wltreshold (self->wltreshold)
#define okreported (self->okreported)
#define criticalReportedWl (self->criticalReportedWl)

/* port operations */

#endif /* _TEMPLATEMODEL_NOTIFYOPERATORWLCMD_UTILS_H_ */

