// **************************************************************************
//
//  WRT54G.C - WRT54G/GS EJTAG Debrick Utility  v4.8
//
//  Note:
//  This program is for De-Bricking the WRT54G/GS and other misc routers.
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
//  New for v4.3 - Corrected Macronix Flash Chip Block Definitions.
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
//
//  wrt54g: read/write flash memory via EJTAG
//   usage: wrt54g [parameter] </noreset> </noemw> </nocwd> </nobreak> </noerase>
//                             </notimestamp> </dma> </nodma>
//                             <start:XXXXXXXX> </length:XXXXXXXX>
//                             </silent> </skipdetect> </instrlen:XX> </fc:XX>
//
//              Required Parameter
//              ------------------
//              -backup:cfe
//              -backup:nvram
//              -backup:kernel
//              -backup:wholeflash
//              -backup:custom
//              -erase:cfe
//              -erase:nvram
//              -erase:kernel
//              -erase:wholeflash
//              -erase:custom
//              -flash:cfe
//              -flash:nvram
//              -flash:kernel
//              -flash:wholeflash
//              -flash:custom
//              -probeonly
//
//              Optional Switches
//              -----------------
//              /noreset ........... prevent Issuing EJTAG CPU reset
//              /noemw ............. prevent Enabling Memory Writes
//              /nocwd ............. prevent Clearing CPU Watchdog Timer
//              /nobreak ........... prevent Issuing Debug Mode JTAGBRK
//              /noerase ........... prevent Forced Erase before Flashing
//              /notimestamp ....... prevent Timestamping of Backups
//              /dma ............... force use of DMA routines
//              /nodma ............. force use of PRACC routines (No DMA)
//              /start:XXXXXXXX .... custom start location (in HEX)
//              /length:XXXXXXXX ... custom length (in HEX)
//              /silent ............ prevent scrolling display of data
//              /skipdetect ........ skip auto detection of CPU Chip ID
//              /instrlen:XX ....... set instruction length manually
//              /wiggler ........... use wiggler cable
//              /fc:XX = Optional (Manual) Flash Chip Selection
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

// Default is Compile for Linux (both #define's below should be commented out)
// #define WINDOWS_VERSION   // uncomment only this for Windows Compile / MS Visual C Compiler
// #define __FreeBSD__       // uncomment only this for FreeBSD

#ifdef WINDOWS_VERSION
   #include <windows.h>      // Only for Windows Compile
   #define strcasecmp  stricmp
   #define strncasecmp strnicmp
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wrt54g.h"

static unsigned int ctrl_reg;

int pfd;
int instruction_length;
int issue_reset      = 1;
int issue_enable_mw  = 1;
int issue_watchdog   = 1;
int issue_break      = 1;
int issue_erase      = 1;
int issue_timestamp  = 1;
int force_dma        = 0;
int force_nodma      = 0;
int selected_fc      = 0;
unsigned int selected_window  = 0;
unsigned int selected_start   = 0;
unsigned int selected_length  = 0;
int custom_options   = 0;
int silent_mode      = 0;
int skipdetect       = 0;
int instrlen         = 0;
int wiggler          = 0;


char            flash_part[128];
unsigned int    flash_size = 0;

int             block_total = 0;
unsigned int    block_addr = 0;
unsigned int    blocks[1024];
unsigned int    cmd_type = 0;

char            AREA_NAME[128];
unsigned int    AREA_START;
unsigned int    AREA_LENGTH;
unsigned int    FLASH_MEMORY_START;
unsigned int    vendid;
unsigned int    devid;

unsigned int    data_register;
unsigned int    address_register;

int USE_DMA       = 0;
int ejtag_version = 0;


typedef struct _processor_chip_type {
    unsigned int        chip_id;        // Processor Chip ID
    int                 instr_length;   // EJTAG Instruction Length
    char*               chip_descr;     // Processor Chip Description
} processor_chip_type;


processor_chip_type  processor_chip_list[] = {
   { 0x0471017F, 5, "Broadcom BCM4702 Rev 1 CPU" },
   { 0x1471217F, 8, "Broadcom BCM4712 Rev 1 CPU" },
   { 0x2471217F, 8, "Broadcom BCM4712 Rev 2 CPU" },
   { 0x0535017F, 8, "Broadcom BCM5350 Rev 1 CPU" },
   { 0x0535217F, 8, "Broadcom BCM5352 Rev 1 CPU" },
   { 0x0536517F, 8, "Broadcom BCM5365 Rev 1 CPU" },         // BCM5365 Not Completely Verified Yet
   { 0x0634817F, 5, "Broadcom BCM6348 Rev 1 CPU" },   
   { 0x0634517F, 5, "Broadcom BCM6345 Rev 1 CPU" },         // BCM6345 Not Completely Verified Yet
   { 0x0000100F, 5, "TI AR7WRD TNETD7300GDU Rev 1 CPU" },   // TI AR7WRD Only Partially Verified
   { 0x0470417F, 8, "Broadcom BCM4704 Rev 8 CPU" },         // BCM4704 chip (used in the WRTSL54GS units)
   { 0x102002E1, 5, "BRECIS MSP2007-CA-A1 CPU" },           // BRECIS chip - Not Completely Verified Yet
   { 0, 0, 0 }
   };


typedef struct _flash_area_type {
    unsigned int        chip_size;
    char*               area_name;
    unsigned int        area_start;
    unsigned int        area_length;
} flash_area_type;


flash_area_type  flash_area_list[] = {
   //---------   ----------     -----------  ------------
   //chip_size   area_name      area_start   area_length
   //---------   ----------     -----------  ------------
   { size1MB,    "CFE",         0x1FC00000,  0x40000 },
   { size2MB,    "CFE",         0x1FC00000,  0x40000 },
   { size4MB,    "CFE",         0x1FC00000,  0x40000 },
   { size8MB,    "CFE",         0x1C000000,  0x40000 },
   { size16MB,   "CFE",         0x1C000000,  0x40000 },

   { size1MB,    "KERNEL",      0x1FC40000,  0xB0000  },
   { size2MB,    "KERNEL",      0x1FC40000,  0x1B0000 },
   { size4MB,    "KERNEL",      0x1FC40000,  0x3B0000 },
   { size8MB,    "KERNEL",      0x1C040000,  0x7A0000 },
   { size16MB,   "KERNEL",      0x1C040000,  0x7A0000 },

   { size1MB,    "NVRAM",       0x1FCF0000,  0x10000 },
   { size2MB,    "NVRAM",       0x1FDF0000,  0x10000 },
   { size4MB,    "NVRAM",       0x1FFF0000,  0x10000 },
   { size8MB,    "NVRAM",       0x1C7E0000,  0x20000 },
   { size16MB,   "NVRAM",       0x1C7E0000,  0x20000 },

   { size1MB,    "WHOLEFLASH",  0x1FC00000,  0x100000 },
   { size2MB,    "WHOLEFLASH",  0x1FC00000,  0x200000 },
   { size4MB,    "WHOLEFLASH",  0x1FC00000,  0x400000 },
   { size8MB,    "WHOLEFLASH",  0x1C000000,  0x800000 },
   { size16MB,   "WHOLEFLASH",  0x1C000000,  0x800000 },

   { 0, 0, 0, 0 }
   };


typedef struct _flash_chip_type {
    unsigned int        vendid;         // Manufacturer Id
    unsigned int        devid;          // Device Id
    unsigned int        flash_size;     // Total size in MBytes
    unsigned int        cmd_type;       // Device CMD TYPE
    char*               flash_part;     // Flash Chip Description
    unsigned int        region1_num;    // Region 1 block count
    unsigned int        region1_size;   // Region 1 block size
    unsigned int        region2_num;    // Region 2 block count
    unsigned int        region2_size;   // Region 2 block size
    unsigned int        region3_num;    // Region 3 block count
    unsigned int        region3_size;   // Region 3 block size
    unsigned int        region4_num;    // Region 4 block count
    unsigned int        region4_size;   // Region 4 block size
} flash_chip_type;


flash_chip_type  flash_chip_list[] = {
   { 0x0001, 0x2249, size2MB, CMD_TYPE_AMD, "AMD 29lv160DB 1Mx16 BotB   (2MB)"   ,1,size16K,    2,size8K,     1,size32K,  31,size64K },
   { 0x0001, 0x22c4, size2MB, CMD_TYPE_AMD, "AMD 29lv160DT 1Mx16 TopB   (2MB)"   ,31,size64K,   1,size32K,    2,size8K,   1,size16K  },
   { 0x0001, 0x22f9, size4MB, CMD_TYPE_AMD, "AMD 29lv320DB 2Mx16 BotB   (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x0001, 0x22f6, size4MB, CMD_TYPE_AMD, "AMD 29lv320DT 2Mx16 TopB   (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0001, 0x2200, size4MB, CMD_TYPE_AMD, "AMD 29lv320MB 2Mx16 BotB   (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x0001, 0x227E, size4MB, CMD_TYPE_AMD, "AMD 29lv320MT 2Mx16 TopB   (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0001, 0x2201, size4MB, CMD_TYPE_AMD, "AMD 29lv320MT 2Mx16 TopB   (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0089, 0x0018,size16MB, CMD_TYPE_SCS, "Intel 28F128J3 8Mx16       (16MB)"  ,128,size128K, 0,0,          0,0,        0,0        },
   { 0x0089, 0x8891, size2MB, CMD_TYPE_BSC, "Intel 28F160B3 1Mx16 BotB  (2MB)"   ,8,size8K,     31,size64K,   0,0,        0,0        },
   { 0x0089, 0x8890, size2MB, CMD_TYPE_BSC, "Intel 28F160B3 1Mx16 TopB  (2MB)"   ,31,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0089, 0x88C3, size2MB, CMD_TYPE_BSC, "Intel 28F160C3 1Mx16 BotB  (2MB)"   ,8,size8K,     31,size64K,   0,0,        0,0        },
   { 0x0089, 0x88C2, size2MB, CMD_TYPE_BSC, "Intel 28F160C3 1Mx16 TopB  (2MB)"   ,31,size64K,   8,size8K,     0,0,        0,0        },
   { 0x00b0, 0x00d0, size2MB, CMD_TYPE_SCS, "Intel 28F160S3/5 1Mx16     (2MB)"   ,32,size64K,   0,0,          0,0,        0,0        },
   { 0x0089, 0x8897, size4MB, CMD_TYPE_BSC, "Intel 28F320B3 2Mx16 BotB  (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x0089, 0x8896, size4MB, CMD_TYPE_BSC, "Intel 28F320B3 2Mx16 TopB  (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0089, 0x88C5, size4MB, CMD_TYPE_BSC, "Intel 28F320C3 2Mx16 BotB  (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x0089, 0x88C4, size4MB, CMD_TYPE_BSC, "Intel 28F320C3 2Mx16 TopB  (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0089, 0x0016, size4MB, CMD_TYPE_SCS, "Intel 28F320J3 2Mx16       (4MB)"   ,32,size128K,  0,0,          0,0,        0,0        },
   { 0x0089, 0x0014, size4MB, CMD_TYPE_SCS, "Intel 28F320J5 2Mx16       (4MB)"   ,32,size128K,  0,0,          0,0,        0,0        },
   { 0x00b0, 0x00d4, size4MB, CMD_TYPE_SCS, "Intel 28F320S3/5 2Mx16     (4MB)"   ,64,size64K,   0,0,          0,0,        0,0        },
   { 0x0089, 0x8899, size8MB, CMD_TYPE_BSC, "Intel 28F640B3 4Mx16 BotB  (8MB)"   ,8,size8K,     127,size64K,  0,0,        0,0        },
   { 0x0089, 0x8898, size8MB, CMD_TYPE_BSC, "Intel 28F640B3 4Mx16 TopB  (8MB)"   ,127,size64K,  8,size8K,     0,0,        0,0        },
   { 0x0089, 0x88CD, size8MB, CMD_TYPE_BSC, "Intel 28F640C3 4Mx16 BotB  (8MB)"   ,8,size8K,     127,size64K,  0,0,        0,0        },
   { 0x0089, 0x88CC, size8MB, CMD_TYPE_BSC, "Intel 28F640C3 4Mx16 TopB  (8MB)"   ,127,size64K,  8,size8K,     0,0,        0,0        },
   { 0x0089, 0x0017, size8MB, CMD_TYPE_SCS, "Intel 28F640J3 4Mx16       (8MB)"   ,64,size128K,  0,0,          0,0,        0,0        },
   { 0x0089, 0x0015, size8MB, CMD_TYPE_SCS, "Intel 28F640J5 4Mx16       (8MB)"   ,64,size128K,  0,0,          0,0,        0,0        },
   { 0x0004, 0x22F9, size4MB, CMD_TYPE_AMD, "MBM29LV320BE 2Mx16 BotB    (4MB)"   ,1,size16K,    2,size8K,     1,size32K,  63,size64K },
   { 0x0004, 0x22F6, size4MB, CMD_TYPE_AMD, "MBM29LV320TE 2Mx16 TopB    (4MB)"   ,63,size64K,   1,size32K,    2,size8K,   1,size16K  },
   // --- These definitions were defined based off the flash.h in GPL source from Linksys, but appear incorrect ---
   //   { 0x00C2, 0x22A8, size4MB, CMD_TYPE_AMD, "MX29LV320B 2Mx16 BotB      (4MB)"   ,1,size16K,    2,size8K,     1,size32K,  63,size64K },
   //   { 0x00C2, 0x00A8, size4MB, CMD_TYPE_AMD, "MX29LV320B 2Mx16 BotB      (4MB)"   ,1,size16K,    2,size8K,     1,size32K,  63,size64K },
   //   { 0x00C2, 0x00A7, size4MB, CMD_TYPE_AMD, "MX29LV320T 2Mx16 TopB      (4MB)"   ,63,size64K,   1,size32K,    2,size8K,   1,size16K  },
   //   { 0x00C2, 0x22A7, size4MB, CMD_TYPE_AMD, "MX29LV320T 2Mx16 TopB      (4MB)"   ,63,size64K,   1,size32K,    2,size8K,   1,size16K  },
   // --- These below are proper however ---
   { 0x00C2, 0x22A8, size4MB, CMD_TYPE_AMD, "MX29LV320B 2Mx16 BotB      (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x00C2, 0x00A8, size4MB, CMD_TYPE_AMD, "MX29LV320B 2Mx16 BotB      (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x00C2, 0x00A7, size4MB, CMD_TYPE_AMD, "MX29LV320T 2Mx16 TopB      (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x00C2, 0x22A7, size4MB, CMD_TYPE_AMD, "MX29LV320T 2Mx16 TopB      (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   //--- End of Changes ----
   { 0x00BF, 0x2783, size4MB, CMD_TYPE_SST, "SST39VF320 2Mx16           (4MB)"   ,64,size64K,   0,0,          0,0,        0,0        },
   { 0x0020, 0x22CB, size4MB, CMD_TYPE_AMD, "ST 29w320DB 2Mx16 BotB     (4MB)"   ,1,size16K,    2,size8K,     1,size32K,  63,size64K },
   { 0x0020, 0x22CA, size4MB, CMD_TYPE_AMD, "ST 29w320DT 2Mx16 TopB     (4MB)"   ,63,size64K,   1,size32K,    2,size8K,   1,size16K  },
   { 0x00b0, 0x00e3, size4MB, CMD_TYPE_BSC, "Sharp 28F320BJE 2Mx16 BotB (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x0098, 0x009C, size4MB, CMD_TYPE_AMD, "TC58FVB321 2Mx16 BotB      (4MB)"   ,1,size16K,    2,size8K,     1,size32K,  63,size64K },
   { 0x0098, 0x009A, size4MB, CMD_TYPE_AMD, "TC58FVT321 2Mx16 TopB      (4MB)"   ,63,size64K,   1,size32K,    2,size8K,   1,size16K  },
   // --- Add a few new Flash Chip Definitions ---
   { 0x001F, 0x00C0, size4MB, CMD_TYPE_AMD, "AT49BV/LV16X 2Mx16 BotB    (4MB)"   ,8,size8K,     63,size64K,   0,0,        0,0        },
   { 0x001F, 0x00C2, size4MB, CMD_TYPE_AMD, "AT49BV/LV16XT 2Mx16 TopB   (4MB)"   ,63,size64K,   8,size8K,     0,0,        0,0        },
   { 0x0004, 0x2249, size2MB, CMD_TYPE_AMD, "MBM29LV160B 1Mx16 BotB     (2MB)"   ,1,size16K,    2,size8K,     1,size32K,  31,size64K },
   { 0x0004, 0x22c4, size2MB, CMD_TYPE_AMD, "MBM29LV160T 1Mx16 TopB     (2MB)"   ,31,size64K,   1,size32K,    2,size8K,   1,size16K  },
   { 0x00C2, 0x2249, size2MB, CMD_TYPE_AMD, "MX29LV161B 1Mx16 BotB      (2MB)"   ,1,size16K,    2,size8K,     1,size32K,  31,size64K },
   { 0x00C2, 0x22c4, size2MB, CMD_TYPE_AMD, "MX29LV161T 1Mx16 TopB      (2MB)"   ,31,size64K,   1,size32K,    2,size8K,   1,size16K  },
   { 0x0020, 0x2249, size2MB, CMD_TYPE_AMD, "ST M29W160EB 1Mx16 BotB    (2MB)"   ,1,size16K,    2,size8K,     1,size32K,  31,size64K },
   { 0x0020, 0x22c4, size2MB, CMD_TYPE_AMD, "ST M29W160ET 1Mx16 TopB    (2MB)"   ,31,size64K,   1,size32K,    2,size8K,   1,size16K  },
   // --- Add a few new Flash Chip Definitions ---
   { 0x00BF, 0x234B, size4MB, CMD_TYPE_SST, "SST39VF1601 1Mx16 BotB     (2MB)"   ,64,size32K,    0,0,          0,0,        0,0        },
   { 0x00BF, 0x234A, size4MB, CMD_TYPE_SST, "SST39VF1602 1Mx16 TopB     (2MB)"   ,64,size32K,    0,0,          0,0,        0,0        },
   { 0x00BF, 0x235B, size4MB, CMD_TYPE_SST, "SST39VF3201 2Mx16 BotB     (4MB)"   ,128,size32K,   0,0,          0,0,        0,0        },
   { 0x00BF, 0x235A, size4MB, CMD_TYPE_SST, "SST39VF3202 2Mx16 TopB     (4MB)"   ,128,size32K,   0,0,          0,0,        0,0        },
   { 0x00BF, 0x236B, size4MB, CMD_TYPE_SST, "SST39VF6401 4Mx16 BotB     (8MB)"   ,256,size32K,   0,0,          0,0,        0,0        },
   { 0x00BF, 0x236A, size4MB, CMD_TYPE_SST, "SST39VF6402 4Mx16 TopB     (8MB)"   ,256,size32K,   0,0,          0,0,        0,0        },
   // --- Add a few new Flash Chip Definitions ---
   { 0x00EC, 0x2275, size2MB, CMD_TYPE_AMD, "K8D1716UTC  1Mx16 TopB     (2MB)"   ,31,size64K,    8,size8K,     0,0,        0,0        },
   { 0x00EC, 0x2277, size2MB, CMD_TYPE_AMD, "K8D1716UBC  1Mx16 BotB     (2MB)"   ,8,size8K,      31,size64K,   0,0,        0,0        },
   // --- Add a few new Flash Chip Definitions ---
   { 0x00C2, 0x22DA, size1MB, CMD_TYPE_AMD, "MX29LV800BTC 512kx16 TopB  (1MB)"   ,15,size32K,    1,size16K,    2,size4K,   1,size8K   },
   { 0x00C2, 0x225B, size1MB, CMD_TYPE_AMD, "MX29LV800BTC 512kx16 BotB  (1MB)"   ,1,size8K,      2,size4K,     1,size16K,  15,size32K },
   // --- Add a few new Flash Chip Definitions ---
   { 0x00EC, 0x22A0, size2MB, CMD_TYPE_AMD, "K8D3216UTC  2Mx16 TopB     (4MB)"   ,63,size64K,    8,size8K,     0,0,        0,0        },
   { 0x00EC, 0x22A2, size2MB, CMD_TYPE_AMD, "K8D3216UBC  2Mx16 BotB     (4MB)"   ,8,size8K,      63,size64K,   0,0,        0,0        },
   // --- Add a few new Flash Chip Definitions ---
   { 0x00BF, 0x236D, size4MB, CMD_TYPE_SST, "SST39VF6401B 4Mx16 BotB    (8MB)"   ,256,size32K,   0,0,          0,0,        0,0        },
   { 0x00BF, 0x236C, size4MB, CMD_TYPE_SST, "SST39VF6402B 4Mx16 TopB    (8MB)"   ,256,size32K,   0,0,          0,0,        0,0        },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
   };


// -----------------------------------------
// ---- Start of Compiler Specific Code ----
// -----------------------------------------


void lpt_openport(void)
{
   #ifdef WINDOWS_VERSION    // ---- Compiler Specific Code ----

      HANDLE h;

      h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if(h == INVALID_HANDLE_VALUE) {  printf("Couldn't access giveio device\n");   CloseHandle(h);   exit(0);   }
      CloseHandle(h);

   #else                     // ---- Compiler Specific Code ----

      #ifdef __FreeBSD__     // ---- Compiler Specific Code ----

         pfd = open("/dev/ppi0", O_RDWR);
         if (pfd < 0)   {   perror("Failed to open /dev/ppi0");   exit(0);   }
         if ((ioctl(pfd, PPEXCL) < 0) || (ioctl(pfd, PPCLAIM) < 0))  {   perror("Failed to lock /dev/ppi0");   close(pfd);   exit(0);   }

      #else                  // ---- Compiler Specific Code ----

         pfd = open("/dev/parport0", O_RDWR);
         if (pfd < 0)   {   perror("Failed to open /dev/parport0");   exit(0);   }
         if ((ioctl(pfd, PPEXCL) < 0) || (ioctl(pfd, PPCLAIM) < 0))   {   perror("Failed to lock /dev/parport0");   close(pfd);   exit(0);   }

      #endif

   #endif
}


void lpt_closeport(void)
{
   #ifndef WINDOWS_VERSION   // ---- Compiler Specific Code ----

      #ifndef __FreeBSD__    // ---- Compiler Specific Code ----

         if (ioctl(pfd, PPRELEASE) < 0)  {  perror("Failed to release /dev/parport0");  close(pfd);  exit(0);  }

      #endif

      close(pfd);

   #endif
}


static unsigned char clockin(int tms, int tdi)
{
   unsigned char data;

   tms = tms ? 1 : 0;
   tdi = tdi ? 1 : 0;
   	
   if(wiggler) data = (1 << WTDO) | (0 << WTCK) | (tms << WTMS) | (tdi << WTDI) | (1 << WTRST_N);
   else        data = (1 << TDO) | (0 << TCK) | (tms << TMS) | (tdi << TDI);
   #ifdef WINDOWS_VERSION   // ---- Compiler Specific Code ----  
      _outp(0x378, data);  
   #else  
      ioctl(pfd, PPWDATA, &data);  
   #endif

   if(wiggler) data = (1 << WTDO) | (1 << WTCK) | (tms << WTMS) | (tdi << WTDI) | (1 << WTRST_N);
   else        data = (1 << TDO) | (1 << TCK) | (tms << TMS) | (tdi << TDI);
   #ifdef WINDOWS_VERSION   // ---- Compiler Specific Code ----  
      _outp(0x378, data);  
   #else  
      ioctl(pfd, PPWDATA, &data);  
   #endif

   #ifdef WINDOWS_VERSION   // ---- Compiler Specific Code ----  
      data = (unsigned char)_inp(0x379);  
   #else  
      ioctl(pfd, PPRSTATUS, &data);  
   #endif

   data ^= 0x80;
   data >>= TDO;
   data &= 1;

   return data;
}


// ---------------------------------------
// ---- End of Compiler Specific Code ----
// ---------------------------------------


void test_reset(void)
{
    clockin(1, 0);  // Run through a handful of clock cycles with TMS high to make sure
    clockin(1, 0);  // we are in the TEST-LOGIC-RESET state.
    clockin(1, 0);
    clockin(1, 0);
    clockin(1, 0);
    clockin(0, 0);  // enter runtest-idle
}


void set_instr(int instr)
{
    int i;
    static int curinstr = 0xFFFFFFFF;

    if (instr == curinstr)
       return;

    clockin(1, 0);  // enter select-dr-scan
    clockin(1, 0);  // enter select-ir-scan
    clockin(0, 0);  // enter capture-ir
    clockin(0, 0);  // enter shift-ir (dummy)
    for (i=0; i < instruction_length; i++)
    {
        clockin(i==(instruction_length - 1), (instr>>i)&1);
    }
    clockin(1, 0);  // enter update-ir
    clockin(0, 0);  // enter runtest-idle

    curinstr = instr;
}


static unsigned int ReadWriteData(unsigned int in_data)
{
    int i;
    unsigned int out_data = 0;
    unsigned char out_bit;

    clockin(1, 0);  // enter select-dr-scan
    clockin(0, 0);  // enter capture-dr
    clockin(0, 0);  // enter shift-dr
    for(i = 0 ; i < 32 ; i++)
    {
       out_bit  = clockin((i == 31), ((in_data >> i) & 1));
       out_data = out_data | (out_bit << i);
    }
    clockin(1,0);   // enter update-dr
    clockin(0,0);   // enter runtest-idle
    return out_data;
}


static unsigned int ReadData(void)
{
    return ReadWriteData(0x00);
}


void WriteData(unsigned int in_data)
{
    ReadWriteData(in_data);
}


void ShowData(unsigned int value)
{
    int i;
    for (i=0; i<32; i++)
        printf("%d", (value >> (31-i)) & 1);
    printf(" (%08X)\n", value);
}


static unsigned int ejtag_read(unsigned int addr)
{
   if (USE_DMA) return(ejtag_dma_read(addr));
   else return(ejtag_pracc_read(addr));
}


static unsigned int ejtag_read_h(unsigned int addr)
{
   if (USE_DMA) return(ejtag_dma_read_h(addr));
   else return(ejtag_pracc_read_h(addr));
}


void ejtag_write(unsigned int addr, unsigned int data)
{
   if (USE_DMA) ejtag_dma_write(addr, data);
   else ejtag_pracc_write(addr, data);
}


void ejtag_write_h(unsigned int addr, unsigned int data)
{
   if (USE_DMA) ejtag_dma_write_h(addr, data);
   else ejtag_pracc_write_h(addr, data);
}


static unsigned int ejtag_dma_read(unsigned int addr)
{
    unsigned int data;
    int retries = RETRY_ATTEMPTS;

begin_ejtag_dma_read:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Initiate DMA Read & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DRWN | DMA_WORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Read Data
    set_instr(INSTR_DATA);
    data = ReadData();

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_read;
        else  printf("DMA Read Addr = %08x  Data = (%08x)ERROR ON READ\n", addr, data);
    }

    return(data);
}


static unsigned int ejtag_dma_read_h(unsigned int addr)
{
    unsigned int data;
    int retries = RETRY_ATTEMPTS;

begin_ejtag_dma_read_h:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Initiate DMA Read & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DRWN | DMA_HALFWORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Read Data
    set_instr(INSTR_DATA);
    data = ReadData();

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_read_h;
        else  printf("DMA Read Addr = %08x  Data = (%08x)ERROR ON READ\n", addr, data);
    }

    // Handle the bigendian/littleendian
    if ( addr & 0x2 )  data = (data>>16)&0xffff ;
    else               data = (data&0x0000ffff) ;

    return(data);
}


void ejtag_dma_write(unsigned int addr, unsigned int data)
{
    int   retries = RETRY_ATTEMPTS;

begin_ejtag_dma_write:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Setup Data
    set_instr(INSTR_DATA);
    WriteData(data);

    // Initiate DMA Write & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DMA_WORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_write;
        else  printf("DMA Write Addr = %08x  Data = ERROR ON WRITE\n", addr);
    }
}


void ejtag_dma_write_h(unsigned int addr, unsigned int data)
{
    int   retries = RETRY_ATTEMPTS;

begin_ejtag_dma_write_h:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Setup Data
    set_instr(INSTR_DATA);
    WriteData(data);

    // Initiate DMA Write & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DMA_HALFWORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_write_h;
        else  printf("DMA Write Addr = %08x  Data = ERROR ON WRITE\n", addr);
    }
}


static unsigned int ejtag_pracc_read(unsigned int addr)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = 0x0;
   ExecuteDebugModule(pracc_readword_code_module);
   return(data_register);
}


void ejtag_pracc_write(unsigned int addr, unsigned int data)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = data;
   ExecuteDebugModule(pracc_writeword_code_module);
}


static unsigned int ejtag_pracc_read_h(unsigned int addr)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = 0x0;
   ExecuteDebugModule(pracc_readhalf_code_module);
   return(data_register);
}


void ejtag_pracc_write_h(unsigned int addr, unsigned int data)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = data;
   ExecuteDebugModule(pracc_writehalf_code_module);
}


void ExecuteDebugModule(unsigned int *pmodule)
{
   unsigned int ctrl_reg;
   unsigned int address;
   unsigned int data   = 0;
   unsigned int offset = 0;
   int finished = 0;
   int DEBUGMSG = 0;
      
   if (DEBUGMSG) printf("DEBUGMODULE: Start module.\n");
   
   // Feed the chip an array of 32 bit values into the processor via the EJTAG port as instructions.
   while (1)
   {
      // Read the control register.  Make sure an access is requested, then do it.
      while(1) 
      {
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PRACC | PROBEN | SETDEV);
         if (ctrl_reg & PRACC)
            break;
         if (DEBUGMSG) printf("DEBUGMODULE: No memory access in progress!\n");
      }
      
      set_instr(INSTR_ADDRESS);
      address = ReadData();
      
      // Check for read or write
      if (ctrl_reg & PRNW) // Bit set for a WRITE
      {
         // Read the data out
         set_instr(INSTR_DATA);
         data = ReadData();
      
         // Clear the access pending bit (let the processor eat!)
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PROBEN | SETDEV);
      
         // Processor is writing to us
         if (DEBUGMSG) printf("DEBUGMODULE: Write 0x%08X to address 0x%08X\n", data, address);
         // Handle Debug Write
         // If processor is writing to one of our psuedo virtual registers then save off data
         if (address == MIPS_VIRTUAL_ADDRESS_ACCESS)  address_register = data;
         if (address == MIPS_VIRTUAL_DATA_ACCESS)     data_register    = data;
      }
      
      else
      
      {
         // Check to see if its reading at the debug vector.  The first pass through
         // the module is always read at the vector, so the first one we allow.  When
         // the second read from the vector occurs we are done and just exit.
         if (address == MIPS_DEBUG_VECTOR_ADDRESS)
         {
            if (finished++) // Allows ONE pass
            {
               if (DEBUGMSG) printf("DEBUGMODULE: Finished module.\n");
               return;
            }
         }
      
         // Processor is reading from us
         if (address >= MIPS_DEBUG_VECTOR_ADDRESS)
         {
            // Reading an instruction from our module so fetch the instruction from the module
            offset = (address - MIPS_DEBUG_VECTOR_ADDRESS) / 4;
            data = *(unsigned int *)(pmodule + offset);
            if (DEBUGMSG) printf("DEBUGMODULE: Instruction read at 0x%08X  offset -> %04d  data -> 0x%08X\n", address, offset, data); //fflush(stdout);
         }
         else
         {
            // Reading from our virtual register area
            if (DEBUGMSG) printf("DEBUGMODULE: Read address 0x%08X  data = 0x%08X\n", address, data);
            // Handle Debug Read
            // If processor is reading from one of our psuedo virtual registers then give it data
            if (address == MIPS_VIRTUAL_ADDRESS_ACCESS)  data = address_register;
            if (address == MIPS_VIRTUAL_DATA_ACCESS)     data = data_register;
         }
      
         // Send the data out
         set_instr(INSTR_DATA);
         data = ReadWriteData(data);
      
         // Clear the access pending bit (let the processor eat!)
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PROBEN | SETDEV);
      
      }
   }
}


void chip_detect(void)
{
    unsigned int id = 0x0;
    
    processor_chip_type*   processor_chip = processor_chip_list;

    lpt_openport();

    printf("Probing bus ... ");
    
    if (skipdetect)
    {
    	 // Manual Override CPU Chip ID
       test_reset();
       instruction_length = instrlen;
       set_instr(INSTR_IDCODE);
       id = ReadData();
       printf("Done\n\n");
       printf("Instruction Length set to %d\n\n",instruction_length);
       printf("CPU Chip ID: ");  ShowData(id);  printf("*** CHIP DETECTION OVERRIDDEN ***\n\n");
       return;
    }
    else
    {
    	 // Auto Detect CPU Chip ID
       while (processor_chip->chip_id)
       {
          test_reset();
          if (instrlen) 
             instruction_length = instrlen;
          else 
             instruction_length = processor_chip->instr_length;
          set_instr(INSTR_IDCODE);
          id = ReadData();
          if (id == processor_chip->chip_id)
          {
             printf("Done\n\n");
             printf("Instruction Length set to %d\n\n",instruction_length);
             printf("CPU Chip ID: ");  ShowData(id);  printf("*** Found a %s chip ***\n\n", processor_chip->chip_descr);
             return;
          }
          processor_chip++;
       }
    }

    printf("Done\n\n");
    printf("Instruction Length set to %d\n\n",instruction_length);
    printf("CPU Chip ID: ");  ShowData(id);  printf("*** Unknown or NO CPU Chip ID Detected ***\n\n");
    
    printf("*** Possible Causes:\n");
    printf("    1) WRT54G/GS is not Connected.\n");
    printf("    2) WRT54G/GS is not Powered On.\n");
    printf("    3) Improper JTAG Cable.\n");
    printf("    4) Unrecognized CPU Chip ID.\n");

    chip_shutdown();;
    exit(0);
}


void check_ejtag_features() 
{
    unsigned int features;
    
    set_instr(INSTR_IMPCODE);
    features = ReadData();

    printf("    - EJTAG IMPCODE ....... : ");   ShowData(features);
  
    // EJTAG Version 
    ejtag_version = (features >> 29) & 7;
    printf("    - EJTAG Version ....... : ");
    if (ejtag_version == 0)       printf("1 or 2.0\n");
    else if (ejtag_version == 1)  printf("2.5\n");
    else if (ejtag_version == 2)  printf("2.6\n");
    else                          printf("Unknown (%d is a reserved value)\n", ejtag_version);

    // EJTAG DMA Support
    USE_DMA = !(features & (1 << 14));
    printf("    - EJTAG DMA Support ... : %s\n", USE_DMA ? "Yes" : "No");

    if (force_dma)   { USE_DMA = 1;  printf("    *** DMA Mode Forced On ***\n"); }
    if (force_nodma) { USE_DMA = 0;  printf("    *** DMA Mode Forced Off ***\n"); }
        
    printf("\n");
}


void chip_shutdown(void)
{
    fflush(stdout);
    test_reset();
    lpt_closeport();
}


void run_backup(char *filename, unsigned int start, unsigned int length)
{
    unsigned int addr, data;
    FILE *fd;
    int counter = 0;
    int percent_complete = 0;
    char newfilename[128] = "";
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;

    struct tm* lt = localtime(&start_time);
    char time_str[15];
    
    sprintf(time_str, "%04d%02d%02d_%02d%02d%02d",
        lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
        lt->tm_hour, lt->tm_min, lt->tm_sec
    );

    printf("*** You Selected to Backup the %s ***\n\n",filename);

    strcpy(newfilename,filename);
    strcat(newfilename,".SAVED");
    if (issue_timestamp)
    {
       strcat(newfilename,"_");
       strcat(newfilename,time_str);
    }

    fd = fopen(newfilename, "wb" );
    if (fd<=0)
    {
        fprintf(stderr,"Could not open %s for writing\n", newfilename);
        exit(1);
    }

    printf("=========================\n");
    printf("Backup Routine Started\n");
    printf("=========================\n");

    printf("\nSaving %s to Disk...\n",newfilename);
    for(addr=start; addr<(start+length); addr+=4)
    {
        counter += 4;
        percent_complete = (counter * 100 / length);
        if (!silent_mode)
           if ((addr&0xF) == 0)  printf("[%3d%% Backed Up]   %08x: ", percent_complete, addr);
         
        data = ejtag_read(addr);
        fwrite( (unsigned char*) &data, 1, sizeof(data), fd);

       if (silent_mode)  printf("%4d%%   bytes = %d\r", percent_complete, counter);
       else              printf("%08x%c", data, (addr&0xF)==0xC?'\n':' ');
       
       fflush(stdout); 
      }
    fclose(fd);

    printf("Done  (%s saved to Disk OK)\n\n",newfilename);

    printf("bytes written: %d\n", counter);
    
    printf("=========================\n");
    printf("Backup Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}



void run_flash(char *filename, unsigned int start, unsigned int length)
{
    unsigned int addr, data ;
    FILE *fd ;
    int counter = 0;
    int percent_complete = 0;
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;

    printf("*** You Selected to Flash the %s ***\n\n",filename);

    fd=fopen(filename, "rb" );
    if (fd<=0)
    {
        fprintf(stderr,"Could not open %s for reading\n", filename);
        exit(1);
    }

    printf("=========================\n");
    printf("Flashing Routine Started\n");
    printf("=========================\n");

    if (issue_erase) sflash_erase_area(start,length);

    printf("\nLoading %s to Flash Memory...\n",filename);
    for(addr=start; addr<(start+length); addr+=4)
    {
        counter += 4;
        percent_complete = (counter * 100 / length);
        if (!silent_mode)
           if ((addr&0xF) == 0)  printf("[%3d%% Flashed]   %08x: ", percent_complete, addr);

        fread( (unsigned char*) &data, 1,sizeof(data), fd);
        // Erasing Flash Sets addresses to 0xFF's so we can avoid writing these (for speed)
        if (issue_erase) {
           if (!(data == 0xFFFFFFFF))
              sflash_write_word(addr, data);
        }
        else sflash_write_word(addr, data);  // Otherwise we gotta flash it all

        if (silent_mode)  printf("%4d%%   bytes = %d\r", percent_complete, counter);
        else              printf("%08x%c", data, (addr&0xF)==0xC?'\n':' ');
        
        fflush(stdout);         	
        data = 0xFFFFFFFF;  // This is in case file is shorter than expected length
      }
    fclose(fd);
    printf("Done  (%s loaded into Flash Memory OK)\n\n",filename);

    sflash_reset();

    printf("=========================\n");
    printf("Flashing Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}


void run_erase(char *filename, unsigned int start, unsigned int length)
{
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;

    printf("*** You Selected to Erase the %s ***\n\n",filename);

    printf("=========================\n");
    printf("Erasing Routine Started\n");
    printf("=========================\n");

    sflash_erase_area(start,length);
    sflash_reset();

    printf("=========================\n");
    printf("Erasing Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}


void identify_flash_part(void)
{
   flash_chip_type*   flash_chip = flash_chip_list;
   flash_area_type*   flash_area = flash_area_list;

   // Important for these to initialize to zero
   block_addr  = 0;
   block_total = 0;
   flash_size  = 0;
   cmd_type    = 0;
   strcpy(flash_part,"");

   // Funky AMD Chip
   if (((vendid & 0x00ff) == 0x0001) && (devid == 0x227E))  devid = ejtag_read_h(FLASH_MEMORY_START+0x1E);  // Get real devid

   while (flash_chip->vendid)
   {
      if ((flash_chip->vendid == vendid) && (flash_chip->devid == devid))
      {
         flash_size = flash_chip->flash_size;
         cmd_type   = flash_chip->cmd_type;
         strcpy(flash_part, flash_chip->flash_part);

         if (flash_size >= size8MB) FLASH_MEMORY_START = 0x1C000000;
         else FLASH_MEMORY_START = 0x1FC00000;

         while (flash_area->chip_size)
         {
            if ((flash_area->chip_size == flash_size) && (strcasecmp(flash_area->area_name, AREA_NAME)==0))
            {
               strcat(AREA_NAME,".BIN");
               AREA_START  = flash_area->area_start;
               AREA_LENGTH = flash_area->area_length;
               break;
            }
            flash_area++;
         }
         
         if (strcasecmp(AREA_NAME,"CUSTOM")==0)
         {
            strcat(AREA_NAME,".BIN");
            FLASH_MEMORY_START = selected_window;
            AREA_START         = selected_start;
            AREA_LENGTH        = selected_length;
         }

         if (flash_chip->region1_num)  define_block(flash_chip->region1_num, flash_chip->region1_size);
         if (flash_chip->region2_num)  define_block(flash_chip->region2_num, flash_chip->region2_size);
         if (flash_chip->region3_num)  define_block(flash_chip->region3_num, flash_chip->region3_size);
         if (flash_chip->region4_num)  define_block(flash_chip->region4_num, flash_chip->region4_size);

         sflash_reset();

         printf("Done\n\n");
         printf("Flash Vendor ID: ");  ShowData(vendid);
         printf("Flash Device ID: ");  ShowData(devid);
         if (selected_fc != 0)
            printf("*** Manually Selected a %s Flash Chip ***\n\n", flash_part);
         else
            printf("*** Found a %s Flash Chip ***\n\n", flash_part);

         printf("    - Flash Chip Window Start .... : %08x\n", FLASH_MEMORY_START);
         printf("    - Flash Chip Window Length ... : %08x\n", flash_size);
         printf("    - Selected Area Start ........ : %08x\n", AREA_START);
         printf("    - Selected Area Length ....... : %08x\n\n", AREA_LENGTH);
         
         break;
      }
      flash_chip++;
   }
}


void define_block(unsigned int block_count, unsigned int block_size)
{
  unsigned int  i;

  if (block_addr == 0)  block_addr = FLASH_MEMORY_START;

  for (i = 1; i <= block_count; i++)
  {
     block_total++;
     blocks[block_total] = block_addr;
     block_addr = block_addr + block_size;
  }
}


void sflash_config(void)
{
   flash_chip_type*   flash_chip = flash_chip_list;
   int counter = 0;

   printf("\nManual Flash Selection ... ");

   while (flash_chip->vendid)
   {
      counter++;
      if (counter == selected_fc)
      {
         vendid = flash_chip->vendid;
         devid  = flash_chip->devid;
         identify_flash_part();
         break;
      }
      flash_chip++;
   }

    if (strcasecmp(flash_part,"")==0)
       printf("*** Unknown or NO Flash Chip Selected ***\n");

}


void sflash_probe(void)
{
   int retries = 300;

    // Default to Standard Flash Window for Detection if not CUSTOM
    if (strcasecmp(AREA_NAME,"CUSTOM")==0)
         FLASH_MEMORY_START = selected_window;
    else FLASH_MEMORY_START = 0x1FC00000;

    printf("\nProbing Flash at (Flash Window: 0x%08x) ... ", FLASH_MEMORY_START);

again:

    strcpy(flash_part,"");

    // Probe using cmd_type for AMD
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_AMD;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START + (0x555 << 1), 0x00AA00AA);
       ejtag_write_h(FLASH_MEMORY_START + (0x2AA << 1), 0x00550055);
       ejtag_write_h(FLASH_MEMORY_START + (0x555 << 1), 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
    }

    // Probe using cmd_type for SST
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_SST;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START + (0x5555 << 1), 0x00AA00AA);
       ejtag_write_h(FLASH_MEMORY_START + (0x2AAA << 1), 0x00550055);
       ejtag_write_h(FLASH_MEMORY_START + (0x5555 << 1), 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
    }

    // Probe using cmd_type for BSC & SCS
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_BSC;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START, 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
    }

    if (strcasecmp(flash_part,"")==0)
    {
       if (retries--)
          goto again;
       else
       {
       	  printf("Done\n\n");
          printf("*** Unknown or NO Flash Chip Detected ***");
       }
    }

    return;
}


void sflash_poll(unsigned int addr, unsigned int data)
{
    
    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {
       // Wait Until Ready
       while ( (ejtag_read_h(FLASH_MEMORY_START) & STATUS_READY) != STATUS_READY );
    }
    else
    {
       // Wait Until Ready
       while ( (ejtag_read_h(addr) & STATUS_READY) != (data & STATUS_READY) );
    }

}


void sflash_erase_area(unsigned int start, unsigned int length)
{
    int cur_block;
    int tot_blocks;
    unsigned int reg_start;
    unsigned int reg_end;


    reg_start = start;
    reg_end   = reg_start + length;

    tot_blocks = 0;

    for (cur_block = 1;  cur_block <= block_total;  cur_block++)
    {
       block_addr = blocks[cur_block];
       if ((block_addr >= reg_start) && (block_addr < reg_end))  tot_blocks++;
    }

    printf("Total Blocks to Erase: %d\n\n", tot_blocks);

    for (cur_block = 1;  cur_block <= block_total;  cur_block++)
    {
       block_addr = blocks[cur_block];
       if ((block_addr >= reg_start) && (block_addr < reg_end))
          {
             printf("Erasing block: %d (addr = %08x)...", cur_block, block_addr);  fflush(stdout);
             sflash_erase_block(block_addr);
             printf("Done\n");  fflush(stdout);
          }
    }

}


void sflash_erase_block(unsigned int addr)
{

    if (cmd_type == CMD_TYPE_AMD)
    {

        //Unlock Block
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00800080);

        //Erase Block
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
        ejtag_write_h(addr, 0x00300030);

        // Wait for Erase Completion
        sflash_poll(addr, 0xFFFF);

    }

    if (cmd_type == CMD_TYPE_SST)
    {

        //Unlock Block
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00800080);

        //Erase Block
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
        ejtag_write_h(addr, 0x00500050);

        // Wait for Erase Completion
        sflash_poll(addr, 0xFFFF);

    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {

        //Unlock Block
        ejtag_write_h(addr, 0x00500050);     // Clear Status Command
        ejtag_write_h(addr, 0x00600060);     // Unlock Flash Block Command
        ejtag_write_h(addr, 0x00D000D0);     // Confirm Command
        
        // Wait for Unlock Completion
        sflash_poll(addr, STATUS_READY);

        //Erase Block
        ejtag_write_h(addr, 0x00500050);     // Clear Status Command
        ejtag_write_h(addr, 0x00200020);     // Block Erase Command
        ejtag_write_h(addr, 0x00D000D0);     // Confirm Command

        // Wait for Erase Completion
        sflash_poll(addr, STATUS_READY);

    }

    sflash_reset();

}


void sflash_reset(void)
{

    if ((cmd_type == CMD_TYPE_AMD) || (cmd_type == CMD_TYPE_SST))
    {
        ejtag_write_h(FLASH_MEMORY_START, 0x00F000F0);    // Set array to read mode
    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {
        ejtag_write_h(FLASH_MEMORY_START, 0x00500050);    // Clear CSR
        ejtag_write_h(FLASH_MEMORY_START, 0x00ff00ff);    // Set array to read mode
    }

}


void sflash_write_word(unsigned int addr, unsigned int data)
{
unsigned int data_lo, data_hi;

    if (USE_DMA)
    {
    	 // DMA Uses Byte Lanes
       data_lo = data;
       data_hi = data;
    }
    else   
    {
    	 // PrAcc Does Not
       data_lo = (data & 0xFFFF);
       data_hi = ((data >> 16) & 0xFFFF);
    }

    if (cmd_type == CMD_TYPE_AMD)
    {
      // Handle Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00A000A0);
      ejtag_write_h(addr, data_lo);

      // Wait for Completion
      sflash_poll(addr, (data & 0xffff));

      // Now Handle Other Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00A000A0);
      ejtag_write_h(addr+2, data_hi);

      // Wait for Completion
      sflash_poll(addr+2, ((data >> 16) & 0xffff));
    }

    if (cmd_type == CMD_TYPE_SST)
    {
      // Handle Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00A000A0);
      ejtag_write_h(addr, data_lo);

      // Wait for Completion
      sflash_poll(addr, (data & 0xffff));

      // Now Handle Other Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00A000A0);
      ejtag_write_h(addr+2, data_hi);

      // Wait for Completion
      sflash_poll(addr+2, ((data >> 16) & 0xffff));
    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {
       // Handle Half Of Word
       ejtag_write_h(addr, 0x00500050);     // Clear Status Command
       ejtag_write_h(addr, 0x00400040);     // Write Command
       ejtag_write_h(addr, data_lo);        // Send HalfWord Data
       ejtag_write_h(addr, 0x00700070);     // Check Status Command

       // Wait for Completion
       sflash_poll(addr, STATUS_READY);

       // Now Handle Other Half Of Word
       ejtag_write_h(addr+2, 0x00500050);   // Clear Status Command
       ejtag_write_h(addr+2, 0x00400040);   // Write Command
       ejtag_write_h(addr+2, data_hi);      // Send HalfWord Data
       ejtag_write_h(addr+2, 0x00700070);   // Check Status Command

       // Wait for Completion
       sflash_poll(addr+2, STATUS_READY);
    }
}


void show_usage(void)
{

   flash_chip_type*      flash_chip = flash_chip_list;
   processor_chip_type*  processor_chip = processor_chip_list;
   int counter = 0;

   printf( " ABOUT: This program reads/writes flash memory on the WRT54G/GS and\n"
           "        compatible routers via EJTAG using either DMA Access routines\n"
           "        or PrAcc routines (slower/more compatible).  Processor chips\n"
           "        supported in this version include the following chips:\n\n"
           "            Supported Chips\n"
           "            ---------------\n");

           while (processor_chip->chip_id)
           {
              printf("            %-40.40s\n", processor_chip->chip_descr);
              processor_chip++;
           }
           

   printf( "\n\n");
   printf( " USAGE: wrt54g [parameter] </noreset> </noemw> </nocwd> </nobreak> </noerase>\n"
           "                      </notimestamp> </dma> </nodma>\n"
           "                      <start:XXXXXXXX> </length:XXXXXXXX>\n"
           "                      </silent> </skipdetect> </instrlen:XX> </fc:XX>\n\n"

           "            Required Parameter\n"
           "            ------------------\n"
           "            -backup:cfe\n"
           "            -backup:nvram\n"
           "            -backup:kernel\n"
           "            -backup:wholeflash\n"
           "            -backup:custom\n"
           "            -erase:cfe\n"
           "            -erase:nvram\n"
           "            -erase:kernel\n"
           "            -erase:wholeflash\n"
           "            -erase:custom\n"
           "            -flash:cfe\n"
           "            -flash:nvram\n"
           "            -flash:kernel\n"
           "            -flash:wholeflash\n"
           "            -flash:custom\n"
           "            -probeonly\n\n"

           "            Optional Switches\n"
           "            -----------------\n"
           "            /noreset ........... prevent Issuing EJTAG CPU reset\n"
           "            /noemw ............. prevent Enabling Memory Writes\n"
           "            /nocwd ............. prevent Clearing CPU Watchdog Timer\n"
           "            /nobreak ........... prevent Issuing Debug Mode JTAGBRK\n"
           "            /noerase ........... prevent Forced Erase before Flashing\n"
           "            /notimestamp ....... prevent Timestamping of Backups\n"
           "            /dma ............... force use of DMA routines\n"
           "            /nodma ............. force use of PRACC routines (No DMA)\n"
           "            /window:XXXXXXXX ... custom flash window base (in HEX)\n"
           "            /start:XXXXXXXX .... custom start location (in HEX)\n"
           "            /length:XXXXXXXX ... custom length (in HEX)\n"
           "            /silent ............ prevent scrolling display of data\n"
           "            /skipdetect ........ skip auto detection of CPU Chip ID\n"
           "            /instrlen:XX ....... set instruction length manually\n"
           "            /wiggler ........... use wiggler cable\n\n"

           "            /fc:XX = Optional (Manual) Flash Chip Selection\n"
           "            -----------------------------------------------\n");

           while (flash_chip->vendid)
           {
              printf("            /fc:%02d ............. %-40.40s\n", ++counter, flash_chip->flash_part);
              flash_chip++;
           }

   printf( "\n\n");
   printf( " NOTES: 1) If 'flashing' - the source filename must exist as follows:\n"
           "           CFE.BIN, NVRAM.BIN, KERNEL.BIN, WHOLEFLASH.BIN or CUSTOM.BIN\n\n"
           
           "        2) If you have difficulty auto-detecting a particular flash part\n"
           "           you can manually specify your exact part using the /fc:XX option.\n\n"

           "        3) If you have difficulty with the older bcm47xx chips or when no CFE\n"
           "           is currently active/operational you may want to try both the\n"
           "           /noreset and /nobreak command line options together.  Some bcm47xx\n"
           "           chips *may* always require both these options to function properly.\n\n"
           
           "        4) When using this utility, usually it is best to type the command line\n"
           "           out, then plug in the router, and then hit <ENTER> quickly to avoid\n"
           "           the CPUs watchdog interfering with the EJTAG operations.\n\n"
           
           " ***************************************************************************\n"
           " * Flashing the KERNEL or WHOLEFLASH will take a very long time using JTAG *\n"
           " * via this utility.  You are better off flashing the CFE & NVRAM files    *\n"
           " * & then using the normal TFTP method to flash the KERNEL via ethernet.   *\n"
           " ***************************************************************************\n\n");
}


int main(int argc, char** argv)
{
    char choice[128];
    int run_option;
    int j;

    printf("\n");
    printf("====================================\n");
    printf("WRT54G/GS EJTAG Debrick Utility v4.8\n");
    printf("====================================\n\n");

    if (argc < 2)
    {
        show_usage();
        exit(1);
    }
    
    strcpy(choice,argv[1]);

    run_option = 0;

    if (strcasecmp(choice,"-backup:cfe")==0)         { run_option = 1;  strcpy(AREA_NAME, "CFE");        }
    if (strcasecmp(choice,"-backup:nvram")==0)       { run_option = 1;  strcpy(AREA_NAME, "NVRAM");      }
    if (strcasecmp(choice,"-backup:kernel")==0)      { run_option = 1;  strcpy(AREA_NAME, "KERNEL");     }
    if (strcasecmp(choice,"-backup:wholeflash")==0)  { run_option = 1;  strcpy(AREA_NAME, "WHOLEFLASH"); }
    if (strcasecmp(choice,"-backup:custom")==0)      { run_option = 1;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-erase:cfe")==0)          { run_option = 2;  strcpy(AREA_NAME, "CFE");        }
    if (strcasecmp(choice,"-erase:nvram")==0)        { run_option = 2;  strcpy(AREA_NAME, "NVRAM");      }
    if (strcasecmp(choice,"-erase:kernel")==0)       { run_option = 2;  strcpy(AREA_NAME, "KERNEL");     }
    if (strcasecmp(choice,"-erase:wholeflash")==0)   { run_option = 2;  strcpy(AREA_NAME, "WHOLEFLASH"); }
    if (strcasecmp(choice,"-erase:custom")==0)       { run_option = 2;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-flash:cfe")==0)          { run_option = 3;  strcpy(AREA_NAME, "CFE");        }
    if (strcasecmp(choice,"-flash:nvram")==0)        { run_option = 3;  strcpy(AREA_NAME, "NVRAM");      }
    if (strcasecmp(choice,"-flash:kernel")==0)       { run_option = 3;  strcpy(AREA_NAME, "KERNEL");     }
    if (strcasecmp(choice,"-flash:wholeflash")==0)   { run_option = 3;  strcpy(AREA_NAME, "WHOLEFLASH"); }
    if (strcasecmp(choice,"-flash:custom")==0)       { run_option = 3;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-probeonly")==0)          { run_option = 4;  }
    

    if (run_option == 0)
    {
        show_usage();
        printf("\n*** ERROR - Invalid [option] specified ***\n\n");
        exit(1);
    }

    if (argc > 2)
    {
       j = 2;
       while (j < argc)
       {
          strcpy(choice,argv[j]);

          if (strcasecmp(choice,"/noreset")==0)              issue_reset = 0;
          else if (strcasecmp(choice,"/noemw")==0)           issue_enable_mw = 0;
          else if (strcasecmp(choice,"/nocwd")==0)           issue_watchdog = 0;
          else if (strcasecmp(choice,"/nobreak")==0)         issue_break = 0;
          else if (strcasecmp(choice,"/noerase")==0)         issue_erase = 0;
          else if (strcasecmp(choice,"/notimestamp")==0)     issue_timestamp = 0;
          else if (strcasecmp(choice,"/dma")==0)             force_dma = 1;
          else if (strcasecmp(choice,"/nodma")==0)           force_nodma = 1;
          else if (strncasecmp(choice,"/fc:",4)==0)          selected_fc = strtoul(((char *)choice + 4),NULL,10);
          else if (strncasecmp(choice,"/window:",8)==0)    { selected_window = strtoul(((char *)choice + 8),NULL,16); custom_options++;  }
          else if (strncasecmp(choice,"/start:",7)==0)     { selected_start  = strtoul(((char *)choice + 7),NULL,16); custom_options++;  }
          else if (strncasecmp(choice,"/length:",8)==0)    { selected_length = strtoul(((char *)choice + 8),NULL,16); custom_options++;  }
          else if (strcasecmp(choice,"/silent")==0)          silent_mode = 1;
          else if (strcasecmp(choice,"/skipdetect")==0)      skipdetect = 1;
          else if (strncasecmp(choice,"/instrlen:",10)==0)   instrlen = strtoul(((char *)choice + 10),NULL,10);
          else if (strcasecmp(choice,"/wiggler")==0)         wiggler = 1;
          else
          {
             show_usage();
             printf("\n*** ERROR - Invalid <option> specified ***\n\n");
             exit(1);
          }
          j++;
       }
    }
    
    if (strcasecmp(AREA_NAME,"CUSTOM")==0)
    {
       if ((custom_options != 0) && (custom_options != 4))
       {
           show_usage();
          printf("\n*** ERROR - 'CUSTOM' also requires '/window' '/start' and '/length' options ***\n\n");
          exit(1);
       }
    }


    // ----------------------------------
    // Detect CPU 
    // ----------------------------------
    chip_detect();


    // ----------------------------------
    // Find Implemented EJTAG Features
    // ----------------------------------
    check_ejtag_features();


    // ----------------------------------
    // Reset State Machine For Good Measure
    // ----------------------------------
    test_reset();


    // ----------------------------------
    // Reset Processor and Peripherals
    // ----------------------------------
    printf("Issuing Processor / Peripheral Reset ... ");
    if (issue_reset)
    {
       set_instr(INSTR_CONTROL);
       ctrl_reg = ReadWriteData(PRRST | PERRST);
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Enable Memory Writes
    // ----------------------------------
    // Always skip for EJTAG versions 2.5 and 2.6 since they do not support DMA transactions.
    // Memory Protection bit only applies to EJTAG 2.0 based chips.
    if (ejtag_version != 0)  issue_enable_mw = 0;
    printf("Enabling Memory Writes ... ");   
    if (issue_enable_mw)
    {
    	 // Clear Memory Protection Bit in DCR
       ejtag_dma_write(0xff300000, (ejtag_dma_read(0xff300000) & ~(1<<2)) );
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Put into EJTAG Debug Mode
    // ----------------------------------
    printf("Halting Processor ... ");
    if (issue_break)
    {
       set_instr(INSTR_CONTROL);
       ctrl_reg = ReadWriteData(PRACC | PROBEN | SETDEV | JTAGBRK );
       if (ReadWriteData(PRACC | PROBEN | SETDEV) & BRKST)  
          printf("<Processor Entered Debug Mode!> ... ");
       else  
          printf("<Processor did NOT enter Debug Mode!> ... ");
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Clear Watchdog
    // ----------------------------------
    printf("Clearing Watchdog ... ");        
    if (issue_watchdog)
    {
       ejtag_write(0xb8000080,0);
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Flash Chip Detection
    // ----------------------------------
    if (selected_fc != 0)
       sflash_config();
    else
       sflash_probe();


    // ----------------------------------
    // Execute Requested Operation
    // ----------------------------------
    if ((flash_size > 0) && (AREA_LENGTH > 0))
    {
       if (run_option == 1 )  run_backup(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 2 )  run_erase(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 3 )  run_flash(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 4 );  // Probe was already run so nothing else needed
    }

    printf("\n\n *** REQUESTED OPERATION IS COMPLETE ***\n\n");

    chip_shutdown();

    return 0;
}


// **************************************************************************
// End of File
// **************************************************************************
