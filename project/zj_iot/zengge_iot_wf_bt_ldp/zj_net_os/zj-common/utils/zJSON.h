/*
  Copyright (c) 2009-2017 Dave Gamble and zJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef zJSON__h
#define zJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 3 define options:

zJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
zJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
zJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the zJSON_API_VISIBILITY flag to "export" the same symbols the way zJSON_EXPORT_SYMBOLS does

*/

#define zJSON_CDECL __cdecl
#define zJSON_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(zJSON_HIDE_SYMBOLS) && !defined(zJSON_IMPORT_SYMBOLS) && !defined(zJSON_EXPORT_SYMBOLS)
#define zJSON_EXPORT_SYMBOLS
#endif

#if defined(zJSON_HIDE_SYMBOLS)
#define zJSON_PUBLIC(type)   type zJSON_STDCALL
#elif defined(zJSON_EXPORT_SYMBOLS)
#define zJSON_PUBLIC(type)   __declspec(dllexport) type zJSON_STDCALL
#elif defined(zJSON_IMPORT_SYMBOLS)
#define zJSON_PUBLIC(type)   __declspec(dllimport) type zJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define zJSON_CDECL
#define zJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(zJSON_API_VISIBILITY)
#define zJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define zJSON_PUBLIC(type) type
#endif
#endif

/* project version */
#define zJSON_VERSION_MAJOR 1
#define zJSON_VERSION_MINOR 7
#define zJSON_VERSION_PATCH 11

#include <stddef.h>

/* zJSON Types: */
#define zJSON_Invalid (0)
#define zJSON_False  (1 << 0)
#define zJSON_True   (1 << 1)
#define zJSON_NULL   (1 << 2)
#define zJSON_Number (1 << 3)
#define zJSON_String (1 << 4)
#define zJSON_Array  (1 << 5)
#define zJSON_Object (1 << 6)
#define zJSON_Raw    (1 << 7) /* raw json */

#define zJSON_IsReference 256
#define zJSON_StringIsConst 512

/* The zJSON structure: */
typedef struct zJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct zJSON *next;
    struct zJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct zJSON *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==zJSON_String  and type == zJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use zJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==zJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} zJSON;

typedef struct zJSON_Hooks
{
      /* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
      void *(zJSON_CDECL *malloc_fn)(size_t sz);
      void (zJSON_CDECL *free_fn)(void *ptr);
} zJSON_Hooks;

typedef int zJSON_bool;

/* Limits how deeply nested arrays/objects can be before zJSON rejects to parse them.
 * This is to prevent stack overflows. */
#ifndef zJSON_NESTING_LIMIT
#define zJSON_NESTING_LIMIT 1000
#endif

/* returns the version of zJSON as a string */
zJSON_PUBLIC(const char*) zJSON_Version(void);

/* Supply malloc, realloc and free functions to zJSON */
zJSON_PUBLIC(void) zJSON_InitHooks(zJSON_Hooks* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of zJSON_Parse (with zJSON_Delete) and zJSON_Print (with stdlib free, zJSON_Hooks.free_fn, or zJSON_free as appropriate). The exception is zJSON_PrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a zJSON object you can interrogate. */
zJSON_PUBLIC(zJSON *) zJSON_Parse(const char *value);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match zJSON_GetErrorPtr(). */
zJSON_PUBLIC(zJSON *) zJSON_ParseWithOpts(const char *value, const char **return_parse_end, zJSON_bool require_null_terminated);

/* Render a zJSON entity to text for transfer/storage. */
zJSON_PUBLIC(char *) zJSON_Print(const zJSON *item);
/* Render a zJSON entity to text for transfer/storage without any formatting. */
zJSON_PUBLIC(char *) zJSON_PrintUnformatted(const zJSON *item);
/* Render a zJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
zJSON_PUBLIC(char *) zJSON_PrintBuffered(const zJSON *item, int prebuffer, zJSON_bool fmt);
/* Render a zJSON entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: zJSON is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
zJSON_PUBLIC(zJSON_bool) zJSON_PrintPreallocated(zJSON *item, char *buffer, const int length, const zJSON_bool format);
/* Delete a zJSON entity and all subentities. */
zJSON_PUBLIC(void) zJSON_Delete(zJSON *c);

/* Returns the number of items in an array (or object). */
zJSON_PUBLIC(int) zJSON_GetArraySize(const zJSON *array);
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
zJSON_PUBLIC(zJSON *) zJSON_GetArrayItem(const zJSON *array, int index);
/* Get item "string" from object. Case insensitive. */
zJSON_PUBLIC(zJSON *) zJSON_GetObjectItem(const zJSON * const object, const char * const string);
zJSON_PUBLIC(zJSON *) zJSON_GetObjectItemCaseSensitive(const zJSON * const object, const char * const string);
zJSON_PUBLIC(zJSON_bool) zJSON_HasObjectItem(const zJSON *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when zJSON_Parse() returns 0. 0 when zJSON_Parse() succeeds. */
zJSON_PUBLIC(const char *) zJSON_GetErrorPtr(void);

/* Check if the item is a string and return its valuestring */
zJSON_PUBLIC(char *) zJSON_GetStringValue(zJSON *item);

/* These functions check the type of an item */
zJSON_PUBLIC(zJSON_bool) zJSON_IsInvalid(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsFalse(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsTrue(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsBool(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsNull(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsNumber(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsString(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsArray(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsObject(const zJSON * const item);
zJSON_PUBLIC(zJSON_bool) zJSON_IsRaw(const zJSON * const item);

/* These calls create a zJSON item of the appropriate type. */
zJSON_PUBLIC(zJSON *) zJSON_CreateNull(void);
zJSON_PUBLIC(zJSON *) zJSON_CreateTrue(void);
zJSON_PUBLIC(zJSON *) zJSON_CreateFalse(void);
zJSON_PUBLIC(zJSON *) zJSON_CreateBool(zJSON_bool boolean);
zJSON_PUBLIC(zJSON *) zJSON_CreateNumber(double num);
zJSON_PUBLIC(zJSON *) zJSON_CreateString(const char *string);
/* raw json */
zJSON_PUBLIC(zJSON *) zJSON_CreateRaw(const char *raw);
zJSON_PUBLIC(zJSON *) zJSON_CreateArray(void);
zJSON_PUBLIC(zJSON *) zJSON_CreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by zJSON_Delete */
zJSON_PUBLIC(zJSON *) zJSON_CreateStringReference(const char *string);
/* Create an object/arrray that only references it's elements so
 * they will not be freed by zJSON_Delete */
zJSON_PUBLIC(zJSON *) zJSON_CreateObjectReference(const zJSON *child);
zJSON_PUBLIC(zJSON *) zJSON_CreateArrayReference(const zJSON *child);

/* These utilities create an Array of count items. */
zJSON_PUBLIC(zJSON *) zJSON_CreateIntArray(const int *numbers, int count);
zJSON_PUBLIC(zJSON *) zJSON_CreateFloatArray(const float *numbers, int count);
zJSON_PUBLIC(zJSON *) zJSON_CreateDoubleArray(const double *numbers, int count);
zJSON_PUBLIC(zJSON *) zJSON_CreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */
zJSON_PUBLIC(void) zJSON_AddItemToArray(zJSON *array, zJSON *item);
zJSON_PUBLIC(void) zJSON_AddItemToObject(zJSON *object, const char *string, zJSON *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the zJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & zJSON_StringIsConst) is zero before
 * writing to `item->string` */
zJSON_PUBLIC(void) zJSON_AddItemToObjectCS(zJSON *object, const char *string, zJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing zJSON to a new zJSON, but don't want to corrupt your existing zJSON. */
zJSON_PUBLIC(void) zJSON_AddItemReferenceToArray(zJSON *array, zJSON *item);
zJSON_PUBLIC(void) zJSON_AddItemReferenceToObject(zJSON *object, const char *string, zJSON *item);

/* Remove/Detatch items from Arrays/Objects. */
zJSON_PUBLIC(zJSON *) zJSON_DetachItemViaPointer(zJSON *parent, zJSON * const item);
zJSON_PUBLIC(zJSON *) zJSON_DetachItemFromArray(zJSON *array, int which);
zJSON_PUBLIC(void) zJSON_DeleteItemFromArray(zJSON *array, int which);
zJSON_PUBLIC(zJSON *) zJSON_DetachItemFromObject(zJSON *object, const char *string);
zJSON_PUBLIC(zJSON *) zJSON_DetachItemFromObjectCaseSensitive(zJSON *object, const char *string);
zJSON_PUBLIC(void) zJSON_DeleteItemFromObject(zJSON *object, const char *string);
zJSON_PUBLIC(void) zJSON_DeleteItemFromObjectCaseSensitive(zJSON *object, const char *string);

/* Update array items. */
zJSON_PUBLIC(void) zJSON_InsertItemInArray(zJSON *array, int which, zJSON *newitem); /* Shifts pre-existing items to the right. */
zJSON_PUBLIC(zJSON_bool) zJSON_ReplaceItemViaPointer(zJSON * const parent, zJSON * const item, zJSON * replacement);
zJSON_PUBLIC(void) zJSON_ReplaceItemInArray(zJSON *array, int which, zJSON *newitem);
zJSON_PUBLIC(void) zJSON_ReplaceItemInObject(zJSON *object,const char *string,zJSON *newitem);
zJSON_PUBLIC(void) zJSON_ReplaceItemInObjectCaseSensitive(zJSON *object,const char *string,zJSON *newitem);

/* Duplicate a zJSON item */
zJSON_PUBLIC(zJSON *) zJSON_Duplicate(const zJSON *item, zJSON_bool recurse);
/* Duplicate will create a new, identical zJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */
/* Recursively compare two zJSON items for equality. If either a or b is NULL or invalid, they will be considered unequal.
 * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
zJSON_PUBLIC(zJSON_bool) zJSON_Compare(const zJSON * const a, const zJSON * const b, const zJSON_bool case_sensitive);


zJSON_PUBLIC(void) zJSON_Minify(char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
zJSON_PUBLIC(zJSON*) zJSON_AddNullToObject(zJSON * const object, const char * const name);
zJSON_PUBLIC(zJSON*) zJSON_AddTrueToObject(zJSON * const object, const char * const name);
zJSON_PUBLIC(zJSON*) zJSON_AddFalseToObject(zJSON * const object, const char * const name);
zJSON_PUBLIC(zJSON*) zJSON_AddBoolToObject(zJSON * const object, const char * const name, const zJSON_bool boolean);
zJSON_PUBLIC(zJSON*) zJSON_AddNumberToObject(zJSON * const object, const char * const name, const double number);
zJSON_PUBLIC(zJSON*) zJSON_AddStringToObject(zJSON * const object, const char * const name, const char * const string);
zJSON_PUBLIC(zJSON*) zJSON_AddRawToObject(zJSON * const object, const char * const name, const char * const raw);
zJSON_PUBLIC(zJSON*) zJSON_AddObjectToObject(zJSON * const object, const char * const name);
zJSON_PUBLIC(zJSON*) zJSON_AddArrayToObject(zJSON * const object, const char * const name);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define zJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the zJSON_SetNumberValue macro */
zJSON_PUBLIC(double) zJSON_SetNumberHelper(zJSON *object, double number);
#define zJSON_SetNumberValue(object, number) ((object != NULL) ? zJSON_SetNumberHelper(object, (double)number) : (number))

/* Macro for iterating over an array or object */
#define zJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)

/* malloc/free objects using the malloc/free functions that have been set with zJSON_InitHooks */
zJSON_PUBLIC(void *) zJSON_malloc(size_t size);
zJSON_PUBLIC(void) zJSON_free(void *object);

#ifdef __cplusplus
}
#endif

#endif
