#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SYS                  0
#define LWIP_SOCKET             1
#define LWIP_NETCONN            1
#define SYS_LIGHTWEIGHT_PROT    1

#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#define ETH_PAD_SIZE            2

#define MEM_ALIGNMENT           4

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#define TCPIP_THREAD_STACKSIZE  1024
#define DEFAULT_THREAD_STACKSIZE 1024

#define MEM_SIZE                (16 * 1024)

#define PBUF_POOL_SIZE          64
#define PBUF_POOL_BUFSIZE       256
#define MEMP_NUM_PBUF           16
#define MEMP_NUM_TCP_SEG        16

#define LWIP_DHCP               0

#define LWIP_UDP                1
#define LWIP_TCP                1
#define TCP_MSS                 1460
#define TCP_WND                 (4 * TCP_MSS)
#define TCP_SND_BUF             (4 * TCP_MSS)
#define TCP_SND_QUEUELEN        8

#define CHECKSUM_GEN_IP         0
#define CHECKSUM_GEN_UDP        0
#define CHECKSUM_GEN_TCP        0
#define CHECKSUM_GEN_ICMP       0
#define CHECKSUM_CHECK_IP       0
#define CHECKSUM_CHECK_UDP      0
#define CHECKSUM_CHECK_TCP      0
#define CHECKSUM_CHECK_ICMP     0

#define MEMP_NUM_SYS_TIMEOUT    (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 8)

#define LWIP_FREERTOS_CHECK_CORE_LOCKING 1
#define LWIP_TCPIP_CORE_LOCKING 1
#define LWIP_NETCONN_SEM_PER_THREAD 1

#if !NO_SYS
void sys_check_core_locking(void);
#define LWIP_ASSERT_CORE_LOCKED()  sys_check_core_locking()
void sys_mark_tcpip_thread(void);
#define LWIP_MARK_TCPIP_THREAD()   sys_mark_tcpip_thread()

#if LWIP_TCPIP_CORE_LOCKING
void sys_lock_tcpip_core(void);
#define LOCK_TCPIP_CORE()          sys_lock_tcpip_core()
void sys_unlock_tcpip_core(void);
#define UNLOCK_TCPIP_CORE()        sys_unlock_tcpip_core()
#endif
#endif

#define LWIP_HTTPD_SSI 1
#define LWIP_HTTPD_CGI 1
#define LWIP_HTTPD_MAX_TAG_NAME_LEN 20
#define LWIP_HTTPD_SSI_INCLUDE_TAG 0

#ifdef __cplusplus
}
#endif
#endif /* __LWIPOPTS_H__ */

