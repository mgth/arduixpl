/*****************************************************************************
CORE XPL OPTIONS
*****************************************************************************/
//interval to send config.basic messages
#define XPL_CONFIG_INTERVAL 1
//interval to send hbeat.basic messages
#define XPL_HBEAT_INTERVAL 5


/*****************************************************************************
DEBUG OPTIONS
*****************************************************************************/
// To print debug info
#define XPL_DEBUG
// to print memory related debug info
#define XPL_DEBUG_MEM
// to print debug info on LCD
#define XPL_DEBUG_LCD

//uncomment to reduce main loop speed, mainly useful to know whitch module hang
//#define XPL_SLOWDEBUG 100


/*****************************************************************************
ETHERNET OPTIONS
*****************************************************************************/

#define ENC28J60_PIN 10
#define XPL_DHCP 
// xPL default port, do not change
#define XPL_PORT 3865
