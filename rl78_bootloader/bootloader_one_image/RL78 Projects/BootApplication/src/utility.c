

#include <r_cg_macrodriver.h>
#include "utility.h"
#include "r_cg_serial.h"
#include "flash_interface.h"



e_packet_handle_t get_packet_type(uint8_t handle)
{
	switch (handle)
	{
		case 0x00:
			return PACKET_DATA;
		case 0x01:
			return PACKET_VERSION;
		case 0x02:
			return PACKET_RESET;
		case 0x03:
			return PACKET_TRANSFER_COMPLETE;
		case 0x04:
			return PACKET_SWAP_AND_RESET;
		default:
			return PACKET_ERROR;
	}
}

void send_response(e_commands_t response, uint8_t *tx_buf)
{
	//set command bytes
	if(response == COMMAND_BEGIN_TRANSMISSION)
	{
		tx_buf[0] = 0x01;
		tx_buf[1] = 0xAA;
		tx_buf[2] = 0xAA;
		tx_buf[3] = 0xAA;
		tx_buf[4] = 0xAA;
	}
	else if(response == COMMAND_ACK_PACKET)
	{
		tx_buf[0] = 0x02;
	}
	else if(response == COMMAND_NAK_PACKET)
	{
		tx_buf[0] = 0x03;

		tx_buf[3] = 0xAA;
		tx_buf[4] = 0xAA;
	}
	else if(response == COMMAND_SEND_VERSION)
	{
		tx_buf[0] = 0x04;
	}
	else if(response == COMMAND_ACK_PROGRAM)
	{
		tx_buf[0] = 0x05;
	}
	else if(response == COMMAND_NAK_PROGRAM)
	{
		tx_buf[0] = 0x06;
	}
	else if(response == COMMAND_NAK_CRC)
	{
		tx_buf[0] = 0x07;

		tx_buf[3] = 0xAA;
		tx_buf[4] = 0xAA;
	}

	//set UART to send this data
	R_UART0_Send(tx_buf, UART_RESPONSE_LENGTH);
}


uint16_t general_crc(uint8_t __far *start_address, uint32_t length, uint16_t crc_seed)
{
    uint32_t i;

    CRCD = crc_seed;

    /* Read 4 bytes of data each loop until range complete */
    for(i = 0; i < length; i++)
    {
        CRCIN = *start_address++;           /* Read 1st byte of Word (32bits) */
    }

    return CRCD;
}

uint16_t verify_application_crc(void)
{
	uint32_t my_address = 0x1000;
	uint8_t buf[BYTES_PER_WRITE];
	uint16_t my_crc = 0;
	uint16_t i;

	for(i = 0; i <= MAX_PACKETS; i++)
	{
		flash_read_packet_data(my_address, buf, BYTES_PER_WRITE);

		my_crc = general_crc(buf, BYTES_PER_WRITE, my_crc);

		my_address += BYTES_PER_WRITE;
	}

	return my_crc;
}

void get_version(uint8_t *tx_buf)
{
	static uint8_t __far *current_version;

	current_version = ((__far uint8_t *) VERSION_ADDRESS);

	/* Place the bytes of the version into the buffer */
	tx_buf[1] = *current_version++;
	tx_buf[2] = *current_version++;
	tx_buf[3] = *current_version++;
	tx_buf[4] = *current_version++;
}


















