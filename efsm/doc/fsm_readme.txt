/*
 * Copyright (c) 2005-2009 by Cisco Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */ 

The finite state machine can be used to model the behavior of 
systems, systems that exist in one or more states.  State transitions
are driven by a set of normalized events derived from one or more
sources.   

The initial state can be any one of the normalized states.  This is 
defined when the state machie is created.  The number of state is
determined by a state table.   Each state table entry, state, references
an event table.  The normalized event table is maps each event to a
specific event handler.  The handler is a user function that handles
the specific event. 



The Demo

The demo is a simple imaginary protocol to demonstrate the state and 
event tables to configure a state machine and a few fsm calls to
drive that state machine.  Included are calls to display the state
machine history of events and resulting state transitions (useful 
for debugging) and a call to display the state-event tables.  The
organization of the demo software suggests an organization for your
real application.   



References

[1] http://en.wikipedia.org/wiki/Finite_state_machine

[2] http://en.wikipedia.org/wiki/Mealy_machine

[3] http://en.wikipedia.org/wiki/Moore_machine 



