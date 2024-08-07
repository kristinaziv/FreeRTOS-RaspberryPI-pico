/**
 * @author generated by eTrice
 *
 * Source File of ProtocolClass fire
 * 
 */

#include "fire.h"
#include "debugging/etMSCLogger.h"
#include "fire_Utils.h"


/*--------------------- port methods */
etInt32 fireReplPort_getReplication(const fireReplPort* self) {
	return ((etReplPort*)self)->size;
}



void fireConjPort_fire(const fireConjPort* self, float32 data__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("fireConjPort", "fire")
		etPort_sendMessage(self, fire_IN_fire, sizeof(float32), &data__et);
		ET_MSC_LOGGER_ASYNC_OUT(self->myInstName, "fire", self->peerInstName)
	ET_MSC_LOGGER_SYNC_EXIT
}

void fireConjReplPort_fire_broadcast(const fireConjReplPort* self, float32 data__et) {
	int i;
	ET_MSC_LOGGER_SYNC_ENTRY("fireConjReplPort", "fire")
	for (i=0; i<((etReplPort*)self)->size; ++i) {
		etPort_sendMessage((&((etReplPort*)self)->ports[i].port), fire_IN_fire, sizeof(float32), &data__et);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[i].port.myInstName, "fire", ((etReplPort*)self)->ports[i].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

void fireConjReplPort_fire(const fireConjReplPort* self, int idx__et, float32 data__et) {
	ET_MSC_LOGGER_SYNC_ENTRY("fireConjReplPort", "fire")
	if (0<=idx__et && idx__et<((etReplPort*)self)->size) {
		etPort_sendMessage((&((etReplPort*)self)->ports[idx__et].port), fire_IN_fire, sizeof(float32), &data__et);
		ET_MSC_LOGGER_ASYNC_OUT(((etReplPort*)self)->ports[idx__et].port.myInstName, "fire", ((etReplPort*)self)->ports[idx__et].port.peerInstName)
	}
	ET_MSC_LOGGER_SYNC_EXIT
}

etInt32 fireConjReplPort_getReplication(const fireConjReplPort* self) {
	return ((etReplPort*)self)->size;
}



/*--------------------- debug helpers */
#ifdef ET_ASYNC_MSC_LOGGER_ACTIVATE
	
	/* message names as strings for debugging (generate MSC) */
	static const char* const fire_messageStrings[] = {"MIN", "fire", "MAX"};
	
	const char* fire_getMessageString(int msg_id) {
		if (msg_id<fire_MSG_MIN || msg_id>fire_MSG_MAX+1){
			/* id out of range */
			return "Message ID out of range";
		}
		else{
			return fire_messageStrings[msg_id];
		}
	}
#endif
