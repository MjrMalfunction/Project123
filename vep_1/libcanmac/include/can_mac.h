#ifndef __CAN_MAC_H__
#define __CAN_MAC_H__

#include "can_phy.h"

/*** this file has to be written by students ***/
#include <stdbool.h>
extern bool can_mac_tx_frame(CAN_FRAME* txFrame);
extern void can_mac_rx_frame(CAN_FRAME* rxFrame);

#endif
