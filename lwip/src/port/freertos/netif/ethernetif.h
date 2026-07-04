#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/sys.h"

#include "xmc_eth_mac.h"
#include "xmc_eth_phy.h"

typedef enum ETHIF_ERROR
{
  ETHIF_ERROR_NONE,          /**< No error*/
  ETHIF_ERROR_PHY_BUSY,      /**< PHY is busy*/
  ETHIF_ERROR_PHY_ERROR,     /**< PHY status error*/
  ETHIF_ERROR_PHY_DEVICE_ID, /**< Wrong PHY ID used*/
  ETHIF_ERROR_PHY_TIMEOUT    /**< PHY read failed*/
} ETHIF_ERROR_t;

typedef struct ETHIF
{ 
  XMC_ETH_MAC_t mac;
  XMC_ETH_PHY_CONFIG_t phy;
  uint8_t phy_addr;
} ETHIF_t;

err_t ethernetif_init(struct netif *netif);
void ethernetif_error(ETHIF_ERROR_t error_code);
void ethernetif_input(void *arg);
void ethernetif_poll(struct netif *netif);

#endif 
