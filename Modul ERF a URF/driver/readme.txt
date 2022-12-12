ERF/URF Configuration Wizards
=============================

The PICAXE ERF and URF/SRF configuration wizards enable you to set the communication radio frequency of the two radio units. 

Both units are supplied factory programmed ready to use - so only use these wizards if you specifically want to change the default frequency.
Both units must be set to the same frequency to be able to communicate. 
There are 6 different frequencies, enabling up to 6 pairs of units to be used in the same room. The default setting is 868.3MHz.
 
The change can be temporary (until the radio unit’s power is removed) or permanent (saved in EEPROM memory and used at every power-up).

The ERF wizard also allows the default (power up) baud rate of the ERF module to be configured. The factory setting is 9600. 
Note that this baud rate is only used for ERF to ERF communication, when the ERF is paired with a PICAXE URF module the baud rate is controlled automatically by the PICAXE URF module. A non-PICAXE URF does not automatically set the baud rate. 

Note the URF does not need a default baud rate setting, it simply uses whatever baud rate is defined by the computer software.

The URF wizard programs the URF directly – simply insert the URF into the computer and select its virtual COM port number within the wizard.

The ERF wizard generates a PICAXE BASIC program that can be copied and pasted into the PICAXE Editor software. 
The program is then downloaded into the PICAXE chip connected to the ERF. When the program runs the ERF module is then updated. 
The BASIC program includes feedback messages, so ensure the programming cable is left in place whilst the program runs (so that the feedback messages can be seen on screen in the Serial Terminal).


URF/XRF/ERF Wizard
==================

The advanced URF/XRF/ERF wizard is for experienced users, so only use if you know what you are doing!


.Net Framework
==============

These wizards require the Microsoft .Net Framework 3.5.1 (SP1) framework to run, which is normally preinstalled in Windows 7/8 by default. 
If not already installed then it can be enabled via Control Panel>Programs>Turn Windows Features On and Off

Windows Vista/XP users may need to manually download and install the framework (if not already present on that computer). 
This download is available at:
http://www.microsoft.com/en-us/download/details.aspx?id=22
