#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SYS                  0

#define LWIP_NETIF_LINK_CALLBACK    1
#define LWIP_NETIF_STATUS_CALLBACK  1

#define MEM_ALIGNMENT           8

#define TCPIP_MBOX_SIZE             32
#define DEFAULT_TCP_RECVMBOX_SIZE   16
#define DEFAULT_UDP_RECVMBOX_SIZE   16
#define DEFAULT_RAW_RECVMBOX_SIZE   16
#define DEFAULT_ACCEPTMBOX_SIZE     16

#define DEFAULT_THREAD_PRIO       tskIDLE_PRIORITY
#define DEFAULT_THREAD_STACKSIZE  configMINIMAL_STACK_SIZE
#define TCPIP_THREAD_PRIO         (configMAX_PRIORITIES - 1)
#define TCPIP_THREAD_STACKSIZE    (configMINIMAL_STACK_SIZE * 8)

#define MEM_SIZE                (32 * 1024)

#define PBUF_POOL_SIZE          128
#define PBUF_POOL_BUFSIZE       256
#define MEMP_NUM_PBUF           128
#define MEMP_NUM_TCP_SEG        32
#define MEMP_NUM_UDP_PCB        4
#define MEMP_NUM_TCP_PCB        4
#define MEMP_NUM_TCP_PCB_LISTEN 4

#define LWIP_UDP                1
#define LWIP_TCP                1
#define TCP_MSS                 1460
#define TCP_WND                 (8 * TCP_MSS)
#define TCP_SND_BUF             (8 * TCP_MSS)
#define TCP_SND_QUEUELEN        16

#define IP_OPTIONS_ALLOWED      1
#define IP_FRAG                 1
#define IP_REASSEMBLY           1

#define CHECKSUM_GEN_IP         0
#define CHECKSUM_GEN_UDP        0
#define CHECKSUM_GEN_TCP        0
#define CHECKSUM_GEN_ICMP       0
#define CHECKSUM_CHECK_IP       0
#define CHECKSUM_CHECK_UDP      0
#define CHECKSUM_CHECK_TCP      0
#define CHECKSUM_CHECK_ICMP     0

#define LWIP_DHCP 1
#define LWIP_DEBUG
#define DHCP_DEBUG LWIP_DBG_ON

#define MEMP_NUM_SYS_TIMEOUT    (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 8)

#define LWIP_SOCKET             (NO_SYS==0)
#define LWIP_NETCONN            (NO_SYS==0)
#define SYS_LIGHTWEIGHT_PROT    (NO_SYS==0)

#define LWIP_COMPAT_SOCKETS        1
#define LWIP_SO_RCVTIMEO           1
#define LWIP_SO_RCVBUF             1

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

#define LWIP_PROVIDE_ERRNO 1

//#define MEMCPY(dst, src, len)   fast_memcpy(dst,src,len)

#define NETIF_COUNT 1
#define ISR_PRIORITY_ETH_IRQ 3

#ifdef __cplusplus
}
#endif
#endif /* __LWIPOPTS_H__ */

