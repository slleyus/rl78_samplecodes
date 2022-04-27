



#include "fsl_types.h"
#include "fsl.h"


#define BLOCK_BOOT0_BEGIN			0
#define BLOCK_BOOT0_END				3
#define BLOCK_BOOT1_BEGIN			4
#define BLOCK_BOOT1_END				7
#define BLOCK_APP_BEGIN				8
#define BLOCK_APP_END				511


#define BYTES_PER_WRITE				64			//max of 256

uint8_t init_fcl(void);
uint8_t flash_erase_app_area(void);
uint8_t flash_boot_swap_reset(void);
uint8_t flash_write_packet_data(uint32_t addr, uint8_t data[], uint16_t size);
void flash_read_packet_data(uint32_t addr, uint8_t data[], uint16_t size);


























