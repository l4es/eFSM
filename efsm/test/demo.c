/*----------------------------------------------------------------------
 * demo.c 
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
#include <string.h>
#include <stdlib.h>

#include "fsm.h"
#include "demo_context.h"

#include "demo_session_fsm.h"



static demo_config_t config;  

static demo_context_t context;  





int main (int argc, char **argv)
{

    demo_fsm_create(&config, &context); 

    context.timeout_count = 0;
    config.tmo_threshold = 3;  

    demo_fsm_engine(start_init_e, &config, &context);  

    demo_fsm_engine(init_tmo_e, &config, &context);  
    demo_fsm_engine(init_tmo_e, &config, &context);  
    demo_fsm_engine(init_ack_e, &config, &context);  

    demo_fsm_engine(start_term_e, &config, &context);  
    demo_fsm_engine(term_ack_e, &config, &context);  

    demo_fsm_show_history(&config, &context);  

    demo_fsm_show_state_table(&config, &context);

    exit (0);
}


