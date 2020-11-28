
#ifndef HC_SR04_H__
#define HC_SR04_H__

#include <stdint.h>

#define TRIGGER_PULSE_DURATION 10 		//in microseconds
#define SOUND_SPEED 34300           //cm/s             

extern uint8_t send_trigger;
extern uint8_t is_capture_1;
extern uint8_t is_capture_2;
extern uint32_t capture_data[2];
//blocking functions
void hc_sr04_send_trigger(int time_in_us);
float hc_sr04_get_time_lapse();
float hc_sr04_get_distance_in_cm();
float hc_sr04_get_distance_in_inch();


#endif