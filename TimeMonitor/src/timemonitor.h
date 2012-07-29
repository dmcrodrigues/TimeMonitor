/*
 * Copyright (c) 2012 David Rodrigues
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */

#ifndef TIMEMONITOR_H
#define TIMEMONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

typedef struct timepoint Timepoint;

struct timepoint {
	struct timespec timepoint;
	char description[256];
	Timepoint *next;
};

typedef struct {
	struct timespec start;
	struct timespec end;
	Timepoint *first_timepoint;
	Timepoint *last_timepoint;
} TimeMonitor;

// creates a new TimeMonitor
TimeMonitor *tm_init();

// frees allocated resources
void tm_cleanup(TimeMonitor **fp);

// start monitoring
void tm_start_profiling(TimeMonitor *fp);

// stop monitoring
void tm_stop_profiling(TimeMonitor *fp);

// add a new timepoint
int tm_add_timepoint(TimeMonitor *fp, const char *description, ...);

// prints the results observed
void tm_print_info(TimeMonitor *fp);


#endif
