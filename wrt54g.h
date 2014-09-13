// **************************************************************************
//
//  WRT54G.H - Header file for the WRT54G/GS EJTAG Debrick Utility  v4.8
//
//  Note:
//  This program is for De-Bricking the WRT54G/GS routers
//
//  New for v4.8 - Added 2 new Flash Chip Parts to the list:
//                     - SST39VF6401B 4Mx16 BotB     (8MB)
//                     - SST39VF6402B 4Mx16 TopB     (8MB)
//               - Added the following New Switch Options
//                     - /wiggler ........... use wiggler cable
//
//  New for v4.7 - Added 2 new Flash Chip Parts to the list:
//                     - K8D3216UTC  2Mx16 TopB     (4MB)
//                     - K8D3216UBC  2Mx16 BotB     (4MB)
//
//  New for v4.6 - Added Common Flash Chip Polling routine
//               - Added "-probeonly" parameter (good idea jmranger)
//               - Added Chip ID for Broadcom BCM4704 Rev 8 CPU
//               - Added TRST Signal Support for Wiggler Cables
//               - Added Chip ID for BRECIS MSP2007-CA-A1 CPU
//               - Added Experimental 1MB Flash Chip Offsets
//               - Added 2 new Flash Chip Parts to the list:
//                     - MX29LV800BTC 512kx16 TopB  (1MB)
//                     - MX29LV800BTC 512kx16 BotB  (1MB)
//
//  New for v4.5 - Added 2 new Flash Chip Parts to the list:
//                     - K8D1716UTC 1Mx16 TopB      (2MB)
//                     - K8D1716UBC 1Mx16 BotB      (2MB)
//
//  New for v4.4 - Added PrAcc routines to support additional MIPS chips
//                 without the ability to use EJTAG DMA Access
//               - Added Chip ID for Broadcom BCM5365 Rev 1 CPU
//               - Added Chip ID for Broadcom BCM6348 Rev 1 CPU (Big Endian)
//               - Added Chip ID for Broadcom BCM6345 Rev 1 CPU
//               - Added 6 new Flash Chip Parts to the list:
//                     - SST39VF1601 1Mx16 BotB     (2MB)
//                     - SST39VF1602 1Mx16 TopB     (2MB)
//                     - SST39VF3201 2Mx16 BotB     (4MB)
//                     - SST39VF3202 2Mx16 TopB     (4MB)
//                     - SST39VF6401 4Mx16 BotB     (8MB)
//                     - SST39VF6402 4Mx16 TopB     (8MB)
//               - Added the following New Switch Options
//                     - /noemw ............. prevent Enabling Memory Writes
//                     - /nocwd ............. prevent Clearing CPU Watchdog Timer
//                     - /dma ............... force use of DMA routines
//                     - /nodma ............. force use of PRACC routines (No DMA)
//                     - /window:XXXXXXXX ... custom flash window base (in HEX)
//                     - /start:XXXXXXXX .... custom start location (in HEX)
//                     - /length:XXXXXXXX ... custom length (in HEX)
//                     - /silent ............ prevent scrolling display of data
//                     - /skipdetect ........ skip auto detection of CPU Chip ID
//                     - /instrlen:XX ....... set instruction length manually
//               - Added elapsed time to Backup, Erase, and Flash routines
//               - Other minor miscellaneous changes/additions.
//
//  New for v4.3 - Corrected Macronix Flash Chip Block Defintions.
//               - Add 8 new Flash Chip Parts to the list:
//                     - AT49BV/LV16X 2Mx16 BotB    (4MB)
//                     - AT49BV/LV16XT 2Mx16 TopB   (4MB)
//                     - MBM29LV160B 1Mx16 BotB     (2MB)
//                     - MBM29LV160T 1Mx16 TopB     (2MB)
//                     - MX29LV161B 1Mx16 BotB      (2MB)
//                     - MX29LV161T 1Mx16 TopB      (2MB)
//                     - ST M29W160EB 1Mx16 BotB    (2MB)
//                     - ST M29W160ET 1Mx16 TopB    (2MB)
//
//  New for v4.2 - Changed the chip_detect routine to allow for easier
//                 additions of new chip id's.
//               - Added detection support for the Broadcom BCM5350 chip.
//               - Fixed DMA routines to check status bit that was
//                 removed in prior version.
//               - Removed clockout routine in an effort to speed up access.
//               - Changed clockin routine in an effort to speed up access.
//               - Changed ReadData and WriteData routines to merely call
//                 ReadWriteData routine.
//               - Removed Defines from .h file and placed flash areas in a
//                 structure list for easier maintenance should they change.
//               - Miscellaneous other minor changes.
//
// **************************************************************************
//  Written by HairyDairyMaid (a.k.a. - lightbulb)
//  hairydairymaid@yahoo.com
// **************************************************************************
//
//  This program is copyright (C) 2004 HairyDairyMaid (a.k.a. Lightbulb)
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of version 2 the GNU General Public License as published
//  by the Free Software Foundation.
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//  more details.
//  To view a copy of the license go to:
//  http://www.fsf.org/copyleft/gpl.html
//  To receive a copy of the GNU General Public License write the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef WINDOWS_VERSION

   #include <unistd.h>
   #include <sys/ioctl.h>

   #ifdef __FreeBSD__
      #include <dev/ppbus/ppi.h>
      #include <dev/ppbus/ppbconf.h>
      #define PPWDATA PPISDATA
      #define PPRSTATUS PPIGSTATUS
   #else
      #include <linux/ppdev.h>
   #endif

#endif

#define true  1
#define false 0

#define RETRY_ATTEMPTS 16

// --- Xilinx Type Cable ---
#define TDI     0
#define TCK     1
#define TMS     2
#define TDO     4

// --- Wiggler Type Cable ---
#define WTDI      3
#define WTCK      2
#define WTMS      1
#define WTDO      7
#define WTRST_N   4

// --- Some EJTAG Instruction Registers ---
#define INSTR_EXTEST    0x00
#define INSTR_IDCODE    0x01
#define INSTR_SAMPLE    0x02
#define INSTR_IMPCODE   0x03
#define INSTR_ADDRESS   0x08
#define INSTR_DATA      0x09
#define INSTR_CONTROL   0x0A
#define INSTR_BYPASS    0xFF

// --- Some EJTAG Bit Masks ---
#define TOF             (1 << 1 )
#define BRKST           (1 << 3 )
#define DRWN            (1 << 9 )
#define DERR            (1 << 10)
#define DSTRT           (1 << 11)
#define SETDEV          (1 << 14)
#define PROBEN          (1 << 15)
#define DMAACC          (1 << 17)
#define PRACC           (1 << 18)
#define PRNW            (1 << 19)
#define DLOCK           (1 << 5 )
#define TIF             (1 << 2 )
#define SYNC            (1 << 23)
#define PRRST           (1 << 16)
#define PERRST          (1 << 20)
#define JTAGBRK         (1 << 12)
#define DNM             (1 << 28)
#define ROCC            (1 << 31)

#define DMA_BYTE        0x00000000  //DMA tranfser size BYTE
#define DMA_HALFWORD    0x00000080  //DMA transfer size HALFWORD
#define DMA_WORD        0x00000100  //DMA transfer size WORD
#define DMA_TRIPLEBYTE  0x00000180  //DMA transfer size TRIPLEBYTE

#define  size4K        0x1000
#define  size8K        0x2000
#define  size16K       0x4000
#define  size32K       0x8000
#define  size64K       0x10000
#define  size128K      0x20000

#define  size1MB       0x100000
#define  size2MB       0x200000
#define  size4MB       0x400000
#define  size8MB       0x800000
#define  size16MB      0x1000000

#define  CMD_TYPE_BSC  0x01
#define  CMD_TYPE_SCS  0x02
#define  CMD_TYPE_AMD  0x03
#define  CMD_TYPE_SST  0x04

#define  STATUS_READY  0x0080


// EJTAG DEBUG Unit Vector on Debug Break
#define MIPS_DEBUG_VECTOR_ADDRESS           0xFF200200

// Our 'Pseudo' Virtual Memory Access Registers
#define MIPS_VIRTUAL_ADDRESS_ACCESS         0xFF200000
#define MIPS_VIRTUAL_DATA_ACCESS            0xFF200004


// --- Uhh, Just Because I Have To ---
void chip_detect(void);
void chip_shutdown(void);
static unsigned char clockin(int tms, int tdi);
void define_block(unsigned int block_count, unsigned int block_size);
static unsigned int ejtag_read(unsigned int addr);
static unsigned int ejtag_read_h(unsigned int addr);
void ejtag_write(unsigned int addr, unsigned int data);
void ejtag_write_h(unsigned int addr, unsigned int data);
static unsigned int ejtag_dma_read(unsigned int addr);
static unsigned int ejtag_dma_read_h(unsigned int addr);
void ejtag_dma_write(unsigned int addr, unsigned int data);
void ejtag_dma_write_h(unsigned int addr, unsigned int data);
static unsigned int ejtag_pracc_read(unsigned int addr);
void ejtag_pracc_write(unsigned int addr, unsigned int data);
static unsigned int ejtag_pracc_read_h(unsigned int addr);
void ejtag_pracc_write_h(unsigned int addr, unsigned int data);
void identify_flash_part(void);
void lpt_closeport(void);
void lpt_openport(void);
static unsigned int ReadData(void);
static unsigned int ReadWriteData(unsigned int in_data);
void run_backup(char *filename, unsigned int start, unsigned int length);
void run_erase(char *filename, unsigned int start, unsigned int length);
void run_flash(char *filename, unsigned int start, unsigned int length);
void set_instr(int instr);
void sflash_config(void);
void sflash_erase_area(unsigned int start, unsigned int length);
void sflash_erase_block(unsigned int addr);
void sflash_probe(void);
void sflash_reset(void);
void sflash_write_word(unsigned int addr, unsigned int data);
void show_usage(void);
void ShowData(unsigned int value);
void test_reset(void);
void WriteData(unsigned int in_data);
void ExecuteDebugModule(unsigned int *pmodule);
void check_ejtag_features(void);


unsigned int pracc_readword_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Read Word Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the read
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the word @R2
  0x8C430000,  // lw $3, 0($2)
               // 
               // # Store the value into the pseudo-data register
  0xAC230004,  // sw $3, 4($1)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_writeword_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Write Word Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the write
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the data from pseudo-data register
  0x8C230004,  // lw $3, 4($1)
               // 
               // # Store the word at @R2 (the address)
  0xAC430000,  // sw $3,  ($2)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_readhalf_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Read HalfWord Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the read
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the half word @R2
  0x94430000,  // lhu $3, 0($2)
               // 
               // # Store the value into the pseudo-data register
  0xAC230004,  // sw $3, 4($1)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_writehalf_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Write HalfWord Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the write
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the data from pseudo-data register
  0x8C230004,  // lw $3, 4($1)
               // 
               // # Store the half word at @R2 (the address)
  0xA4430000,  // sh $3,  ($2)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


// **************************************************************************
// End of File
// **************************************************************************
