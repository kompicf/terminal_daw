#ifndef UPDATE_H
#define UPDATE_H

#include "global.h"

void update_init(void){
	gettimeofday(&t1, NULL);
}

void update_all(void){
		// delta time calculation
		gettimeofday(&t2, NULL);
		dt = (t2.tv_sec - t1.tv_sec) * 1000.0;
    dt += (t2.tv_usec - t1.tv_usec) / 1000.0;
		gettimeofday(&t1, NULL);

		input_key = getch();
		erase();
}

#endif

