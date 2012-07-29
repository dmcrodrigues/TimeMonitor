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

#include "timemonitor.h"

// ======================================================
// === Private API ======================================

void tm_collect_time();
long tm_get_elapsed_miliseconds(struct timespec a, struct timespec b);

// ======================================================
// === Public API =======================================

TimeMonitor *tm_init() {
	TimeMonitor *fp = (TimeMonitor *) malloc(sizeof(TimeMonitor));
	if ( !fp )
		return NULL;
	fp->first_timepoint = fp->last_timepoint = NULL;
	return fp;
}

void tm_cleanup(TimeMonitor **fp) {
	if ( !(*fp) )
		return;
	Timepoint *aux = NULL;
	while ( (*fp)->first_timepoint ) {
		aux = (*fp)->first_timepoint;
		(*fp)->first_timepoint = (*fp)->first_timepoint->next;
		free(aux);
        aux = NULL;
	}
	(*fp)->first_timepoint = (*fp)->last_timepoint = NULL;
	free((*fp));
	(*fp) = NULL;
}

int tm_add_timepoint(TimeMonitor *fp, const char *description, ...) {
    va_list args;
	Timepoint *t = (Timepoint *) malloc(sizeof(Timepoint));
	if ( !t ) {
		fprintf(stderr, "can't init timepoint\n");
		return 1;
	}
	
	tm_collect_time(&t->timepoint);
	va_start (args, description);
    vsprintf (t->description, description, args);
    va_end (args);
	t->next = NULL;
	
	if ( fp->last_timepoint ) {
		fp->last_timepoint->next = t;
		fp->last_timepoint = t;
	}
	else {
		fp->first_timepoint = fp->last_timepoint = t;
	}
	
	return 0;
}

void tm_start_profiling(TimeMonitor *fp) {
	tm_collect_time(&fp->start);
}

void tm_stop_profiling(TimeMonitor *fp) {
	tm_collect_time(&fp->end);
}

void tm_print_info(TimeMonitor *fp) {
    long total_time = tm_get_elapsed_miliseconds(fp->start, fp->end);
    if ( fp->first_timepoint ) {
        Timepoint *prev = NULL;
        Timepoint *act = fp->first_timepoint;
        long elapsed_time = 0;
        double percentage = 0;
        int i = 1;
        if ( act ) {
            elapsed_time = tm_get_elapsed_miliseconds(fp->start, act->timepoint);
            percentage = (elapsed_time * 100) / total_time;
            fprintf(stdout, "| timepoint %2d | %6ld ms (%6.2f%%) | %s |\n", i++, elapsed_time, percentage, act->description);
            prev = act;
            act = act->next;
            while ( act ) {
                elapsed_time = tm_get_elapsed_miliseconds(prev->timepoint, act->timepoint);
                percentage = (elapsed_time * 100) / total_time;
                fprintf(stdout, "| timepoint %2d | %6ld ms (%6.2f%%) | %s |\n", i++, elapsed_time, percentage, act->description);
                prev = act;
                act = act->next;
            }
        }
    }
    fprintf(stdout, "------------------------------------------------------\n");
    fprintf(stdout, "[TOTAL] - elapsed time: %ld (ms)\n", total_time);
}

// ==================================================
// === PRIVATE METHODS ==============================

void tm_collect_time(struct timespec *time_result) {
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	time_result->tv_sec = mts.tv_sec;
	time_result->tv_nsec = mts.tv_nsec;
#else
	clock_gettime(CLOCK_REALTIME, time_result);
#endif
}

long tm_get_elapsed_miliseconds(struct timespec a, struct timespec b) {
	long seconds, nseconds;
	
	seconds  = b.tv_sec  - a.tv_sec;
    nseconds = b.tv_nsec - a.tv_nsec;
    
    return ((seconds) * 1000 + (nseconds/1000.0/1000.0)) + 0.5;
}
