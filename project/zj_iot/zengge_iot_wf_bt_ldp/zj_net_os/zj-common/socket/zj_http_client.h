/* tcp_perf Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#ifndef __ZJHTTP_CLIENT_H__
#define __ZJHTTP_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/opt.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/altcp.h"
#include "lwip/prot/iana.h"
#include "lwip/pbuf.h"

/**
 * @ingroup httpc 
 * The default TCP port used for HTTP
 */
#define ZJHTTP_DEFAULT_PORT         LWIP_IANA_PORT_HTTP

/**
 * @ingroup httpc 
 * HTTP client result codes
 */
typedef enum ezjhttpc_result {
  /** File successfully received */
  ZJHTTPC_RESULT_OK            = 0,
  /** Unknown error */
  ZJHTTPC_RESULT_ERR_UNKNOWN   = 1,
  /** Connection to server failed */
  ZJHTTPC_RESULT_ERR_CONNECT   = 2,
  /** Failed to resolve server hostname */
  ZJHTTPC_RESULT_ERR_HOSTNAME  = 3,
  /** Connection unexpectedly closed by remote server */
  ZJHTTPC_RESULT_ERR_CLOSED    = 4,
  /** Connection timed out (server didn't respond in time) */
  ZJHTTPC_RESULT_ERR_TIMEOUT   = 5,
  /** Server responded with an error code */
  ZJHTTPC_RESULT_ERR_SVR_RESP  = 6,
  /** Local memory error */
  ZJHTTPC_RESULT_ERR_MEM       = 7,
  /** Local abort */
  ZJHTTPC_RESULT_LOCAL_ABORT   = 8,
  /** Content length mismatch */
  ZJHTTPC_RESULT_ERR_CONTENT_LEN = 9
} zj_httpc_result_t;

typedef struct _zjhttpc_state zjhttpc_state_t;

/**
 * @ingroup httpc 
 * Prototype of a http client callback function
 *
 * @param arg argument specified when initiating the request
 * @param httpc_result result of the http transfer (see enum zj_httpc_result_t)
 * @param rx_content_len number of bytes received (without headers)
 * @param srv_res this contains the http status code received (if any)
 * @param err an error returned by internal lwip functions, can help to specify
 *            the source of the error but must not necessarily be != ERR_OK
 */
typedef void (*zj_httpc_result_fn)(void *arg, zj_httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err);

/**
 * @ingroup httpc 
 * Prototype of http client callback: called when the headers are received
 *
 * @param connection http client connection
 * @param arg argument specified when initiating the request
 * @param hdr header pbuf(s) (may contain data also)
 * @param hdr_len length of the heders in 'hdr'
 * @param content_len content length as received in the headers (-1 if not received)
 * @return if != ERR_OK is returned, the connection is aborted
 */
typedef err_t (*zj_httpc_headers_done_fn)(zjhttpc_state_t *connection, void *arg, struct pbuf *hdr, u16_t hdr_len, u32_t content_len,u32_t resp_code);//modify by hings

typedef struct _zj_httpc_connection {
  ip_addr_t proxy_addr;
  u16_t proxy_port;
  u8_t use_proxy;
#define REQ_TYPE_GET   0
#define REQ_TYPE_POST  1
  u8_t req_type;
  u8_t req_content_len;
  char *req_params;
#define CONTENT_TYPE_WWW   0 // application/x-www-form-urlencoded
#define CONTENT_TYPE_JSON  1 // application/json
#define CONTENT_TYPE_MULT  2 // multipart/form-data
#define CONTENT_TYPE_TEXT  3 // text/xml
  u8_t content_type;
  
  /* @todo: add username:pass? */

  altcp_allocator_t *altcp_allocator;

  /* this callback is called when the transfer is finished (or aborted) */
  zj_httpc_result_fn result_fn;
  /* this callback is called after receiving the http headers
     It can abort the connection by returning != ERR_OK */
  zj_httpc_headers_done_fn headers_done_fn;
} zj_httpc_connection_t;

err_t
zjhttpc_connect(
              const zj_httpc_connection_t *settings,
              altcp_recv_fn recv_fn,
              void* callback_arg, 
              zjhttpc_state_t **connection,
              const char* server_name, 
              u16_t port,
              const char* uri
              );

err_t zj_httpc_delete(zjhttpc_state_t* req);   

#endif /*#ifndef __TCP_PERF_H__*/

