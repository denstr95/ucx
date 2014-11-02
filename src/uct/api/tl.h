/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2014.  ALL RIGHTS RESERVED.
*
* $COPYRIGHT$
* $HEADER$
*/

#ifndef UCT_TL_H_
#define UCT_TL_H_

#if !defined(UCT_H_)
#  error "You should not include this header directly. Include uct.h instead."
#endif

#include "uct_def.h"

#include <ucs/type/status.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sched.h>


/**
 * Communication interface context
 */
typedef struct uct_iface {
    uct_tl_ops_t             *ops;
} uct_iface_t;


/**
 * Remote endpoint
 */
typedef struct uct_ep {
    uct_tl_ops_t             *ops;
} uct_ep_t;


/**
 * Send completion callback.
 */
typedef void (*uct_completion_cb_t)(uct_req_h req, ucs_status_t status);


/**
 * Interface attributes: capabilities and limitations.
 */
typedef struct uct_iface_attr {
    size_t                   max_short;
    size_t                   max_bcopy;
    size_t                   max_zcopy;
    size_t                   iface_addr_len;
    size_t                   ep_addr_len;
    unsigned                 flags;
} uct_iface_attr_t;


/**
 * Communication resource.
 */
typedef struct uct_resource_desc {
    char                     tl_name[64];  /* Transport name */
    char                     hw_name[64];  /* Hardware resource name */
    uint64_t                 latency;      /* Latency, nanoseconds */
    size_t                   bandwidth;    /* Bandwidth, bytes/second */
    cpu_set_t                local_cpus;   /* Mask of CPUs near the resource */
    socklen_t                addrlen;      /* Size of address */
    struct sockaddr_storage  subnet_addr;  /* Subnet address. Devices which can
                                              reach each other have same address */
} uct_resource_desc_t;


/**
 * Transport operations.
 */
struct uct_tl_ops {

    ucs_status_t (*query_resources)(uct_context_h context,
                                    uct_resource_desc_t **resources_p,
                                    unsigned *num_resources_p);

    ucs_status_t (*iface_open)(uct_context_h context, const char *hw_name,
                               uct_iface_h *iface_p);
    void         (*iface_close)(uct_iface_h iface);

    ucs_status_t (*iface_query)(uct_iface_h iface,
                                uct_iface_attr_t *iface_attr);
    ucs_status_t (*iface_get_address)(uct_iface_h iface,
                                      uct_iface_addr_t *iface_addr);

    ucs_status_t (*ep_create)(uct_ep_h *ep_p);
    void         (*ep_destroy)(uct_ep_h ep);

    ucs_status_t (*ep_get_address)(uct_ep_h *ep,
                                     uct_ep_addr_t *ep_addr);
    ucs_status_t (*ep_connect_to_iface)(uct_iface_addr_t *iface_addr);
    ucs_status_t (*ep_connect_to_ep)(uct_iface_addr_t *iface_addr,
                                       uct_ep_addr_t *ep_addr);

    ucs_status_t (*ep_put_short)(uct_ep_h ep, void *buffer, unsigned length,
                                   uct_rkey_t rkey, uct_req_h *req_p,
                                   uct_completion_cb_t cb);

    ucs_status_t (*iface_flush)(uct_iface_h iface, uct_req_h *req_p,
                                  uct_completion_cb_t cb);
};


#endif
