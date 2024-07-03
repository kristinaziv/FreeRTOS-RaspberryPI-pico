/**
 * @author generated by eTrice
 *
 * Source File of ProtocolClass PLoggerFlush
 * 
 */

#include "PLoggerFlush.h"
#include "debugging/etMSCLogger.h"
#include "PLoggerFlush_Utils.h"


/*--------------------- port methods */

void PLoggerFlushPort_done(const PLoggerFlushPort* self) {
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushPort", "done")
		etPort_sendMessage(self, PLoggerFlush_OUT_done, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(self->myInstName, "done", self->peerInstName)
	ET_MSC_LOGGER_SYNC_EXIT
}

void PLoggerFlushReplPort_done_broadcast(const PLoggerFlushReplPort* self) {
	int i;
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushReplPort", "done")
	for (i=0; i<((etReplPort*)self)->size; ++i) {
		etPort_sendMessage((&((etReplPort*)self)->ports[i].port), PLoggerFlush_OUT_done, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[i].port.myInstName, "done", ((etReplPort*)self)->ports[i].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

void PLoggerFlushReplPort_done(const PLoggerFlushReplPort* self, int idx__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushReplPort", "done")
	if (0<=idx__et && idx__et<((etReplPort*)self)->size) {
		etPort_sendMessage((&((etReplPort*)self)->ports[idx__et].port), PLoggerFlush_OUT_done, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[idx__et].port.myInstName, "done", ((etReplPort*)self)->ports[idx__et].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

etInt32 PLoggerFlushReplPort_getReplication(const PLoggerFlushReplPort* self) {
	return ((etReplPort*)self)->size;
}



void PLoggerFlushConjPort_flush(const PLoggerFlushConjPort* self) {
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushConjPort", "flush")
		etPort_sendMessage(self, PLoggerFlush_IN_flush, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(self->myInstName, "flush", self->peerInstName)
	ET_MSC_LOGGER_SYNC_EXIT
}

void PLoggerFlushConjReplPort_flush_broadcast(const PLoggerFlushConjReplPort* self) {
	int i;
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushConjReplPort", "flush")
	for (i=0; i<((etReplPort*)self)->size; ++i) {
		etPort_sendMessage((&((etReplPort*)self)->ports[i].port), PLoggerFlush_IN_flush, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[i].port.myInstName, "flush", ((etReplPort*)self)->ports[i].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

void PLoggerFlushConjReplPort_flush(const PLoggerFlushConjReplPort* self, int idx__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("PLoggerFlushConjReplPort", "flush")
	if (0<=idx__et && idx__et<((etReplPort*)self)->size) {
		etPort_sendMessage((&((etReplPort*)self)->ports[idx__et].port), PLoggerFlush_IN_flush, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[idx__et].port.myInstName, "flush", ((etReplPort*)self)->ports[idx__et].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

etInt32 PLoggerFlushConjReplPort_getReplication(const PLoggerFlushConjReplPort* self) {
	return ((etReplPort*)self)->size;
}



/*--------------------- debug helpers */
#ifdef ET_ASYNC_MSC_LOGGER_ACTIVATE
	
	/* message names as strings for debugging (generate MSC) */
	static const char* const PLoggerFlush_messageStrings[] = {"MIN", "done","flush", "MAX"};
	
	const char* PLoggerFlush_getMessageString(int msg_id) {
		if (msg_id<PLoggerFlush_MSG_MIN || msg_id>PLoggerFlush_MSG_MAX+1){
			/* id out of range */
			return "Message ID out of range";
		}
		else{
			return PLoggerFlush_messageStrings[msg_id];
		}
	}
#endif
