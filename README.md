HairyDairyMaid WRT54G Debrick Utility
=====================================

This is a mirror of the original HairyDairyMaid JTAG Debrick Utility v4.8.

This program is for De-Bricking the WRT54G/GS routers.

This software was found on the [download.modem-help.co.uk][1] website, along with other helpful JTAG tools.

I have made this source code freely available according to the terms of the GPLv2 
for purposes of gifting it back to the community, and to spur the sharing of further improvements and 
forks of this great software that has brought so many routers back to life :smile_cat:.

Forks & Modifications
=====================

I am not currently interested in maintaining this software, but am completely open to anyone improving it,
forking and taking it in new directions.

Many forks and modifications are available for this utility including but not limited to:

 - [zjtag][4] - An EJTAG Debrick Utility based on BrJTAG
 - [brjtag][5] - Broadcom EJTAG Debrick Utility - by hugebird
 - [jpt][6] - JTag programming tools (looks like a small wrapper script project over brjtag to modify a [CFE (Common Firmware Environment)][7] and modify the stored MAC address for a router so routers with broken CFE can be repaired and programmed with their original MAC)
 - [bpjtag][8] - HairyDairyMaid's debrick tool ported to use Bus Pirate OpenOCD mode
 - [Various JTAG Software][9] - Modem-Help.org's listing of JTAG Tools (Please consider making a donation to help support their community)


If you have found a fork that you want to put on this list, feel free to submit a Pull Request to have it added to the README.

License
=======

This software is the original work of "HairyDairyMaid (a.k.a. Lightbulb)".  The original source code for `wrt54g.h` and `wrt54g.c`
contain the [GPLv2][2] license notice, and are:

Copyright (c) 2004 HairyDairyMaid (a.k.a. Lightbulb)

[Copyleft][3] &nbsp;&nbsp; ᵓ ⃝  &nbsp;2004 HairyDairyMaid (a.k.a. Lightbulb)

The license text has been also included in the file `LICENSE` in this repo.

[1]: http://download.modem-help.co.uk/utilities/JTAG/Software/
[2]: http://choosealicense.com/licenses/gpl-2.0/
[3]: http://en.wikipedia.org/wiki/Copyleft
[4]: https://github.com/zoobab/zjtag "EJTAG Debrick Utility based on BrJTAG"
[5]: http://www.codeforge.com/article/211104
[6]: https://github.com/beoran/jpt
[7]: http://www.tiaowiki.com/w/Debrick_Routers_Using_JTAG_Cable#Common_Firmware_Environment_.28CFE.29
[8]: https://github.com/notch/bpjtag
[9]: http://download.modem-help.co.uk/utilities/JTAG/Software/
