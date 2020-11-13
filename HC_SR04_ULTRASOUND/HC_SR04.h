
#ifndef HC_SR04_H__
#define HC_SR04_H__

#define ECHO_PULSE_DURATION 10 		//in microseconds

//blocking functions
void hc_sr04_send_echo(int time_in_us);
int hc_sr04_get_time_lapse();
float hc_sr04_get_distance_in_cm();
float hc_sr04_get_distance_in_inch();


#endif