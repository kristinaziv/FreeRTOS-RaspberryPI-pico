/**
 * @author generated by eTrice
 *
 * Header File of ProtocolClass PLoggerFlush
 * 
 */

#ifndef _ETRICE_API_LOGGER_PLOGGERFLUSH_H_
#define _ETRICE_API_LOGGER_PLOGGERFLUSH_H_

#include "etDatatypes.h"
#include "modelbase/etPort.h"


/* include all referenced room classes */


/* message IDs */
enum PLoggerFlush_msg_ids {
	PLoggerFlush_MSG_MIN = 0,
	PLoggerFlush_OUT_done = 1,
	PLoggerFlush_IN_flush = 2,
	PLoggerFlush_MSG_MAX = 3
};

/*--------------------- port structs and methods */
typedef etPort PLoggerFlushPort;
typedef etReplPort PLoggerFlushReplPort;


void PLoggerFlushPort_done(const PLoggerFlushPort* self);
void PLoggerFlushReplPort_done_broadcast(const PLoggerFlushReplPort* self);
void PLoggerFlushReplPort_done(const PLoggerFlushReplPort* self, int idx);


etInt32 PLoggerFlushReplPort_getReplication(const PLoggerFlushReplPort* self);
typedef etPort PLoggerFlushConjPort;
typedef etReplPort PLoggerFlushConjReplPort;


void PLoggerFlushConjPort_flush(const PLoggerFlushConjPort* self);
void PLoggerFlushConjReplPort_flush_broadcast(const PLoggerFlushConjReplPort* self);
void PLoggerFlushConjReplPort_flush(const PLoggerFlushConjReplPort* self, int idx);


etInt32 PLoggerFlushConjReplPort_getReplication(const PLoggerFlushConjReplPort* self);

/*--------------------- debug helpers */

/* get message string for message id */
#ifdef ET_ASYNC_MSC_LOGGER_ACTIVATE
	const char* PLoggerFlush_getMessageString(int msg_id);
#endif


#endif /* _ETRICE_API_LOGGER_PLOGGERFLUSH_H_ */

