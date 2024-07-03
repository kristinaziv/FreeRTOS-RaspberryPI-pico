/**
 * @author generated by eTrice
 *
 * Source File of ActorClass AContractMonitorBase
 *
 */

#include "AContractMonitorBase.h"

#include "debugging/etLogger.h"
#include "debugging/etMSCLogger.h"
#include "etUnit/etUnit.h"
#include "base/etMemory.h"
#ifdef ET_MSC_TRACER_ACTIVATE
	#include "debugging/etMSCTracer.h"
#endif

#include "etrice/api/contracts/monitors/PContractMonitorControl.h"

#include "AContractMonitorBase_Utils.h"


/* interface item IDs */
enum interface_items {
	IFITEM_monitor_ctrl = 1
};

/* state IDs */
enum state_ids {
	NO_STATE = 0,
	STATE_TOP = 1,
	STATE_MAX = 2
};

/* transition chains */
enum ChainIDs {
	CHAIN_TRANS__trStatus_FROM__trBase_TO__trBase_BY_getAndResetStatusmonitor_ctrl__trStatus = 1,
	CHAIN_TRANS__trForward_FROM__trBase_TO__trBase_BY_setForwardInvalidMessagesmonitor_ctrl__trForward = 2
};

/* triggers */
enum triggers {
	POLLING = 0,
	TRIG_monitor_ctrl__getAndResetStatus = IFITEM_monitor_ctrl + EVT_SHIFT*PContractMonitorControl_IN_getAndResetStatus,
	TRIG_monitor_ctrl__setForwardInvalidMessages = IFITEM_monitor_ctrl + EVT_SHIFT*PContractMonitorControl_IN_setForwardInvalidMessages
};

/* state names */
#ifdef ET_MSC_LOGGER_ACTIVATE
	static const char* stateStrings[] = {"<no state>","<top>",};
#endif

static void setState(AContractMonitorBase* self, etInt16 new_state) {
	self->state = new_state;
	ET_MSC_LOGGER_CHANGE_STATE(self->constData->instName, stateStrings[new_state]);
}

static etInt16 getState(AContractMonitorBase* self) {
	return self->state;
}

/* Entry and Exit Codes */

/* Action Codes */
static void action_TRANS__trStatus_FROM__trBase_TO__trBase_BY_getAndResetStatusmonitor_ctrl__trStatus(AContractMonitorBase* self, const InterfaceItemBase* ifitem) {
	monitor_ctrl_violationStatus(_violation);
	_violation = false;
}
static void action_TRANS__trForward_FROM__trBase_TO__trBase_BY_setForwardInvalidMessagesmonitor_ctrl__trForward(AContractMonitorBase* self, const InterfaceItemBase* ifitem, bool transitionData) {
	forwardInvalidMessages = transitionData;
}

/* State Switch Methods */
/**
 * calls exit codes while exiting from the current state to one of its
 * parent states while remembering the history
 * @param current__et - the current state
 * @param to - the final parent state
* @param handler__et - entry and exit codes are called only if not handler (for handler TransitionPoints)
 */
static void exitTo(AContractMonitorBase* self, etInt16 current__et, etInt16 to, etBool handler__et) {
	while (current__et!=to) {
		switch (current__et) {
			default:
				/* should not occur */
				break;
		}
	}
}

/**
 * calls action, entry and exit codes along a transition chain. The generic data are cast to typed data
 * matching the trigger of this chain. The ID of the final state is returned
 * @param chain__et - the chain ID
 * @param generic_data__et - the generic data pointer
 * @return the +/- ID of the final state either with a positive sign, that indicates to execute the state's entry code, or a negative sign vice versa
 */
static etInt16 executeTransitionChain(AContractMonitorBase* self, int chain__et, const InterfaceItemBase* ifitem, void* generic_data__et) {
	switch (chain__et) {
		case CHAIN_TRANS__trForward_FROM__trBase_TO__trBase_BY_setForwardInvalidMessagesmonitor_ctrl__trForward:
		{
			bool transitionData = *((bool*) generic_data__et);
			ET_TOUCH(transitionData);
			action_TRANS__trForward_FROM__trBase_TO__trBase_BY_setForwardInvalidMessagesmonitor_ctrl__trForward(self, ifitem, transitionData);
			return STATE_TOP + STATE_MAX;
		}
		case CHAIN_TRANS__trStatus_FROM__trBase_TO__trBase_BY_getAndResetStatusmonitor_ctrl__trStatus:
		{
			action_TRANS__trStatus_FROM__trBase_TO__trBase_BY_getAndResetStatusmonitor_ctrl__trStatus(self, ifitem);
			return STATE_TOP + STATE_MAX;
		}
			default:
				/* should not occur */
				break;
	}
	return NO_STATE;
}

/**
 * calls entry codes while entering a state's history. The ID of the final leaf state is returned
 * @param state__et - the state which is entered
* @param handler__et - entry code is executed if not handler
 * @return - the ID of the final leaf state
 */
static etInt16 enterHistory(AContractMonitorBase* self, etInt16 state__et, etBool handler__et) {
	return NO_STATE;
}

static void AContractMonitorBase_executeInitTransition(AContractMonitorBase* self) {
	/* no initial transition allowed for abstract actor class */
}

/* receiveEvent contains the main implementation of the FSM */
static void AContractMonitorBase_receiveEventInternal(AContractMonitorBase* self, InterfaceItemBase* ifitem, int localId, int evt, void* generic_data__et) {
	int trigger__et = localId + EVT_SHIFT*evt;
	int chain__et = NOT_CAUGHT;
	etInt16 catching_state__et = NO_STATE;
	etBool is_handler__et = ET_FALSE;
	((void)trigger__et);	/* avoids unused warning */

	if (!handleSystemEvent(ifitem, evt, generic_data__et)) {
		switch (getState(self)) {
			default:
				/* should not occur */
				break;
		}
	}
	if (chain__et != NOT_CAUGHT) {
		exitTo(self, getState(self), catching_state__et, is_handler__et);
		{
			etInt16 next__et = executeTransitionChain(self, chain__et, ifitem, generic_data__et);
			next__et = enterHistory(self, next__et, is_handler__et);
			setState(self, next__et);
		}
	}
}
static void AContractMonitorBase_receiveEvent(AContractMonitorBase* self, InterfaceItemBase* ifitem, int evt, void* generic_data__et) {
	int localId = (ifitem==NULL)? 0 : ifitem->localId;
	AContractMonitorBase_receiveEventInternal(self, ifitem, localId, evt, generic_data__et);
}

void AContractMonitorBase_init(AContractMonitorBase* self){
	ET_MSC_LOGGER_SYNC_ENTRY("AContractMonitorBase", "init")
	self->state = STATE_TOP;
	{
		int i;
		for (i=0; i<ACONTRACTMONITORBASE_HISTORY_SIZE; ++i)
			self->history[i] = NO_STATE;
	}
	AContractMonitorBase_executeInitTransition(self);
	ET_MSC_LOGGER_SYNC_EXIT
}


void AContractMonitorBase_receiveMessage(void* self, const void* ifitem, const etMessage* msg){
	ET_MSC_LOGGER_SYNC_ENTRY("AContractMonitorBase", "_receiveMessage")
	AContractMonitorBase_receiveEvent((AContractMonitorBase*) self, (etPort*)ifitem, msg->evtID, (void*)(((char*)msg)+MEM_CEIL(sizeof(etMessage))));

	ET_MSC_LOGGER_SYNC_EXIT
}


/*--------------------- user constructor/destructor ---------------------*/

/*--------------------- operations ---------------------*/
 void AContractMonitorBase_onViolation(AContractMonitorBase* self, charPtr comment) {
	_violation = true;
	ET_MSC_LOGGER_VISIBLE_COMMENT(comment);
	monitor_ctrl_violationOccured();
}
