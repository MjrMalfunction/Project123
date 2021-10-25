#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include "can_phy.h"
#include "can_phy.c"
#include "can_mac.h"
#include "settings.h"
#include "io.h"
#include "io.c"

int main (void)
{
#if (defined(TEST_CASE_9) || defined(TEST_CASE_12))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_9\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  //Send ID = PRIO_BUTTON2 on B2
  //Set LED 0 to blue when trying to send frame for the first time,
  //red after losing arbitration and green when there is nothing to send
  //void set_led_color(uint8_t led_id, uint8_t r, uint8_t g, uint8_t b)
uint8_t button = read_button_state(2);
uint8_t sequence = 1;
bool temp = 1;
CAN_FRAME frame;
while(true){
	if(button){
		if(temp){
			set_led_color(0, 0, 0, 1);
		}
		else{
			set_led_color(0, 1, 0, 0);
		}
		frame.ID = PRIO_BUTTON2;
		frame.DLC = 1;
		frame.Data = sequence;
		frame.CRC = 1;
		temp = can_mac_tx_frame(&frame);
		sequence++;
	}
	else{
		set_led_color(0, 0, 1, 0);
	}
	if(temp){
		button = read_button_state(2);
	}
}

#elif (defined(TEST_CASE_10) || defined(TEST_CASE_11))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_10))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_10\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_11\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Send ID = PRIO_PERIOD1 on period of PERIOD1

#else
  /******* CAN Client is disabled in this test case *******/
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d is idle\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while (1) { asm("wfi"); }
#endif
  return 0;
}
