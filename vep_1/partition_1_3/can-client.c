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
#if (defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_7) || defined(TEST_CASE_9) || defined(TEST_CASE_12))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_2))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_3))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_3\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_7))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_7\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_9))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_9\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Toggle L0 on ID = PRIO_BUTTON0
CAN_FRAME frame;
while(true){
	can_mac_rx_frame(&frame);
	if(frame.ID == PRIO_BUTTON0){
		uint8_t state = read_green_led_state(0);
		if(state){
			turn_led_off(0);
		}
		else{
			turn_led_on(0);
		}
	}
}

#elif (defined(TEST_CASE_4) || defined(TEST_CASE_5) || defined(TEST_CASE_6) || defined(TEST_CASE_10) || defined(TEST_CASE_11))
  uint64_t t = read_global_timer();
  #if (defined(TEST_CASE_4))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_4\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_5))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_5\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_6))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_10))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_10\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_11))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_11\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  //Send ID = PRIO_PERIOD0 with period PERIOD0
CAN_FRAME frame;
uint64_t time = read_global_timer();
uint8_t sequence = 0;
frame.ID = PRIO_PERIOD0;
frame.DLC = 1;
frame.Data = sequence;
frame.CRC = 1;
while(true){
	xil_printf("time = %lu\n", read_global_timer());
	can_mac_tx_frame(&frame);
	t = read_global_timer();
	while(t < time + PRIO_PERIOD0){}
	time = time + PRIO_PERIOD0;
	frame.Data++;
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
