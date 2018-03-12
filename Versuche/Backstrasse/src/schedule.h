/*
 * schedule.h - quick'n'dirty scheduler using the PIT. This file just defines SCHEDULE_TICKS_PER_SECOND, the rest of the scheduler is mostly within main.c
 * (setting up the PIT; Schedule semaphore)
 *
 *  Created on: Jan 12, 2012
 *      Author: kindt
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_


#define SCHEDULE_TICKS_PER_SECOND 500			//the number of times the scheduler will emit a schedule stroke and thus call all active scheduled functions

#endif /* SCHEDULE_H_ */
