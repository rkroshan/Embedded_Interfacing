
#include "HC_SR04.h"

//blocking functions
void hc_sr04_send_echo(int time_in_us)
{
	//init the timer 1

	//pull the echo pin of echo high

	//start the timer with specific time_in_us

	//pull the echo pin low on timer interrupt

	//wait for trig pin to get high

	//on getting high start counting the time
}

int hc_sr04_get_time_lapse()
{
	return 0;
}


float hc_sr04_get_distance_in_cm()
{
	return 0;
}

float hc_sr04_get_distance_in_inch()
{
	return 0;
}
