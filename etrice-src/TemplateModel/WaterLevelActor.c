/**
 * @author generated by eTrice
 *
 * Source File of ActorClass WaterLevelActor
 *
 */

#include "WaterLevelActor.h"

#include "debugging/etLogger.h"
#include "debugging/etMSCLogger.h"
#include "etUnit/etUnit.h"
#include "base/etMemory.h"
#ifdef ET_MSC_TRACER_ACTIVATE
	#include "debugging/etMSCTracer.h"
#endif

#include "TemplateModel/flLvlInfo.h"
#include "etrice/api/timer/PTimer.h"

#include "WaterLevelActor_Utils.h"


/* interface item IDs */
enum interface_items {
	IFITEM_sendFluelLevel = 1,
	IFITEM_timer = 2
};

/* state IDs */
enum state_ids {
	NO_STATE = 0,
	STATE_TOP = 1,
	STATE_measure = 2,
	STATE_MAX = 3
};

/* transition chains */
enum ChainIDs {
	CHAIN_TRANS_INITIAL_TO__measure = 1,
	CHAIN_TRANS_tr0_FROM_measure_TO_measure_BY_timeouttimer_tr0 = 2
};

/* triggers */
enum triggers {
	POLLING = 0,
	TRIG_timer__timeout = IFITEM_timer + EVT_SHIFT*PTimer_OUT_timeout,
	TRIG_timer___timeout = IFITEM_timer + EVT_SHIFT*PTimer_OUT__timeout
};

/* state names */
#ifdef ET_MSC_LOGGER_ACTIVATE
	static const char* stateStrings[] = {"<no state>","<top>","measure"
	};
#endif

static void setState(WaterLevelActor* self, etInt16 new_state) {
	self->state = new_state;
	ET_MSC_LOGGER_CHANGE_STATE(self->constData->instName, stateStrings[new_state]);
}

static etInt16 getState(WaterLevelActor* self) {
	return self->state;
}

/* Entry and Exit Codes */
static void entry_measure(WaterLevelActor* self) {
	float32 waterLevel;
	if (xSemaphoreTake(adcMutex, (TickType_t)10) == pdTRUE)
	{
	            adc_select_input(0);
	            waterLevel = (float32)adc_read() * (3.3f / (1 << 12));
	            xSemaphoreGive(adcMutex);
	}
	sendFluelLevel_value(waterLevel);
}

/* Action Codes */
static void action_TRANS_INITIAL_TO__measure(WaterLevelActor* self) {
	initMutex();
						timer_startTimer(150);
}

/* State Switch Methods */
/**
 * calls exit codes while exiting from the current state to one of its
 * parent states while remembering the history
 * @param current__et - the current state
 * @param to - the final parent state
 */
static void exitTo(WaterLevelActor* self, etInt16 current__et, etInt16 to) {
	while (current__et!=to) {
		switch (current__et) {
			case STATE_measure:
				self->history[STATE_TOP] = STATE_measure;
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
static etInt16 executeTransitionChain(WaterLevelActor* self, int chain__et, const InterfaceItemBase* ifitem, void* generic_data__et) {
	switch (chain__et) {
		case CHAIN_TRANS_INITIAL_TO__measure:
		{
			action_TRANS_INITIAL_TO__measure(self);
			return STATE_measure;
		}
		case CHAIN_TRANS_tr0_FROM_measure_TO_measure_BY_timeouttimer_tr0:
		{
			return STATE_measure;
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
static etInt16 enterHistory(WaterLevelActor* self, etInt16 state__et) {
	etBool skip_entry__et = ET_FALSE;
	if (state__et >= STATE_MAX) {
		state__et = (etInt16) (state__et - STATE_MAX);
		skip_entry__et = ET_TRUE;
	}
	while (ET_TRUE) {
		switch (state__et) {
			case STATE_measure:
				if (!(skip_entry__et)) entry_measure(self);
				/* in leaf state: return state id */
				return STATE_measure;
			case STATE_TOP:
				state__et = self->history[STATE_TOP];
				break;
			default:
				/* should not occur */
				break;
		}
		skip_entry__et = ET_FALSE;
	}
	/* return NO_STATE; // required by CDT but detected as unreachable by JDT because of while (true) */
}

static void WaterLevelActor_executeInitTransition(WaterLevelActor* self) {
	int chain__et = CHAIN_TRANS_INITIAL_TO__measure;
	etInt16 next__et = executeTransitionChain(self, chain__et, NULL, NULL);
	next__et = enterHistory(self, next__et);
	setState(self, next__et);
}

/* receiveEvent contains the main implementation of the FSM */
static void WaterLevelActor_receiveEventInternal(WaterLevelActor* self, InterfaceItemBase* ifitem, int localId, int evt, void* generic_data__et) {
	int trigger__et = localId + EVT_SHIFT*evt;
	int chain__et = NOT_CAUGHT;
	etInt16 catching_state__et = NO_STATE;
	((void)trigger__et);	/* avoids unused warning */

	if (!handleSystemEvent(ifitem, evt, generic_data__et)) {
		switch (getState(self)) {
			case STATE_measure:
				switch(trigger__et) {
					case TRIG_timer__timeout:
						{
							chain__et = CHAIN_TRANS_tr0_FROM_measure_TO_measure_BY_timeouttimer_tr0;
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
static void WaterLevelActor_receiveEvent(WaterLevelActor* self, InterfaceItemBase* ifitem, int evt, void* generic_data__et) {
	int localId = (ifitem==NULL)? 0 : ifitem->localId;
	WaterLevelActor_receiveEventInternal(self, ifitem, localId, evt, generic_data__et);
}

void WaterLevelActor_init(WaterLevelActor* self){
	ET_MSC_LOGGER_SYNC_ENTRY("WaterLevelActor", "init")
	self->state = STATE_TOP;
	{
		int i;
		for (i=0; i<WATERLEVELACTOR_HISTORY_SIZE; ++i)
			self->history[i] = NO_STATE;
	}
	WaterLevelActor_executeInitTransition(self);
	ET_MSC_LOGGER_SYNC_EXIT
}


void WaterLevelActor_receiveMessage(void* self, const void* ifitem, const etMessage* msg){
	ET_MSC_LOGGER_SYNC_ENTRY("WaterLevelActor", "_receiveMessage")
	WaterLevelActor_receiveEvent((WaterLevelActor*) self, (etPort*)ifitem, msg->evtID, (void*)(((char*)msg)+MEM_CEIL(sizeof(etMessage))));

	ET_MSC_LOGGER_SYNC_EXIT
}


/*--------------------- user constructor/destructor ---------------------*/

/*--------------------- operations ---------------------*/

