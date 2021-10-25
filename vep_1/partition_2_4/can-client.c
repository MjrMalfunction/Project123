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
#if (defined(TEST_CASE_3) || defined(TEST_CASE_7))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_3))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_3\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_7\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Toggle LED L0(2) on ID = PRIO_BUTTON0
CAN_FRAME frame;
while(true){
	can_mac_rx_frame(&frame);
	if(frame.ID == PRIO_BUTTON0){
		uint8_t state = read_green_led_state(2);
		if(state){
			turn_led_off(2);
		}
		else{
			turn_led_on(2);
		}
	}
}

#elif (defined(TEST_CASE_8))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_8\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  //Same as 7, but with DLC > 1


#elif (defined(TEST_CASE_9) || TEST_CASE_12))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_9\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  //Toggle LED L2 on ID = PRIO_BUTTON2
CAN_FRAME frame;
while(true){
	can_mac_rx_frame(&frame);
	if(frame.ID == PRIO_BUTTON2){
		uint8_t state = read_green_led_state(2);
		if(state){
			turn_led_off(2);
		}
		else{
			turn_led_on(2);
		}
	}
}

#elif (defined(TEST_CASE_10) || defined(TEST_CASE_11))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_10))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_10\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_11\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Toggle LED L0 on ID = PRIO_PERIOD2


#else
  /******* CAN Client is disabled in this test case *******/
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d is idle\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while (1) { asm("wfi"); }
#endif
  return 0;
}
