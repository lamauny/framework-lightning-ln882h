#ifndef ZG_UTILS_H_
#define ZG_UTILS_H_

#include "utils_json.h"

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MAX3
#define MAX3(a,b,c) MAX(MAX(a,b),c)
#endif

#ifndef MIN3
#define MIN3(a,b,c) MIN(MIN(a,b),c)
#endif

#ifndef ARRAY_SIZE
/* Evaluates to 0 if cond is true-ish; compile error otherwise */
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

/* Evaluates to 0 if array is an array; compile error if not array (e.g.
 * pointer)
 */
#define IS_ARRAY(array) \
 ZERO_OR_COMPILE_ERROR( \
  !__builtin_types_compatible_p(__typeof__(array), \
           __typeof__(&(array)[0])))

#define ARRAY_SIZE(array) \
 ((unsigned long) (IS_ARRAY(array) + \
  (sizeof(array) / sizeof((array)[0]))))
#endif

/** \brief bit移位 */
#ifndef BIT
#define BIT(bit)                  (1u << (bit))
#endif
/** \brief 值移位 */
#define SBF(value, field)         ((value) << (field))

/** \brief bit置位 */
#define BIT_SET(data, bit)        ((data) |= BIT(bit))

/** \brief bit清零 */
#define BIT_CLR(data, bit)        ((data) &= ~BIT(bit))

/** \brief bit置位, 根据 mask 指定的位 */
#define BIT_SET_MASK(data, mask)  ((data) |= (mask))

/** \brief bit清零, 根据 mask 指定的位 */
#define BIT_CLR_MASK(data, mask)  ((data) &= ~(mask))

/** \brief bit翻转 */
#define BIT_TOGGLE(data, bit)     ((data) ^= BIT(bit))

/** \brief bit修改 */
#define BIT_MODIFY(data, bit, value) \
    ((value) ? BIT_SET(data, bit) : BIT_CLR(data, bit))

/** \brief 测试bit是否置位 */
#define BIT_ISSET(data, bit)      ((data) & BIT(bit))

/** \brief 获取bit值 */
#define BIT_GET(data, bit)        (BIT_ISSET(data, bit) ? 1 : 0)

/** \brief 获取 n bits 掩码值 */
#define BITS_MASK(n)              (~((~0u) << (n)))

/** \brief 获取位段值 */
#define BITS_GET(data, start, len)  \
    (((data) >> (start)) & BITS_MASK(len))

/** \brief 设置位段值 */
#define BITS_SET(data, start, len, value) \
    ((data) = (((data) & ~SBF(BITS_MASK(len), (start))) | \
        SBF((value) & (BITS_MASK(len)), (start))))

typedef struct 
{
    uint32_t run_time_st1;
    uint32_t run_time_st2;
    uint32_t run_time;
}run_time_t;

void running_time_init(run_time_t *t);
void running_time_calculation(run_time_t *t);
void running_time_clear(run_time_t *t);
double newsqrt(double n);
int StrToHex(uint8_t *s,uint8_t *bits,int len);
const char *zj_hex(const void *buf, size_t len);
uint8_t sn_generate();

#endif 
