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
#if (defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_7))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_1))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_1\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_2))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_3))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_3\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_7))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_7\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Send ID = PRIO_BUTTON0 on B0
uint8_t button = read_button_state(0);
uint8_t sequence = 1;
bool temp = 1;
CAN_FRAME frame;
while(true){
	if(button){
		frame.ID = PRIO_BUTTON0;
		frame.DLC = 1;
		frame.Data = sequence;
		frame.CRC = 1;
		temp = can_mac_tx_frame(&frame);
		sequence++;
	}
	if(temp){
		button = read_button_state(0);
	}
}

#elif (defined(TEST_CASE_5) || defined(TEST_CASE_6) || defined(TEST_CASE_10) || defined(TEST_CASE_11))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_5))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_5\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_6))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_10))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_10\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_11)
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_11\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Toggle LED L2 on ID = PRIO_PERIOD0
  //Must print whether deadline is met
CAN_FRAME frame;
uint64_t time = read_global_timer();
while(true){
	can_mac_rx_frame(&frame);
	if(frame.ID = PRIO_PERIOD0){
		uint8_t state = read_green_led_state(2);
		if(state){
			turn_led_off(2);
		}
		else{
			turn_led_on(2);
		}
		t = read_global_timer();
		if(t <= time + PRIO_PERIOD0){
			xil_printf("%04u/%010u: CAN client %d %d received message before deadline\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
		}
		else{
			xil_printf("%04u/%010u: CAN client %d %d received message too late\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
		}
		time = time + PRIO_PERIOD;
	}
}

#elif (defined(TEST_CASE_8))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_8\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  //Same as 7, but with DLC > 1

#else
  /******* CAN Client is disabled in this test case *******/
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d is idle\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while (1) { asm("wfi"); }
#endif
  return 0;
}
