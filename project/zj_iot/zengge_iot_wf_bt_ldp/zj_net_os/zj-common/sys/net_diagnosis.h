#ifndef NET_DIAG_H_
#define NET_DIAG_H_


#ifdef __cplusplus
extern "C" {
#endif

#define PING_MAX_LIST         5
#define PING_DOMAIN_MAX_LEN   32

typedef enum{

    S2_TCPIP_ERROR = 1,
    S2_PING_LOCAL_IP_FAIL ,
    S2_PING_MASK_FAIL
}s2_err_status_t;

typedef enum{

    S3_REMOTE_OFF = 1,
    S3_DNS_FAIL ,
    S3_SERVER_CONNECT_FAIL,
    S3_LOGIN_FAIL_OTHER,
    S3_LOGIN_KEY_LOSS,
    S3_JSON_WRONG,
    S3_LOGIN_FAIL_MAC,
    S3_LOGIN_FAIL_ENC
}s3_err_status_t;


typedef enum{
    ANALYZE_END,
    ANALYZE_LOCAL_CONNECTION,
    ANALYZE_LOCAL_PING,
    ANALYZE_CLOUD_CONNECTION,
    ANALYZE_EXTRANET_PING,
    ANALYZE_STOP
}diagnosis_step_t;

int net_diagnosis_set_extranet_name(char *msg);
void net_diagnosis_start();
void net_diagnosis_get_result(char *msg);
uint8_t net_diagnosis_get_step();
int net_repair_start();

#ifdef __cplusplus
}
#endif



#endif 

