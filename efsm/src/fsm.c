/*------------------------------------------------------------------
 * fsm.c -- Finite State Machine 
 *
 * February 2005, Bo Berry
 *
 * Copyright (c) 2005-2009 by Cisco Systems, Inc.
 * All rights reserved. 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsm.h"


/*
 * These are the maximum states and events that fsm uses 
 * for sizing during create. 
 */ 
#define FSM_MAX_STATES  ( 64 ) 
#define FSM_MAX_EVENTS  ( 64 ) 



/** 
 * NAME
 *    fsm_display_table
 *
 * SYNOPSIS 
 *    #include "fsm.h" 
 *    void
 *    fsm_display_table(fsm_t *fsm)
 *
 * DESCRIPTION
 *    Displays the designated state machine table to console. 
 *
 * INPUT PARAMETERS
 *    fsm - handle to fsm
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none 
 * 
 */
void
fsm_display_table (fsm_t *fsm)
{
    uint32_t  i;
    uint32_t  j;
    state_tuple_t *state_ptr;
    event_tuple_t *event_ptr;

    state_description_t *p2state_description; 
    event_description_t *p2event_description;

    if (fsm == NULL) {
        return;
    }

    if (fsm->tag != FSM_TAG) {
        return;
    }

    p2state_description = fsm->state_description_table; 
    p2event_description = fsm->event_description_table; 

    printf("\nFSM: %s \n", fsm->fsm_name);
    printf("    number_states = %d\n", fsm->number_states);
    printf("    number_events = %d\n", fsm->number_events);
    printf("    curr_state = %s\n", 
              p2state_description[fsm->curr_state].description );
    printf("\n");

    /*
     * For the normalized state table, list the normalized 
     * events and state transitions.
     */ 
    for (i=0; i<fsm->number_states; i++) {

        state_ptr = &fsm->state_table[i];

        printf(" State: %s \n", 
                p2state_description[state_ptr->state_id].description);
        printf(" Event   /   Next State     \n");
        printf("----------------------------\n");

        for (j=0; j<fsm->number_events; j++) {

            event_ptr = &state_ptr->p2event_tuple[j];

            /*
             * Display the name of the state associated with the next state.
             */
            printf("  %u-%s / %s \n", 
                     j,  
                     p2event_description[j].description, 
                     p2state_description[event_ptr->next_state].description);
        }
        printf("\n");
    }
    printf("\n");
    return;
}


/** 
 * NAME
 *    fsm_show_history
 *
 * SYNOPSIS 
 *    #include "fsm.h' 
 *    void
 *    fsm_show_history(fsm_t *fsm)
 *
 * DESCRIPTION
 *    Displays history of the state transitions. 
 *
 * INPUT PARAMETERS
 *    *fsm - handle of the state machine 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
fsm_show_history (fsm_t *fsm)
{
    uint32_t         i, j;
    fsm_history_t  *history_ptr;

    state_description_t *p2state_description; 
    event_description_t *p2event_description;

    if (fsm == NULL) {
        return;
    }

    if (fsm->tag != FSM_TAG) {
        return;
    }

    p2state_description = fsm->state_description_table; 
    p2event_description = fsm->event_description_table; 

    printf("\nFSM: %s History \n", fsm->fsm_name);
    printf("Current State  /   Event   /  New State  /  rc  \n");
    printf("------------------------------------------------\n");

    j = fsm->history_index;
    for (i=0; i<FSM_HISTORY; i++) {

        /*
         * Get a local pointer to the history buffer
         */
        history_ptr = &fsm->history[i];

        if (history_ptr->stateID == FSM_NULL_STATE_ID) {
            continue;
        }

        printf(" %u-%s  /  %u-%s  /  %u-%s  /  %u\n",
             history_ptr->prevStateID,
             p2state_description[history_ptr->prevStateID].description,
             history_ptr->eventID, 
             p2event_description[history_ptr->eventID].description,
             history_ptr->stateID,
             p2state_description[history_ptr->stateID].description,
             history_ptr->handler_rc);

        if (j==0) {
            j=FSM_HISTORY;
        }
        j--;
    }

    printf("\n");
    return;
}


/** 
 * NAME
 *    fsm_get_state
 *
 * SYNOPSIS
 *    #include "fsm.h"
 *    RC_FSM_t
 *    fsm_get_state(fsm_t *fsm, uint32_t*p2state)
 *
 * DESCRIPTION
 *    Function to return the current state.
 *
 * INPUT PARAMETERS
 *    *fsm - state machine handle
 *
 *    p2state - Pointer to a state variable to be updated.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    error otherwise
 * 
 */
RC_FSM_t
fsm_get_state (fsm_t *fsm, uint32_t *p2state)
{
    if (fsm == NULL) {
        return (RC_FSM_NULL);
    }

    if (fsm->tag != FSM_TAG) {
        return (RC_FSM_INVALID_HANDLE);
    }

    *p2state = fsm->curr_state;
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    fsm_set_exception_state
 * 
 * SYNOPSIS
 *    #include "fsm.h"
 *    RC_FSM_t
 *    fsm_set_exception_state(fsm_t *fsm, 
 *                                uint32_t exception_state)
 *
 * DESCRIPTION
 *    To be called from an event handler to alter
 *    the next state from the event table when an
 *    exception has been detected
 *
 * INPUT PARAMETERS
 *    *fsm - state machine handle
 *
 *    exception_state - New state to be transitioned
 *            when the event handler returns.
 *
 * OUTPUT PARAMETERS
 *    none 
 * 
 * RETURN VALUE
 *    RC_FSM_OK
 *    error otherwise
 * 
 */
RC_FSM_t
fsm_set_exception_state (fsm_t *fsm, uint32_t exception_state)
{
    if (fsm == NULL) {
        return (RC_FSM_NULL);
    }

    if (fsm->tag != FSM_TAG) {
        return (RC_FSM_INVALID_HANDLE);
    }

    /*
     * Change the state of this FSM to the requested state.
     */
    if (exception_state > fsm->number_states-1) {
        return (RC_FSM_INVALID_STATE);
    }

    fsm->exception_state = exception_state;
    fsm->exception_state_indicator = TRUE;
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    fsm_destroy
 *
 * SYNOPSIS
 *    #include "fsm.h" 
 *    RC_FSM_t
 *    fsm_destroy(fsm_t **fsm)
 * 
 * DESCRIPTION
 *    Destroys the specified state machine.
 *
 * INPUT PARAMETERS
 *    fsm - pointer to fsm handle
 *
 * OUTPUT PARAMETERS
 *    fsm - is nulled
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    error otherwise
 * 
 */
RC_FSM_t
fsm_destroy (fsm_t **fsm)
{
     fsm_t *p2fsm;

     if (fsm == NULL) {
         return (RC_FSM_NULL);
     }

     p2fsm = *fsm;
     if (p2fsm->tag != FSM_TAG) {
         return (RC_FSM_INVALID_HANDLE);
     }

     free(p2fsm->history); 
     *fsm = NULL;
     free(p2fsm);
     return (RC_FSM_OK);
}


/** 
 * NAME
 *    fsm_create
 *
 * SYNOPSIS
 *    #include "fsm.h" 
 *    RC_FSM_t
 *    fsm_create(fsm_t **fsm,
 *               char *name,
 *               uint32_t initial_state,
 *               state_description_t *state_description_table,
 *               event_description_t *event_description_table,
 *               state_tuple_t *state_table) 
 *
 * DESCRIPTION
 *    Creates and initializes a state machine. The
 *    initial state is specified by the user.
 *
 * INPUT PARAMETERS
 *    fsm                pointer to fsm handle to be returned
 *                       once created
 *
 *    name               pointer to fsm name
 *
 *    initial_state      Initial start state
 *
 *    state_description_table
 *                       Pointer to the user table which
 *                       provides a description of each state. 
 *                       The table is used when displaying
 *                       state info to the console.   
 * 
 *    event_description_table
 *                       Pointer to the user table which
 *                       provides a description of each event. 
 *                       The table is used when displaying
 *                       state info to the console.   
 * 
 *    state_table        Pointer to user defined state
 *                       table.  The state table is indexed
 *                       by the normalized state ID, 0, 1, ...
 *                       Each state table tuple must reference
 *                       an event table.
 *
 * OUTPUT PARAMETERS
 *     none   
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    error otherwise
 * 
 */
RC_FSM_t
fsm_create (fsm_t **fsm,
            char *name,
            uint32_t initial_state,
            state_description_t *state_description_table,
            event_description_t *event_description_table,
            state_tuple_t *state_table)  
{
    fsm_t *temp_fsm;
    uint32_t i;
    uint32_t j;
    state_tuple_t *state_ptr;
    event_tuple_t *event_ptr;


    if (fsm == NULL) {
        return (RC_FSM_NULL);
    }

    if (state_description_table == NULL) {
        return (RC_FSM_INVALID_STATE_TABLE);
    }

    if (event_description_table == NULL) {
        return (RC_FSM_INVALID_EVENT_TABLE);
    }

    if (state_table == NULL) {
        return (RC_FSM_INVALID_STATE_TABLE);
    }


    /*
     * allocate memory to manage state machine
     */
    temp_fsm = (fsm_t *)malloc( sizeof(fsm_t) );
    if (temp_fsm == NULL) {
        return (RC_FSM_NO_RESOURCES);
    }

    /*
     * default a name if needed
     */
    if (name) {
        strncpy(temp_fsm->fsm_name, name, FSM_NAME_LEN);
    } else {
        strncpy(temp_fsm->fsm_name, "State Machine", FSM_NAME_LEN);
    }

    /*
     * initialize fsm config parms
     */
    temp_fsm->tag = FSM_TAG;    /* for sanity cchecks */

    temp_fsm->curr_state    = initial_state;
    temp_fsm->next_state    = initial_state;
    temp_fsm->exception_state_indicator = FALSE;

    /* save the event description table */ 
    temp_fsm->state_description_table = state_description_table;
    temp_fsm->event_description_table = event_description_table;

    /* save the pointer to the state table */
    temp_fsm->state_table = state_table;

    /*
     * Find the size of the state table
     */ 
    temp_fsm->number_states = 0;
    for (i=0; i<FSM_MAX_STATES; i++) {
        if (state_description_table[i].state_id != FSM_NULL_STATE_ID)  { 
#if 0
            printf("%u  %u  %u \n", 
                i,  
                state_description_table[i].state_id, 
                state_table[i].state_id); 
#endif
            if (state_description_table[i].state_id == i && 
                state_table[i].state_id == i && 
                state_table[i].p2event_tuple!= NULL)  {
                temp_fsm->number_states++;
            } else {
                free(temp_fsm); 
                return (RC_FSM_INVALID_STATE_TABLE);
            }   
        } else { 
            break;
        }
    }
    if (temp_fsm->number_states < 1 ||  
        temp_fsm->number_states > FSM_MAX_STATES-1) { 
        free(temp_fsm); 
        return (RC_FSM_INVALID_STATE_TABLE);
    } 
#if 0 
    printf("number states=%u \n", temp_fsm->number_states);
#endif

    /*
     * check zero based range for state
     */
    if (initial_state > temp_fsm->number_states-1) {
        free(temp_fsm); 
        return (RC_FSM_INVALID_STATE);
    }


    /*
     * Find the size of the event table
     */ 
    temp_fsm->number_events = 0;
    for (i=0; i<FSM_MAX_EVENTS; i++) {
        if (event_description_table[i].event_id == FSM_NULL_EVENT_ID)  {
            break;
        }
#if 0 
        printf(" %u event_id=%u \n",
                i,
                event_description_table[i].event_id);
#endif

        if (event_description_table[i].event_id != i)  {
            free(temp_fsm); 
            return (RC_FSM_INVALID_EVENT_TABLE); 
        }
        temp_fsm->number_events++;
    }
    if (temp_fsm->number_events < 1 ||  
        temp_fsm->number_events > FSM_MAX_EVENTS-1) { 
        free(temp_fsm); 
        return (RC_FSM_INVALID_EVENT_TABLE);
    } 
#if 0 
    printf("number events=%u \n", temp_fsm->number_events);
#endif

    /*
     * Now verify the state table - event table relationships and
     * that the IDs are normalized.   
     */
    for (i=0; i<temp_fsm->number_states; i++) {
        state_ptr = &temp_fsm->state_table[i];

        event_ptr = state_ptr->p2event_tuple;

        for (j=0; j<temp_fsm->number_events; j++) {
#if 0 
            printf(" %u eventID=%u \n",
                   j,
                   event_ptr[j].eventID);
#endif
            if (j != event_ptr[j].eventID) {
                free(temp_fsm); 
                return (RC_FSM_INVALID_EVENT_TABLE);
            }
        }
    }

    /*
     * allocate memory for history
     */ 
    temp_fsm->history = malloc(FSM_HISTORY * sizeof(fsm_history_t));
    if (temp_fsm->history == NULL) {
        free(temp_fsm);
        return (RC_FSM_NO_RESOURCES);
    }

    /*
     * initialize history buffer
     */
    temp_fsm->history_index = 0;
    for (i=0; i<FSM_HISTORY; i++) {
        temp_fsm->history[i].prevStateID = FSM_NULL_STATE_ID;
        temp_fsm->history[i].stateID = FSM_NULL_STATE_ID;
        temp_fsm->history[i].eventID = FSM_NULL_EVENT_ID;
        temp_fsm->history[i].handler_rc = RC_FSM_NULL;
    }

    /* return handle to the user */
    *fsm = temp_fsm;
    return (RC_FSM_OK);
}


/*
 * internal routine to record a state transition history
 */
static void
fsm_record_history (fsm_t *fsm,
                   uint32_t normalized_event,
                   uint32_t nextState,
                   RC_FSM_t handler_rc)
{
    fsm_history_t   *history_ptr;

    /*
     * get next index to record a little history
     */
    fsm->history_index = (fsm->history_index+1)%FSM_HISTORY;

    /*
     * Get a local pointer to the history buffer to populate
     */
    history_ptr = &fsm->history[fsm->history_index];

    history_ptr->prevStateID = fsm->curr_state;
    history_ptr->stateID = nextState;
    history_ptr->eventID = normalized_event;
    history_ptr->handler_rc = handler_rc;
    return;
}


/** 
 * NAME
 *    fsm_engine
 *
 * SYNOPSIS
 *    #include "fsm.h" 
 *    RC_FSM_t
 *    fsm_engine(fsm_t *fsm, 
 *               uint32_t normalized_event,  
 *               void *p2event_buffer, 
 *               void *p2parm)
 *
 * DESCRIPTION
 *    Drives a state machine defined by normalized event 
 *    and a states.
 *
 * INPUT PARAMETERS
 *    *fsm             state machine handle
 *
 *    normalized_event the event id to process 
 *
 *    *p2event         pointer to the raw event which
 *                     is driving the event. This is 
 *                     passed through to the handler.  
 *
 *    *p2parm          pointer parameter that is simply
 *                     passed through to each event
 *                     handler.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    error otherwise
 * 
 */
RC_FSM_t
fsm_engine (fsm_t *fsm, 
            uint32_t normalized_event, 
            void *p2event_buffer, 
            void *p2parm)
{
    event_tuple_t      *event_ptr;
    event_cb_t          event_handler;
    RC_FSM_t            rc;

    /*
     * verify pointers & handles are valid
     */
    if (fsm == NULL) {
        return (RC_FSM_NULL);
    }

    if (fsm->tag != FSM_TAG) {
        return (RC_FSM_INVALID_HANDLE);
    }

    /*
     * verify that "event id" is valid: [0-(number_events-1)]
     */
    if (normalized_event > fsm->number_events-1) {
        fsm_record_history(fsm, normalized_event, 
                           fsm->curr_state, RC_FSM_INVALID_EVENT);
        return (RC_FSM_INVALID_EVENT);
    }

    /*
     * Index into the state table to get to the event table
     * so we can get the next state and the event handler.
     */
    event_ptr = 
         &fsm->state_table[fsm->curr_state].p2event_tuple[normalized_event];

    /*
     * If the handler was NULL then we have a quiet event ,
     * no processing possible.
     */
    event_handler = event_ptr->event_handler;
    if (event_handler == NULL) {
        fsm_record_history(fsm, 
                           normalized_event, 
                           event_ptr->next_state,
                           RC_FSM_INVALID_EVENT_HANDLER);
        return (RC_FSM_OK);
    }

    rc = (*event_handler)(p2event_buffer, p2parm);

    /*
     * Event handler wants to stop processing events. There is no access
     * to the fsm data structure in case the state machine has ended. 
     */
    if (rc == RC_FSM_STOP_PROCESSING) {
        return (rc);
    }

    /*
     * If the return code is not OK, simply record the
     * result without a state change.  
     */
    if (rc != RC_FSM_OK) {
        fsm_record_history(fsm, normalized_event, 
                               event_ptr->next_state, rc);
        return (rc);
    }

    /*
     * If the exception state indicator is set, use the exception
     * state provided by the event handler.  This is an unexpected
     * state transition.  Else use the event table next state.
     */
    if (fsm->exception_state_indicator) {
        /*
         * event handler detected an exception to the state transition 
         */
        fsm->exception_state_indicator = FALSE;
        fsm->next_state = fsm->exception_state;

    } else {
        /*
         * we have a valid event table transition
         */
        fsm->next_state = event_ptr->next_state;
    }

    /*
     * Validate the next state from the event table
     * knowing that the event id is 0,1,2,...
     * Then use the event id to directly index into the state
     * table to set the next state.
     */
    if (fsm->next_state > (fsm->number_states-1)) {
        fsm_record_history(fsm, 
                           normalized_event, 
                           fsm->next_state,
                           RC_FSM_INVALID_STATE);
        rc = RC_FSM_INVALID_STATE;

    } else { 

        /* record a bit of history. */
        fsm_record_history(fsm, 
                           normalized_event, 
                           fsm->next_state, 
                           rc);

        /*
         * and update the current state completing the transition
         */
        fsm->curr_state = fsm->next_state;
    } 
    return (rc);
}

