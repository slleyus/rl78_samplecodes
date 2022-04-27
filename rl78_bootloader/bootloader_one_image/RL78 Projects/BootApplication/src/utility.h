//Packet layout
	//bytes 0 - 0
		//Packet type
	//Bytes 1 - 2
		//Data packet counter
	//Bytes 3 - 66
		//Data of Data packet
	//Bytes 66 - 68
		//Packet CRC

#define UART_PACKET_LENGTH         	69
#define UART_RESPONSE_LENGTH		5
#define MAX_PACKETS					0x1FB7

#define CRC_ZERO_SEED				0
#define CRC_OK						0

#define ADDRESS_BOOT0_START			0x0
#define ADDRESS_BOOT1_START			0x1000
#define ADDRESS_APP_START			0x2000

#define VERSION_ADDRESS				0x00D8



typedef enum
{
	PACKET_ERROR = 0,
	PACKET_DATA,
	PACKET_VERSION,
	PACKET_RESET,
	PACKET_TRANSFER_COMPLETE,
	PACKET_SWAP_AND_RESET
} e_packet_handle_t;

typedef enum
{
	COMMAND_BEGIN_TRANSMISSION,
	COMMAND_ACK_PACKET,
	COMMAND_NAK_PACKET,
	COMMAND_ACK_PROGRAM,
	COMMAND_NAK_PROGRAM,
	COMMAND_SEND_VERSION,
	COMMAND_NAK_CRC
} e_commands_t;


e_packet_handle_t get_packet_type(uint8_t handle);
void send_response(e_commands_t response, uint8_t *tx_buf);
uint16_t general_crc(uint8_t __far *start_address, uint32_t length, uint16_t crc_seed);
void get_version(uint8_t *tx_buf);
uint16_t verify_application_crc(void);















