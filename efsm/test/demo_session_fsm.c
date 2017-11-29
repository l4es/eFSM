/*----------------------------------------------------------------------
 * demo_session_fsm.c
 *
 * January 2009, Bo Berry
 *
 * Copyright (c) 2009 Cisco Systems, Inc 
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
 *---------------------------------------------------------------------
 */

#include <stdio.h>

#include "safe_limits.h"
#include "fsm.h"
#include "demo_context.h"
#include "demo_event_handlers.h"

#include "demo_session_fsm.h"




/*
 * table of normalized event descriptions 
 */
static event_description_t normalized_event_table[] =
  {{start_init_e, "Start Session Init"},
   {init_rcvd_e, "Session Init"},
   {init_tmo_e, "Session Init ACK TMO"},
   {init_ack_e, "Session Init ACK"},
   {start_term_e, "Start Session Termination"},
   {term_rcvd_e, "Session Terminate"},
   {term_ack_e, "Session Terminate ACK"},
   {FSM_NULL_EVENT_ID, NULL} };


/*
 * table of normalized state descriptions
 */
static state_description_t normalized_state_table[] =
  {{idle_s, "Idle State"},  
   {wait_for_init_ack_s, "Wait for Init Ack State"}, 
   {established_s, "Established State"}, 
   {wait_for_term_ack_s, "Wait for Terminate Ack State"}, 
   {FSM_NULL_STATE_ID, NULL} };


/* state machine handle */ 
static fsm_t *fsm_handle;


/*
 *  catch-all ignore event processing
 */
static RC_FSM_t event_ignore (void *p2event, void *p2parm)
{
    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
}



/*
 * State event tables
 */
static event_tuple_t  state_idle_events[] =
         /* The client sending the init */ 
    {  { start_init_e,    event_start_init,        wait_for_init_ack_s}, 
         /* The server rcvd an init */ 
       { init_rcvd_e,     event_init_rcvd,         established_s}, 
       
       { init_tmo_e,      event_ignore,            idle_s},
       { init_ack_e,      event_ignore,            idle_s},
       { start_term_e,    event_ignore,            idle_s},
       { term_rcvd_e,     event_ignore,            idle_s},
       { term_ack_e,      event_ignore,            idle_s} }; 

static event_tuple_t  state_wait_for_init_ack_events[] =
    {  { start_init_e,    event_ignore,            wait_for_init_ack_s},
       { init_rcvd_e,     event_ignore,            wait_for_init_ack_s},
       { init_tmo_e,      event_init_ack_tmo,      wait_for_init_ack_s},
       { init_ack_e,      event_init_ack_rcvd,     established_s},
       { start_term_e,    event_term_rcvd,         wait_for_init_ack_s},
       { term_rcvd_e,     event_term_rcvd,         idle_s},
       { term_ack_e,      event_ignore,            wait_for_init_ack_s} }; 

static event_tuple_t  state_established_events[] =
    {  { start_init_e,    event_ignore,            established_s},
       { init_rcvd_e,     event_ignore,            established_s},
       { init_tmo_e,      event_ignore,            established_s},
       { init_ack_e,      event_ignore,            established_s},
       { start_term_e,    event_start_term,        wait_for_term_ack_s},
       { term_rcvd_e,     event_term_rcvd,         idle_s},
       { term_ack_e,      event_ignore,            established_s} }; 

static event_tuple_t  state_wait_for_term_ack_events[] =
/******  Event ID  *******  Handler *************  Next State ID *****/ 
    {  { start_init_e,    event_ignore,            wait_for_term_ack_s},
       { init_rcvd_e,     event_ignore,            wait_for_term_ack_s},
       { init_tmo_e,      event_ignore,            wait_for_term_ack_s},
       { init_ack_e,      event_ignore,            wait_for_term_ack_s},
       { start_term_e,    event_ignore,            wait_for_term_ack_s},
       { term_rcvd_e,     event_ignore,            idle_s},
       { term_ack_e,      event_term_ack_rcvd,     idle_s} }; 


/*
 * Session State table
 */
static state_tuple_t  demo_state_table[] =
/***   State ID  ******************   Event Table ***/ 
    { {idle_s,                        state_idle_events},
      {wait_for_init_ack_s,           state_wait_for_init_ack_events}, 
      {established_s,                 state_established_events}, 
      {wait_for_term_ack_s,           state_wait_for_term_ack_events},
      {FSM_NULL_STATE_ID, NULL} };  


/*
 * This function is used to return the current state 
 */
uint32_t
demo_fsm_get_state (demo_config_t *p2config,
                    demo_context_t *p2context)
{
    uint32_t state;

    state = (-1);

    if (p2context) {
        fsm_get_state(fsm_handle, &state); 
    } else {
        /* ignore */
    }
    return (state);
}



/*
 * This function is used to display the state machine sate table  
 */
void 
demo_fsm_show_state_table (demo_config_t *p2config, 
                           demo_context_t *p2context)
{
    if (p2context) {
        fsm_display_table(fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}


/*
 * This function is used to display a state machine's history    
 */
void 
demo_fsm_show_history (demo_config_t *p2config, 
                       demo_context_t *p2context)
{
    if (p2context) {
        fsm_show_history(fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}  


/*
 * demo state machine engine  
 */
void 
demo_fsm_engine (uint32_t normalized_event, 
                 demo_config_t *p2config, 
                 demo_context_t *p2context)
{
    RC_FSM_t  rc;

    if (!p2config) {
        DEMO_ERROR("%s-%u Error: Config is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    } 

    if (!p2context) {
        DEMO_ERROR("%s-%u Error: Context is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    } 

    rc = fsm_engine(fsm_handle, 
                    normalized_event, 
                    p2config, 
                    p2context);
    if (rc != RC_FSM_OK) {
        DEMO_ERROR("%s-%u Error: demo state machine rc=%u \n", 
                     __FUNCTION__,
                     __LINE__, 
                     rc);  
    }
    return;
}


/*
 * This function is used to destroy a demo state machine engine  
 */
void 
demo_fsm_destroy (demo_config_t *p2config,
                  demo_context_t *p2context)
{
    if (!p2config) {
        DEMO_ERROR("%s-%u Error: Config is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    }

    if (!p2context) {
        DEMO_ERROR("%s-%u Error: Context is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    }

    fsm_destroy((fsm_t **)&fsm_handle); 
    return;
}


/*
 * This function is used to create a demo state machine
 * when the session is instaniated.  
 */
void 
demo_fsm_create (demo_config_t *p2config, 
                 demo_context_t *p2context)
{
    RC_FSM_t  rc;

    if (!p2config) {
        DEMO_ERROR("%s-%u Error: Config is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    } 

    if (!p2context) {
        DEMO_ERROR("%s-%u Error: Context is null \n",
                      __FUNCTION__,
                      __LINE__);
        return;
    } 

    fsm_handle = NULL;
    rc = fsm_create((fsm_t **)&fsm_handle, 
                    "Demo State Machine", 
                     idle_s,
                     normalized_state_table, 
                     normalized_event_table, 
                     demo_state_table);  
   if (rc != RC_FSM_OK) {   
       DEMO_ERROR("%s-%u Error: demo state machine rc=%u \n", 
                __FUNCTION__,
                __LINE__, 
                rc);  
   } 

   return; 
} 


