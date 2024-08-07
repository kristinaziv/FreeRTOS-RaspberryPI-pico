/**
 * @author generated by eTrice
 *
 * Source File of ActorClass NotifyOperatorWLCmd
 *
 */

#include "NotifyOperatorWLCmd.h"

#include "debugging/etLogger.h"
#include "debugging/etMSCLogger.h"
#include "etUnit/etUnit.h"
#include "base/etMemory.h"
#ifdef ET_MSC_TRACER_ACTIVATE
	#include "debugging/etMSCTracer.h"
#endif

#include "TemplateModel/flLvlInfo.h"
#include "TemplateModel/sendInfoCriticalFL.h"

#include "NotifyOperatorWLCmd_Utils.h"


/* interface item IDs */
enum interface_items {
	IFITEM_receiveWLLevel = 1,
	IFITEM_sendCritical = 2
};

/* state IDs */
enum state_ids {
	NO_STATE = 0,
	STATE_TOP = 1,
	STATE_fuelLevelNotify = 2,
	STATE_MAX = 3
};

/* transition chains */
enum ChainIDs {
	CHAIN_TRANS_INITIAL_TO__fuelLevelNotify = 1,
	CHAIN_TRANS_tr0_FROM_fuelLevelNotify_TO_fuelLevelNotify_BY_valuereceiveWLLevel_tr0 = 2
};

/* triggers */
enum triggers {
	POLLING = 0,
	TRIG_receiveWLLevel__value = IFITEM_receiveWLLevel + EVT_SHIFT*flLvlInfo_IN_value
};

/* state names */
#ifdef ET_MSC_LOGGER_ACTIVATE
	static const char* stateStrings[] = {"<no state>","<top>","fuelLevelNotify"
	};
#endif

static void setState(NotifyOperatorWLCmd* self, etInt16 new_state) {
	self->state = new_state;
	ET_MSC_LOGGER_CHANGE_STATE(self->constData->instName, stateStrings[new_state]);
}

static etInt16 getState(NotifyOperatorWLCmd* self) {
	return self->state;
}

/* Entry and Exit Codes */

/* Action Codes */
static void action_TRANS_INITIAL_TO__fuelLevelNotify(NotifyOperatorWLCmd* self) {
	 criticalCount = 0;
	 threshold = 5;
	criticalReportedWl = false;
	 wltreshold = 0.5;
	okreported = false;
}
static void action_TRANS_tr0_FROM_fuelLevelNotify_TO_fuelLevelNotify_BY_valuereceiveWLLevel_tr0(NotifyOperatorWLCmd* self, const InterfaceItemBase* ifitem, float32 transitionData) {
	if (transitionData < wltreshold)
	        {
	            criticalCount++;
	            if (criticalCount >= threshold && !criticalReportedWl)
	            {
	                
	                lcd_print(0, "CR: fuelvl low");
	                gpio_put(15, 1);
	                printf("CRITICAL: Nivo goriva je ispod praga %f: %f\n", wltreshold, transitionData);
	                criticalReportedWl = true;
	                sendCritical_criticalNOK();
	            }
	        }
	        else
	        {
	            if (criticalReportedWl)
	            {
	                lcd_print(0, "Inf: fuelvl ok");
	                gpio_put(15, 0);
	                printf("INFO: Nivo goriva se stabilizovao! %f\n", transitionData);
	                criticalReportedWl = false;
	                sendCritical_criticalOk();
	            }
	            else
	            {
	                if (!okreported)
	                {
	                    lcd_print(0, "Inf: fuelvl ok");
	                    printf("INFO: Nivo goriva je stabilan! %f\n", transitionData);
	                    okreported = true;
	                }
	            }
	            criticalCount = 0;
	        }
}

/* State Switch Methods */
/**
 * calls exit codes while exiting from the current state to one of its
 * parent states while remembering the history
 * @param current__et - the current state
 * @param to - the final parent state
 */
static void exitTo(NotifyOperatorWLCmd* self, etInt16 current__et, etInt16 to) {
	while (current__et!=to) {
		switch (current__et) {
			case STATE_fuelLevelNotify:
				self->history[STATE_TOP] = STATE_fuelLevelNotify;
				current__et = STATE_TOP;
				break;
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
static etInt16 executeTransitionChain(NotifyOperatorWLCmd* self, int chain__et, const InterfaceItemBase* ifitem, void* generic_data__et) {
	switch (chain__et) {
		case CHAIN_TRANS_INITIAL_TO__fuelLevelNotify:
		{
			action_TRANS_INITIAL_TO__fuelLevelNotify(self);
			return STATE_fuelLevelNotify;
		}
		case CHAIN_TRANS_tr0_FROM_fuelLevelNotify_TO_fuelLevelNotify_BY_valuereceiveWLLevel_tr0:
		{
			float32 transitionData = *((float32*) generic_data__et);
			ET_TOUCH(transitionData);
			action_TRANS_tr0_FROM_fuelLevelNotify_TO_fuelLevelNotify_BY_valuereceiveWLLevel_tr0(self, ifitem, transitionData);
			return STATE_fuelLevelNotify;
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
 * @return - the ID of the final leaf state
 */
static etInt16 enterHistory(NotifyOperatorWLCmd* self, etInt16 state__et) {
	if (state__et >= STATE_MAX) {
		state__et = (etInt16) (state__et - STATE_MAX);
	}
	while (ET_TRUE) {
		switch (state__et) {
			case STATE_fuelLevelNotify:
				/* in leaf state: return state id */
				return STATE_fuelLevelNotify;
			case STATE_TOP:
				state__et = self->history[STATE_TOP];
				break;
			default:
				/* should not occur */
				break;
		}
	}
	/* return NO_STATE; // required by CDT but detected as unreachable by JDT because of while (true) */
}

static void NotifyOperatorWLCmd_executeInitTransition(NotifyOperatorWLCmd* self) {
	int chain__et = CHAIN_TRANS_INITIAL_TO__fuelLevelNotify;
	etInt16 next__et = executeTransitionChain(self, chain__et, NULL, NULL);
	next__et = enterHistory(self, next__et);
	setState(self, next__et);
}

/* receiveEvent contains the main implementation of the FSM */
static void NotifyOperatorWLCmd_receiveEventInternal(NotifyOperatorWLCmd* self, InterfaceItemBase* ifitem, int localId, int evt, void* generic_data__et) {
	int trigger__et = localId + EVT_SHIFT*evt;
	int chain__et = NOT_CAUGHT;
	etInt16 catching_state__et = NO_STATE;
	((void)trigger__et);	/* avoids unused warning */

	if (!handleSystemEvent(ifitem, evt, generic_data__et)) {
		switch (getState(self)) {
			case STATE_fuelLevelNotify:
				switch(trigger__et) {
					case TRIG_receiveWLLevel__value:
						{
							chain__et = CHAIN_TRANS_tr0_FROM_fuelLevelNotify_TO_fuelLevelNotify_BY_valuereceiveWLLevel_tr0;
							catching_state__et = STATE_TOP;
						}
					break;
					default:
						/* should not occur */
						break;
				}
				break;
			default:
				/* should not occur */
				break;
		}
	}
	if (chain__et != NOT_CAUGHT) {
		exitTo(self, getState(self), catching_state__et);
		{
			etInt16 next__et = executeTransitionChain(self, chain__et, ifitem, generic_data__et);
			next__et = enterHistory(self, next__et);
			setState(self, next__et);
		}
	}
}
static void NotifyOperatorWLCmd_receiveEvent(NotifyOperatorWLCmd* self, InterfaceItemBase* ifitem, int evt, void* generic_data__et) {
	int localId = (ifitem==NULL)? 0 : ifitem->localId;
	NotifyOperatorWLCmd_receiveEventInternal(self, ifitem, localId, evt, generic_data__et);
}

void NotifyOperatorWLCmd_init(NotifyOperatorWLCmd* self){
	ET_MSC_LOGGER_SYNC_ENTRY("NotifyOperatorWLCmd", "init")
	self->state = STATE_TOP;
	{
		int i;
		for (i=0; i<NOTIFYOPERATORWLCMD_HISTORY_SIZE; ++i)
			self->history[i] = NO_STATE;
	}
	NotifyOperatorWLCmd_executeInitTransition(self);
	ET_MSC_LOGGER_SYNC_EXIT
}


void NotifyOperatorWLCmd_receiveMessage(void* self, const void* ifitem, const etMessage* msg){
	ET_MSC_LOGGER_SYNC_ENTRY("NotifyOperatorWLCmd", "_receiveMessage")
	NotifyOperatorWLCmd_receiveEvent((NotifyOperatorWLCmd*) self, (etPort*)ifitem, msg->evtID, (void*)(((char*)msg)+MEM_CEIL(sizeof(etMessage))));

	ET_MSC_LOGGER_SYNC_EXIT
}


/*--------------------- user constructor/destructor ---------------------*/

/*--------------------- operations ---------------------*/

