/**
 * @file    include/kernel/types.h
 * @brief   some useful types
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#define bool _Bool
#define true  1
#define false 0

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

typedef unsigned char   u8_t;
typedef signed   char   s8_t;
typedef unsigned short  u16_t;
typedef signed   short  s16_t;
typedef unsigned long   u32_t;
typedef signed   long   s32_t;

typedef int* stk_t;
typedef int err_t;
typedef int cpu_sr_t;
typedef int tid_t;
typedef int size_t;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;
typedef unsigned int	uint;

typedef enum {
	VAR_LONG = 32,
	VAR_SHORT = 16,
	VAR_CHAR = 8
} VAR_TYPE;

typedef char *va_list;
#define va_start(ap, p)		(ap = (char *) (&(p)+1))
#define va_arg(ap, type)	((type *) (ap += sizeof(type)))[-1]
#define va_end(ap)

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#endif
