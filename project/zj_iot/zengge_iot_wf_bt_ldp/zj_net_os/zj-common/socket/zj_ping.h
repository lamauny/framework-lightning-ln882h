#ifndef ZJ_PING_H
#define ZJ_PING_H

typedef struct{

	uint32_t max_ms;
	uint32_t min_ms;
}ping_ms_t;


int zj_net_ping(char *domain_name);
uint8_t zj_get_ping_loss_rate(void);
ping_ms_t zj_get_ping_echo_time(uint32_t wait_time);

#endif