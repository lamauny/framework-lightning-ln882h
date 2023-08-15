/* tcp_perf Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#ifndef __UTILS_JSON_H__
#define __UTILS_JSON_H__



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This api is used only for zj json internal use
 */
int __zj_json_parse( const char *json_str,  const char *key,  void *value, int value_type);
ssize_t __zj_json_pack(  char *json_str, const char *key, int value, int value_type);


/**
 * @brief  zj_json_parse(_IN_ const char *json_str, _IN_ const char *key, void *value)
 *         Parse the json formatted string
 *
 * @param  json_str The string pointer to be parsed
 * @param  key      Nuild value pairs
 * @param  value    You must ensure that the incoming type is consistent with the
 *                  post-resolution type
 *
 * @note   does not support the analysis of array types
 *
 * @return
 *     - zj_OK: succeed
 *     - zj_FAIL: failed
 */
#define zj_json_parse(json_str, key, value) \
    __zj_json_parse(( const char *)(json_str), (const char *)(key), value, \
                      __builtin_types_compatible_p(__typeof__(value), short *) * 1\
                      + __builtin_types_compatible_p(__typeof__(value), int *) * 1\
                      + __builtin_types_compatible_p(__typeof__(value), uint16_t *) * 1\
                      + __builtin_types_compatible_p(__typeof__(value), uint32_t *) * 1\
                      + __builtin_types_compatible_p(__typeof__(value), float *) * 2\
                      + __builtin_types_compatible_p(__typeof__(value), double *) * 3\
                      + __builtin_types_compatible_p(__typeof__(value), char *) * 4)

/**
 * @brief  zj_json_pack(char *json_str, _IN_ const char *key, int/double/char value);
 *         Create a json string
 *
 * @param  json_str Save the generated json string
 * @param  key      Build value pairs
 * @param  value    This is a generic, support int/double/char/char *
 *
 * @note   if the value is double or float type only retains the integer part,
 *         requires complete data calling  zj_json_pack_double()
 *
 * @return
 *     - lenght: generates the length of the json string
 *     - zj_FAIL: failed
 */
#define zj_json_pack(json_str, key, value) \
    __zj_json_pack((char *)(json_str), ( const char *)(key), (int)(value), \
                     __builtin_types_compatible_p(__typeof__(value), char) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), short) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), int) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), int8_t) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), uint8_t) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), uint16_t) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), uint32_t) * 1\
                     + __builtin_types_compatible_p(__typeof__(value), float) * 2\
                     + __builtin_types_compatible_p(__typeof__(value), double) * 2\
                     + __builtin_types_compatible_p(__typeof__(value), char *) * 3\
                     + __builtin_types_compatible_p(__typeof__(value), const char *) * 3\
                     + __builtin_types_compatible_p(__typeof__(value), unsigned char *) * 3\
                     + __builtin_types_compatible_p(__typeof__(value), const unsigned char *) * 3)

/**
 * @brief  create a double type json string, Make up for the lack of zj_json_pack()
 *
 * @param  json_str Save the generated json string
 * @param  key      Build value pairs
 * @param  value    The value to be stored
 *
 * @return
 *     - lenght: generates the length of the json string
 *     - zj_FAIL: failed
 */
int zj_json_pack_double(char *json_str,  const char *key, double value);

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

