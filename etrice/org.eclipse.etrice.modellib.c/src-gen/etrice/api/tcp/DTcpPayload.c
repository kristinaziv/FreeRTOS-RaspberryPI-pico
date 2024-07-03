/**
 * @author generated by eTrice
 *
 * Source File of DataClass DTcpPayload
 * 
 */

#include <string.h>

#include "DTcpPayload.h"
#include "DTcpPayload_Utils.h"



/*--------------------- operations ---------------------*/
 uint32 DTcpPayload_getMaxLength(DTcpPayload* self) {
	return 32;
}
 void DTcpPayload_setData(DTcpPayload* self, int8* value, uint32 size, int32 channel) {
	/* used macros: data, length */
	strncpy((char*)data, (char*)value, ((size>32)?32:size));
	length = size;
	connectionId = channel;
}
 charPtr DTcpPayload_getAsString(DTcpPayload* self) {
	/* used macros: data */
	data[31] = '\0';
	return (charPtr)data;
}
 void DTcpPayload_setAsString(DTcpPayload* self, charPtr value) {
	/* used macros: data, length */
	size_t valLength = (strlen(value)+1 > 32)? 32 : strlen(value) + 1;
	memcpy(data, value, valLength);
	data[31] = '\0';
	length = (uint32)valLength;
}

void DTcpPayload_deepCopy(DTcpPayload* source, DTcpPayload* target) {
	memcpy(target, source, sizeof(DTcpPayload));
}

