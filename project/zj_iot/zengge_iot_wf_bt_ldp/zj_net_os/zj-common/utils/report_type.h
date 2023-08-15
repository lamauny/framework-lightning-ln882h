#ifndef __DREPORT_H__
#define __DREPORT_H__


#ifdef __cplusplus
extern "C" {
#endif
typedef enum{

	ACTIVE_QUERY,
	LOCAL_CONTROL,
	CLOUD_CONTROL,
	TIMER_CONTROL,
	HARDWARE_CONTROL,
	RESON_NULL

}report_reson_t;

void set_report_reson(report_reson_t reson);
report_reson_t get_report_reason();


#ifdef __cplusplus
}
#endif


#endif 
