/*----------------------------------------------------------------------
 * demo_event_handlers.c
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
#include <inttypes.h>

#include "fsm.h"
#include "demo_context.h"
#include "demo_session_fsm.h"



/*
 * This event is triggered when the start  init is rcvd
 */
RC_FSM_t event_start_init (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
}


/*
 * This event is triggered when an init is rcvd
 */
RC_FSM_t event_init_rcvd (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
}


/*
 * This event is triggered when an init ack is rcvd
 */
RC_FSM_t event_init_ack_rcvd (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
}


/*
 * This event is triggered when a timeout for the init ack occurrs  
 */
RC_FSM_t event_init_ack_tmo (void *p2event, void *p2parm)
{
    RC_FSM_t rc;

    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);

    p2context->timeout_count++; 
    if (p2context->timeout_count >= p2config->tmo_threshold) {  

        DEMO_TRACE("%s-%u Error: init time outs exceeded \n",  
                     __FUNCTION__,
                     __LINE__); 

        rc = RC_FSM_STOP_PROCESSING; 
        rc = RC_FSM_OK; 

    } else {   

        DEMO_TRACE("%s-%u init time out count=%u  threshold=%u \n", 
                     __FUNCTION__, 
                     __LINE__, 
                     p2context->timeout_count,
                     p2config->tmo_threshold);  

        rc = RC_FSM_OK; 
    }

    return (rc);
}


/*
 * This event is triggered when to start the termination   
 */
RC_FSM_t event_start_term (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
}


/*
 * This event is triggered when a terminate is rcvd
 */
RC_FSM_t event_term_rcvd (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
    //return (RC_FSM_STOP_PROCESSING); 
}


/*
 * This event is triggered when a terminate ack is rcvd
 */
RC_FSM_t event_term_ack_rcvd (void *p2event, void *p2parm)
{
    demo_config_t *p2config;
    demo_context_t *p2context;

    p2config = p2event; 
    p2context = p2parm; 

    DEMO_TRACE("%s-%u\n", __FUNCTION__, __LINE__);
    return (RC_FSM_OK);
    //return (RC_FSM_STOP_PROCESSING); 
}

