#ifndef RWR_H
#define RWR_H

// Rwr Symbol Class table Entries

#define RWRSYM_NONE			0	// nothing
#define RWRSYM_UNKNOWN 		1	// U
#define RWRSYM_ADVANCED_INTERCEPTOR	2	// not used
#define RWRSYM_BASIC_INTERCEPTOR 	3 	// not used
#define RWRSYM_ACTIVE_MISSILE 		4 	// M
#define RWRSYM_HAWK 		5 	// H
#define RWRSYM_PATRIOT 		6 	// P
#define RWRSYM_SA2 			7	// 2
#define RWRSYM_SA3 			8	// 3
#define RWRSYM_SA4 			9 	// 4
#define RWRSYM_SA5 			10 	// 5
#define RWRSYM_SA6 			11 	// 6
#define RWRSYM_SA8 			12 	// 8
#define RWRSYM_SA9 			13 	// 9
#define RWRSYM_SA10 		14 	// 10
#define RWRSYM_SA13 		15 	// 13
#define RWRSYM_AAA 			16 	// A or S alternating
#define RWRSYM_SEARCH 		17 	// S
#define RWRSYM_NAVAL 		18 	// boat symbol
#define RWRSYM_CHAPARAL 	19 	// C
#define RWRSYM_SA15 		20 	// 15 or M alternating
#define RWRSYM_NIKE 		21 	// N
#define RWRSYM_A1 			22 	// A with a single dot under it
#define RWRSYM_A2 			23 	// A with two dots under it
#define RWRSYM_A3 			24 	// not used??
#define RWRSYM_PDOT 		25 	// P with a dot under it
#define RWRSYM_PSLASH 		26 	// P with a vertical bar after it
#define RWRSYM_UNK1 		27 	// U with one dot under it
#define RWRSYM_UNK2 		28 	// U with two dots under it
#define RWRSYM_UNK3 		29 	// U with three dots under it
#define RWRSYM_KSAM 		30 	// C
#define RWRSYM_V4 			32 	// 4
#define RWRSYM_V5 			33 	// 5
#define RWRSYM_V6 			34 	// 6
#define RWRSYM_V14 			35 	// 14
#define RWRSYM_V15 			36 	// 15
#define RWRSYM_V16 			37 	// 16
#define RWRSYM_V18 			38 	// 18
#define RWRSYM_V19 			39 	// 19
#define RWRSYM_V20 			40 	// 20
#define RWRSYM_V21 			41 	// 21
#define RWRSYM_V22 			42 	// 22
#define RWRSYM_V23 			43 	// 23
#define RWRSYM_V25 			44 	// 25
#define RWRSYM_V27 			45 	// 27
#define RWRSYM_V29 			46 	// 29
#define RWRSYM_V30 			47 	// 30
#define RWRSYM_V31 			48 	// 31
#define RWRSYM_VP 			49 	// P
#define RWRSYM_VPD 			50	// PD
#define RWRSYM_VA 			51	// A
#define RWRSYM_VB 			52	// B
#define RWRSYM_VS 			53	// S
#define RWRSYM_Aa 			54	// A with a vertical bar after it
#define RWRSYM_Ab 			55	// A with vertical bars before and after it
#define RWRSYM_Ac 			56	// A with vertical bars before and after it and one through the middle
#define RWRSYM_MIB_F_S 		57	// F or S alternating
#define RWRSYM_MIB_F_A 		58	// F or A alternating
#define RWRSYM_MIB_F_M 		59	// F or M alternating
#define RWRSYM_MIB_F_U 		60	// F or U alternating
#define RWRSYM_MIB_F_BW 	61	// F or basic interceptor shape
#define RWRSYM_MIB_BW_S 	62	// S or basic interceptor shape
#define RWRSYM_MIB_BW_A 	63	// A or basic interceptor shape
#define RWRSYM_MIB_BW_M 	64	// M or basic interceptor shape

//misc symbols. These are not part of Sharedmem and are determined by USIM
#define RWRSYM_SA0			0xFA
#define RWRSYM_SA1			0xFB
#define RWRSYM_SA7			0xFC
#define RWRSYM_DIAMOND		0xFD
#define RWRSYM_LAUNCH		0xFE
#define RWRSYM_TEST			0xFF

#define MAX_RWR_SYMBOLS		40

#define MISSILEACTIVITY		0x01
#define MISSILELAUNCH		0x02
#define NEWGUY				0x04
#define SELECTED			0x08

#define RWR_PORT1			1
#define RWR_PORT2			2

typedef struct _STRUCT_CRTRWR_OUTPUT
{
	// 4 BYTES
	unsigned symbol:8;
	unsigned xPos:8;
	unsigned yPos:8;
	unsigned missileActivity:1;	// draw circle flash
	unsigned missileLaunch:1;	// draw circle solid
	unsigned newguy:1;
	unsigned selected:1;		// draw diamond if 1
	unsigned :4;	
} CRTRWR_OUTPUT;

#endif // RWR_H
