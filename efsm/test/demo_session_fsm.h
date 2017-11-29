/*----------------------------------------------------------------------
 * demo_session_fsm.h
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

#ifndef  __DEMO_SESSION_FSM_H__
#define  __DEMO_SESSION_FSM_H__

#include "demo_context.h"


/*
 * normalized events
 */
typedef enum {
    /*
     * The state machine remains dormant until the start
     * event.  The start event handler will send an init
     * message to the server, and transition to the 
     * wait for ack state.  
     */ 
    start_init_e,

    /*
     * When the server receives the init, the init rcvd 
     * event is processed by the state machine. 
     */ 
    init_rcvd_e,

    /*
     * If the client does not receive an acknowledgement
     * to the init, a guard timer expires.  The guard
     * timer expiration is processed as an init ack timeout. 
     */ 
    init_tmo_e,

    /*
     * When the client receives an acknowledgement to the
     * init request from the server, the ack event is 
     * processed by the state machine.
     */ 
    init_ack_e,

    /*
     * The start terminate event is used to terminate the
     * established session.  This event will generate a
     * terminate message.
     */ 
    start_term_e,

    /*
     * When the terminate request is received, the terminate
     * received event is processed to terminate the session
     * and send an acknowledgement. 
     */ 
    term_rcvd_e,

    /*
     * This is an acknowledgement to the terminate request.
     */ 
    term_ack_e,
} session_events_e;


/*
 * normalized states
 */
typedef enum {
    /*
     * The idle state represents a dormant state.  The start event 
     * kicks things off.
     */  
    idle_s = 0,

    /*
     * This state expects the acknowledgement to the init request
     * of the guard timer expiration.  
     */  
    wait_for_init_ack_s,

    /*
     * The established state is reached once the init request and
     * init ack have been exchanged.  
     */   
    established_s,

    /*
     * Ths state expects the acknowledgement to the terminate 
     */  
    wait_for_term_ack_s,
} demo_states_e;




extern uint32_t
demo_fsm_get_state(demo_config_t *p2config,
                   demo_context_t *p2context); 

extern void 
demo_fsm_show_counter(demo_config_t *p2config,
                      demo_context_t *p2context); 

extern void 
demo_fsm_show_state_table(demo_config_t *p2config,
                          demo_context_t *p2context); 

extern void 
demo_fsm_show_history(demo_config_t *p2config,
                      demo_context_t *p2context); 

extern void 
demo_fsm_engine(uint32_t normalized_event,
                demo_config_t *p2config,
                demo_context_t *p2context);

extern void
demo_fsm_destroy(demo_config_t *p2config,
                 demo_context_t *p2context); 

extern void
demo_fsm_create(demo_config_t *p2config,
                demo_context_t *p2context); 


#endif

