/**
 * @author generated by eTrice
 *
 * Header File of ProtocolClass uwbInfo
 * 
 */

#ifndef _TEMPLATEMODEL_UWBINFO_H_
#define _TEMPLATEMODEL_UWBINFO_H_

#include "etDatatypes.h"
#include "modelbase/etPort.h"


/* include all referenced room classes */


/* message IDs */
enum uwbInfo_msg_ids {
	uwbInfo_MSG_MIN = 0,
	uwbInfo_IN_value = 1,
	uwbInfo_MSG_MAX = 2
};

/*--------------------- port structs and methods */
typedef etPort uwbInfoPort;
typedef etReplPort uwbInfoReplPort;




etInt32 uwbInfoReplPort_getReplication(const uwbInfoReplPort* self);
typedef etPort uwbInfoConjPort;
typedef etReplPort uwbInfoConjReplPort;


void uwbInfoConjPort_value(const uwbInfoConjPort* self, float32 data);
void uwbInfoConjReplPort_value_broadcast(const uwbInfoConjReplPort* self, float32 data);
void uwbInfoConjReplPort_value(const uwbInfoConjReplPort* self, int idx, float32 data);


etInt32 uwbInfoConjReplPort_getReplication(const uwbInfoConjReplPort* self);

/*--------------------- debug helpers */

/* get message string for message id */
#ifdef ET_ASYNC_MSC_LOGGER_ACTIVATE
	const char* uwbInfo_getMessageString(int msg_id);
#endif


#endif /* _TEMPLATEMODEL_UWBINFO_H_ */

