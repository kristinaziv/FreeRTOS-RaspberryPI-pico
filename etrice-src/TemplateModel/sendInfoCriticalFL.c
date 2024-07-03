/**
 * @author generated by eTrice
 *
 * Source File of ProtocolClass sendInfoCriticalFL
 * 
 */

#include "sendInfoCriticalFL.h"
#include "debugging/etMSCLogger.h"
#include "sendInfoCriticalFL_Utils.h"


/*--------------------- port methods */
etInt32 sendInfoCriticalFLReplPort_getReplication(const sendInfoCriticalFLReplPort* self) {
	return ((etReplPort*)self)->size;
}



void sendInfoCriticalFLConjPort_criticalOk(const sendInfoCriticalFLConjPort* self) {
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjPort", "criticalOk")
		etPort_sendMessage(self, sendInfoCriticalFL_IN_criticalOk, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(self->myInstName, "criticalOk", self->peerInstName)
	ET_MSC_LOGGER_SYNC_EXIT
}

void sendInfoCriticalFLConjReplPort_criticalOk_broadcast(const sendInfoCriticalFLConjReplPort* self) {
	int i;
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjReplPort", "criticalOk")
	for (i=0; i<((etReplPort*)self)->size; ++i) {
		etPort_sendMessage((&((etReplPort*)self)->ports[i].port), sendInfoCriticalFL_IN_criticalOk, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[i].port.myInstName, "criticalOk", ((etReplPort*)self)->ports[i].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

void sendInfoCriticalFLConjReplPort_criticalOk(const sendInfoCriticalFLConjReplPort* self, int idx__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjReplPort", "criticalOk")
	if (0<=idx__et && idx__et<((etReplPort*)self)->size) {
		etPort_sendMessage((&((etReplPort*)self)->ports[idx__et].port), sendInfoCriticalFL_IN_criticalOk, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[idx__et].port.myInstName, "criticalOk", ((etReplPort*)self)->ports[idx__et].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}


void sendInfoCriticalFLConjPort_criticalNOK(const sendInfoCriticalFLConjPort* self) {
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjPort", "criticalNOK")
		etPort_sendMessage(self, sendInfoCriticalFL_IN_criticalNOK, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(self->myInstName, "criticalNOK", self->peerInstName)
	ET_MSC_LOGGER_SYNC_EXIT
}

void sendInfoCriticalFLConjReplPort_criticalNOK_broadcast(const sendInfoCriticalFLConjReplPort* self) {
	int i;
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjReplPort", "criticalNOK")
	for (i=0; i<((etReplPort*)self)->size; ++i) {
		etPort_sendMessage((&((etReplPort*)self)->ports[i].port), sendInfoCriticalFL_IN_criticalNOK, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[i].port.myInstName, "criticalNOK", ((etReplPort*)self)->ports[i].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

void sendInfoCriticalFLConjReplPort_criticalNOK(const sendInfoCriticalFLConjReplPort* self, int idx__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("sendInfoCriticalFLConjReplPort", "criticalNOK")
	if (0<=idx__et && idx__et<((etReplPort*)self)->size) {
		etPort_sendMessage((&((etReplPort*)self)->ports[idx__et].port), sendInfoCriticalFL_IN_criticalNOK, 0, NULL);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[idx__et].port.myInstName, "criticalNOK", ((etReplPort*)self)->ports[idx__et].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

etInt32 sendInfoCriticalFLConjReplPort_getReplication(const sendInfoCriticalFLConjReplPort* self) {
	return ((etReplPort*)self)->size;
}



/*--------------------- debug helpers */
#ifdef ET_ASYNC_MSC_LOGGER_ACTIVATE
	
	/* message names as strings for debugging (generate MSC) */
	static const char* const sendInfoCriticalFL_messageStrings[] = {"MIN", "criticalOk", "criticalNOK", "MAX"};
	
	const char* sendInfoCriticalFL_getMessageString(int msg_id) {
		if (msg_id<sendInfoCriticalFL_MSG_MIN || msg_id>sendInfoCriticalFL_MSG_MAX+1){
			/* id out of range */
			return "Message ID out of range";
		}
		else{
			return sendInfoCriticalFL_messageStrings[msg_id];
		}
	}
#endif
