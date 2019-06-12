// FLASH Mode EEPROM record index
//   BYTE
//   0   Report ID
//   1   REC_LEN
//   2   ADD_L
//   3   ADD_U
//   4   REC_TYPE
//   5   SOD
//
#define REPORT_ID		0
#define REC_LEN			1
#define ADD_L			2
#define ADD_H			3
#define REC_TYPE		4   // Also used as command index
#define CMD_FLAGS		5
#define SOD				6
#define NODE_REC_TYPE	6	// NODE Only. Used as command index
#define NODE_ADDRESS	7
#define NODE_READ_LEN	8
#define NODE_SOD		9

// Misc Commands
#define READ_VERSION		0x06
#define DEVICE_SERIAL		0x07
#define ERASE_DEVICE		0x08
#define RESET_CMD			0x09
#define READ_EEPROM			0x0A
#define READ_EEPROM_BYTE	0x0B
#define WRITE_EEPROM_BYTE	0x0C
#define WRITE_EE_ANALOG		0x0D
#define WRITE_EE_DIGITAL	0x0E
#define WRITE_EE_INPUT		0x0E
#define WRITE_EE_HAT		0x0F
#define WRITE_EE_MISC		0x10
#define WRITE_EE_ACE		0x10
#define WRITE_EE_DAC		0x10
#define WRITE_EE_SPI		0x10
#define READ_EE_ANALOG		0x11
#define READ_EE_DIGITAL		0x12
#define READ_EE_INPUT		0x12
#define READ_EE_HAT			0x13
#define READ_EE_MISC		0x14
#define READ_EE_ACE			0x14
#define READ_EE_DAC			0x14
#define READ_EE_SPI			0x14
#define READ_EE_SSI			0x15
#define WRITE_EE_SSI		0x16
#define READ_EE_ROTARY		0x17
#define WRITE_EE_ROTARY		0x18
#define RESET_ROTARY		0x19
#define READ_ROTARY			0x1A
#define NODE_WRITE			0x1B
#define NODE_COMMAND		0x1C
#define NODE_DATA			0x1D
#define READ_EE_INC			0x1E
#define WRITE_EE_INC		0x1F
#define READ_EE_HS			0x20
#define WRITE_EE_HS			0x21
#define WRITE_SHIFTER		0x21
#define READ_EE_CAL			0x22
#define READ_EE_TWK			0x22
#define WRITE_EE_CAL		0x23
#define WRITE_EE_TWK		0x23
#define WRITE_EEPROM		0x24
#define READ_EE_PORT		0x25
#define WRITE_EE_PORT		0x26
#define READ_EE_OUTPUT		0x27
#define WRITE_EE_OUTPUT		0x28
#define NODE_WRITE_PORT		0x29
#define NODE_WRITE_DISPLAY	0x2A
#define WRITE_CLCD			0x2B
#define NODE_WRITE_RWR		0x2C
#define NODE_WRITE_GLCD		0x2D
#define NODE_WRITE_DAC		0x2E
#define WRITE_DACS			0x2F
#define AXIS_CAL_ON			0x30
#define AXIS_CAL_OFF		0x31
#define AXIS_CAL_CANCEL		0x32
#define AXIS_CAL_MAN		0x33
#define READ_EE_GLCD		0x34
#define WRITE_EE_GLCD		0x35
#define NODE_WRITE_DOTMATRIX 0x36
//#define READ_EE_DMX		0x37
#define READ_PORT			0x38
#define WRITE_PORT			0x39
#define WRITE_FUSION		0x39
#define READ_EE_STEPPER		0x3A
#define WRITE_EE_STEPPER	0x3B
#define NODE_WRITE_STEPPER	0x3C
#define READ_INPUTS			0x3D
#define PORTY_DATA			0x3E
#define READ_EE_RWR			0x3F
#define WRITE_EE_RWR		0x40

#define AU_STOP				0x36
#define AU_RUN				0x37
#define CODEC				0xFE

#define EXECUTE_CMD			0xFF

// Misc Flags
#define EE_WRITE			0x1
#define NODE_NO_REPLY		0x2
#define CFG_WRITE			0x4

// Misc Results
#define SUCCESS			0x00
#define END_FILE		0x01
#define CMD_FAIL		0x02
#define BOOTADDR_FAIL	0x03
#define FLASH_FAIL		0x04
#define CONFIG_FAIL		0x05
#define EEPROM_FAIL		0x06
#define FLASH_OK		0x07
#define CONFIG_OK		0x08
#define EEPROM_OK		0x09
#define NODE_FAIL		0x0A
#define CRC_FAIL		0x0B
#define NODE_OK			0x0C
#define CMD_WAIT		0x0D

//******************************************************************************
// API constants
//******************************************************************************
#define MAX_NODES		13
#define PORTY_IDX		MAX_NODES
#define PORTY_ADDRESS	(MAX_NODES-1)