#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <timers.h>
#include "can_phy.h"
#include "can_mac.h"
#include <stdbool.h>

#define IDLENGTH 11
#define DLCLENGTH 4
#define DATALENGTH 64
#define CRCLENGTH 16

bool can_mac_tx_frame(CAN_FRAME* txFrame){
	int idLength = 11;
	int dlcLength = 4;
	int dataLength = txFrame->DLC * 8;
	int crcLength = 16;

	bool ID[IDLENGTH] = {};
	bool DLC[DLCLENGTH] = {};
	bool DATA[DATALENGTH] = {};
	bool CRC[CRCLENGTH] = {};

	int temp = txFrame->ID;
	for(int i = idLength - 1; i >=0; i--){
		ID[i] = temp%2;
		temp = temp/2;
	}
	temp = txFrame->DLC;
	for(int i = dlcLength - 1; i >= 0; i--){
		DLC[i] = temp%2;
		temp = temp/2;
	}
	temp = txFrame->Data;
	for(int i = dataLength - 1; i >= 0; i--){
		DATA[i] = temp%2;
		temp = temp/2;
	}
	temp = txFrame->CRC;
	for(int i = crcLength - 1; i >= 0; i--){
		CRC[i] = temp%2;
		temp = temp/2;
	}

	CAN_SYMBOL symbol;
	can_phy_rx_symbol(&symbol);
	for(int i = 0; i < 10 && symbol == RECESSIVE; i++){
		can_phy_rx_symbol(&symbol);
	}
	if(symbol == RECESSIVE){			//Bus is idle
		can_phy_tx_symbol(DOMINANT);		//Send SOF
		can_phy_rx_symbol(&symbol);
		for(int i = 0; i < idLength; i++){	//Send ID
			can_phy_tx_symbol(ID[i]);
			can_phy_rx_symbol(&symbol);
			if(symbol != ID[i]){		//Lost arbitration
				xil_printf("Lost arbitration\n");
				return false;
			}
		}
		can_phy_tx_symbol(DOMINANT);		//Send dominant RTR
		can_phy_rx_symbol(&symbol);
		can_phy_tx_symbol(DOMINANT);		//Send dominant IDE
		can_phy_rx_symbol(&symbol);
		can_phy_tx_symbol(DOMINANT);		//Send dominant r0
		can_phy_rx_symbol(&symbol);
		for(int i = 0; i < dlcLength; i++){	//Send DLC
			can_phy_tx_symbol(DLC[i]);
			can_phy_rx_symbol(&symbol);
		}
		for(int i = 0; i < dataLength; i++){	//Send data
			can_phy_tx_symbol(DATA[i]);
			can_phy_rx_symbol(&symbol);
		}
		for(int i = 0; i < crcLength; i++){	//Send CRC
			can_phy_tx_symbol(CRC[i]);
			can_phy_rx_symbol(&symbol);
		}
		can_phy_tx_symbol(RECESSIVE);
		can_phy_rx_symbol(&symbol);		//ACK space
		if(symbol == RECESSIVE){		//Frame was not received correctly
			return false;
		}
		can_phy_tx_symbol(RECESSIVE);		//ACK delimiter
		can_phy_rx_symbol(&symbol);
		for(int i = 0; i < 10; i++){		//Send EOF and IFS
			can_phy_tx_symbol(RECESSIVE);
			can_phy_rx_symbol(&symbol);
		}
	}
	else{						//Bus is busy
		xil_printf("Bus is busy, trying again later\n");
		while(symbol == DOMINANT){
			can_phy_rx_symbol(&symbol);
		}
		bool value = can_mac_tx_frame(txFrame);
		return value;
	}
	return true;
}


void can_mac_rx_frame(CAN_FRAME* rxFrame){
	int idLength = 11;
	int dlcLength = 4;
	int crcLength = 16;

	bool ID[IDLENGTH] = {};
	bool DLC[DLCLENGTH] = {};
	bool DATA[DATALENGTH] = {};
	bool CRC[CRCLENGTH] = {};

	CAN_SYMBOL symbol;
	can_phy_rx_symbol(&symbol);
	for(int i = 0; i < 5 && symbol == RECESSIVE; i++){
		can_phy_rx_symbol(&symbol);
	}
	if(symbol == RECESSIVE){			//Bus is idle
		while(symbol == RECESSIVE){		//Wait until someone starts sending
			can_phy_rx_symbol(&symbol);
		}
	}
	else{						//Bus is busy
		int recessiveCounter = 0;
		while(recessiveCounter < 11){		//Once 11 or more recessives have occured,
			recessiveCounter = 0;		//we have had an ACK delimiter, EOF and IFS
			can_phy_rx_symbol(&symbol);	//and have reached an SOF
			while(symbol == RECESSIVE){
				recessiveCounter++;
				can_phy_rx_symbol(&symbol);
			}
		}
	}
	for(int i = idLength - 1; i >= 0; i--){		//Read ID
		can_phy_rx_symbol(&symbol);
		ID[i] =  symbol;
	}
	can_phy_rx_symbol(&symbol);			//RTR
	can_phy_rx_symbol(&symbol);			//IDE
	can_phy_rx_symbol(&symbol);			//r0
	for(int i = dlcLength - 1; i >= 0; i--){	//Read DLC
		can_phy_rx_symbol(&symbol);
		DLC[i] = symbol;
	}

	int dataLength = 0;				//Calculate DLC
	int base = 1;
	for(int i = 0; i < dlcLength; i++){
		dataLength = dataLength + DLC[i] * base;
		base = base * 2;
	}

	for(int i = dataLength * 8 - 1; i >= 0; i--){	//Read data
		can_phy_rx_symbol(&symbol);
		DATA[i] = symbol;
	}
	for(int i = crcLength - 1; i >= 0; i--){	//Read CRC
		can_phy_rx_symbol(&symbol);
		CRC[i] = symbol;
	}
	can_phy_tx_symbol(DOMINANT);			//Send ACK
	can_phy_rx_symbol(&symbol);			//ACK delimiter

	int idValue = 0;				//Calculate ID
	base = 1;
	for(int i = 0; i < idLength; i++){
		idValue = idValue + ID[i] * base;
		base = base * 2;
	}
	int dataValue = 0;				//Calculate data
	base = 1;
	for(int i = 0; i < dataLength; i++){
		dataValue = dataValue + DATA[i] * base;
		base = base * 2;
	}
	int crcValue = 0;				//Calculate CRC
	base = 1;
	for(int i = 0; i < crcLength; i++){
		crcValue = crcValue + CRC[i] * base;
		base = base * 2;
	}
	rxFrame->ID = idValue;
	rxFrame->DLC = dataLength;
	rxFrame->Data = dataValue;
	rxFrame->CRC = crcValue;
}

	/*uint32_t ID;
	uint32_t DLC;
	uint64_t Data;
	uint32_t CRC*/
