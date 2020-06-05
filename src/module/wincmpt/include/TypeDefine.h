#ifndef __TYPEDEFINE_H__
#define __TYPEDEFINE_H__

// null define
#ifndef NULL
#define NULL	0
#endif

// private c function define
#ifndef	CPRIVATE
#define	CPRIVATE	static
#endif

// C class define
#ifndef	Cclass
#define	Cclass		typedef struct
#endif

//////////////////////////////////////////////////////////////////////////////
// C++ bool definition

#ifndef __cplusplus
#ifndef bool
#define bool	unsigned char
#endif
#ifndef true
#define true	1
#endif
#ifndef false
#define false	0
#endif
#endif

//////////////////////////////////////////////////////////////////////////////
// Windows type definition

// TRUE & FALSE for BOOL
#ifndef TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif

#ifndef _MSC_VER
// rename to avoid conflicts
#define LPCSTR		yLPCSTR
#define LONG		yLONG
#define	DWORD		yDWORD
#define	BOOL		yBOOL
#define	BYTE		yBYTE
#define	WORD		yWORD
#define	FLOAT		yFLOAT
#define PFLOAT		yPFLOAT
#define	PBOOL		yPBOOL
#define	LPBOOL		yLPBOOL
#define UCHAR		yUCHAR
#define	PBYTE		yPBYTE
#define	LPBYTE		yLPBYTE
#define	PINT		yPINT
#define	LPINT		yLPINT
#define	PWORD		yPWORD
#define	LPWORD		yLPWORD
#define	LPLONG		yLPLONG
#define	PDWORD		yPDWORD
#define	LPDWORD		yLPDWORD
#define	LPVOID		yLPVOID
#define	LPCVOID		yLPCVOID
#define	INT			yINT
#define	UINT		yUINT
#define	PUINT		yPUINT
#define HANDLE		yHANDLE
#define HWND		yHWND
#define WPARAM		yWPARAM
#define LPARAM		yLPARAM
#define LRESULT		yLRESULT
#define INT_PTR		yINT_PTR
#define PINT_PTR	yPINT_PTR
#define UINT_PTR	yUINT_PTR
#define PUINT_PTR	yPUINT_PTR
#define WM_USER		0x0400
// WINDEF for none windows OS
typedef char 			*LPCSTR;
typedef long			LONG;
typedef unsigned long	DWORD;
typedef int				BOOL;
typedef int				INT;
typedef unsigned int	UINT;
typedef unsigned char 	UCHAR;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef float			FLOAT;
typedef FLOAT			*PFLOAT;
typedef BOOL			*PBOOL;
typedef BOOL			*LPBOOL;
typedef BYTE			*PBYTE;
typedef BYTE			*LPBYTE;
typedef int				*PINT;
typedef int				*LPINT;
typedef WORD			*PWORD;
typedef WORD			*LPWORD;
typedef long			*LPLONG;
typedef DWORD			*PDWORD;
typedef DWORD			*LPDWORD;
typedef void			*LPVOID;
typedef const void		*LPCVOID;
typedef unsigned int	*PUINT;
typedef void			*HANDLE;
typedef unsigned long	WPARAM;
typedef long			LPARAM;
typedef long			LRESULT;
typedef HANDLE			HWND;
typedef long			INT_PTR, *PINT_PTR;
typedef unsigned long	UINT_PTR, *PUINT_PTR;
#endif

//////////////////////////////////////////////////////////////////////////////
// stdint.h compatibility

// windows
#ifdef _MSC_VER
#if _MSC_VER >= 1600
#define STDINT_INCLUDE
#include <stdint.h>
#endif
#endif

// gcc or g++
#if ((defined(__STDC__) && __STDC__ && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
     (defined (__WATCOMC__) && (defined(_STDINT_H_INCLUDED) || __WATCOMC__ >= 1250)) || \
     (defined(__GNUC__) && (__GNUC__ > 3 || defined(_STDINT_H) || defined(_STDINT_H_) || defined(__UINT_FAST64_TYPE__))))
#define STDINT_INCLUDE
#include <stdint.h>
#endif

#ifndef STDINT_INCLUDE
// rename to avoid conflicts
#define	int8_t		yint8_t	
#define	int16_t		yint16_t
#define	int32_t		yint32_t
#define	uint8_t		yuint8_t
#define	uint16_t	yuint16_t
#define	uint32_t	yuint32_t
#define	int64_t		yint64_t
#define	uint64_t	yuint64_t
// Exact-width integer types
typedef char				int8_t;
typedef short				int16_t;
typedef int					int32_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
#ifdef _MSC_VER
typedef __int64				int64_t;
typedef unsigned __int64	uint64_t;
#else
typedef	long long			int64_t;
typedef	unsigned long long	uint64_t;
#endif
// Minimum-width integer types
typedef int8_t    int_least8_t;
typedef int16_t   int_least16_t;
typedef int32_t   int_least32_t;
typedef int64_t   int_least64_t;
typedef uint8_t   uint_least8_t;
typedef uint16_t  uint_least16_t;
typedef uint32_t  uint_least32_t;
typedef uint64_t  uint_least64_t;
// Fastest minimum-width integer types
typedef int8_t    int_fast8_t;
typedef int16_t   int_fast16_t;
typedef int32_t   int_fast32_t;
typedef int64_t   int_fast64_t;
typedef uint8_t   uint_fast8_t;
typedef uint16_t  uint_fast16_t;
typedef uint32_t  uint_fast32_t;
typedef uint64_t  uint_fast64_t;
// Limits of exact-width integer types
#define INT8_MAX	0x7F
#define INT16_MAX	0x7FFF
#define INT32_MAX	0x7FFFFFFF
#define INT64_MAX	0x7FFFFFFFFFFFFFFF
#define UINT8_MAX	0xFF
#define UINT16_MAX	0xFFFF
#define UINT32_MAX	0xFFFFFFFF
#define UINT64_MAX	0xFFFFFFFFFFFFFFFF
#define INT8_MIN	(-INT8_MAX-1)
#define INT16_MIN	(-INT16_MAX-1)
#define INT32_MIN	(-INT32_MAX-1)
#define INT64_MIN	(-INT64_MAX-1)
// Limits of minimum-width integer types
#define INT_LEAST8_MIN		INT8_MIN
#define INT_LEAST8_MAX		INT8_MAX
#define INT_LEAST16_MIN		INT16_MIN
#define INT_LEAST16_MAX		INT16_MAX
#define INT_LEAST32_MIN		INT32_MIN
#define INT_LEAST32_MAX		INT32_MAX
#define INT_LEAST64_MIN		INT64_MIN
#define INT_LEAST64_MAX		INT64_MAX
#define UINT_LEAST8_MAX		UINT8_MAX
#define UINT_LEAST16_MAX	UINT16_MAX
#define UINT_LEAST32_MAX	UINT32_MAX
#define UINT_LEAST64_MAX	UINT64_MAX
// Limits of fastest minimum-width integer types
#define INT_FAST8_MIN		INT8_MIN
#define INT_FAST8_MAX		INT8_MAX
#define INT_FAST16_MIN		INT16_MIN
#define INT_FAST16_MAX		INT16_MAX
#define INT_FAST32_MIN		INT32_MIN
#define INT_FAST32_MAX		INT32_MAX
#define INT_FAST64_MIN		INT64_MIN
#define INT_FAST64_MAX		INT64_MAX
#define UINT_FAST8_MAX		UINT8_MAX
#define UINT_FAST16_MAX		UINT16_MAX
#define UINT_FAST32_MAX		UINT32_MAX
#define UINT_FAST64_MAX		UINT64_MAX
#endif

//////////////////////////////////////////////////////////////////////////////
// other type definition

#ifdef _MSC_VER
typedef	__int64				y64int;			// printf("%lld",n);
typedef	unsigned __int64	y64uint;
#else
typedef	long long			y64int;			// printf("%I64d",n);
typedef	unsigned long long	y64uint;
#endif

#endif // #ifndef __TYPEDEFINE_H__
