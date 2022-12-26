/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

// (Partial copyright from inline included files dsound.h, dinput.h)
/****************************************************************************
 *
 *  Copyright (C) 1996-2000 Microsoft Corporation.  All Rights Reserved.
 *
 ****************************************************************************/

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif

#include "minisdl_audio.h"
#ifndef _SDL_H
#define _SDL_H

#ifndef _SDL_main_h
#define _SDL_main_h

#ifndef SDL_MAIN_HANDLED
#if defined(__WIN32__)

#define SDL_MAIN_AVAILABLE

#elif defined(__WINRT__)

#define SDL_MAIN_NEEDED

#elif defined(__IPHONEOS__)

#define SDL_MAIN_NEEDED

#elif defined(__ANDROID__)

#define SDL_MAIN_NEEDED

#endif
#endif

#ifdef __cplusplus
#define C_LINKAGE   "C"
#else
#define C_LINKAGE
#endif

#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
#define main    SDL_main
#endif

extern C_LINKAGE int SDL_main(int argc, char *argv[]);

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

extern DECLSPEC void SDLCALL SDL_SetMainReady(void);

#ifdef __WIN32__

extern DECLSPEC int SDLCALL SDL_RegisterApp(char *name, Uint32 style,
                                            void *hInst);
extern DECLSPEC void SDLCALL SDL_UnregisterApp(void);

#endif

#ifdef __WINRT__

extern DECLSPEC int SDLCALL SDL_WinRTRunApp(int (*mainFunction)(int, char **), void * xamlBackgroundPanel);

#endif

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_assert_h
#define _SDL_assert_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SDL_ASSERT_LEVEL
#ifdef SDL_DEFAULT_ASSERT_LEVEL
#define SDL_ASSERT_LEVEL SDL_DEFAULT_ASSERT_LEVEL
#elif defined(_DEBUG) || defined(DEBUG) || \
      (defined(__GNUC__) && !defined(__OPTIMIZE__))
#define SDL_ASSERT_LEVEL 2
#else
#define SDL_ASSERT_LEVEL 1
#endif
#endif

#if defined(_MSC_VER)

    extern void __cdecl __debugbreak(void);
    #define SDL_TriggerBreakpoint() __debugbreak()
#elif (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
    #define SDL_TriggerBreakpoint() __asm__ __volatile__ ( "int $3\n\t" )
#elif defined(HAVE_SIGNAL_H)
    #include <signal.h>
    #define SDL_TriggerBreakpoint() raise(SIGTRAP)
#else

    #define SDL_TriggerBreakpoint()
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#   define SDL_FUNCTION __func__
#elif ((__GNUC__ >= 2) || defined(_MSC_VER))
#   define SDL_FUNCTION __FUNCTION__
#else
#   define SDL_FUNCTION "???"
#endif
#define SDL_FILE    __FILE__
#define SDL_LINE    __LINE__

#ifdef _MSC_VER
#define SDL_NULL_WHILE_LOOP_CONDITION (-1 == __LINE__)
#else
#define SDL_NULL_WHILE_LOOP_CONDITION (0)
#endif

#define SDL_disabled_assert(condition) \
    do { (void) sizeof ((condition)); } while (SDL_NULL_WHILE_LOOP_CONDITION)

typedef enum
{
    SDL_ASSERTION_RETRY,
    SDL_ASSERTION_BREAK,
    SDL_ASSERTION_ABORT,
    SDL_ASSERTION_IGNORE,
    SDL_ASSERTION_ALWAYS_IGNORE
} SDL_assert_state;

typedef struct SDL_assert_data
{
    int always_ignore;
    unsigned int trigger_count;
    const char *condition;
    const char *filename;
    int linenum;
    const char *function;
    const struct SDL_assert_data *next;
} SDL_assert_data;

#if (SDL_ASSERT_LEVEL > 0)

extern DECLSPEC SDL_assert_state SDLCALL SDL_ReportAssertion(SDL_assert_data *,
                                                             const char *,
                                                             const char *, int)
#if defined(__clang__)
#if __has_feature(attribute_analyzer_noreturn)

   __attribute__((analyzer_noreturn))
#endif
#endif
;

#define SDL_enabled_assert(condition) \
    do { \
        while ( !(condition) ) { \
            static struct SDL_assert_data assert_data = { \
                0, 0, #condition, 0, 0, 0, 0 \
            }; \
            const SDL_assert_state state = SDL_ReportAssertion(&assert_data, \
                                                               SDL_FUNCTION, \
                                                               SDL_FILE, \
                                                               SDL_LINE); \
            if (state == SDL_ASSERTION_RETRY) { \
                continue; \
            } else if (state == SDL_ASSERTION_BREAK) { \
                SDL_TriggerBreakpoint(); \
            } \
            break; \
        } \
    } while (SDL_NULL_WHILE_LOOP_CONDITION)

#endif

#if SDL_ASSERT_LEVEL == 0
#   define SDL_assert(condition) SDL_disabled_assert(condition)
#   define SDL_assert_release(condition) SDL_disabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 1
#   define SDL_assert(condition) SDL_disabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 2
#   define SDL_assert(condition) SDL_enabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)
#elif SDL_ASSERT_LEVEL == 3
#   define SDL_assert(condition) SDL_enabled_assert(condition)
#   define SDL_assert_release(condition) SDL_enabled_assert(condition)
#   define SDL_assert_paranoid(condition) SDL_enabled_assert(condition)
#else
#   error Unknown assertion level.
#endif

#define SDL_assert_always(condition) SDL_enabled_assert(condition)

typedef SDL_assert_state (SDLCALL *SDL_AssertionHandler)(
                                 const SDL_assert_data* data, void* userdata);

extern DECLSPEC void SDLCALL SDL_SetAssertionHandler(
                                            SDL_AssertionHandler handler,
                                            void *userdata);

extern DECLSPEC SDL_AssertionHandler SDLCALL SDL_GetDefaultAssertionHandler(void);

extern DECLSPEC SDL_AssertionHandler SDLCALL SDL_GetAssertionHandler(void **puserdata);

extern DECLSPEC const SDL_assert_data * SDLCALL SDL_GetAssertionReport(void);

extern DECLSPEC void SDLCALL SDL_ResetAssertionReport(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_clipboard_h
#define _SDL_clipboard_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern DECLSPEC int SDLCALL SDL_SetClipboardText(const char *text);

extern DECLSPEC char * SDLCALL SDL_GetClipboardText(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasClipboardText(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_cpuinfo_h
#define _SDL_cpuinfo_h

#if defined(_MSC_VER) && (_MSC_VER >= 1500) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
#ifndef _WIN64
#define __MMX__
#define __3dNOW__
#endif
#define __SSE__
#define __SSE2__
#elif defined(__MINGW64_VERSION_MAJOR)
#include <intrin.h>
#else
#ifdef __ALTIVEC__
#if HAVE_ALTIVEC_H && !defined(__APPLE_ALTIVEC__)
#include <altivec.h>
#undef pixel
#endif
#endif
#ifdef __MMX__
#include <mmintrin.h>
#endif
#ifdef __3dNOW__
#include <mm3dnow.h>
#endif
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_CACHELINE_SIZE  128

extern DECLSPEC int SDLCALL SDL_GetCPUCount(void);

extern DECLSPEC int SDLCALL SDL_GetCPUCacheLineSize(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasRDTSC(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasAltiVec(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasMMX(void);

extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNow(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE2(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE3(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE41(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE42(void);

extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX(void);

extern DECLSPEC int SDLCALL SDL_GetSystemRAM(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_events_h
#define _SDL_events_h

#ifndef _SDL_video_h
#define _SDL_video_h

#ifndef _SDL_pixels_h
#define _SDL_pixels_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0

enum
{
    SDL_PIXELTYPE_UNKNOWN,
    SDL_PIXELTYPE_INDEX1,
    SDL_PIXELTYPE_INDEX4,
    SDL_PIXELTYPE_INDEX8,
    SDL_PIXELTYPE_PACKED8,
    SDL_PIXELTYPE_PACKED16,
    SDL_PIXELTYPE_PACKED32,
    SDL_PIXELTYPE_ARRAYU8,
    SDL_PIXELTYPE_ARRAYU16,
    SDL_PIXELTYPE_ARRAYU32,
    SDL_PIXELTYPE_ARRAYF16,
    SDL_PIXELTYPE_ARRAYF32
};

enum
{
    SDL_BITMAPORDER_NONE,
    SDL_BITMAPORDER_4321,
    SDL_BITMAPORDER_1234
};

enum
{
    SDL_PACKEDORDER_NONE,
    SDL_PACKEDORDER_XRGB,
    SDL_PACKEDORDER_RGBX,
    SDL_PACKEDORDER_ARGB,
    SDL_PACKEDORDER_RGBA,
    SDL_PACKEDORDER_XBGR,
    SDL_PACKEDORDER_BGRX,
    SDL_PACKEDORDER_ABGR,
    SDL_PACKEDORDER_BGRA
};

enum
{
    SDL_ARRAYORDER_NONE,
    SDL_ARRAYORDER_RGB,
    SDL_ARRAYORDER_RGBA,
    SDL_ARRAYORDER_ARGB,
    SDL_ARRAYORDER_BGR,
    SDL_ARRAYORDER_BGRA,
    SDL_ARRAYORDER_ABGR
};

enum
{
    SDL_PACKEDLAYOUT_NONE,
    SDL_PACKEDLAYOUT_332,
    SDL_PACKEDLAYOUT_4444,
    SDL_PACKEDLAYOUT_1555,
    SDL_PACKEDLAYOUT_5551,
    SDL_PACKEDLAYOUT_565,
    SDL_PACKEDLAYOUT_8888,
    SDL_PACKEDLAYOUT_2101010,
    SDL_PACKEDLAYOUT_1010102
};

#define SDL_DEFINE_PIXELFOURCC(A, B, C, D) SDL_FOURCC(A, B, C, D)

#define SDL_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes) \
    ((1 << 28) | ((type) << 24) | ((order) << 20) | ((layout) << 16) | \
     ((bits) << 8) | ((bytes) << 0))

#define SDL_PIXELFLAG(X)    (((X) >> 28) & 0x0F)
#define SDL_PIXELTYPE(X)    (((X) >> 24) & 0x0F)
#define SDL_PIXELORDER(X)   (((X) >> 20) & 0x0F)
#define SDL_PIXELLAYOUT(X)  (((X) >> 16) & 0x0F)
#define SDL_BITSPERPIXEL(X) (((X) >> 8) & 0xFF)
#define SDL_BYTESPERPIXEL(X) \
    (SDL_ISPIXELFORMAT_FOURCC(X) ? \
        ((((X) == SDL_PIXELFORMAT_YUY2) || \
          ((X) == SDL_PIXELFORMAT_UYVY) || \
          ((X) == SDL_PIXELFORMAT_YVYU)) ? 2 : 1) : (((X) >> 0) & 0xFF))

#define SDL_ISPIXELFORMAT_INDEXED(format)   \
    (!SDL_ISPIXELFORMAT_FOURCC(format) && \
     ((SDL_PIXELTYPE(format) == SDL_PIXELTYPE_INDEX1) || \
      (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_INDEX4) || \
      (SDL_PIXELTYPE(format) == SDL_PIXELTYPE_INDEX8)))

#define SDL_ISPIXELFORMAT_ALPHA(format)   \
    (!SDL_ISPIXELFORMAT_FOURCC(format) && \
     ((SDL_PIXELORDER(format) == SDL_PACKEDORDER_ARGB) || \
      (SDL_PIXELORDER(format) == SDL_PACKEDORDER_RGBA) || \
      (SDL_PIXELORDER(format) == SDL_PACKEDORDER_ABGR) || \
      (SDL_PIXELORDER(format) == SDL_PACKEDORDER_BGRA)))

#define SDL_ISPIXELFORMAT_FOURCC(format)    \
    ((format) && (SDL_PIXELFLAG(format) != 1))

enum
{
    SDL_PIXELFORMAT_UNKNOWN,
    SDL_PIXELFORMAT_INDEX1LSB =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1, SDL_BITMAPORDER_4321, 0,
                               1, 0),
    SDL_PIXELFORMAT_INDEX1MSB =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX1, SDL_BITMAPORDER_1234, 0,
                               1, 0),
    SDL_PIXELFORMAT_INDEX4LSB =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX4, SDL_BITMAPORDER_4321, 0,
                               4, 0),
    SDL_PIXELFORMAT_INDEX4MSB =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX4, SDL_BITMAPORDER_1234, 0,
                               4, 0),
    SDL_PIXELFORMAT_INDEX8 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_INDEX8, 0, 0, 8, 1),
    SDL_PIXELFORMAT_RGB332 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED8, SDL_PACKEDORDER_XRGB,
                               SDL_PACKEDLAYOUT_332, 8, 1),
    SDL_PIXELFORMAT_RGB444 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_XRGB,
                               SDL_PACKEDLAYOUT_4444, 12, 2),
    SDL_PIXELFORMAT_RGB555 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_XRGB,
                               SDL_PACKEDLAYOUT_1555, 15, 2),
    SDL_PIXELFORMAT_BGR555 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_XBGR,
                               SDL_PACKEDLAYOUT_1555, 15, 2),
    SDL_PIXELFORMAT_ARGB4444 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_ARGB,
                               SDL_PACKEDLAYOUT_4444, 16, 2),
    SDL_PIXELFORMAT_RGBA4444 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_RGBA,
                               SDL_PACKEDLAYOUT_4444, 16, 2),
    SDL_PIXELFORMAT_ABGR4444 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_ABGR,
                               SDL_PACKEDLAYOUT_4444, 16, 2),
    SDL_PIXELFORMAT_BGRA4444 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_BGRA,
                               SDL_PACKEDLAYOUT_4444, 16, 2),
    SDL_PIXELFORMAT_ARGB1555 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_ARGB,
                               SDL_PACKEDLAYOUT_1555, 16, 2),
    SDL_PIXELFORMAT_RGBA5551 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_RGBA,
                               SDL_PACKEDLAYOUT_5551, 16, 2),
    SDL_PIXELFORMAT_ABGR1555 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_ABGR,
                               SDL_PACKEDLAYOUT_1555, 16, 2),
    SDL_PIXELFORMAT_BGRA5551 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_BGRA,
                               SDL_PACKEDLAYOUT_5551, 16, 2),
    SDL_PIXELFORMAT_RGB565 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_XRGB,
                               SDL_PACKEDLAYOUT_565, 16, 2),
    SDL_PIXELFORMAT_BGR565 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED16, SDL_PACKEDORDER_XBGR,
                               SDL_PACKEDLAYOUT_565, 16, 2),
    SDL_PIXELFORMAT_RGB24 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU8, SDL_ARRAYORDER_RGB, 0,
                               24, 3),
    SDL_PIXELFORMAT_BGR24 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_ARRAYU8, SDL_ARRAYORDER_BGR, 0,
                               24, 3),
    SDL_PIXELFORMAT_RGB888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_XRGB,
                               SDL_PACKEDLAYOUT_8888, 24, 4),
    SDL_PIXELFORMAT_RGBX8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBX,
                               SDL_PACKEDLAYOUT_8888, 24, 4),
    SDL_PIXELFORMAT_BGR888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_XBGR,
                               SDL_PACKEDLAYOUT_8888, 24, 4),
    SDL_PIXELFORMAT_BGRX8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_BGRX,
                               SDL_PACKEDLAYOUT_8888, 24, 4),
    SDL_PIXELFORMAT_ARGB8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_ARGB,
                               SDL_PACKEDLAYOUT_8888, 32, 4),
    SDL_PIXELFORMAT_RGBA8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_RGBA,
                               SDL_PACKEDLAYOUT_8888, 32, 4),
    SDL_PIXELFORMAT_ABGR8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_ABGR,
                               SDL_PACKEDLAYOUT_8888, 32, 4),
    SDL_PIXELFORMAT_BGRA8888 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_BGRA,
                               SDL_PACKEDLAYOUT_8888, 32, 4),
    SDL_PIXELFORMAT_ARGB2101010 =
        SDL_DEFINE_PIXELFORMAT(SDL_PIXELTYPE_PACKED32, SDL_PACKEDORDER_ARGB,
                               SDL_PACKEDLAYOUT_2101010, 32, 4),

    SDL_PIXELFORMAT_YV12 =
        SDL_DEFINE_PIXELFOURCC('Y', 'V', '1', '2'),
    SDL_PIXELFORMAT_IYUV =
        SDL_DEFINE_PIXELFOURCC('I', 'Y', 'U', 'V'),
    SDL_PIXELFORMAT_YUY2 =
        SDL_DEFINE_PIXELFOURCC('Y', 'U', 'Y', '2'),
    SDL_PIXELFORMAT_UYVY =
        SDL_DEFINE_PIXELFOURCC('U', 'Y', 'V', 'Y'),
    SDL_PIXELFORMAT_YVYU =
        SDL_DEFINE_PIXELFOURCC('Y', 'V', 'Y', 'U')
};

typedef struct SDL_Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} SDL_Color;
#define SDL_Colour SDL_Color

typedef struct SDL_Palette
{
    int ncolors;
    SDL_Color *colors;
    Uint32 version;
    int refcount;
} SDL_Palette;

typedef struct SDL_PixelFormat
{
    Uint32 format;
    SDL_Palette *palette;
    Uint8 BitsPerPixel;
    Uint8 BytesPerPixel;
    Uint8 padding[2];
    Uint32 Rmask;
    Uint32 Gmask;
    Uint32 Bmask;
    Uint32 Amask;
    Uint8 Rloss;
    Uint8 Gloss;
    Uint8 Bloss;
    Uint8 Aloss;
    Uint8 Rshift;
    Uint8 Gshift;
    Uint8 Bshift;
    Uint8 Ashift;
    int refcount;
    struct SDL_PixelFormat *next;
} SDL_PixelFormat;

extern DECLSPEC const char* SDLCALL SDL_GetPixelFormatName(Uint32 format);

extern DECLSPEC SDL_bool SDLCALL SDL_PixelFormatEnumToMasks(Uint32 format,
                                                            int *bpp,
                                                            Uint32 * Rmask,
                                                            Uint32 * Gmask,
                                                            Uint32 * Bmask,
                                                            Uint32 * Amask);

extern DECLSPEC Uint32 SDLCALL SDL_MasksToPixelFormatEnum(int bpp,
                                                          Uint32 Rmask,
                                                          Uint32 Gmask,
                                                          Uint32 Bmask,
                                                          Uint32 Amask);

extern DECLSPEC SDL_PixelFormat * SDLCALL SDL_AllocFormat(Uint32 pixel_format);

extern DECLSPEC void SDLCALL SDL_FreeFormat(SDL_PixelFormat *format);

extern DECLSPEC SDL_Palette *SDLCALL SDL_AllocPalette(int ncolors);

extern DECLSPEC int SDLCALL SDL_SetPixelFormatPalette(SDL_PixelFormat * format,
                                                      SDL_Palette *palette);

extern DECLSPEC int SDLCALL SDL_SetPaletteColors(SDL_Palette * palette,
                                                 const SDL_Color * colors,
                                                 int firstcolor, int ncolors);

extern DECLSPEC void SDLCALL SDL_FreePalette(SDL_Palette * palette);

extern DECLSPEC Uint32 SDLCALL SDL_MapRGB(const SDL_PixelFormat * format,
                                          Uint8 r, Uint8 g, Uint8 b);

extern DECLSPEC Uint32 SDLCALL SDL_MapRGBA(const SDL_PixelFormat * format,
                                           Uint8 r, Uint8 g, Uint8 b,
                                           Uint8 a);

extern DECLSPEC void SDLCALL SDL_GetRGB(Uint32 pixel,
                                        const SDL_PixelFormat * format,
                                        Uint8 * r, Uint8 * g, Uint8 * b);

extern DECLSPEC void SDLCALL SDL_GetRGBA(Uint32 pixel,
                                         const SDL_PixelFormat * format,
                                         Uint8 * r, Uint8 * g, Uint8 * b,
                                         Uint8 * a);

extern DECLSPEC void SDLCALL SDL_CalculateGammaRamp(float gamma, Uint16 * ramp);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_rect_h
#define _SDL_rect_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_Point
{
    int x;
    int y;
} SDL_Point;

typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;

SDL_FORCE_INLINE SDL_bool SDL_RectEmpty(const SDL_Rect *r)
{
    return ((!r) || (r->w <= 0) || (r->h <= 0)) ? SDL_TRUE : SDL_FALSE;
}

SDL_FORCE_INLINE SDL_bool SDL_RectEquals(const SDL_Rect *a, const SDL_Rect *b)
{
    return (a && b && (a->x == b->x) && (a->y == b->y) &&
            (a->w == b->w) && (a->h == b->h)) ? SDL_TRUE : SDL_FALSE;
}

extern DECLSPEC SDL_bool SDLCALL SDL_HasIntersection(const SDL_Rect * A,
                                                     const SDL_Rect * B);

extern DECLSPEC SDL_bool SDLCALL SDL_IntersectRect(const SDL_Rect * A,
                                                   const SDL_Rect * B,
                                                   SDL_Rect * result);

extern DECLSPEC void SDLCALL SDL_UnionRect(const SDL_Rect * A,
                                           const SDL_Rect * B,
                                           SDL_Rect * result);

extern DECLSPEC SDL_bool SDLCALL SDL_EnclosePoints(const SDL_Point * points,
                                                   int count,
                                                   const SDL_Rect * clip,
                                                   SDL_Rect * result);

extern DECLSPEC SDL_bool SDLCALL SDL_IntersectRectAndLine(const SDL_Rect *
                                                          rect, int *X1,
                                                          int *Y1, int *X2,
                                                          int *Y2);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_surface_h
#define _SDL_surface_h

#ifndef _SDL_blendmode_h
#define _SDL_blendmode_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SDL_BLENDMODE_NONE = 0x00000000,
    SDL_BLENDMODE_BLEND = 0x00000001,
    SDL_BLENDMODE_ADD = 0x00000002,
    SDL_BLENDMODE_MOD = 0x00000004
} SDL_BlendMode;

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_SWSURFACE       0
#define SDL_PREALLOC        0x00000001
#define SDL_RLEACCEL        0x00000002
#define SDL_DONTFREE        0x00000004

#define SDL_MUSTLOCK(S) (((S)->flags & SDL_RLEACCEL) != 0)

typedef struct SDL_Surface
{
    Uint32 flags;
    SDL_PixelFormat *format;
    int w, h;
    int pitch;
    void *pixels;

    void *userdata;

    int locked;
    void *lock_data;

    SDL_Rect clip_rect;

    struct SDL_BlitMap *map;

    int refcount;
} SDL_Surface;

typedef int (*SDL_blit) (struct SDL_Surface * src, SDL_Rect * srcrect,
                         struct SDL_Surface * dst, SDL_Rect * dstrect);

extern DECLSPEC SDL_Surface *SDLCALL SDL_CreateRGBSurface
    (Uint32 flags, int width, int height, int depth,
     Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern DECLSPEC SDL_Surface *SDLCALL SDL_CreateRGBSurfaceFrom(void *pixels,
                                                              int width,
                                                              int height,
                                                              int depth,
                                                              int pitch,
                                                              Uint32 Rmask,
                                                              Uint32 Gmask,
                                                              Uint32 Bmask,
                                                              Uint32 Amask);
extern DECLSPEC void SDLCALL SDL_FreeSurface(SDL_Surface * surface);

extern DECLSPEC int SDLCALL SDL_SetSurfacePalette(SDL_Surface * surface,
                                                  SDL_Palette * palette);

extern DECLSPEC int SDLCALL SDL_LockSurface(SDL_Surface * surface);

extern DECLSPEC void SDLCALL SDL_UnlockSurface(SDL_Surface * surface);

extern DECLSPEC SDL_Surface *SDLCALL SDL_LoadBMP_RW(SDL_RWops * src,
                                                    int freesrc);

#define SDL_LoadBMP(file)   SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1)

extern DECLSPEC int SDLCALL SDL_SaveBMP_RW
    (SDL_Surface * surface, SDL_RWops * dst, int freedst);

#define SDL_SaveBMP(surface, file) \
        SDL_SaveBMP_RW(surface, SDL_RWFromFile(file, "wb"), 1)

extern DECLSPEC int SDLCALL SDL_SetSurfaceRLE(SDL_Surface * surface,
                                              int flag);

extern DECLSPEC int SDLCALL SDL_SetColorKey(SDL_Surface * surface,
                                            int flag, Uint32 key);

extern DECLSPEC int SDLCALL SDL_GetColorKey(SDL_Surface * surface,
                                            Uint32 * key);

extern DECLSPEC int SDLCALL SDL_SetSurfaceColorMod(SDL_Surface * surface,
                                                   Uint8 r, Uint8 g, Uint8 b);

extern DECLSPEC int SDLCALL SDL_GetSurfaceColorMod(SDL_Surface * surface,
                                                   Uint8 * r, Uint8 * g,
                                                   Uint8 * b);

extern DECLSPEC int SDLCALL SDL_SetSurfaceAlphaMod(SDL_Surface * surface,
                                                   Uint8 alpha);

extern DECLSPEC int SDLCALL SDL_GetSurfaceAlphaMod(SDL_Surface * surface,
                                                   Uint8 * alpha);

extern DECLSPEC int SDLCALL SDL_SetSurfaceBlendMode(SDL_Surface * surface,
                                                    SDL_BlendMode blendMode);

extern DECLSPEC int SDLCALL SDL_GetSurfaceBlendMode(SDL_Surface * surface,
                                                    SDL_BlendMode *blendMode);

extern DECLSPEC SDL_bool SDLCALL SDL_SetClipRect(SDL_Surface * surface,
                                                 const SDL_Rect * rect);

extern DECLSPEC void SDLCALL SDL_GetClipRect(SDL_Surface * surface,
                                             SDL_Rect * rect);

extern DECLSPEC SDL_Surface *SDLCALL SDL_ConvertSurface
    (SDL_Surface * src, const SDL_PixelFormat * fmt, Uint32 flags);
extern DECLSPEC SDL_Surface *SDLCALL SDL_ConvertSurfaceFormat
    (SDL_Surface * src, Uint32 pixel_format, Uint32 flags);

extern DECLSPEC int SDLCALL SDL_ConvertPixels(int width, int height,
                                              Uint32 src_format,
                                              const void * src, int src_pitch,
                                              Uint32 dst_format,
                                              void * dst, int dst_pitch);

extern DECLSPEC int SDLCALL SDL_FillRect
    (SDL_Surface * dst, const SDL_Rect * rect, Uint32 color);
extern DECLSPEC int SDLCALL SDL_FillRects
    (SDL_Surface * dst, const SDL_Rect * rects, int count, Uint32 color);

#define SDL_BlitSurface SDL_UpperBlit

extern DECLSPEC int SDLCALL SDL_UpperBlit
    (SDL_Surface * src, const SDL_Rect * srcrect,
     SDL_Surface * dst, SDL_Rect * dstrect);

extern DECLSPEC int SDLCALL SDL_LowerBlit
    (SDL_Surface * src, SDL_Rect * srcrect,
     SDL_Surface * dst, SDL_Rect * dstrect);

extern DECLSPEC int SDLCALL SDL_SoftStretch(SDL_Surface * src,
                                            const SDL_Rect * srcrect,
                                            SDL_Surface * dst,
                                            const SDL_Rect * dstrect);

#define SDL_BlitScaled SDL_UpperBlitScaled

extern DECLSPEC int SDLCALL SDL_UpperBlitScaled
    (SDL_Surface * src, const SDL_Rect * srcrect,
    SDL_Surface * dst, SDL_Rect * dstrect);

extern DECLSPEC int SDLCALL SDL_LowerBlitScaled
    (SDL_Surface * src, SDL_Rect * srcrect,
    SDL_Surface * dst, SDL_Rect * dstrect);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    Uint32 format;
    int w;
    int h;
    int refresh_rate;
    void *driverdata;
} SDL_DisplayMode;

typedef struct SDL_Window SDL_Window;

typedef enum
{
    SDL_WINDOW_FULLSCREEN = 0x00000001,
    SDL_WINDOW_OPENGL = 0x00000002,
    SDL_WINDOW_SHOWN = 0x00000004,
    SDL_WINDOW_HIDDEN = 0x00000008,
    SDL_WINDOW_BORDERLESS = 0x00000010,
    SDL_WINDOW_RESIZABLE = 0x00000020,
    SDL_WINDOW_MINIMIZED = 0x00000040,
    SDL_WINDOW_MAXIMIZED = 0x00000080,
    SDL_WINDOW_INPUT_GRABBED = 0x00000100,
    SDL_WINDOW_INPUT_FOCUS = 0x00000200,
    SDL_WINDOW_MOUSE_FOCUS = 0x00000400,
    SDL_WINDOW_FULLSCREEN_DESKTOP = ( SDL_WINDOW_FULLSCREEN | 0x00001000 ),
    SDL_WINDOW_FOREIGN = 0x00000800,
    SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000
} SDL_WindowFlags;

#define SDL_WINDOWPOS_UNDEFINED_MASK    0x1FFF0000
#define SDL_WINDOWPOS_UNDEFINED_DISPLAY(X)  (SDL_WINDOWPOS_UNDEFINED_MASK|(X))
#define SDL_WINDOWPOS_UNDEFINED         SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)
#define SDL_WINDOWPOS_ISUNDEFINED(X)    \
            (((X)&0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

#define SDL_WINDOWPOS_CENTERED_MASK    0x2FFF0000
#define SDL_WINDOWPOS_CENTERED_DISPLAY(X)  (SDL_WINDOWPOS_CENTERED_MASK|(X))
#define SDL_WINDOWPOS_CENTERED         SDL_WINDOWPOS_CENTERED_DISPLAY(0)
#define SDL_WINDOWPOS_ISCENTERED(X)    \
            (((X)&0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)

typedef enum
{
    SDL_WINDOWEVENT_NONE,
    SDL_WINDOWEVENT_SHOWN,
    SDL_WINDOWEVENT_HIDDEN,
    SDL_WINDOWEVENT_EXPOSED,
    SDL_WINDOWEVENT_MOVED,
    SDL_WINDOWEVENT_RESIZED,
    SDL_WINDOWEVENT_SIZE_CHANGED,
    SDL_WINDOWEVENT_MINIMIZED,
    SDL_WINDOWEVENT_MAXIMIZED,
    SDL_WINDOWEVENT_RESTORED,
    SDL_WINDOWEVENT_ENTER,
    SDL_WINDOWEVENT_LEAVE,
    SDL_WINDOWEVENT_FOCUS_GAINED,
    SDL_WINDOWEVENT_FOCUS_LOST,
    SDL_WINDOWEVENT_CLOSE
} SDL_WindowEventID;

typedef void *SDL_GLContext;

typedef enum
{
    SDL_GL_RED_SIZE,
    SDL_GL_GREEN_SIZE,
    SDL_GL_BLUE_SIZE,
    SDL_GL_ALPHA_SIZE,
    SDL_GL_BUFFER_SIZE,
    SDL_GL_DOUBLEBUFFER,
    SDL_GL_DEPTH_SIZE,
    SDL_GL_STENCIL_SIZE,
    SDL_GL_ACCUM_RED_SIZE,
    SDL_GL_ACCUM_GREEN_SIZE,
    SDL_GL_ACCUM_BLUE_SIZE,
    SDL_GL_ACCUM_ALPHA_SIZE,
    SDL_GL_STEREO,
    SDL_GL_MULTISAMPLEBUFFERS,
    SDL_GL_MULTISAMPLESAMPLES,
    SDL_GL_ACCELERATED_VISUAL,
    SDL_GL_RETAINED_BACKING,
    SDL_GL_CONTEXT_MAJOR_VERSION,
    SDL_GL_CONTEXT_MINOR_VERSION,
    SDL_GL_CONTEXT_EGL,
    SDL_GL_CONTEXT_FLAGS,
    SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
    SDL_GL_FRAMEBUFFER_SRGB_CAPABLE
} SDL_GLattr;

typedef enum
{
    SDL_GL_CONTEXT_PROFILE_CORE           = 0x0001,
    SDL_GL_CONTEXT_PROFILE_COMPATIBILITY  = 0x0002,
    SDL_GL_CONTEXT_PROFILE_ES             = 0x0004
} SDL_GLprofile;

typedef enum
{
    SDL_GL_CONTEXT_DEBUG_FLAG              = 0x0001,
    SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG = 0x0002,
    SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG      = 0x0004,
    SDL_GL_CONTEXT_RESET_ISOLATION_FLAG    = 0x0008
} SDL_GLcontextFlag;

extern DECLSPEC int SDLCALL SDL_GetNumVideoDrivers(void);

extern DECLSPEC const char *SDLCALL SDL_GetVideoDriver(int index);

extern DECLSPEC int SDLCALL SDL_VideoInit(const char *driver_name);

extern DECLSPEC void SDLCALL SDL_VideoQuit(void);

extern DECLSPEC const char *SDLCALL SDL_GetCurrentVideoDriver(void);

extern DECLSPEC int SDLCALL SDL_GetNumVideoDisplays(void);

extern DECLSPEC const char * SDLCALL SDL_GetDisplayName(int displayIndex);

extern DECLSPEC int SDLCALL SDL_GetDisplayBounds(int displayIndex, SDL_Rect * rect);

extern DECLSPEC int SDLCALL SDL_GetNumDisplayModes(int displayIndex);

extern DECLSPEC int SDLCALL SDL_GetDisplayMode(int displayIndex, int modeIndex,
                                               SDL_DisplayMode * mode);

extern DECLSPEC int SDLCALL SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode);

extern DECLSPEC int SDLCALL SDL_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode * mode);

extern DECLSPEC SDL_DisplayMode * SDLCALL SDL_GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode * mode, SDL_DisplayMode * closest);

extern DECLSPEC int SDLCALL SDL_GetWindowDisplayIndex(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_SetWindowDisplayMode(SDL_Window * window,
                                                     const SDL_DisplayMode
                                                         * mode);

extern DECLSPEC int SDLCALL SDL_GetWindowDisplayMode(SDL_Window * window,
                                                     SDL_DisplayMode * mode);

extern DECLSPEC Uint32 SDLCALL SDL_GetWindowPixelFormat(SDL_Window * window);

extern DECLSPEC SDL_Window * SDLCALL SDL_CreateWindow(const char *title,
                                                      int x, int y, int w,
                                                      int h, Uint32 flags);

extern DECLSPEC SDL_Window * SDLCALL SDL_CreateWindowFrom(const void *data);

extern DECLSPEC Uint32 SDLCALL SDL_GetWindowID(SDL_Window * window);

extern DECLSPEC SDL_Window * SDLCALL SDL_GetWindowFromID(Uint32 id);

extern DECLSPEC Uint32 SDLCALL SDL_GetWindowFlags(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_SetWindowTitle(SDL_Window * window,
                                                const char *title);

extern DECLSPEC const char *SDLCALL SDL_GetWindowTitle(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_SetWindowIcon(SDL_Window * window,
                                               SDL_Surface * icon);

extern DECLSPEC void* SDLCALL SDL_SetWindowData(SDL_Window * window,
                                                const char *name,
                                                void *userdata);

extern DECLSPEC void *SDLCALL SDL_GetWindowData(SDL_Window * window,
                                                const char *name);

extern DECLSPEC void SDLCALL SDL_SetWindowPosition(SDL_Window * window,
                                                   int x, int y);

extern DECLSPEC void SDLCALL SDL_GetWindowPosition(SDL_Window * window,
                                                   int *x, int *y);

extern DECLSPEC void SDLCALL SDL_SetWindowSize(SDL_Window * window, int w,
                                               int h);

extern DECLSPEC void SDLCALL SDL_GetWindowSize(SDL_Window * window, int *w,
                                               int *h);

extern DECLSPEC void SDLCALL SDL_SetWindowMinimumSize(SDL_Window * window,
                                                      int min_w, int min_h);

extern DECLSPEC void SDLCALL SDL_GetWindowMinimumSize(SDL_Window * window,
                                                      int *w, int *h);

extern DECLSPEC void SDLCALL SDL_SetWindowMaximumSize(SDL_Window * window,
                                                      int max_w, int max_h);

extern DECLSPEC void SDLCALL SDL_GetWindowMaximumSize(SDL_Window * window,
                                                      int *w, int *h);

extern DECLSPEC void SDLCALL SDL_SetWindowBordered(SDL_Window * window,
                                                   SDL_bool bordered);

extern DECLSPEC void SDLCALL SDL_ShowWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_HideWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_RaiseWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_MaximizeWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_MinimizeWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_RestoreWindow(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_SetWindowFullscreen(SDL_Window * window,
                                                    Uint32 flags);

extern DECLSPEC SDL_Surface * SDLCALL SDL_GetWindowSurface(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_UpdateWindowSurface(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_UpdateWindowSurfaceRects(SDL_Window * window,
                                                         const SDL_Rect * rects,
                                                         int numrects);

extern DECLSPEC void SDLCALL SDL_SetWindowGrab(SDL_Window * window,
                                               SDL_bool grabbed);

extern DECLSPEC SDL_bool SDLCALL SDL_GetWindowGrab(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_SetWindowBrightness(SDL_Window * window, float brightness);

extern DECLSPEC float SDLCALL SDL_GetWindowBrightness(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_SetWindowGammaRamp(SDL_Window * window,
                                                   const Uint16 * red,
                                                   const Uint16 * green,
                                                   const Uint16 * blue);

extern DECLSPEC int SDLCALL SDL_GetWindowGammaRamp(SDL_Window * window,
                                                   Uint16 * red,
                                                   Uint16 * green,
                                                   Uint16 * blue);

extern DECLSPEC void SDLCALL SDL_DestroyWindow(SDL_Window * window);

extern DECLSPEC SDL_bool SDLCALL SDL_IsScreenSaverEnabled(void);

extern DECLSPEC void SDLCALL SDL_EnableScreenSaver(void);

extern DECLSPEC void SDLCALL SDL_DisableScreenSaver(void);

extern DECLSPEC int SDLCALL SDL_GL_LoadLibrary(const char *path);

extern DECLSPEC void *SDLCALL SDL_GL_GetProcAddress(const char *proc);

extern DECLSPEC void SDLCALL SDL_GL_UnloadLibrary(void);

extern DECLSPEC SDL_bool SDLCALL SDL_GL_ExtensionSupported(const char
                                                           *extension);

extern DECLSPEC void SDLCALL SDL_GL_ResetAttributes(void);

extern DECLSPEC int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);

extern DECLSPEC int SDLCALL SDL_GL_GetAttribute(SDL_GLattr attr, int *value);

extern DECLSPEC SDL_GLContext SDLCALL SDL_GL_CreateContext(SDL_Window *
                                                           window);

extern DECLSPEC int SDLCALL SDL_GL_MakeCurrent(SDL_Window * window,
                                               SDL_GLContext context);

extern DECLSPEC SDL_Window* SDLCALL SDL_GL_GetCurrentWindow(void);

extern DECLSPEC SDL_GLContext SDLCALL SDL_GL_GetCurrentContext(void);

extern DECLSPEC void SDLCALL SDL_GL_GetDrawableSize(SDL_Window * window, int *w,
                                                    int *h);

extern DECLSPEC int SDLCALL SDL_GL_SetSwapInterval(int interval);

extern DECLSPEC int SDLCALL SDL_GL_GetSwapInterval(void);

extern DECLSPEC void SDLCALL SDL_GL_SwapWindow(SDL_Window * window);

extern DECLSPEC void SDLCALL SDL_GL_DeleteContext(SDL_GLContext context);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_keyboard_h
#define _SDL_keyboard_h

#ifndef _SDL_keycode_h
#define _SDL_keycode_h

#ifndef _SDL_scancode_h
#define _SDL_scancode_h

typedef enum
{
    SDL_SCANCODE_UNKNOWN = 0,

    SDL_SCANCODE_A = 4,
    SDL_SCANCODE_B = 5,
    SDL_SCANCODE_C = 6,
    SDL_SCANCODE_D = 7,
    SDL_SCANCODE_E = 8,
    SDL_SCANCODE_F = 9,
    SDL_SCANCODE_G = 10,
    SDL_SCANCODE_H = 11,
    SDL_SCANCODE_I = 12,
    SDL_SCANCODE_J = 13,
    SDL_SCANCODE_K = 14,
    SDL_SCANCODE_L = 15,
    SDL_SCANCODE_M = 16,
    SDL_SCANCODE_N = 17,
    SDL_SCANCODE_O = 18,
    SDL_SCANCODE_P = 19,
    SDL_SCANCODE_Q = 20,
    SDL_SCANCODE_R = 21,
    SDL_SCANCODE_S = 22,
    SDL_SCANCODE_T = 23,
    SDL_SCANCODE_U = 24,
    SDL_SCANCODE_V = 25,
    SDL_SCANCODE_W = 26,
    SDL_SCANCODE_X = 27,
    SDL_SCANCODE_Y = 28,
    SDL_SCANCODE_Z = 29,

    SDL_SCANCODE_1 = 30,
    SDL_SCANCODE_2 = 31,
    SDL_SCANCODE_3 = 32,
    SDL_SCANCODE_4 = 33,
    SDL_SCANCODE_5 = 34,
    SDL_SCANCODE_6 = 35,
    SDL_SCANCODE_7 = 36,
    SDL_SCANCODE_8 = 37,
    SDL_SCANCODE_9 = 38,
    SDL_SCANCODE_0 = 39,

    SDL_SCANCODE_RETURN = 40,
    SDL_SCANCODE_ESCAPE = 41,
    SDL_SCANCODE_BACKSPACE = 42,
    SDL_SCANCODE_TAB = 43,
    SDL_SCANCODE_SPACE = 44,

    SDL_SCANCODE_MINUS = 45,
    SDL_SCANCODE_EQUALS = 46,
    SDL_SCANCODE_LEFTBRACKET = 47,
    SDL_SCANCODE_RIGHTBRACKET = 48,
    SDL_SCANCODE_BACKSLASH = 49,
    SDL_SCANCODE_NONUSHASH = 50,
    SDL_SCANCODE_SEMICOLON = 51,
    SDL_SCANCODE_APOSTROPHE = 52,
    SDL_SCANCODE_GRAVE = 53,
    SDL_SCANCODE_COMMA = 54,
    SDL_SCANCODE_PERIOD = 55,
    SDL_SCANCODE_SLASH = 56,

    SDL_SCANCODE_CAPSLOCK = 57,

    SDL_SCANCODE_F1 = 58,
    SDL_SCANCODE_F2 = 59,
    SDL_SCANCODE_F3 = 60,
    SDL_SCANCODE_F4 = 61,
    SDL_SCANCODE_F5 = 62,
    SDL_SCANCODE_F6 = 63,
    SDL_SCANCODE_F7 = 64,
    SDL_SCANCODE_F8 = 65,
    SDL_SCANCODE_F9 = 66,
    SDL_SCANCODE_F10 = 67,
    SDL_SCANCODE_F11 = 68,
    SDL_SCANCODE_F12 = 69,

    SDL_SCANCODE_PRINTSCREEN = 70,
    SDL_SCANCODE_SCROLLLOCK = 71,
    SDL_SCANCODE_PAUSE = 72,
    SDL_SCANCODE_INSERT = 73,
    SDL_SCANCODE_HOME = 74,
    SDL_SCANCODE_PAGEUP = 75,
    SDL_SCANCODE_DELETE = 76,
    SDL_SCANCODE_END = 77,
    SDL_SCANCODE_PAGEDOWN = 78,
    SDL_SCANCODE_RIGHT = 79,
    SDL_SCANCODE_LEFT = 80,
    SDL_SCANCODE_DOWN = 81,
    SDL_SCANCODE_UP = 82,

    SDL_SCANCODE_NUMLOCKCLEAR = 83,
    SDL_SCANCODE_KP_DIVIDE = 84,
    SDL_SCANCODE_KP_MULTIPLY = 85,
    SDL_SCANCODE_KP_MINUS = 86,
    SDL_SCANCODE_KP_PLUS = 87,
    SDL_SCANCODE_KP_ENTER = 88,
    SDL_SCANCODE_KP_1 = 89,
    SDL_SCANCODE_KP_2 = 90,
    SDL_SCANCODE_KP_3 = 91,
    SDL_SCANCODE_KP_4 = 92,
    SDL_SCANCODE_KP_5 = 93,
    SDL_SCANCODE_KP_6 = 94,
    SDL_SCANCODE_KP_7 = 95,
    SDL_SCANCODE_KP_8 = 96,
    SDL_SCANCODE_KP_9 = 97,
    SDL_SCANCODE_KP_0 = 98,
    SDL_SCANCODE_KP_PERIOD = 99,

    SDL_SCANCODE_NONUSBACKSLASH = 100,
    SDL_SCANCODE_APPLICATION = 101,
    SDL_SCANCODE_POWER = 102,
    SDL_SCANCODE_KP_EQUALS = 103,
    SDL_SCANCODE_F13 = 104,
    SDL_SCANCODE_F14 = 105,
    SDL_SCANCODE_F15 = 106,
    SDL_SCANCODE_F16 = 107,
    SDL_SCANCODE_F17 = 108,
    SDL_SCANCODE_F18 = 109,
    SDL_SCANCODE_F19 = 110,
    SDL_SCANCODE_F20 = 111,
    SDL_SCANCODE_F21 = 112,
    SDL_SCANCODE_F22 = 113,
    SDL_SCANCODE_F23 = 114,
    SDL_SCANCODE_F24 = 115,
    SDL_SCANCODE_EXECUTE = 116,
    SDL_SCANCODE_HELP = 117,
    SDL_SCANCODE_MENU = 118,
    SDL_SCANCODE_SELECT = 119,
    SDL_SCANCODE_STOP = 120,
    SDL_SCANCODE_AGAIN = 121,
    SDL_SCANCODE_UNDO = 122,
    SDL_SCANCODE_CUT = 123,
    SDL_SCANCODE_COPY = 124,
    SDL_SCANCODE_PASTE = 125,
    SDL_SCANCODE_FIND = 126,
    SDL_SCANCODE_MUTE = 127,
    SDL_SCANCODE_VOLUMEUP = 128,
    SDL_SCANCODE_VOLUMEDOWN = 129,

    SDL_SCANCODE_KP_COMMA = 133,
    SDL_SCANCODE_KP_EQUALSAS400 = 134,

    SDL_SCANCODE_INTERNATIONAL1 = 135,
    SDL_SCANCODE_INTERNATIONAL2 = 136,
    SDL_SCANCODE_INTERNATIONAL3 = 137,
    SDL_SCANCODE_INTERNATIONAL4 = 138,
    SDL_SCANCODE_INTERNATIONAL5 = 139,
    SDL_SCANCODE_INTERNATIONAL6 = 140,
    SDL_SCANCODE_INTERNATIONAL7 = 141,
    SDL_SCANCODE_INTERNATIONAL8 = 142,
    SDL_SCANCODE_INTERNATIONAL9 = 143,
    SDL_SCANCODE_LANG1 = 144,
    SDL_SCANCODE_LANG2 = 145,
    SDL_SCANCODE_LANG3 = 146,
    SDL_SCANCODE_LANG4 = 147,
    SDL_SCANCODE_LANG5 = 148,
    SDL_SCANCODE_LANG6 = 149,
    SDL_SCANCODE_LANG7 = 150,
    SDL_SCANCODE_LANG8 = 151,
    SDL_SCANCODE_LANG9 = 152,

    SDL_SCANCODE_ALTERASE = 153,
    SDL_SCANCODE_SYSREQ = 154,
    SDL_SCANCODE_CANCEL = 155,
    SDL_SCANCODE_CLEAR = 156,
    SDL_SCANCODE_PRIOR = 157,
    SDL_SCANCODE_RETURN2 = 158,
    SDL_SCANCODE_SEPARATOR = 159,
    SDL_SCANCODE_OUT = 160,
    SDL_SCANCODE_OPER = 161,
    SDL_SCANCODE_CLEARAGAIN = 162,
    SDL_SCANCODE_CRSEL = 163,
    SDL_SCANCODE_EXSEL = 164,

    SDL_SCANCODE_KP_00 = 176,
    SDL_SCANCODE_KP_000 = 177,
    SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
    SDL_SCANCODE_DECIMALSEPARATOR = 179,
    SDL_SCANCODE_CURRENCYUNIT = 180,
    SDL_SCANCODE_CURRENCYSUBUNIT = 181,
    SDL_SCANCODE_KP_LEFTPAREN = 182,
    SDL_SCANCODE_KP_RIGHTPAREN = 183,
    SDL_SCANCODE_KP_LEFTBRACE = 184,
    SDL_SCANCODE_KP_RIGHTBRACE = 185,
    SDL_SCANCODE_KP_TAB = 186,
    SDL_SCANCODE_KP_BACKSPACE = 187,
    SDL_SCANCODE_KP_A = 188,
    SDL_SCANCODE_KP_B = 189,
    SDL_SCANCODE_KP_C = 190,
    SDL_SCANCODE_KP_D = 191,
    SDL_SCANCODE_KP_E = 192,
    SDL_SCANCODE_KP_F = 193,
    SDL_SCANCODE_KP_XOR = 194,
    SDL_SCANCODE_KP_POWER = 195,
    SDL_SCANCODE_KP_PERCENT = 196,
    SDL_SCANCODE_KP_LESS = 197,
    SDL_SCANCODE_KP_GREATER = 198,
    SDL_SCANCODE_KP_AMPERSAND = 199,
    SDL_SCANCODE_KP_DBLAMPERSAND = 200,
    SDL_SCANCODE_KP_VERTICALBAR = 201,
    SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
    SDL_SCANCODE_KP_COLON = 203,
    SDL_SCANCODE_KP_HASH = 204,
    SDL_SCANCODE_KP_SPACE = 205,
    SDL_SCANCODE_KP_AT = 206,
    SDL_SCANCODE_KP_EXCLAM = 207,
    SDL_SCANCODE_KP_MEMSTORE = 208,
    SDL_SCANCODE_KP_MEMRECALL = 209,
    SDL_SCANCODE_KP_MEMCLEAR = 210,
    SDL_SCANCODE_KP_MEMADD = 211,
    SDL_SCANCODE_KP_MEMSUBTRACT = 212,
    SDL_SCANCODE_KP_MEMMULTIPLY = 213,
    SDL_SCANCODE_KP_MEMDIVIDE = 214,
    SDL_SCANCODE_KP_PLUSMINUS = 215,
    SDL_SCANCODE_KP_CLEAR = 216,
    SDL_SCANCODE_KP_CLEARENTRY = 217,
    SDL_SCANCODE_KP_BINARY = 218,
    SDL_SCANCODE_KP_OCTAL = 219,
    SDL_SCANCODE_KP_DECIMAL = 220,
    SDL_SCANCODE_KP_HEXADECIMAL = 221,

    SDL_SCANCODE_LCTRL = 224,
    SDL_SCANCODE_LSHIFT = 225,
    SDL_SCANCODE_LALT = 226,
    SDL_SCANCODE_LGUI = 227,
    SDL_SCANCODE_RCTRL = 228,
    SDL_SCANCODE_RSHIFT = 229,
    SDL_SCANCODE_RALT = 230,
    SDL_SCANCODE_RGUI = 231,

    SDL_SCANCODE_MODE = 257,

    SDL_SCANCODE_AUDIONEXT = 258,
    SDL_SCANCODE_AUDIOPREV = 259,
    SDL_SCANCODE_AUDIOSTOP = 260,
    SDL_SCANCODE_AUDIOPLAY = 261,
    SDL_SCANCODE_AUDIOMUTE = 262,
    SDL_SCANCODE_MEDIASELECT = 263,
    SDL_SCANCODE_WWW = 264,
    SDL_SCANCODE_MAIL = 265,
    SDL_SCANCODE_CALCULATOR = 266,
    SDL_SCANCODE_COMPUTER = 267,
    SDL_SCANCODE_AC_SEARCH = 268,
    SDL_SCANCODE_AC_HOME = 269,
    SDL_SCANCODE_AC_BACK = 270,
    SDL_SCANCODE_AC_FORWARD = 271,
    SDL_SCANCODE_AC_STOP = 272,
    SDL_SCANCODE_AC_REFRESH = 273,
    SDL_SCANCODE_AC_BOOKMARKS = 274,

    SDL_SCANCODE_BRIGHTNESSDOWN = 275,
    SDL_SCANCODE_BRIGHTNESSUP = 276,
    SDL_SCANCODE_DISPLAYSWITCH = 277,
    SDL_SCANCODE_KBDILLUMTOGGLE = 278,
    SDL_SCANCODE_KBDILLUMDOWN = 279,
    SDL_SCANCODE_KBDILLUMUP = 280,
    SDL_SCANCODE_EJECT = 281,
    SDL_SCANCODE_SLEEP = 282,

    SDL_SCANCODE_APP1 = 283,
    SDL_SCANCODE_APP2 = 284,

    SDL_NUM_SCANCODES = 512
} SDL_Scancode;

#endif

typedef Sint32 SDL_Keycode;

#define SDLK_SCANCODE_MASK (1<<30)
#define SDL_SCANCODE_TO_KEYCODE(X)  (X | SDLK_SCANCODE_MASK)

enum
{
    SDLK_UNKNOWN = 0,

    SDLK_RETURN = '\r',
    SDLK_ESCAPE = '\033',
    SDLK_BACKSPACE = '\b',
    SDLK_TAB = '\t',
    SDLK_SPACE = ' ',
    SDLK_EXCLAIM = '!',
    SDLK_QUOTEDBL = '"',
    SDLK_HASH = '#',
    SDLK_PERCENT = '%',
    SDLK_DOLLAR = '$',
    SDLK_AMPERSAND = '&',
    SDLK_QUOTE = '\'',
    SDLK_LEFTPAREN = '(',
    SDLK_RIGHTPAREN = ')',
    SDLK_ASTERISK = '*',
    SDLK_PLUS = '+',
    SDLK_COMMA = ',',
    SDLK_MINUS = '-',
    SDLK_PERIOD = '.',
    SDLK_SLASH = '/',
    SDLK_0 = '0',
    SDLK_1 = '1',
    SDLK_2 = '2',
    SDLK_3 = '3',
    SDLK_4 = '4',
    SDLK_5 = '5',
    SDLK_6 = '6',
    SDLK_7 = '7',
    SDLK_8 = '8',
    SDLK_9 = '9',
    SDLK_COLON = ':',
    SDLK_SEMICOLON = ';',
    SDLK_LESS = '<',
    SDLK_EQUALS = '=',
    SDLK_GREATER = '>',
    SDLK_QUESTION = '?',
    SDLK_AT = '@',

    SDLK_LEFTBRACKET = '[',
    SDLK_BACKSLASH = '\\',
    SDLK_RIGHTBRACKET = ']',
    SDLK_CARET = '^',
    SDLK_UNDERSCORE = '_',
    SDLK_BACKQUOTE = '`',
    SDLK_a = 'a',
    SDLK_b = 'b',
    SDLK_c = 'c',
    SDLK_d = 'd',
    SDLK_e = 'e',
    SDLK_f = 'f',
    SDLK_g = 'g',
    SDLK_h = 'h',
    SDLK_i = 'i',
    SDLK_j = 'j',
    SDLK_k = 'k',
    SDLK_l = 'l',
    SDLK_m = 'm',
    SDLK_n = 'n',
    SDLK_o = 'o',
    SDLK_p = 'p',
    SDLK_q = 'q',
    SDLK_r = 'r',
    SDLK_s = 's',
    SDLK_t = 't',
    SDLK_u = 'u',
    SDLK_v = 'v',
    SDLK_w = 'w',
    SDLK_x = 'x',
    SDLK_y = 'y',
    SDLK_z = 'z',

    SDLK_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

    SDLK_F1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
    SDLK_F2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
    SDLK_F3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
    SDLK_F4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
    SDLK_F5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
    SDLK_F6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
    SDLK_F7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
    SDLK_F8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
    SDLK_F9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
    SDLK_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
    SDLK_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
    SDLK_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

    SDLK_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
    SDLK_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
    SDLK_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
    SDLK_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
    SDLK_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
    SDLK_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
    SDLK_DELETE = '\177',
    SDLK_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
    SDLK_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
    SDLK_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
    SDLK_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
    SDLK_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
    SDLK_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),

    SDLK_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
    SDLK_KP_DIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
    SDLK_KP_MULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
    SDLK_KP_MINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
    SDLK_KP_PLUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
    SDLK_KP_ENTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
    SDLK_KP_1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
    SDLK_KP_2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
    SDLK_KP_3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
    SDLK_KP_4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
    SDLK_KP_5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
    SDLK_KP_6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
    SDLK_KP_7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
    SDLK_KP_8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
    SDLK_KP_9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
    SDLK_KP_0 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
    SDLK_KP_PERIOD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

    SDLK_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
    SDLK_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
    SDLK_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
    SDLK_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
    SDLK_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
    SDLK_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
    SDLK_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
    SDLK_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
    SDLK_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
    SDLK_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
    SDLK_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
    SDLK_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
    SDLK_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
    SDLK_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
    SDLK_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
    SDLK_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
    SDLK_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
    SDLK_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
    SDLK_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
    SDLK_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
    SDLK_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
    SDLK_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
    SDLK_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
    SDLK_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
    SDLK_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
    SDLK_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
    SDLK_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
    SDLK_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
    SDLK_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
    SDLK_KP_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
    SDLK_KP_EQUALSAS400 =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

    SDLK_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
    SDLK_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
    SDLK_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
    SDLK_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
    SDLK_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
    SDLK_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
    SDLK_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
    SDLK_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
    SDLK_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
    SDLK_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
    SDLK_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
    SDLK_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

    SDLK_KP_00 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
    SDLK_KP_000 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
    SDLK_THOUSANDSSEPARATOR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
    SDLK_DECIMALSEPARATOR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
    SDLK_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
    SDLK_CURRENCYSUBUNIT =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
    SDLK_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
    SDLK_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
    SDLK_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
    SDLK_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
    SDLK_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
    SDLK_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
    SDLK_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
    SDLK_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
    SDLK_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
    SDLK_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
    SDLK_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
    SDLK_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
    SDLK_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
    SDLK_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
    SDLK_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
    SDLK_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
    SDLK_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
    SDLK_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
    SDLK_KP_DBLAMPERSAND =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
    SDLK_KP_VERTICALBAR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
    SDLK_KP_DBLVERTICALBAR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
    SDLK_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
    SDLK_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
    SDLK_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
    SDLK_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
    SDLK_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
    SDLK_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
    SDLK_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
    SDLK_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
    SDLK_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
    SDLK_KP_MEMSUBTRACT =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
    SDLK_KP_MEMMULTIPLY =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
    SDLK_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
    SDLK_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
    SDLK_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
    SDLK_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
    SDLK_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
    SDLK_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
    SDLK_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
    SDLK_KP_HEXADECIMAL =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

    SDLK_LCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
    SDLK_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
    SDLK_LALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
    SDLK_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
    SDLK_RCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
    SDLK_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
    SDLK_RALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
    SDLK_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

    SDLK_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

    SDLK_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
    SDLK_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
    SDLK_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
    SDLK_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
    SDLK_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
    SDLK_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
    SDLK_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
    SDLK_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
    SDLK_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
    SDLK_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
    SDLK_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
    SDLK_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
    SDLK_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
    SDLK_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
    SDLK_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
    SDLK_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
    SDLK_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

    SDLK_BRIGHTNESSDOWN =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
    SDLK_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
    SDLK_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
    SDLK_KBDILLUMTOGGLE =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
    SDLK_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
    SDLK_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
    SDLK_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
    SDLK_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP)
};

typedef enum
{
    KMOD_NONE = 0x0000,
    KMOD_LSHIFT = 0x0001,
    KMOD_RSHIFT = 0x0002,
    KMOD_LCTRL = 0x0040,
    KMOD_RCTRL = 0x0080,
    KMOD_LALT = 0x0100,
    KMOD_RALT = 0x0200,
    KMOD_LGUI = 0x0400,
    KMOD_RGUI = 0x0800,
    KMOD_NUM = 0x1000,
    KMOD_CAPS = 0x2000,
    KMOD_MODE = 0x4000,
    KMOD_RESERVED = 0x8000
} SDL_Keymod;

#define KMOD_CTRL   (KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT    (KMOD_LALT|KMOD_RALT)
#define KMOD_GUI    (KMOD_LGUI|KMOD_RGUI)

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_Keysym
{
    SDL_Scancode scancode;
    SDL_Keycode sym;
    Uint16 mod;
    Uint32 unused;
} SDL_Keysym;

extern DECLSPEC SDL_Window * SDLCALL SDL_GetKeyboardFocus(void);

extern DECLSPEC const Uint8 *SDLCALL SDL_GetKeyboardState(int *numkeys);

extern DECLSPEC SDL_Keymod SDLCALL SDL_GetModState(void);

extern DECLSPEC void SDLCALL SDL_SetModState(SDL_Keymod modstate);

extern DECLSPEC SDL_Keycode SDLCALL SDL_GetKeyFromScancode(SDL_Scancode scancode);

extern DECLSPEC SDL_Scancode SDLCALL SDL_GetScancodeFromKey(SDL_Keycode key);

extern DECLSPEC const char *SDLCALL SDL_GetScancodeName(SDL_Scancode scancode);

extern DECLSPEC SDL_Scancode SDLCALL SDL_GetScancodeFromName(const char *name);

extern DECLSPEC const char *SDLCALL SDL_GetKeyName(SDL_Keycode key);

extern DECLSPEC SDL_Keycode SDLCALL SDL_GetKeyFromName(const char *name);

extern DECLSPEC void SDLCALL SDL_StartTextInput(void);

extern DECLSPEC SDL_bool SDLCALL SDL_IsTextInputActive(void);

extern DECLSPEC void SDLCALL SDL_StopTextInput(void);

extern DECLSPEC void SDLCALL SDL_SetTextInputRect(SDL_Rect *rect);

extern DECLSPEC SDL_bool SDLCALL SDL_HasScreenKeyboardSupport(void);

extern DECLSPEC SDL_bool SDLCALL SDL_IsScreenKeyboardShown(SDL_Window *window);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_mouse_h
#define _SDL_mouse_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_Cursor SDL_Cursor;

typedef enum
{
    SDL_SYSTEM_CURSOR_ARROW,
    SDL_SYSTEM_CURSOR_IBEAM,
    SDL_SYSTEM_CURSOR_WAIT,
    SDL_SYSTEM_CURSOR_CROSSHAIR,
    SDL_SYSTEM_CURSOR_WAITARROW,
    SDL_SYSTEM_CURSOR_SIZENWSE,
    SDL_SYSTEM_CURSOR_SIZENESW,
    SDL_SYSTEM_CURSOR_SIZEWE,
    SDL_SYSTEM_CURSOR_SIZENS,
    SDL_SYSTEM_CURSOR_SIZEALL,
    SDL_SYSTEM_CURSOR_NO,
    SDL_SYSTEM_CURSOR_HAND,
    SDL_NUM_SYSTEM_CURSORS
} SDL_SystemCursor;

extern DECLSPEC SDL_Window * SDLCALL SDL_GetMouseFocus(void);

extern DECLSPEC Uint32 SDLCALL SDL_GetMouseState(int *x, int *y);

extern DECLSPEC Uint32 SDLCALL SDL_GetRelativeMouseState(int *x, int *y);

extern DECLSPEC void SDLCALL SDL_WarpMouseInWindow(SDL_Window * window,
                                                   int x, int y);

extern DECLSPEC int SDLCALL SDL_SetRelativeMouseMode(SDL_bool enabled);

extern DECLSPEC SDL_bool SDLCALL SDL_GetRelativeMouseMode(void);

extern DECLSPEC SDL_Cursor *SDLCALL SDL_CreateCursor(const Uint8 * data,
                                                     const Uint8 * mask,
                                                     int w, int h, int hot_x,
                                                     int hot_y);

extern DECLSPEC SDL_Cursor *SDLCALL SDL_CreateColorCursor(SDL_Surface *surface,
                                                          int hot_x,
                                                          int hot_y);

extern DECLSPEC SDL_Cursor *SDLCALL SDL_CreateSystemCursor(SDL_SystemCursor id);

extern DECLSPEC void SDLCALL SDL_SetCursor(SDL_Cursor * cursor);

extern DECLSPEC SDL_Cursor *SDLCALL SDL_GetCursor(void);

extern DECLSPEC SDL_Cursor *SDLCALL SDL_GetDefaultCursor(void);

extern DECLSPEC void SDLCALL SDL_FreeCursor(SDL_Cursor * cursor);

extern DECLSPEC int SDLCALL SDL_ShowCursor(int toggle);

#define SDL_BUTTON(X)       (1 << ((X)-1))
#define SDL_BUTTON_LEFT     1
#define SDL_BUTTON_MIDDLE   2
#define SDL_BUTTON_RIGHT    3
#define SDL_BUTTON_X1       4
#define SDL_BUTTON_X2       5
#define SDL_BUTTON_LMASK    SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK    SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK    SDL_BUTTON(SDL_BUTTON_RIGHT)
#define SDL_BUTTON_X1MASK   SDL_BUTTON(SDL_BUTTON_X1)
#define SDL_BUTTON_X2MASK   SDL_BUTTON(SDL_BUTTON_X2)

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_joystick_h
#define _SDL_joystick_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

typedef struct {
    Uint8 data[16];
} SDL_JoystickGUID;

typedef Sint32 SDL_JoystickID;

extern DECLSPEC int SDLCALL SDL_NumJoysticks(void);

extern DECLSPEC const char *SDLCALL SDL_JoystickNameForIndex(int device_index);

extern DECLSPEC SDL_Joystick *SDLCALL SDL_JoystickOpen(int device_index);

extern DECLSPEC const char *SDLCALL SDL_JoystickName(SDL_Joystick * joystick);

extern DECLSPEC SDL_JoystickGUID SDLCALL SDL_JoystickGetDeviceGUID(int device_index);

extern DECLSPEC SDL_JoystickGUID SDLCALL SDL_JoystickGetGUID(SDL_Joystick * joystick);

extern DECLSPEC void SDLCALL SDL_JoystickGetGUIDString(SDL_JoystickGUID guid, char *pszGUID, int cbGUID);

extern DECLSPEC SDL_JoystickGUID SDLCALL SDL_JoystickGetGUIDFromString(const char *pchGUID);

extern DECLSPEC SDL_bool SDLCALL SDL_JoystickGetAttached(SDL_Joystick * joystick);

extern DECLSPEC SDL_JoystickID SDLCALL SDL_JoystickInstanceID(SDL_Joystick * joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumAxes(SDL_Joystick * joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumBalls(SDL_Joystick * joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumHats(SDL_Joystick * joystick);

extern DECLSPEC int SDLCALL SDL_JoystickNumButtons(SDL_Joystick * joystick);

extern DECLSPEC void SDLCALL SDL_JoystickUpdate(void);

extern DECLSPEC int SDLCALL SDL_JoystickEventState(int state);

extern DECLSPEC Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick * joystick,
                                                   int axis);

#define SDL_HAT_CENTERED    0x00
#define SDL_HAT_UP      0x01
#define SDL_HAT_RIGHT       0x02
#define SDL_HAT_DOWN        0x04
#define SDL_HAT_LEFT        0x08
#define SDL_HAT_RIGHTUP     (SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN   (SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP      (SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN    (SDL_HAT_LEFT|SDL_HAT_DOWN)

extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetHat(SDL_Joystick * joystick,
                                                 int hat);

extern DECLSPEC int SDLCALL SDL_JoystickGetBall(SDL_Joystick * joystick,
                                                int ball, int *dx, int *dy);

extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick * joystick,
                                                    int button);

extern DECLSPEC void SDLCALL SDL_JoystickClose(SDL_Joystick * joystick);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_gamecontroller_h
#define _SDL_gamecontroller_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

typedef enum
{
    SDL_CONTROLLER_BINDTYPE_NONE = 0,
    SDL_CONTROLLER_BINDTYPE_BUTTON,
    SDL_CONTROLLER_BINDTYPE_AXIS,
    SDL_CONTROLLER_BINDTYPE_HAT
} SDL_GameControllerBindType;

typedef struct SDL_GameControllerButtonBind
{
    SDL_GameControllerBindType bindType;
    union
    {
        int button;
        int axis;
        struct {
            int hat;
            int hat_mask;
        } hat;
    } value;

} SDL_GameControllerButtonBind;

extern DECLSPEC int SDLCALL SDL_GameControllerAddMappingsFromRW( SDL_RWops * rw, int freerw );

#define SDL_GameControllerAddMappingsFromFile(file)   SDL_GameControllerAddMappingsFromRW(SDL_RWFromFile(file, "rb"), 1)

extern DECLSPEC int SDLCALL SDL_GameControllerAddMapping( const char* mappingString );

extern DECLSPEC char * SDLCALL SDL_GameControllerMappingForGUID( SDL_JoystickGUID guid );

extern DECLSPEC char * SDLCALL SDL_GameControllerMapping( SDL_GameController * gamecontroller );

extern DECLSPEC SDL_bool SDLCALL SDL_IsGameController(int joystick_index);

extern DECLSPEC const char *SDLCALL SDL_GameControllerNameForIndex(int joystick_index);

extern DECLSPEC SDL_GameController *SDLCALL SDL_GameControllerOpen(int joystick_index);

extern DECLSPEC const char *SDLCALL SDL_GameControllerName(SDL_GameController *gamecontroller);

extern DECLSPEC SDL_bool SDLCALL SDL_GameControllerGetAttached(SDL_GameController *gamecontroller);

extern DECLSPEC SDL_Joystick *SDLCALL SDL_GameControllerGetJoystick(SDL_GameController *gamecontroller);

extern DECLSPEC int SDLCALL SDL_GameControllerEventState(int state);

extern DECLSPEC void SDLCALL SDL_GameControllerUpdate(void);

typedef enum
{
    SDL_CONTROLLER_AXIS_INVALID = -1,
    SDL_CONTROLLER_AXIS_LEFTX,
    SDL_CONTROLLER_AXIS_LEFTY,
    SDL_CONTROLLER_AXIS_RIGHTX,
    SDL_CONTROLLER_AXIS_RIGHTY,
    SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    SDL_CONTROLLER_AXIS_MAX
} SDL_GameControllerAxis;

extern DECLSPEC SDL_GameControllerAxis SDLCALL SDL_GameControllerGetAxisFromString(const char *pchString);

extern DECLSPEC const char* SDLCALL SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis axis);

extern DECLSPEC SDL_GameControllerButtonBind SDLCALL
SDL_GameControllerGetBindForAxis(SDL_GameController *gamecontroller,
                                 SDL_GameControllerAxis axis);

extern DECLSPEC Sint16 SDLCALL
SDL_GameControllerGetAxis(SDL_GameController *gamecontroller,
                          SDL_GameControllerAxis axis);

typedef enum
{
    SDL_CONTROLLER_BUTTON_INVALID = -1,
    SDL_CONTROLLER_BUTTON_A,
    SDL_CONTROLLER_BUTTON_B,
    SDL_CONTROLLER_BUTTON_X,
    SDL_CONTROLLER_BUTTON_Y,
    SDL_CONTROLLER_BUTTON_BACK,
    SDL_CONTROLLER_BUTTON_GUIDE,
    SDL_CONTROLLER_BUTTON_START,
    SDL_CONTROLLER_BUTTON_LEFTSTICK,
    SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    SDL_CONTROLLER_BUTTON_DPAD_UP,
    SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    SDL_CONTROLLER_BUTTON_MAX
} SDL_GameControllerButton;

extern DECLSPEC SDL_GameControllerButton SDLCALL SDL_GameControllerGetButtonFromString(const char *pchString);

extern DECLSPEC const char* SDLCALL SDL_GameControllerGetStringForButton(SDL_GameControllerButton button);

extern DECLSPEC SDL_GameControllerButtonBind SDLCALL
SDL_GameControllerGetBindForButton(SDL_GameController *gamecontroller,
                                   SDL_GameControllerButton button);

extern DECLSPEC Uint8 SDLCALL SDL_GameControllerGetButton(SDL_GameController *gamecontroller,
                                                          SDL_GameControllerButton button);

extern DECLSPEC void SDLCALL SDL_GameControllerClose(SDL_GameController *gamecontroller);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_quit_h
#define _SDL_quit_h

#define SDL_QuitRequested() \
        (SDL_PumpEvents(), (SDL_PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUIT,SDL_QUIT) > 0))

#endif
#ifndef _SDL_gesture_h
#define _SDL_gesture_h

#ifndef _SDL_touch_h
#define _SDL_touch_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef Sint64 SDL_TouchID;
typedef Sint64 SDL_FingerID;

typedef struct SDL_Finger
{
    SDL_FingerID id;
    float x;
    float y;
    float pressure;
} SDL_Finger;

#define SDL_TOUCH_MOUSEID ((Uint32)-1)

extern DECLSPEC int SDLCALL SDL_GetNumTouchDevices(void);

extern DECLSPEC SDL_TouchID SDLCALL SDL_GetTouchDevice(int index);

extern DECLSPEC int SDLCALL SDL_GetNumTouchFingers(SDL_TouchID touchID);

extern DECLSPEC SDL_Finger * SDLCALL SDL_GetTouchFinger(SDL_TouchID touchID, int index);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef Sint64 SDL_GestureID;

extern DECLSPEC int SDLCALL SDL_RecordGesture(SDL_TouchID touchId);

extern DECLSPEC int SDLCALL SDL_SaveAllDollarTemplates(SDL_RWops *dst);

extern DECLSPEC int SDLCALL SDL_SaveDollarTemplate(SDL_GestureID gestureId,SDL_RWops *dst);

extern DECLSPEC int SDLCALL SDL_LoadDollarTemplates(SDL_TouchID touchId, SDL_RWops *src);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_RELEASED    0
#define SDL_PRESSED 1

typedef enum
{
    SDL_FIRSTEVENT     = 0,

    SDL_QUIT           = 0x100,

    SDL_APP_TERMINATING,
    SDL_APP_LOWMEMORY,
    SDL_APP_WILLENTERBACKGROUND,
    SDL_APP_DIDENTERBACKGROUND,
    SDL_APP_WILLENTERFOREGROUND,
    SDL_APP_DIDENTERFOREGROUND,

    SDL_WINDOWEVENT    = 0x200,
    SDL_SYSWMEVENT,

    SDL_KEYDOWN        = 0x300,
    SDL_KEYUP,
    SDL_TEXTEDITING,
    SDL_TEXTINPUT,

    SDL_MOUSEMOTION    = 0x400,
    SDL_MOUSEBUTTONDOWN,
    SDL_MOUSEBUTTONUP,
    SDL_MOUSEWHEEL,

    SDL_JOYAXISMOTION  = 0x600,
    SDL_JOYBALLMOTION,
    SDL_JOYHATMOTION,
    SDL_JOYBUTTONDOWN,
    SDL_JOYBUTTONUP,
    SDL_JOYDEVICEADDED,
    SDL_JOYDEVICEREMOVED,

    SDL_CONTROLLERAXISMOTION  = 0x650,
    SDL_CONTROLLERBUTTONDOWN,
    SDL_CONTROLLERBUTTONUP,
    SDL_CONTROLLERDEVICEADDED,
    SDL_CONTROLLERDEVICEREMOVED,
    SDL_CONTROLLERDEVICEREMAPPED,

    SDL_FINGERDOWN      = 0x700,
    SDL_FINGERUP,
    SDL_FINGERMOTION,

    SDL_DOLLARGESTURE   = 0x800,
    SDL_DOLLARRECORD,
    SDL_MULTIGESTURE,

    SDL_CLIPBOARDUPDATE = 0x900,

    SDL_DROPFILE        = 0x1000,

    SDL_RENDER_TARGETS_RESET = 0x2000,

    SDL_USEREVENT    = 0x8000,

    SDL_LASTEVENT    = 0xFFFF
} SDL_EventType;

typedef struct SDL_CommonEvent
{
    Uint32 type;
    Uint32 timestamp;
} SDL_CommonEvent;

typedef struct SDL_WindowEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8 event;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint32 data1;
    Sint32 data2;
} SDL_WindowEvent;

typedef struct SDL_KeyboardEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8 state;
    Uint8 repeat;
    Uint8 padding2;
    Uint8 padding3;
    SDL_Keysym keysym;
} SDL_KeyboardEvent;

#define SDL_TEXTEDITINGEVENT_TEXT_SIZE (32)

typedef struct SDL_TextEditingEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    char text[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    Sint32 start;
    Sint32 length;
} SDL_TextEditingEvent;

#define SDL_TEXTINPUTEVENT_TEXT_SIZE (32)

typedef struct SDL_TextInputEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
} SDL_TextInputEvent;

typedef struct SDL_MouseMotionEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint32 state;
    Sint32 x;
    Sint32 y;
    Sint32 xrel;
    Sint32 yrel;
} SDL_MouseMotionEvent;

typedef struct SDL_MouseButtonEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint8 button;
    Uint8 state;
    Uint8 clicks;
    Uint8 padding1;
    Sint32 x;
    Sint32 y;
} SDL_MouseButtonEvent;

typedef struct SDL_MouseWheelEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Sint32 x;
    Sint32 y;
} SDL_MouseWheelEvent;

typedef struct SDL_JoyAxisEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 axis;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;
    Uint16 padding4;
} SDL_JoyAxisEvent;

typedef struct SDL_JoyBallEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 ball;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 xrel;
    Sint16 yrel;
} SDL_JoyBallEvent;

typedef struct SDL_JoyHatEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 hat;
    Uint8 value;
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyHatEvent;

typedef struct SDL_JoyButtonEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 button;
    Uint8 state;
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyButtonEvent;

typedef struct SDL_JoyDeviceEvent
{
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
} SDL_JoyDeviceEvent;

typedef struct SDL_ControllerAxisEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 axis;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;
    Uint16 padding4;
} SDL_ControllerAxisEvent;

typedef struct SDL_ControllerButtonEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 button;
    Uint8 state;
    Uint8 padding1;
    Uint8 padding2;
} SDL_ControllerButtonEvent;

typedef struct SDL_ControllerDeviceEvent
{
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
} SDL_ControllerDeviceEvent;

typedef struct SDL_TouchFingerEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_TouchID touchId;
    SDL_FingerID fingerId;
    float x;
    float y;
    float dx;
    float dy;
    float pressure;
} SDL_TouchFingerEvent;

typedef struct SDL_MultiGestureEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_TouchID touchId;
    float dTheta;
    float dDist;
    float x;
    float y;
    Uint16 numFingers;
    Uint16 padding;
} SDL_MultiGestureEvent;

typedef struct SDL_DollarGestureEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_TouchID touchId;
    SDL_GestureID gestureId;
    Uint32 numFingers;
    float error;
    float x;
    float y;
} SDL_DollarGestureEvent;

typedef struct SDL_DropEvent
{
    Uint32 type;
    Uint32 timestamp;
    char *file;
} SDL_DropEvent;

typedef struct SDL_QuitEvent
{
    Uint32 type;
    Uint32 timestamp;
} SDL_QuitEvent;

typedef struct SDL_OSEvent
{
    Uint32 type;
    Uint32 timestamp;
} SDL_OSEvent;

typedef struct SDL_UserEvent
{
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Sint32 code;
    void *data1;
    void *data2;
} SDL_UserEvent;

struct SDL_SysWMmsg;
typedef struct SDL_SysWMmsg SDL_SysWMmsg;

typedef struct SDL_SysWMEvent
{
    Uint32 type;
    Uint32 timestamp;
    SDL_SysWMmsg *msg;
} SDL_SysWMEvent;

typedef union SDL_Event
{
    Uint32 type;
    SDL_CommonEvent common;
    SDL_WindowEvent window;
    SDL_KeyboardEvent key;
    SDL_TextEditingEvent edit;
    SDL_TextInputEvent text;
    SDL_MouseMotionEvent motion;
    SDL_MouseButtonEvent button;
    SDL_MouseWheelEvent wheel;
    SDL_JoyAxisEvent jaxis;
    SDL_JoyBallEvent jball;
    SDL_JoyHatEvent jhat;
    SDL_JoyButtonEvent jbutton;
    SDL_JoyDeviceEvent jdevice;
    SDL_ControllerAxisEvent caxis;
    SDL_ControllerButtonEvent cbutton;
    SDL_ControllerDeviceEvent cdevice;
    SDL_QuitEvent quit;
    SDL_UserEvent user;
    SDL_SysWMEvent syswm;
    SDL_TouchFingerEvent tfinger;
    SDL_MultiGestureEvent mgesture;
    SDL_DollarGestureEvent dgesture;
    SDL_DropEvent drop;

    Uint8 padding[56];
} SDL_Event;

extern DECLSPEC void SDLCALL SDL_PumpEvents(void);

typedef enum
{
    SDL_ADDEVENT,
    SDL_PEEKEVENT,
    SDL_GETEVENT
} SDL_eventaction;

extern DECLSPEC int SDLCALL SDL_PeepEvents(SDL_Event * events, int numevents,
                                           SDL_eventaction action,
                                           Uint32 minType, Uint32 maxType);

extern DECLSPEC SDL_bool SDLCALL SDL_HasEvent(Uint32 type);
extern DECLSPEC SDL_bool SDLCALL SDL_HasEvents(Uint32 minType, Uint32 maxType);

extern DECLSPEC void SDLCALL SDL_FlushEvent(Uint32 type);
extern DECLSPEC void SDLCALL SDL_FlushEvents(Uint32 minType, Uint32 maxType);

extern DECLSPEC int SDLCALL SDL_PollEvent(SDL_Event * event);

extern DECLSPEC int SDLCALL SDL_WaitEvent(SDL_Event * event);

extern DECLSPEC int SDLCALL SDL_WaitEventTimeout(SDL_Event * event,
                                                 int timeout);

extern DECLSPEC int SDLCALL SDL_PushEvent(SDL_Event * event);

typedef int (SDLCALL * SDL_EventFilter) (void *userdata, SDL_Event * event);

extern DECLSPEC void SDLCALL SDL_SetEventFilter(SDL_EventFilter filter,
                                                void *userdata);

extern DECLSPEC SDL_bool SDLCALL SDL_GetEventFilter(SDL_EventFilter * filter,
                                                    void **userdata);

extern DECLSPEC void SDLCALL SDL_AddEventWatch(SDL_EventFilter filter,
                                               void *userdata);

extern DECLSPEC void SDLCALL SDL_DelEventWatch(SDL_EventFilter filter,
                                               void *userdata);

extern DECLSPEC void SDLCALL SDL_FilterEvents(SDL_EventFilter filter,
                                              void *userdata);

#define SDL_QUERY   -1
#define SDL_IGNORE   0
#define SDL_DISABLE  0
#define SDL_ENABLE   1

extern DECLSPEC Uint8 SDLCALL SDL_EventState(Uint32 type, int state);

#define SDL_GetEventState(type) SDL_EventState(type, SDL_QUERY)

extern DECLSPEC Uint32 SDLCALL SDL_RegisterEvents(int numevents);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#include "SDL_filesystem.h"
#include "SDL_haptic.h"
#ifndef _SDL_hints_h
#define _SDL_hints_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_HINT_FRAMEBUFFER_ACCELERATION   "SDL_FRAMEBUFFER_ACCELERATION"

#define SDL_HINT_RENDER_DRIVER              "SDL_RENDER_DRIVER"

#define SDL_HINT_RENDER_OPENGL_SHADERS      "SDL_RENDER_OPENGL_SHADERS"

#define SDL_HINT_RENDER_DIRECT3D_THREADSAFE "SDL_RENDER_DIRECT3D_THREADSAFE"

#define SDL_HINT_RENDER_DIRECT3D11_DEBUG    "SDL_HINT_RENDER_DIRECT3D11_DEBUG"

#define SDL_HINT_RENDER_SCALE_QUALITY       "SDL_RENDER_SCALE_QUALITY"

#define SDL_HINT_RENDER_VSYNC               "SDL_RENDER_VSYNC"

#define SDL_HINT_VIDEO_ALLOW_SCREENSAVER    "SDL_VIDEO_ALLOW_SCREENSAVER"

#define SDL_HINT_VIDEO_X11_XVIDMODE         "SDL_VIDEO_X11_XVIDMODE"

#define SDL_HINT_VIDEO_X11_XINERAMA         "SDL_VIDEO_X11_XINERAMA"

#define SDL_HINT_VIDEO_X11_XRANDR           "SDL_VIDEO_X11_XRANDR"

#define SDL_HINT_GRAB_KEYBOARD              "SDL_GRAB_KEYBOARD"

#define SDL_HINT_MOUSE_RELATIVE_MODE_WARP    "SDL_MOUSE_RELATIVE_MODE_WARP"

#define SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS   "SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS"

#define SDL_HINT_IDLE_TIMER_DISABLED "SDL_IOS_IDLE_TIMER_DISABLED"

#define SDL_HINT_ORIENTATIONS "SDL_IOS_ORIENTATIONS"

#define SDL_HINT_ACCELEROMETER_AS_JOYSTICK "SDL_ACCELEROMETER_AS_JOYSTICK"

#define SDL_HINT_XINPUT_ENABLED "SDL_XINPUT_ENABLED"

#define SDL_HINT_GAMECONTROLLERCONFIG "SDL_GAMECONTROLLERCONFIG"

#define SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS "SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"

#define SDL_HINT_ALLOW_TOPMOST "SDL_ALLOW_TOPMOST"

#define SDL_HINT_TIMER_RESOLUTION "SDL_TIMER_RESOLUTION"

#define SDL_HINT_VIDEO_HIGHDPI_DISABLED "SDL_VIDEO_HIGHDPI_DISABLED"

#define SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK "SDL_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK"

#define SDL_HINT_VIDEO_WIN_D3DCOMPILER              "SDL_VIDEO_WIN_D3DCOMPILER"

#define SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT    "SDL_VIDEO_WINDOW_SHARE_PIXEL_FORMAT"

#define SDL_HINT_WINRT_PRIVACY_POLICY_URL "SDL_HINT_WINRT_PRIVACY_POLICY_URL"

#define SDL_HINT_WINRT_PRIVACY_POLICY_LABEL "SDL_HINT_WINRT_PRIVACY_POLICY_LABEL"

#define SDL_HINT_WINRT_HANDLE_BACK_BUTTON "SDL_HINT_WINRT_HANDLE_BACK_BUTTON"

#define SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES    "SDL_VIDEO_MAC_FULLSCREEN_SPACES"

typedef enum
{
    SDL_HINT_DEFAULT,
    SDL_HINT_NORMAL,
    SDL_HINT_OVERRIDE
} SDL_HintPriority;

extern DECLSPEC SDL_bool SDLCALL SDL_SetHintWithPriority(const char *name,
                                                         const char *value,
                                                         SDL_HintPriority priority);

extern DECLSPEC SDL_bool SDLCALL SDL_SetHint(const char *name,
                                             const char *value);

extern DECLSPEC const char * SDLCALL SDL_GetHint(const char *name);

typedef void (*SDL_HintCallback)(void *userdata, const char *name, const char *oldValue, const char *newValue);
extern DECLSPEC void SDLCALL SDL_AddHintCallback(const char *name,
                                                 SDL_HintCallback callback,
                                                 void *userdata);

extern DECLSPEC void SDLCALL SDL_DelHintCallback(const char *name,
                                                 SDL_HintCallback callback,
                                                 void *userdata);

extern DECLSPEC void SDLCALL SDL_ClearHints(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_loadso_h
#define _SDL_loadso_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern DECLSPEC void *SDLCALL SDL_LoadObject(const char *sofile);

extern DECLSPEC void *SDLCALL SDL_LoadFunction(void *handle,
                                               const char *name);

extern DECLSPEC void SDLCALL SDL_UnloadObject(void *handle);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#include "SDL_log.h"
#ifndef _SDL_messagebox_h
#define _SDL_messagebox_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SDL_MESSAGEBOX_ERROR        = 0x00000010,
    SDL_MESSAGEBOX_WARNING      = 0x00000020,
    SDL_MESSAGEBOX_INFORMATION  = 0x00000040
} SDL_MessageBoxFlags;

typedef enum
{
    SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT = 0x00000001,
    SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT = 0x00000002
} SDL_MessageBoxButtonFlags;

typedef struct
{
    Uint32 flags;
    int buttonid;
    const char * text;
} SDL_MessageBoxButtonData;

typedef struct
{
    Uint8 r, g, b;
} SDL_MessageBoxColor;

typedef enum
{
    SDL_MESSAGEBOX_COLOR_BACKGROUND,
    SDL_MESSAGEBOX_COLOR_TEXT,
    SDL_MESSAGEBOX_COLOR_BUTTON_BORDER,
    SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND,
    SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED,
    SDL_MESSAGEBOX_COLOR_MAX
} SDL_MessageBoxColorType;

typedef struct
{
    SDL_MessageBoxColor colors[SDL_MESSAGEBOX_COLOR_MAX];
} SDL_MessageBoxColorScheme;

typedef struct
{
    Uint32 flags;
    SDL_Window *window;
    const char *title;
    const char *message;

    int numbuttons;
    const SDL_MessageBoxButtonData *buttons;

    const SDL_MessageBoxColorScheme *colorScheme;
} SDL_MessageBoxData;

extern DECLSPEC int SDLCALL SDL_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid);

extern DECLSPEC int SDLCALL SDL_ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#include "SDL_power.h"
#ifndef _SDL_render_h
#define _SDL_render_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SDL_RENDERER_SOFTWARE = 0x00000001,
    SDL_RENDERER_ACCELERATED = 0x00000002,
    SDL_RENDERER_PRESENTVSYNC = 0x00000004,
    SDL_RENDERER_TARGETTEXTURE = 0x00000008
} SDL_RendererFlags;

typedef struct SDL_RendererInfo
{
    const char *name;
    Uint32 flags;
    Uint32 num_texture_formats;
    Uint32 texture_formats[16];
    int max_texture_width;
    int max_texture_height;
} SDL_RendererInfo;

typedef enum
{
    SDL_TEXTUREACCESS_STATIC,
    SDL_TEXTUREACCESS_STREAMING,
    SDL_TEXTUREACCESS_TARGET
} SDL_TextureAccess;

typedef enum
{
    SDL_TEXTUREMODULATE_NONE = 0x00000000,
    SDL_TEXTUREMODULATE_COLOR = 0x00000001,
    SDL_TEXTUREMODULATE_ALPHA = 0x00000002
} SDL_TextureModulate;

typedef enum
{
    SDL_FLIP_NONE = 0x00000000,
    SDL_FLIP_HORIZONTAL = 0x00000001,
    SDL_FLIP_VERTICAL = 0x00000002
} SDL_RendererFlip;

struct SDL_Renderer;
typedef struct SDL_Renderer SDL_Renderer;

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

extern DECLSPEC int SDLCALL SDL_GetNumRenderDrivers(void);

extern DECLSPEC int SDLCALL SDL_GetRenderDriverInfo(int index,
                                                    SDL_RendererInfo * info);

extern DECLSPEC int SDLCALL SDL_CreateWindowAndRenderer(
                                int width, int height, Uint32 window_flags,
                                SDL_Window **window, SDL_Renderer **renderer);

extern DECLSPEC SDL_Renderer * SDLCALL SDL_CreateRenderer(SDL_Window * window,
                                               int index, Uint32 flags);

extern DECLSPEC SDL_Renderer * SDLCALL SDL_CreateSoftwareRenderer(SDL_Surface * surface);

extern DECLSPEC SDL_Renderer * SDLCALL SDL_GetRenderer(SDL_Window * window);

extern DECLSPEC int SDLCALL SDL_GetRendererInfo(SDL_Renderer * renderer,
                                                SDL_RendererInfo * info);

extern DECLSPEC int SDLCALL SDL_GetRendererOutputSize(SDL_Renderer * renderer,
                                                      int *w, int *h);

extern DECLSPEC SDL_Texture * SDLCALL SDL_CreateTexture(SDL_Renderer * renderer,
                                                        Uint32 format,
                                                        int access, int w,
                                                        int h);

extern DECLSPEC SDL_Texture * SDLCALL SDL_CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface);

extern DECLSPEC int SDLCALL SDL_QueryTexture(SDL_Texture * texture,
                                             Uint32 * format, int *access,
                                             int *w, int *h);

extern DECLSPEC int SDLCALL SDL_SetTextureColorMod(SDL_Texture * texture,
                                                   Uint8 r, Uint8 g, Uint8 b);

extern DECLSPEC int SDLCALL SDL_GetTextureColorMod(SDL_Texture * texture,
                                                   Uint8 * r, Uint8 * g,
                                                   Uint8 * b);

extern DECLSPEC int SDLCALL SDL_SetTextureAlphaMod(SDL_Texture * texture,
                                                   Uint8 alpha);

extern DECLSPEC int SDLCALL SDL_GetTextureAlphaMod(SDL_Texture * texture,
                                                   Uint8 * alpha);

extern DECLSPEC int SDLCALL SDL_SetTextureBlendMode(SDL_Texture * texture,
                                                    SDL_BlendMode blendMode);

extern DECLSPEC int SDLCALL SDL_GetTextureBlendMode(SDL_Texture * texture,
                                                    SDL_BlendMode *blendMode);

extern DECLSPEC int SDLCALL SDL_UpdateTexture(SDL_Texture * texture,
                                              const SDL_Rect * rect,
                                              const void *pixels, int pitch);

extern DECLSPEC int SDLCALL SDL_UpdateYUVTexture(SDL_Texture * texture,
                                                 const SDL_Rect * rect,
                                                 const Uint8 *Yplane, int Ypitch,
                                                 const Uint8 *Uplane, int Upitch,
                                                 const Uint8 *Vplane, int Vpitch);

extern DECLSPEC int SDLCALL SDL_LockTexture(SDL_Texture * texture,
                                            const SDL_Rect * rect,
                                            void **pixels, int *pitch);

extern DECLSPEC void SDLCALL SDL_UnlockTexture(SDL_Texture * texture);

extern DECLSPEC SDL_bool SDLCALL SDL_RenderTargetSupported(SDL_Renderer *renderer);

extern DECLSPEC int SDLCALL SDL_SetRenderTarget(SDL_Renderer *renderer,
                                                SDL_Texture *texture);

extern DECLSPEC SDL_Texture * SDLCALL SDL_GetRenderTarget(SDL_Renderer *renderer);

extern DECLSPEC int SDLCALL SDL_RenderSetLogicalSize(SDL_Renderer * renderer, int w, int h);

extern DECLSPEC void SDLCALL SDL_RenderGetLogicalSize(SDL_Renderer * renderer, int *w, int *h);

extern DECLSPEC int SDLCALL SDL_RenderSetViewport(SDL_Renderer * renderer,
                                                  const SDL_Rect * rect);

extern DECLSPEC void SDLCALL SDL_RenderGetViewport(SDL_Renderer * renderer,
                                                   SDL_Rect * rect);

extern DECLSPEC int SDLCALL SDL_RenderSetClipRect(SDL_Renderer * renderer,
                                                  const SDL_Rect * rect);

extern DECLSPEC void SDLCALL SDL_RenderGetClipRect(SDL_Renderer * renderer,
                                                   SDL_Rect * rect);

extern DECLSPEC int SDLCALL SDL_RenderSetScale(SDL_Renderer * renderer,
                                               float scaleX, float scaleY);

extern DECLSPEC void SDLCALL SDL_RenderGetScale(SDL_Renderer * renderer,
                                               float *scaleX, float *scaleY);

extern DECLSPEC int SDLCALL SDL_SetRenderDrawColor(SDL_Renderer * renderer,
                                           Uint8 r, Uint8 g, Uint8 b,
                                           Uint8 a);

extern DECLSPEC int SDLCALL SDL_GetRenderDrawColor(SDL_Renderer * renderer,
                                           Uint8 * r, Uint8 * g, Uint8 * b,
                                           Uint8 * a);

extern DECLSPEC int SDLCALL SDL_SetRenderDrawBlendMode(SDL_Renderer * renderer,
                                                       SDL_BlendMode blendMode);

extern DECLSPEC int SDLCALL SDL_GetRenderDrawBlendMode(SDL_Renderer * renderer,
                                                       SDL_BlendMode *blendMode);

extern DECLSPEC int SDLCALL SDL_RenderClear(SDL_Renderer * renderer);

extern DECLSPEC int SDLCALL SDL_RenderDrawPoint(SDL_Renderer * renderer,
                                                int x, int y);

extern DECLSPEC int SDLCALL SDL_RenderDrawPoints(SDL_Renderer * renderer,
                                                 const SDL_Point * points,
                                                 int count);

extern DECLSPEC int SDLCALL SDL_RenderDrawLine(SDL_Renderer * renderer,
                                               int x1, int y1, int x2, int y2);

extern DECLSPEC int SDLCALL SDL_RenderDrawLines(SDL_Renderer * renderer,
                                                const SDL_Point * points,
                                                int count);

extern DECLSPEC int SDLCALL SDL_RenderDrawRect(SDL_Renderer * renderer,
                                               const SDL_Rect * rect);

extern DECLSPEC int SDLCALL SDL_RenderDrawRects(SDL_Renderer * renderer,
                                                const SDL_Rect * rects,
                                                int count);

extern DECLSPEC int SDLCALL SDL_RenderFillRect(SDL_Renderer * renderer,
                                               const SDL_Rect * rect);

extern DECLSPEC int SDLCALL SDL_RenderFillRects(SDL_Renderer * renderer,
                                                const SDL_Rect * rects,
                                                int count);

extern DECLSPEC int SDLCALL SDL_RenderCopy(SDL_Renderer * renderer,
                                           SDL_Texture * texture,
                                           const SDL_Rect * srcrect,
                                           const SDL_Rect * dstrect);

extern DECLSPEC int SDLCALL SDL_RenderCopyEx(SDL_Renderer * renderer,
                                           SDL_Texture * texture,
                                           const SDL_Rect * srcrect,
                                           const SDL_Rect * dstrect,
                                           const double angle,
                                           const SDL_Point *center,
                                           const SDL_RendererFlip flip);

extern DECLSPEC int SDLCALL SDL_RenderReadPixels(SDL_Renderer * renderer,
                                                 const SDL_Rect * rect,
                                                 Uint32 format,
                                                 void *pixels, int pitch);

extern DECLSPEC void SDLCALL SDL_RenderPresent(SDL_Renderer * renderer);

extern DECLSPEC void SDLCALL SDL_DestroyTexture(SDL_Texture * texture);

extern DECLSPEC void SDLCALL SDL_DestroyRenderer(SDL_Renderer * renderer);

extern DECLSPEC int SDLCALL SDL_GL_BindTexture(SDL_Texture *texture, float *texw, float *texh);

extern DECLSPEC int SDLCALL SDL_GL_UnbindTexture(SDL_Texture *texture);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_system_h
#define _SDL_system_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN32__

extern DECLSPEC int SDLCALL SDL_Direct3D9GetAdapterIndex( int displayIndex );

typedef struct IDirect3DDevice9 IDirect3DDevice9;
extern DECLSPEC IDirect3DDevice9* SDLCALL SDL_RenderGetD3D9Device(SDL_Renderer * renderer);

extern DECLSPEC void SDLCALL SDL_DXGIGetOutputInfo( int displayIndex, int *adapterIndex, int *outputIndex );

#endif

#if defined(__IPHONEOS__) && __IPHONEOS__

extern DECLSPEC int SDLCALL SDL_iPhoneSetAnimationCallback(SDL_Window * window, int interval, void (*callback)(void*), void *callbackParam);
extern DECLSPEC void SDLCALL SDL_iPhoneSetEventPump(SDL_bool enabled);

#endif

#if defined(__ANDROID__) && __ANDROID__

extern DECLSPEC void * SDLCALL SDL_AndroidGetJNIEnv();

extern DECLSPEC void * SDLCALL SDL_AndroidGetActivity();

#define SDL_ANDROID_EXTERNAL_STORAGE_READ   0x01
#define SDL_ANDROID_EXTERNAL_STORAGE_WRITE  0x02

extern DECLSPEC const char * SDLCALL SDL_AndroidGetInternalStoragePath();

extern DECLSPEC int SDLCALL SDL_AndroidGetExternalStorageState();

extern DECLSPEC const char * SDLCALL SDL_AndroidGetExternalStoragePath();

#endif

#if defined(__WINRT__) && __WINRT__

typedef enum
{

    SDL_WINRT_PATH_INSTALLED_LOCATION,

    SDL_WINRT_PATH_LOCAL_FOLDER,

    SDL_WINRT_PATH_ROAMING_FOLDER,

    SDL_WINRT_PATH_TEMP_FOLDER
} SDL_WinRT_Path;

extern DECLSPEC const wchar_t * SDLCALL SDL_WinRTGetFSPathUNICODE(SDL_WinRT_Path pathType);

extern DECLSPEC const char * SDLCALL SDL_WinRTGetFSPathUTF8(SDL_WinRT_Path pathType);

#endif

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_timer_h
#define _SDL_timer_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern DECLSPEC Uint32 SDLCALL SDL_GetTicks(void);

#define SDL_TICKS_PASSED(A, B)  ((Sint32)((B) - (A)) <= 0)

extern DECLSPEC Uint64 SDLCALL SDL_GetPerformanceCounter(void);

extern DECLSPEC Uint64 SDLCALL SDL_GetPerformanceFrequency(void);

extern DECLSPEC void SDLCALL SDL_Delay(Uint32 ms);

typedef Uint32 (SDLCALL * SDL_TimerCallback) (Uint32 interval, void *param);

typedef int SDL_TimerID;

extern DECLSPEC SDL_TimerID SDLCALL SDL_AddTimer(Uint32 interval,
                                                 SDL_TimerCallback callback,
                                                 void *param);

extern DECLSPEC SDL_bool SDLCALL SDL_RemoveTimer(SDL_TimerID id);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_version_h
#define _SDL_version_h

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_version
{
    Uint8 major;
    Uint8 minor;
    Uint8 patch;
} SDL_version;

#define SDL_MAJOR_VERSION   2
#define SDL_MINOR_VERSION   0
#define SDL_PATCHLEVEL      3

#define SDL_VERSION(x)                          \
{                                   \
    (x)->major = SDL_MAJOR_VERSION;                 \
    (x)->minor = SDL_MINOR_VERSION;                 \
    (x)->patch = SDL_PATCHLEVEL;                    \
}

#define SDL_VERSIONNUM(X, Y, Z)                     \
    ((X)*1000 + (Y)*100 + (Z))

#define SDL_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL)

#define SDL_VERSION_ATLEAST(X, Y, Z) \
    (SDL_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

extern DECLSPEC void SDLCALL SDL_GetVersion(SDL_version * ver);

extern DECLSPEC const char *SDLCALL SDL_GetRevision(void);

extern DECLSPEC int SDLCALL SDL_GetRevisionNumber(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif

#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__)
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif

#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64

#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif

#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_INIT_TIMER          0x00000001
#define SDL_INIT_AUDIO          0x00000010
#define SDL_INIT_VIDEO          0x00000020
#define SDL_INIT_JOYSTICK       0x00000200
#define SDL_INIT_HAPTIC         0x00001000
#define SDL_INIT_GAMECONTROLLER 0x00002000
#define SDL_INIT_EVENTS         0x00004000
#define SDL_INIT_NOPARACHUTE    0x00100000
#define SDL_INIT_EVERYTHING ( \
                SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | \
                SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER \
            )

extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags);

extern DECLSPEC int SDLCALL SDL_InitSubSystem(Uint32 flags);

extern DECLSPEC void SDLCALL SDL_QuitSubSystem(Uint32 flags);

extern DECLSPEC Uint32 SDLCALL SDL_WasInit(Uint32 flags);

extern DECLSPEC void SDLCALL SDL_Quit(void);

#ifdef __cplusplus
}
#endif
#undef _begin_code_h

#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(pop)
#endif

#endif
#ifndef _SDL_internal_h
#define _SDL_internal_h

#endif
#ifndef _SDL_internal_h
#define _SDL_internal_h

#endif

#ifndef _SDL_error_c_h
#define _SDL_error_c_h

#define ERR_MAX_STRLEN  128
#define ERR_MAX_ARGS    5

typedef struct SDL_error
{

    int error;

    char key[ERR_MAX_STRLEN];

    int argc;
    union
    {
        void *value_ptr;
#if 0
        unsigned char value_c;
#endif
        int value_i;
        double value_f;
        char buf[ERR_MAX_STRLEN];
    } args[ERR_MAX_ARGS];
} SDL_error;

extern SDL_error *SDL_GetErrBuf(void);

#endif
extern SDL_AudioFormat SDL_FirstAudioFormat(SDL_AudioFormat format);
extern SDL_AudioFormat SDL_NextAudioFormat(void);

extern void SDL_CalculateAudioSpec(SDL_AudioSpec * spec);

extern int SDLCALL SDL_RunAudio(void *audiop);

typedef struct
{
    SDL_AudioFormat src_fmt;
    SDL_AudioFormat dst_fmt;
    SDL_AudioFilter filter;
} SDL_AudioTypeFilters;
extern const SDL_AudioTypeFilters sdl_audio_type_filters[];

typedef struct
{
    SDL_AudioFormat fmt;
    int channels;
    int upsample;
    int multiple;
    SDL_AudioFilter filter;
} SDL_AudioRateFilters;
extern const SDL_AudioRateFilters sdl_audio_rate_filters[];
#ifndef _SDL_sysaudio_h
#define _SDL_sysaudio_h

typedef struct SDL_AudioDevice SDL_AudioDevice;
#define _THIS   SDL_AudioDevice *_this

typedef void (*SDL_AddAudioDevice)(const char *name);

typedef struct SDL_AudioDriverImpl
{
    void (*DetectDevices) (int iscapture, SDL_AddAudioDevice addfn);
    int (*OpenDevice) (_THIS, const char *devname, int iscapture);
    void (*ThreadInit) (_THIS);
    void (*WaitDevice) (_THIS);
    void (*PlayDevice) (_THIS);
    Uint8 *(*GetDeviceBuf) (_THIS);
    void (*WaitDone) (_THIS);
    void (*CloseDevice) (_THIS);
    void (*LockDevice) (_THIS);
    void (*UnlockDevice) (_THIS);
    void (*Deinitialize) (void);

    int ProvidesOwnCallbackThread;
    int SkipMixerLock;
    int HasCaptureSupport;
    int OnlyHasDefaultOutputDevice;
    int OnlyHasDefaultInputDevice;
} SDL_AudioDriverImpl;

typedef struct SDL_AudioDriver
{

    const char *name;

    const char *desc;

    SDL_AudioDriverImpl impl;

    char **outputDevices;
    int outputDeviceCount;

    char **inputDevices;
    int inputDeviceCount;
} SDL_AudioDriver;

typedef struct
{
    Uint8 *buffer;
    int max_len;
    int read_pos, write_pos;
} SDL_AudioStreamer;

struct SDL_AudioDevice
{

    SDL_AudioSpec spec;

    SDL_AudioCVT convert;

    int use_streamer;
    SDL_AudioStreamer streamer;

    int iscapture;
    int enabled;
    int paused;
    int opened;

    Uint8 *fake_stream;

    SDL_mutex *mixer_lock;

    SDL_Thread *thread;
    SDL_threadID threadid;

    struct SDL_PrivateAudioData *hidden;
};
#undef _THIS

typedef struct AudioBootStrap
{
    const char *name;
    const char *desc;
    int (*init) (SDL_AudioDriverImpl * impl);
    int demand_only;
} AudioBootStrap;

#endif
#ifndef _SDL_thread_c_h
#define _SDL_thread_c_h

#if SDL_THREADS_DISABLED
typedef int SYS_ThreadHandle;
#elif SDL_THREAD_BEOS
#include "beos/SDL_systhread_c.h"
#elif SDL_THREAD_EPOC
#include "epoc/SDL_systhread_c.h"
#elif SDL_THREAD_PTHREAD
#include <pthread.h>

typedef pthread_t SYS_ThreadHandle;
#elif SDL_THREAD_WINDOWS
#ifndef _SDL_systhread_c_h
#define _SDL_systhread_c_h

#ifndef _INCLUDED_WINDOWS_H
#define _INCLUDED_WINDOWS_H

#if defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#define STRICT
#ifndef UNICODE
#define UNICODE 1
#endif
#undef _WIN32_WINNT
#define _WIN32_WINNT  0x501
#endif

#include <windows.h>

#if UNICODE
#define WIN_StringToUTF8(S) SDL_iconv_string("UTF-8", "UTF-16LE", (char *)(S), (SDL_wcslen(S)+1)*sizeof(WCHAR))
#define WIN_UTF8ToString(S) (WCHAR *)SDL_iconv_string("UTF-16LE", "UTF-8", (char *)(S), SDL_strlen(S)+1)
#else

#define WIN_StringToUTF8(S) SDL_iconv_string("UTF-8", "ASCII", (char *)(S), (SDL_strlen(S)+1))
#define WIN_UTF8ToString(S) SDL_iconv_string("ASCII", "UTF-8", (char *)(S), SDL_strlen(S)+1)
#endif

extern int WIN_SetErrorFromHRESULT(const char *prefix, HRESULT hr);

extern int WIN_SetError(const char *prefix);

extern HRESULT WIN_CoInitialize(void);
extern void WIN_CoUninitialize(void);

#endif

typedef HANDLE SYS_ThreadHandle;

#endif
#elif SDL_THREAD_PSP
#include "psp/SDL_systhread_c.h"
#else
#error Need thread implementation for this platform
typedef int SYS_ThreadHandle;
#endif

struct SDL_Thread
{
    SDL_threadID threadid;
    SYS_ThreadHandle handle;
    int status;
    SDL_error errbuf;
    char *name;
    void *data;
};

extern void SDL_RunThread(void *data);

#endif
#ifndef _SDL_systhread_h
#define _SDL_systhread_h

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
extern int SDL_SYS_CreateThread(SDL_Thread * thread, void *args,
                                pfnSDL_CurrentBeginThread pfnBeginThread,
                                pfnSDL_CurrentEndThread pfnEndThread);
#else
extern int SDL_SYS_CreateThread(SDL_Thread * thread, void *args);
#endif

extern void SDL_SYS_SetupThread(const char *name);

extern int SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority);

extern void SDL_SYS_WaitThread(SDL_Thread * thread);

#endif
#define ROUND_RESOLUTION(X) \
    (((X+TIMER_RESOLUTION-1)/TIMER_RESOLUTION)*TIMER_RESOLUTION)

extern void SDL_TicksInit(void);
extern void SDL_TicksQuit(void);
extern int SDL_TimerInit(void);
extern void SDL_TimerQuit(void);
#ifndef _SDL_udev_h
#define _SDL_udev_h

#if HAVE_LIBUDEV_H

#ifndef SDL_USE_LIBUDEV
#define SDL_USE_LIBUDEV 1
#endif

#include <libudev.h>
#include <sys/time.h>
#include <sys/types.h>

typedef enum
{
    SDL_UDEV_DEVICEADDED = 0x0001,
    SDL_UDEV_DEVICEREMOVED
} SDL_UDEV_deviceevent;

typedef enum
{
    SDL_UDEV_DEVICE_MOUSE       = 0x0001,
    SDL_UDEV_DEVICE_KEYBOARD    = 0x0002,
    SDL_UDEV_DEVICE_JOYSTICK    = 0x0004,
    SDL_UDEV_DEVICE_SOUND       = 0x0008
} SDL_UDEV_deviceclass;

typedef void (*SDL_UDEV_Callback)(SDL_UDEV_deviceevent udev_type, int udev_class, const char *devpath);

typedef struct SDL_UDEV_CallbackList {
    SDL_UDEV_Callback callback;
    struct SDL_UDEV_CallbackList *next;
} SDL_UDEV_CallbackList;

typedef struct SDL_UDEV_PrivateData
{
    const char *udev_library;
    void *udev_handle;
    struct udev *udev;
    struct udev_monitor *udev_mon;
    int ref_count;
    SDL_UDEV_CallbackList *first, *last;

    const char *(*udev_device_get_action)(struct udev_device *);
    const char *(*udev_device_get_devnode)(struct udev_device *);
    const char *(*udev_device_get_subsystem)(struct udev_device *);
    const char *(*udev_device_get_property_value)(struct udev_device *, const char *);
    struct udev_device *(*udev_device_new_from_syspath)(struct udev *, const char *);
    void (*udev_device_unref)(struct udev_device *);
    int (*udev_enumerate_add_match_property)(struct udev_enumerate *, const char *, const char *);
    int (*udev_enumerate_add_match_subsystem)(struct udev_enumerate *, const char *);
    struct udev_list_entry *(*udev_enumerate_get_list_entry)(struct udev_enumerate *);
    struct udev_enumerate *(*udev_enumerate_new)(struct udev *);
    int (*udev_enumerate_scan_devices)(struct udev_enumerate *);
    void (*udev_enumerate_unref)(struct udev_enumerate *);
    const char *(*udev_list_entry_get_name)(struct udev_list_entry *);
    struct udev_list_entry *(*udev_list_entry_get_next)(struct udev_list_entry *);
    int (*udev_monitor_enable_receiving)(struct udev_monitor *);
    int (*udev_monitor_filter_add_match_subsystem_devtype)(struct udev_monitor *, const char *, const char *);
    int (*udev_monitor_get_fd)(struct udev_monitor *);
    struct udev_monitor *(*udev_monitor_new_from_netlink)(struct udev *, const char *);
    struct udev_device *(*udev_monitor_receive_device)(struct udev_monitor *);
    void (*udev_monitor_unref)(struct udev_monitor *);
    struct udev *(*udev_new)(void);
    void (*udev_unref)(struct udev *);
    struct udev_device * (*udev_device_new_from_devnum)(struct udev *udev, char type, dev_t devnum);
    dev_t (*udev_device_get_devnum) (struct udev_device *udev_device);
} SDL_UDEV_PrivateData;

extern int SDL_UDEV_Init(void);
extern void SDL_UDEV_Quit(void);
extern void SDL_UDEV_UnloadLibrary(void);
extern int SDL_UDEV_LoadLibrary(void);
extern void SDL_UDEV_Poll(void);
extern void SDL_UDEV_Scan(void);
extern int SDL_UDEV_AddCallback(SDL_UDEV_Callback cb);
extern void SDL_UDEV_DelCallback(SDL_UDEV_Callback cb);

#endif

#endif

#ifdef SDL_USE_LIBUDEV

static char* SDL_UDEV_LIBS[] = { "libudev.so.1", "libudev.so.0" };

#undef _THIS
#define _THIS SDL_UDEV_PrivateData *_this
static _THIS = NULL;

static SDL_bool SDL_UDEV_load_sym(const char *fn, void **addr);
static int SDL_UDEV_load_syms(void);
static SDL_bool SDL_UDEV_hotplug_update_available(void);
static void device_event(SDL_UDEV_deviceevent type, struct udev_device *dev);

static SDL_bool
SDL_UDEV_load_sym(const char *fn, void **addr)
{
    *addr = SDL_LoadFunction(_this->udev_handle, fn);
    if (*addr == NULL) {

        return SDL_FALSE;
    }

    return SDL_TRUE;
}

static int
SDL_UDEV_load_syms(void)
{

    #define SDL_UDEV_SYM(x) \
        if (!SDL_UDEV_load_sym(#x, (void **) (char *) & _this->x)) return -1

    SDL_UDEV_SYM(udev_device_get_action);
    SDL_UDEV_SYM(udev_device_get_devnode);
    SDL_UDEV_SYM(udev_device_get_subsystem);
    SDL_UDEV_SYM(udev_device_get_property_value);
    SDL_UDEV_SYM(udev_device_new_from_syspath);
    SDL_UDEV_SYM(udev_device_unref);
    SDL_UDEV_SYM(udev_enumerate_add_match_property);
    SDL_UDEV_SYM(udev_enumerate_add_match_subsystem);
    SDL_UDEV_SYM(udev_enumerate_get_list_entry);
    SDL_UDEV_SYM(udev_enumerate_new);
    SDL_UDEV_SYM(udev_enumerate_scan_devices);
    SDL_UDEV_SYM(udev_enumerate_unref);
    SDL_UDEV_SYM(udev_list_entry_get_name);
    SDL_UDEV_SYM(udev_list_entry_get_next);
    SDL_UDEV_SYM(udev_monitor_enable_receiving);
    SDL_UDEV_SYM(udev_monitor_filter_add_match_subsystem_devtype);
    SDL_UDEV_SYM(udev_monitor_get_fd);
    SDL_UDEV_SYM(udev_monitor_new_from_netlink);
    SDL_UDEV_SYM(udev_monitor_receive_device);
    SDL_UDEV_SYM(udev_monitor_unref);
    SDL_UDEV_SYM(udev_new);
    SDL_UDEV_SYM(udev_unref);
    SDL_UDEV_SYM(udev_device_new_from_devnum);
    SDL_UDEV_SYM(udev_device_get_devnum);
    #undef SDL_UDEV_SYM

    return 0;
}

static SDL_bool
SDL_UDEV_hotplug_update_available(void)
{
    if (_this->udev_mon != NULL) {
        const int fd = _this->udev_monitor_get_fd(_this->udev_mon);
        fd_set fds;
        struct timeval tv;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        if ((select(fd+1, &fds, NULL, NULL, &tv) > 0) && (FD_ISSET(fd, &fds))) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

int
SDL_UDEV_Init(void)
{
    int retval = 0;

    if (_this == NULL) {
        _this = (SDL_UDEV_PrivateData *) SDL_calloc(1, sizeof(*_this));
        if(_this == NULL) {
            return SDL_OutOfMemory();
        }

        retval = SDL_UDEV_LoadLibrary();
        if (retval < 0) {
            SDL_UDEV_Quit();
            return retval;
        }

        _this->udev = _this->udev_new();
        if (_this->udev == NULL) {
            SDL_UDEV_Quit();
            return SDL_SetError("udev_new() failed");
        }

        _this->udev_mon = _this->udev_monitor_new_from_netlink(_this->udev, "udev");
        if (_this->udev_mon == NULL) {
            SDL_UDEV_Quit();
            return SDL_SetError("udev_monitor_new_from_netlink() failed");
        }

        _this->udev_monitor_filter_add_match_subsystem_devtype(_this->udev_mon, "input", NULL);
        _this->udev_monitor_filter_add_match_subsystem_devtype(_this->udev_mon, "sound", NULL);
        _this->udev_monitor_enable_receiving(_this->udev_mon);

        SDL_UDEV_Scan();

    }

    _this->ref_count += 1;

    return retval;
}

void
SDL_UDEV_Quit(void)
{
    SDL_UDEV_CallbackList *item;

    if (_this == NULL) {
        return;
    }

    _this->ref_count -= 1;

    if (_this->ref_count < 1) {

        if (_this->udev_mon != NULL) {
            _this->udev_monitor_unref(_this->udev_mon);
            _this->udev_mon = NULL;
        }
        if (_this->udev != NULL) {
            _this->udev_unref(_this->udev);
            _this->udev = NULL;
        }

        while (_this->first != NULL) {
            item = _this->first;
            _this->first = _this->first->next;
            SDL_free(item);
        }

        SDL_UDEV_UnloadLibrary();
        SDL_free(_this);
        _this = NULL;
    }
}

void
SDL_UDEV_Scan(void)
{
    struct udev_enumerate *enumerate = NULL;
    struct udev_list_entry *devs = NULL;
    struct udev_list_entry *item = NULL;

    if (_this == NULL) {
        return;
    }

    enumerate = _this->udev_enumerate_new(_this->udev);
    if (enumerate == NULL) {
        SDL_UDEV_Quit();
        SDL_SetError("udev_monitor_new_from_netlink() failed");
        return;
    }

    _this->udev_enumerate_add_match_subsystem(enumerate, "input");
    _this->udev_enumerate_add_match_subsystem(enumerate, "sound");

    _this->udev_enumerate_scan_devices(enumerate);
    devs = _this->udev_enumerate_get_list_entry(enumerate);
    for (item = devs; item; item = _this->udev_list_entry_get_next(item)) {
        const char *path = _this->udev_list_entry_get_name(item);
        struct udev_device *dev = _this->udev_device_new_from_syspath(_this->udev, path);
        if (dev != NULL) {
            device_event(SDL_UDEV_DEVICEADDED, dev);
            _this->udev_device_unref(dev);
        }
    }

    _this->udev_enumerate_unref(enumerate);
}

void
SDL_UDEV_UnloadLibrary(void)
{
    if (_this == NULL) {
        return;
    }

    if (_this->udev_handle != NULL) {
        SDL_UnloadObject(_this->udev_handle);
        _this->udev_handle = NULL;
    }
}

int
SDL_UDEV_LoadLibrary(void)
{
    int retval = 0, i;

    if (_this == NULL) {
        return SDL_SetError("UDEV not initialized");
    }

    if (_this->udev_handle == NULL) {
        for( i = 0 ; i < SDL_arraysize(SDL_UDEV_LIBS); i++) {
            _this->udev_handle = SDL_LoadObject(SDL_UDEV_LIBS[i]);
            if (_this->udev_handle != NULL) {
                retval = SDL_UDEV_load_syms();
                if (retval < 0) {
                    SDL_UDEV_UnloadLibrary();
                }
                else {
                    break;
                }
            }
        }

        if (_this->udev_handle == NULL) {
            retval = -1;

        }
    }

    return retval;
}

static void
device_event(SDL_UDEV_deviceevent type, struct udev_device *dev)
{
    const char *subsystem;
    const char *val = NULL;
    int devclass = 0;
    const char *path;
    SDL_UDEV_CallbackList *item;

    path = _this->udev_device_get_devnode(dev);
    if (path == NULL) {
        return;
    }

    subsystem = _this->udev_device_get_subsystem(dev);
    if (SDL_strcmp(subsystem, "sound") == 0) {
        devclass = SDL_UDEV_DEVICE_SOUND;
    } else if (SDL_strcmp(subsystem, "input") == 0) {
        val = _this->udev_device_get_property_value(dev, "ID_INPUT_JOYSTICK");
        if (val != NULL && SDL_strcmp(val, "1") == 0 ) {
            devclass |= SDL_UDEV_DEVICE_JOYSTICK;
        }

        val = _this->udev_device_get_property_value(dev, "ID_INPUT_MOUSE");
        if (val != NULL && SDL_strcmp(val, "1") == 0 ) {
            devclass |= SDL_UDEV_DEVICE_MOUSE;
        }

        val = _this->udev_device_get_property_value(dev, "ID_INPUT_KEYBOARD");
        if (val != NULL && SDL_strcmp(val, "1") == 0 ) {
            devclass |= SDL_UDEV_DEVICE_KEYBOARD;
        }

        if (devclass == 0) {
            return;
        }
    } else {
        return;
    }

    for (item = _this->first; item != NULL; item = item->next) {
        item->callback(type, devclass, path);
    }
}

void
SDL_UDEV_Poll(void)
{
    struct udev_device *dev = NULL;
    const char *action = NULL;

    if (_this == NULL) {
        return;
    }

    while (SDL_UDEV_hotplug_update_available()) {
        dev = _this->udev_monitor_receive_device(_this->udev_mon);
        if (dev == NULL) {
            break;
        }
        action = _this->udev_device_get_action(dev);

        if (SDL_strcmp(action, "add") == 0) {
            device_event(SDL_UDEV_DEVICEADDED, dev);
        } else if (SDL_strcmp(action, "remove") == 0) {
            device_event(SDL_UDEV_DEVICEREMOVED, dev);
        }

        _this->udev_device_unref(dev);
    }
}

int
SDL_UDEV_AddCallback(SDL_UDEV_Callback cb)
{
    SDL_UDEV_CallbackList *item;
    item = (SDL_UDEV_CallbackList *) SDL_calloc(1, sizeof (SDL_UDEV_CallbackList));
    if (item == NULL) {
        return SDL_OutOfMemory();
    }

    item->callback = cb;

    if (_this->last == NULL) {
        _this->first = _this->last = item;
    } else {
        _this->last->next = item;
        _this->last = item;
    }

    return 1;
}

void
SDL_UDEV_DelCallback(SDL_UDEV_Callback cb)
{
    SDL_UDEV_CallbackList *item;
    SDL_UDEV_CallbackList *prev = NULL;

    for (item = _this->first; item != NULL; item = item->next) {

        if (item->callback == cb) {
            if (prev != NULL) {
                prev->next = item->next;
            } else {
                SDL_assert(_this->first == item);
                _this->first = item->next;
            }
            if (item == _this->last) {
                _this->last = prev;
            }
            SDL_free(item);
            return;
        }
        prev = item;
    }

}

#endif
#ifdef SDL_LOADSO_DLOPEN

#include <stdio.h>
#include <dlfcn.h>

void *
SDL_LoadObject(const char *sofile)
{
    void *handle = dlopen(sofile, RTLD_NOW|RTLD_LOCAL);
    const char *loaderror = (char *) dlerror();
    if (handle == NULL) {
        SDL_SetError("Failed loading %s: %s", sofile, loaderror);
    }
    return (handle);
}

void *
SDL_LoadFunction(void *handle, const char *name)
{
    void *symbol = dlsym(handle, name);
    if (symbol == NULL) {

        size_t len = 1 + SDL_strlen(name) + 1;
        char *_name = SDL_stack_alloc(char, len);
        _name[0] = '_';
        SDL_strlcpy(&_name[1], name, len);
        symbol = dlsym(handle, _name);
        SDL_stack_free(_name);
        if (symbol == NULL) {
            SDL_SetError("Failed loading %s: %s", name,
                         (const char *) dlerror());
        }
    }
    return (symbol);
}

void
SDL_UnloadObject(void *handle)
{
    if (handle != NULL) {
        dlclose(handle);
    }
}

#endif
#if defined(SDL_LOADSO_DUMMY) || defined(SDL_LOADSO_DISABLED)

void *
SDL_LoadObject(const char *sofile)
{
    const char *loaderror = "SDL_LoadObject() not implemented";
    SDL_SetError("Failed loading %s: %s", sofile, loaderror);
    return (NULL);
}

void *
SDL_LoadFunction(void *handle, const char *name)
{
    const char *loaderror = "SDL_LoadFunction() not implemented";
    SDL_SetError("Failed loading %s: %s", name, loaderror);
    return (NULL);
}

void
SDL_UnloadObject(void *handle)
{

}

#endif
#ifdef SDL_LOADSO_WINDOWS

void *
SDL_LoadObject(const char *sofile)
{
    LPTSTR tstr = WIN_UTF8ToString(sofile);
#ifdef __WINRT__

    void *handle = (void *) LoadPackagedLibrary(tstr, 0);
#else
    void *handle = (void *) LoadLibrary(tstr);
#endif
    SDL_free(tstr);

    if (handle == NULL) {
        char errbuf[512];
        SDL_strlcpy(errbuf, "Failed loading ", SDL_arraysize(errbuf));
        SDL_strlcat(errbuf, sofile, SDL_arraysize(errbuf));
        WIN_SetError(errbuf);
    }
    return handle;
}

void *
SDL_LoadFunction(void *handle, const char *name)
{
    void *symbol = (void *) GetProcAddress((HMODULE) handle, name);
    if (symbol == NULL) {
        char errbuf[512];
        SDL_strlcpy(errbuf, "Failed loading ", SDL_arraysize(errbuf));
        SDL_strlcat(errbuf, name, SDL_arraysize(errbuf));
        WIN_SetError(errbuf);
    }
    return symbol;
}

void
SDL_UnloadObject(void *handle)
{
    if (handle != NULL) {
        FreeLibrary((HMODULE) handle);
    }
}

#endif
#if SDL_AUDIO_DRIVER_ALSA

#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define SDL_AllocAudioMem   SDL_malloc
#define SDL_FreeAudioMem    SDL_free
#ifndef _SDL_ALSA_audio_h
#define _SDL_ALSA_audio_h

#include <alsa/asoundlib.h>

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataALSA*)this->hidden)
struct SDL_PrivateAudioDataALSA
{

    snd_pcm_t *pcm_handle;

    Uint8 *mixbuf;
    int mixlen;
};

#endif

#ifdef SDL_AUDIO_DRIVER_ALSA_DYNAMIC
#endif

static int (*ALSA_snd_pcm_open)
  (snd_pcm_t **, const char *, snd_pcm_stream_t, int);
static int (*ALSA_snd_pcm_close) (snd_pcm_t * pcm);
static snd_pcm_sframes_t(*ALSA_snd_pcm_writei)
  (snd_pcm_t *, const void *, snd_pcm_uframes_t);
static int (*ALSA_snd_pcm_recover) (snd_pcm_t *, int, int);
static int (*ALSA_snd_pcm_prepare) (snd_pcm_t *);
static int (*ALSA_snd_pcm_drain) (snd_pcm_t *);
static const char *(*ALSA_snd_strerror) (int);
static size_t(*ALSA_snd_pcm_hw_params_sizeof) (void);
static size_t(*ALSA_snd_pcm_sw_params_sizeof) (void);
static void (*ALSA_snd_pcm_hw_params_copy)
  (snd_pcm_hw_params_t *, const snd_pcm_hw_params_t *);
static int (*ALSA_snd_pcm_hw_params_any) (snd_pcm_t *, snd_pcm_hw_params_t *);
static int (*ALSA_snd_pcm_hw_params_set_access)
  (snd_pcm_t *, snd_pcm_hw_params_t *, snd_pcm_access_t);
static int (*ALSA_snd_pcm_hw_params_set_format)
  (snd_pcm_t *, snd_pcm_hw_params_t *, snd_pcm_format_t);
static int (*ALSA_snd_pcm_hw_params_set_channels)
  (snd_pcm_t *, snd_pcm_hw_params_t *, unsigned int);
static int (*ALSA_snd_pcm_hw_params_get_channels)
  (const snd_pcm_hw_params_t *, unsigned int *);
static int (*ALSA_snd_pcm_hw_params_set_rate_near)
  (snd_pcm_t *, snd_pcm_hw_params_t *, unsigned int *, int *);
static int (*ALSA_snd_pcm_hw_params_set_period_size_near)
  (snd_pcm_t *, snd_pcm_hw_params_t *, snd_pcm_uframes_t *, int *);
static int (*ALSA_snd_pcm_hw_params_get_period_size)
  (const snd_pcm_hw_params_t *, snd_pcm_uframes_t *, int *);
static int (*ALSA_snd_pcm_hw_params_set_periods_near)
  (snd_pcm_t *, snd_pcm_hw_params_t *, unsigned int *, int *);
static int (*ALSA_snd_pcm_hw_params_get_periods)
  (const snd_pcm_hw_params_t *, unsigned int *, int *);
static int (*ALSA_snd_pcm_hw_params_set_buffer_size_near)
  (snd_pcm_t *pcm, snd_pcm_hw_params_t *, snd_pcm_uframes_t *);
static int (*ALSA_snd_pcm_hw_params_get_buffer_size)
  (const snd_pcm_hw_params_t *, snd_pcm_uframes_t *);
static int (*ALSA_snd_pcm_hw_params) (snd_pcm_t *, snd_pcm_hw_params_t *);
static int (*ALSA_snd_pcm_sw_params_current) (snd_pcm_t *,
                                              snd_pcm_sw_params_t *);
static int (*ALSA_snd_pcm_sw_params_set_start_threshold)
  (snd_pcm_t *, snd_pcm_sw_params_t *, snd_pcm_uframes_t);
static int (*ALSA_snd_pcm_sw_params) (snd_pcm_t *, snd_pcm_sw_params_t *);
static int (*ALSA_snd_pcm_nonblock) (snd_pcm_t *, int);
static int (*ALSA_snd_pcm_wait)(snd_pcm_t *, int);
static int (*ALSA_snd_pcm_sw_params_set_avail_min)
  (snd_pcm_t *, snd_pcm_sw_params_t *, snd_pcm_uframes_t);

#ifdef SDL_AUDIO_DRIVER_ALSA_DYNAMIC
#define snd_pcm_hw_params_sizeof ALSA_snd_pcm_hw_params_sizeof
#define snd_pcm_sw_params_sizeof ALSA_snd_pcm_sw_params_sizeof

static const char *alsa_library = SDL_AUDIO_DRIVER_ALSA_DYNAMIC;
static void *alsa_handle = NULL;

static int
load_alsa_sym(const char *fn, void **addr)
{
    *addr = SDL_LoadFunction(alsa_handle, fn);
    if (*addr == NULL) {

        return 0;
    }

    return 1;
}

#define SDL_ALSA_SYM(x) \
    if (!load_alsa_sym(#x, (void **) (char *) &ALSA_##x)) return -1
#else
#define SDL_ALSA_SYM(x) ALSA_##x = x
#endif

static int
load_alsa_syms(void)
{
    SDL_ALSA_SYM(snd_pcm_open);
    SDL_ALSA_SYM(snd_pcm_close);
    SDL_ALSA_SYM(snd_pcm_writei);
    SDL_ALSA_SYM(snd_pcm_recover);
    SDL_ALSA_SYM(snd_pcm_prepare);
    SDL_ALSA_SYM(snd_pcm_drain);
    SDL_ALSA_SYM(snd_strerror);
    SDL_ALSA_SYM(snd_pcm_hw_params_sizeof);
    SDL_ALSA_SYM(snd_pcm_sw_params_sizeof);
    SDL_ALSA_SYM(snd_pcm_hw_params_copy);
    SDL_ALSA_SYM(snd_pcm_hw_params_any);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_access);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_format);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_channels);
    SDL_ALSA_SYM(snd_pcm_hw_params_get_channels);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_rate_near);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_period_size_near);
    SDL_ALSA_SYM(snd_pcm_hw_params_get_period_size);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_periods_near);
    SDL_ALSA_SYM(snd_pcm_hw_params_get_periods);
    SDL_ALSA_SYM(snd_pcm_hw_params_set_buffer_size_near);
    SDL_ALSA_SYM(snd_pcm_hw_params_get_buffer_size);
    SDL_ALSA_SYM(snd_pcm_hw_params);
    SDL_ALSA_SYM(snd_pcm_sw_params_current);
    SDL_ALSA_SYM(snd_pcm_sw_params_set_start_threshold);
    SDL_ALSA_SYM(snd_pcm_sw_params);
    SDL_ALSA_SYM(snd_pcm_nonblock);
    SDL_ALSA_SYM(snd_pcm_wait);
    SDL_ALSA_SYM(snd_pcm_sw_params_set_avail_min);
    return 0;
}

#undef SDL_ALSA_SYM

#ifdef SDL_AUDIO_DRIVER_ALSA_DYNAMIC

static void
UnloadALSALibrary(void)
{
    if (alsa_handle != NULL) {
        SDL_UnloadObject(alsa_handle);
        alsa_handle = NULL;
    }
}

static int
LoadALSALibrary(void)
{
    int retval = 0;
    if (alsa_handle == NULL) {
        alsa_handle = SDL_LoadObject(alsa_library);
        if (alsa_handle == NULL) {
            retval = -1;

        } else {
            retval = load_alsa_syms();
            if (retval < 0) {
                UnloadALSALibrary();
            }
        }
    }
    return retval;
}

#else

static void
UnloadALSALibrary(void)
{
}

static int
LoadALSALibrary(void)
{
    load_alsa_syms();
    return 0;
}

#endif

static const char *
ALSA_get_audio_device(int channels)
{
    const char *device;

    device = SDL_getenv("AUDIODEV");
    if (device == NULL) {
        switch (channels) {
        case 6:
            device = "plug:surround51";
            break;
        case 4:
            device = "plug:surround40";
            break;
        default:
            device = "default";
            break;
        }
    }
    return device;
}

static void
ALSA_WaitDevice(_THIS)
{

}

#define SWIZ6(T) \
    T *ptr = (T *) SDLAUDIOHIDDEN->mixbuf; \
    Uint32 i; \
    for (i = 0; i < this->spec.samples; i++, ptr += 6) { \
        T tmp; \
        tmp = ptr[2]; ptr[2] = ptr[4]; ptr[4] = tmp; \
        tmp = ptr[3]; ptr[3] = ptr[5]; ptr[5] = tmp; \
    }

static SDL_INLINE void
swizzle_alsa_channels_6_64bit(_THIS)
{
    SWIZ6(Uint64);
}

static SDL_INLINE void
swizzle_alsa_channels_6_32bit(_THIS)
{
    SWIZ6(Uint32);
}

static SDL_INLINE void
swizzle_alsa_channels_6_16bit(_THIS)
{
    SWIZ6(Uint16);
}

static SDL_INLINE void
swizzle_alsa_channels_6_8bit(_THIS)
{
    SWIZ6(Uint8);
}

#undef SWIZ6

static SDL_INLINE void
swizzle_alsa_channels(_THIS)
{
    if (this->spec.channels == 6) {
        const Uint16 fmtsize = (this->spec.format & 0xFF);
        if (fmtsize == 16)
            swizzle_alsa_channels_6_16bit(this);
        else if (fmtsize == 8)
            swizzle_alsa_channels_6_8bit(this);
        else if (fmtsize == 32)
            swizzle_alsa_channels_6_32bit(this);
        else if (fmtsize == 64)
            swizzle_alsa_channels_6_64bit(this);
    }

}

static void
ALSA_PlayDevice(_THIS)
{
    int status;
    const Uint8 *sample_buf = (const Uint8 *) SDLAUDIOHIDDEN->mixbuf;
    const int frame_size = (((int) (this->spec.format & 0xFF)) / 8) *
                                this->spec.channels;
    snd_pcm_uframes_t frames_left = ((snd_pcm_uframes_t) this->spec.samples);

    swizzle_alsa_channels(this);

    while ( frames_left > 0 && this->enabled ) {

        status = ALSA_snd_pcm_writei(SDLAUDIOHIDDEN->pcm_handle,
                                     sample_buf, frames_left);

        if (status < 0) {
            if (status == -EAGAIN) {

                SDL_Delay(1);
                continue;
            }
            status = ALSA_snd_pcm_recover(SDLAUDIOHIDDEN->pcm_handle, status, 0);
            if (status < 0) {

                fprintf(stderr, "ALSA write failed (unrecoverable): %s\n",
                        ALSA_snd_strerror(status));
                this->enabled = 0;
                return;
            }
            continue;
        }
        sample_buf += status * frame_size;
        frames_left -= status;
    }
}

static Uint8 *
ALSA_GetDeviceBuf(_THIS)
{
    return (SDLAUDIOHIDDEN->mixbuf);
}

static void
ALSA_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        SDL_FreeAudioMem(SDLAUDIOHIDDEN->mixbuf);
        SDLAUDIOHIDDEN->mixbuf = NULL;
        if (SDLAUDIOHIDDEN->pcm_handle) {
            ALSA_snd_pcm_drain(SDLAUDIOHIDDEN->pcm_handle);
            ALSA_snd_pcm_close(SDLAUDIOHIDDEN->pcm_handle);
            SDLAUDIOHIDDEN->pcm_handle = NULL;
        }
        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
ALSA_finalize_hardware(_THIS, snd_pcm_hw_params_t *hwparams, int override)
{
    int status;
    snd_pcm_uframes_t bufsize;

    status = ALSA_snd_pcm_hw_params(SDLAUDIOHIDDEN->pcm_handle, hwparams);
    if ( status < 0 ) {
        return(-1);
    }

    status = ALSA_snd_pcm_hw_params_get_buffer_size(hwparams, &bufsize);
    if ( status < 0 ) {
        return(-1);
    }
    if ( !override && bufsize != this->spec.samples * 2 ) {
        return(-1);
    }

    this->spec.samples = bufsize / 2;

    if ( SDL_getenv("SDL_AUDIO_ALSA_DEBUG") ) {
        snd_pcm_uframes_t persize = 0;
        unsigned int periods = 0;

        ALSA_snd_pcm_hw_params_get_period_size(hwparams, &persize, NULL);
        ALSA_snd_pcm_hw_params_get_periods(hwparams, &periods, NULL);

        fprintf(stderr,
            "ALSA: period size = %ld, periods = %u, buffer size = %lu\n",
            persize, periods, bufsize);
    }

    return(0);
}

static int
ALSA_set_period_size(_THIS, snd_pcm_hw_params_t *params, int override)
{
    const char *env;
    int status;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_uframes_t frames;
    unsigned int periods;

    snd_pcm_hw_params_alloca(&hwparams);
    ALSA_snd_pcm_hw_params_copy(hwparams, params);

    if ( !override ) {
        env = SDL_getenv("SDL_AUDIO_ALSA_SET_PERIOD_SIZE");
        if ( env ) {
            override = SDL_atoi(env);
            if ( override == 0 ) {
                return(-1);
            }
        }
    }

    frames = this->spec.samples;
    status = ALSA_snd_pcm_hw_params_set_period_size_near(
                SDLAUDIOHIDDEN->pcm_handle, hwparams, &frames, NULL);
    if ( status < 0 ) {
        return(-1);
    }

    periods = 2;
    status = ALSA_snd_pcm_hw_params_set_periods_near(
                SDLAUDIOHIDDEN->pcm_handle, hwparams, &periods, NULL);
    if ( status < 0 ) {
        return(-1);
    }

    return ALSA_finalize_hardware(this, hwparams, override);
}

static int
ALSA_set_buffer_size(_THIS, snd_pcm_hw_params_t *params, int override)
{
    const char *env;
    int status;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_uframes_t frames;

    snd_pcm_hw_params_alloca(&hwparams);
    ALSA_snd_pcm_hw_params_copy(hwparams, params);

    if ( !override ) {
        env = SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE");
        if ( env ) {
            override = SDL_atoi(env);
            if ( override == 0 ) {
                return(-1);
            }
        }
    }

    frames = this->spec.samples * 2;
    status = ALSA_snd_pcm_hw_params_set_buffer_size_near(
                    SDLAUDIOHIDDEN->pcm_handle, hwparams, &frames);
    if ( status < 0 ) {
        return(-1);
    }

    return ALSA_finalize_hardware(this, hwparams, override);
}

static int
ALSA_OpenDevice(_THIS, const char *devname, int iscapture)
{
    int status = 0;
    snd_pcm_t *pcm_handle = NULL;
    snd_pcm_hw_params_t *hwparams = NULL;
    snd_pcm_sw_params_t *swparams = NULL;
    snd_pcm_format_t format = 0;
    SDL_AudioFormat test_format = 0;
    unsigned int rate = 0;
    unsigned int channels = 0;

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    status = ALSA_snd_pcm_open(&pcm_handle,
                               ALSA_get_audio_device(this->spec.channels),
                               SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't open audio device: %s",
                            ALSA_snd_strerror(status));
    }

    SDLAUDIOHIDDEN->pcm_handle = pcm_handle;

    snd_pcm_hw_params_alloca(&hwparams);
    status = ALSA_snd_pcm_hw_params_any(pcm_handle, hwparams);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't get hardware config: %s",
                            ALSA_snd_strerror(status));
    }

    status = ALSA_snd_pcm_hw_params_set_access(pcm_handle, hwparams,
                                               SND_PCM_ACCESS_RW_INTERLEAVED);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't set interleaved access: %s",
                     ALSA_snd_strerror(status));
    }

    status = -1;
    for (test_format = SDL_FirstAudioFormat(this->spec.format);
         test_format && (status < 0);) {
        status = 0;
        switch (test_format) {
        case AUDIO_U8:
            format = SND_PCM_FORMAT_U8;
            break;
        case AUDIO_S8:
            format = SND_PCM_FORMAT_S8;
            break;
        case AUDIO_S16LSB:
            format = SND_PCM_FORMAT_S16_LE;
            break;
        case AUDIO_S16MSB:
            format = SND_PCM_FORMAT_S16_BE;
            break;
        case AUDIO_U16LSB:
            format = SND_PCM_FORMAT_U16_LE;
            break;
        case AUDIO_U16MSB:
            format = SND_PCM_FORMAT_U16_BE;
            break;
        case AUDIO_S32LSB:
            format = SND_PCM_FORMAT_S32_LE;
            break;
        case AUDIO_S32MSB:
            format = SND_PCM_FORMAT_S32_BE;
            break;
        case AUDIO_F32LSB:
            format = SND_PCM_FORMAT_FLOAT_LE;
            break;
        case AUDIO_F32MSB:
            format = SND_PCM_FORMAT_FLOAT_BE;
            break;
        default:
            status = -1;
            break;
        }
        if (status >= 0) {
            status = ALSA_snd_pcm_hw_params_set_format(pcm_handle,
                                                       hwparams, format);
        }
        if (status < 0) {
            test_format = SDL_NextAudioFormat();
        }
    }
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't find any hardware audio formats");
    }
    this->spec.format = test_format;

    status = ALSA_snd_pcm_hw_params_set_channels(pcm_handle, hwparams,
                                                 this->spec.channels);
    channels = this->spec.channels;
    if (status < 0) {
        status = ALSA_snd_pcm_hw_params_get_channels(hwparams, &channels);
        if (status < 0) {
            ALSA_CloseDevice(this);
            return SDL_SetError("ALSA: Couldn't set audio channels");
        }
        this->spec.channels = channels;
    }

    rate = this->spec.freq;
    status = ALSA_snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams,
                                                  &rate, NULL);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't set audio frequency: %s",
                            ALSA_snd_strerror(status));
    }
    this->spec.freq = rate;

    if ( ALSA_set_period_size(this, hwparams, 0) < 0 &&
         ALSA_set_buffer_size(this, hwparams, 0) < 0 ) {

        if ( ALSA_set_period_size(this, hwparams, 1) < 0 ) {
            ALSA_CloseDevice(this);
            return SDL_SetError("Couldn't set hardware audio parameters: %s", ALSA_snd_strerror(status));
        }
    }

    snd_pcm_sw_params_alloca(&swparams);
    status = ALSA_snd_pcm_sw_params_current(pcm_handle, swparams);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't get software config: %s",
                            ALSA_snd_strerror(status));
    }
    status = ALSA_snd_pcm_sw_params_set_avail_min(pcm_handle, swparams, this->spec.samples);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("Couldn't set minimum available samples: %s",
                            ALSA_snd_strerror(status));
    }
    status =
        ALSA_snd_pcm_sw_params_set_start_threshold(pcm_handle, swparams, 1);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("ALSA: Couldn't set start threshold: %s",
                            ALSA_snd_strerror(status));
    }
    status = ALSA_snd_pcm_sw_params(pcm_handle, swparams);
    if (status < 0) {
        ALSA_CloseDevice(this);
        return SDL_SetError("Couldn't set software audio parameters: %s",
                            ALSA_snd_strerror(status));
    }

    SDL_CalculateAudioSpec(&this->spec);

    SDLAUDIOHIDDEN->mixlen = this->spec.size;
    SDLAUDIOHIDDEN->mixbuf = (Uint8 *) SDL_AllocAudioMem(SDLAUDIOHIDDEN->mixlen);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        ALSA_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN->mixbuf, this->spec.silence, SDLAUDIOHIDDEN->mixlen);

    ALSA_snd_pcm_nonblock(pcm_handle, 0);

    return 0;
}

static void
ALSA_Deinitialize(void)
{
    UnloadALSALibrary();
}

static int
ALSA_Init(SDL_AudioDriverImpl * impl)
{
    if (LoadALSALibrary() < 0) {
        return 0;
    }

    impl->OpenDevice = ALSA_OpenDevice;
    impl->WaitDevice = ALSA_WaitDevice;
    impl->GetDeviceBuf = ALSA_GetDeviceBuf;
    impl->PlayDevice = ALSA_PlayDevice;
    impl->CloseDevice = ALSA_CloseDevice;
    impl->Deinitialize = ALSA_Deinitialize;
    impl->OnlyHasDefaultOutputDevice = 1;

    return 1;
}

AudioBootStrap ALSA_bootstrap = {
    "alsa", "ALSA PCM audio", ALSA_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_ARTS

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#include <unistd.h>
#include <errno.h>

#define SDL_AllocAudioMem   SDL_malloc
#define SDL_FreeAudioMem    SDL_free
#ifndef _SDL_artscaudio_h
#define _SDL_artscaudio_h

#include <artsc.h>

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataARTS*)this->hidden)
struct SDL_PrivateAudioDataARTS
{

    arts_stream_t stream;

    pid_t parent;

    Uint8 *mixbuf;
    int mixlen;

    float frame_ticks;
    float next_frame;
};
#define FUDGE_TICKS 10

#endif

#ifdef SDL_AUDIO_DRIVER_ARTS_DYNAMIC
#ifndef _SDLname_h_
#define _SDLname_h_

#if defined(__STDC__) || defined(__cplusplus)
#define NeedFunctionPrototypes 1
#endif

#define SDL_NAME(X) SDL_##X

#endif
#else
#define SDL_NAME(X)	X
#endif

#ifdef SDL_AUDIO_DRIVER_ARTS_DYNAMIC

static const char *arts_library = SDL_AUDIO_DRIVER_ARTS_DYNAMIC;
static void *arts_handle = NULL;

static int (*SDL_NAME(arts_init)) (void);
static void (*SDL_NAME(arts_free)) (void);
static arts_stream_t(*SDL_NAME(arts_play_stream)) (int rate, int bits,
                                                   int channels,
                                                   const char *name);
static int (*SDL_NAME(arts_stream_set)) (arts_stream_t s,
                                         arts_parameter_t param, int value);
static int (*SDL_NAME(arts_stream_get)) (arts_stream_t s,
                                         arts_parameter_t param);
static int (*SDL_NAME(arts_write)) (arts_stream_t s, const void *buffer,
                                    int count);
static void (*SDL_NAME(arts_close_stream)) (arts_stream_t s);
static int (*SDL_NAME(arts_suspend))(void);
static int (*SDL_NAME(arts_suspended)) (void);
static const char *(*SDL_NAME(arts_error_text)) (int errorcode);

#define SDL_ARTS_SYM(x) { #x, (void **) (char *) &SDL_NAME(x) }
static struct
{
    const char *name;
    void **func;
} arts_functions[] = {

    SDL_ARTS_SYM(arts_init),
    SDL_ARTS_SYM(arts_free),
    SDL_ARTS_SYM(arts_play_stream),
    SDL_ARTS_SYM(arts_stream_set),
    SDL_ARTS_SYM(arts_stream_get),
    SDL_ARTS_SYM(arts_write),
    SDL_ARTS_SYM(arts_close_stream),
    SDL_ARTS_SYM(arts_suspend),
    SDL_ARTS_SYM(arts_suspended),
    SDL_ARTS_SYM(arts_error_text),

};

#undef SDL_ARTS_SYM

static void
UnloadARTSLibrary()
{
    if (arts_handle != NULL) {
        SDL_UnloadObject(arts_handle);
        arts_handle = NULL;
    }
}

static int
LoadARTSLibrary(void)
{
    int i, retval = -1;

    if (arts_handle == NULL) {
        arts_handle = SDL_LoadObject(arts_library);
        if (arts_handle != NULL) {
            retval = 0;
            for (i = 0; i < SDL_arraysize(arts_functions); ++i) {
                *arts_functions[i].func =
                    SDL_LoadFunction(arts_handle, arts_functions[i].name);
                if (!*arts_functions[i].func) {
                    retval = -1;
                    UnloadARTSLibrary();
                    break;
                }
            }
        }
    }

    return retval;
}

#else

static void
UnloadARTSLibrary()
{
    return;
}

static int
LoadARTSLibrary(void)
{
    return 0;
}

#endif

static void
ARTS_WaitDevice(_THIS)
{
    Sint32 ticks;

    {
        static int cnt = 0;

        if (SDLAUDIOHIDDEN->parent && (((++cnt) % 10) == 0)) {
            if (kill(SDLAUDIOHIDDEN->parent, 0) < 0 && errno == ESRCH) {
                this->enabled = 0;
            }
        }
    }

    ticks =
        ((Sint32) (SDLAUDIOHIDDEN->next_frame - SDL_GetTicks())) - FUDGE_TICKS;
    if (ticks > 0) {
        SDL_Delay(ticks);
    }
}

static void
ARTS_PlayDevice(_THIS)
{

    int written = SDL_NAME(arts_write) (SDLAUDIOHIDDEN->stream,
                                        SDLAUDIOHIDDEN->mixbuf,
                                        SDLAUDIOHIDDEN->mixlen);

    if (SDLAUDIOHIDDEN->frame_ticks) {
        SDLAUDIOHIDDEN->next_frame += SDLAUDIOHIDDEN->frame_ticks;
    }

    if (written < 0) {
        this->enabled = 0;
    }
#ifdef DEBUG_AUDIO
    fprintf(stderr, "Wrote %d bytes of audio data\n", written);
#endif
}

static void
ARTS_WaitDone(_THIS)
{

}

static Uint8 *
ARTS_GetDeviceBuf(_THIS)
{
    return (SDLAUDIOHIDDEN->mixbuf);
}

static void
ARTS_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        SDL_FreeAudioMem(SDLAUDIOHIDDEN->mixbuf);
        SDLAUDIOHIDDEN->mixbuf = NULL;
        if (SDLAUDIOHIDDEN->stream) {
            SDL_NAME(arts_close_stream) (SDLAUDIOHIDDEN->stream);
            SDLAUDIOHIDDEN->stream = 0;
        }
        SDL_NAME(arts_free) ();
        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
ARTS_Suspend(void)
{
    const Uint32 abortms = SDL_GetTicks() + 3000;
    while ( (!SDL_NAME(arts_suspended)()) && !SDL_TICKS_PASSED(SDL_GetTicks(), abortms) ) {
        if ( SDL_NAME(arts_suspend)() ) {
            break;
        }
    }
    return SDL_NAME(arts_suspended)();
}

static int
ARTS_OpenDevice(_THIS, const char *devname, int iscapture)
{
    int rc = 0;
    int bits = 0, frag_spec = 0;
    SDL_AudioFormat test_format = 0, format = 0;

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    for (test_format = SDL_FirstAudioFormat(this->spec.format);
         !format && test_format;) {
#ifdef DEBUG_AUDIO
        fprintf(stderr, "Trying format 0x%4.4x\n", test_format);
#endif
        switch (test_format) {
        case AUDIO_U8:
            bits = 8;
            format = 1;
            break;
        case AUDIO_S16LSB:
            bits = 16;
            format = 1;
            break;
        default:
            format = 0;
            break;
        }
        if (!format) {
            test_format = SDL_NextAudioFormat();
        }
    }
    if (format == 0) {
        ARTS_CloseDevice(this);
        return SDL_SetError("Couldn't find any hardware audio formats");
    }
    this->spec.format = test_format;

    if ((rc = SDL_NAME(arts_init) ()) != 0) {
        ARTS_CloseDevice(this);
        return SDL_SetError("Unable to initialize ARTS: %s",
                            SDL_NAME(arts_error_text) (rc));
    }

    if (!ARTS_Suspend()) {
        ARTS_CloseDevice(this);
        return SDL_SetError("ARTS can not open audio device");
    }

    SDLAUDIOHIDDEN->stream = SDL_NAME(arts_play_stream) (this->spec.freq,
                                                       bits,
                                                       this->spec.channels,
                                                       "SDL");

    SDL_NAME(arts_write) (SDLAUDIOHIDDEN->stream, "", 0);

    SDL_CalculateAudioSpec(&this->spec);

    for (frag_spec = 0; (0x01 << frag_spec) < this->spec.size; ++frag_spec);
    if ((0x01 << frag_spec) != this->spec.size) {
        ARTS_CloseDevice(this);
        return SDL_SetError("Fragment size must be a power of two");
    }
    frag_spec |= 0x00020000;

#ifdef ARTS_P_PACKET_SETTINGS
    SDL_NAME(arts_stream_set) (SDLAUDIOHIDDEN->stream,
                               ARTS_P_PACKET_SETTINGS, frag_spec);
#else
    SDL_NAME(arts_stream_set) (SDLAUDIOHIDDEN->stream, ARTS_P_PACKET_SIZE,
                               frag_spec & 0xffff);
    SDL_NAME(arts_stream_set) (SDLAUDIOHIDDEN->stream, ARTS_P_PACKET_COUNT,
                               frag_spec >> 16);
#endif
    this->spec.size = SDL_NAME(arts_stream_get) (SDLAUDIOHIDDEN->stream,
                                                 ARTS_P_PACKET_SIZE);

    SDLAUDIOHIDDEN->mixlen = this->spec.size;
    SDLAUDIOHIDDEN->mixbuf = (Uint8 *) SDL_AllocAudioMem(SDLAUDIOHIDDEN->mixlen);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        ARTS_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN->mixbuf, this->spec.silence, this->spec.size);

    SDLAUDIOHIDDEN->parent = getpid();

    return 0;
}

static void
ARTS_Deinitialize(void)
{
    UnloadARTSLibrary();
}

static int
ARTS_Init(SDL_AudioDriverImpl * impl)
{
    if (LoadARTSLibrary() < 0) {
        return 0;
    } else {
        if (SDL_NAME(arts_init) () != 0) {
            UnloadARTSLibrary();
            SDL_SetError("ARTS: arts_init failed (no audio server?)");
            return 0;
        }

        if (ARTS_Suspend()) {
            arts_stream_t stream;
            stream = SDL_NAME(arts_play_stream) (44100, 16, 2, "SDL");
            SDL_NAME(arts_write) (stream, "", 0);
            SDL_NAME(arts_close_stream) (stream);
        }

        SDL_NAME(arts_free) ();
    }

    impl->OpenDevice = ARTS_OpenDevice;
    impl->PlayDevice = ARTS_PlayDevice;
    impl->WaitDevice = ARTS_WaitDevice;
    impl->GetDeviceBuf = ARTS_GetDeviceBuf;
    impl->CloseDevice = ARTS_CloseDevice;
    impl->WaitDone = ARTS_WaitDone;
    impl->Deinitialize = ARTS_Deinitialize;
    impl->OnlyHasDefaultOutputDevice = 1;

    return 1;
}

AudioBootStrap ARTS_bootstrap = {
    "arts", "Analog RealTime Synthesizer", ARTS_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_BSD

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/audioio.h>

#define SDL_AllocAudioMem   SDL_malloc
#define SDL_FreeAudioMem    SDL_free
#ifdef USE_BLOCKING_WRITES
#define OPEN_FLAGS_OUTPUT O_WRONLY
#define OPEN_FLAGS_INPUT O_RDONLY
#else
#define OPEN_FLAGS_OUTPUT (O_WRONLY|O_NONBLOCK)
#define OPEN_FLAGS_INPUT (O_RDONLY|O_NONBLOCK)
#endif

void SDL_EnumUnixAudioDevices(int iscapture, int classic,
                              int (*test) (int fd), SDL_AddAudioDevice addfn);
#ifndef _SDL_bsdaudio_h
#define _SDL_bsdaudio_h

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataBSDAUDIO*)this->hidden)
struct SDL_PrivateAudioDataBSDAUDIO
{

    int audio_fd;

    pid_t parent;

    Uint8 *mixbuf;
    int mixlen;

    float frame_ticks;
    float next_frame;
};

#define FUDGE_TICKS 10

#endif

static void
BSDAUDIO_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    SDL_EnumUnixAudioDevices(iscapture, 0, NULL, addfn);
}

static void
BSDAUDIO_Status(_THIS)
{
#ifdef DEBUG_AUDIO

    audio_info_t info;

    if (ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_GETINFO, &info) < 0) {
        fprintf(stderr, "AUDIO_GETINFO failed.\n");
        return;
    }
    fprintf(stderr, "\n"
            "[play/record info]\n"
            "buffer size	:   %d bytes\n"
            "sample rate	:   %i Hz\n"
            "channels	:   %i\n"
            "precision	:   %i-bit\n"
            "encoding	:   0x%x\n"
            "seek		:   %i\n"
            "sample count	:   %i\n"
            "EOF count	:   %i\n"
            "paused		:   %s\n"
            "error occured	:   %s\n"
            "waiting		:   %s\n"
            "active		:   %s\n"
            "",
            info.play.buffer_size,
            info.play.sample_rate,
            info.play.channels,
            info.play.precision,
            info.play.encoding,
            info.play.seek,
            info.play.samples,
            info.play.eof,
            info.play.pause ? "yes" : "no",
            info.play.error ? "yes" : "no",
            info.play.waiting ? "yes" : "no",
            info.play.active ? "yes" : "no");

    fprintf(stderr, "\n"
            "[audio info]\n"
            "monitor_gain	:   %i\n"
            "hw block size	:   %d bytes\n"
            "hi watermark	:   %i\n"
            "lo watermark	:   %i\n"
            "audio mode	:   %s\n"
            "",
            info.monitor_gain,
            info.blocksize,
            info.hiwat, info.lowat,
            (info.mode == AUMODE_PLAY) ? "PLAY"
            : (info.mode = AUMODE_RECORD) ? "RECORD"
            : (info.mode == AUMODE_PLAY_ALL ? "PLAY_ALL" : "?"));

#endif
}

static void
BSDAUDIO_WaitDevice(_THIS)
{
#ifndef USE_BLOCKING_WRITES

    if (SDLAUDIOHIDDEN->frame_ticks) {

        Sint32 ticks;

        ticks = ((Sint32) (SDLAUDIOHIDDEN->next_frame - SDL_GetTicks())) - FUDGE_TICKS;
        if (ticks > 0) {
            SDL_Delay(ticks);
        }
    } else {

        fd_set fdset;
        struct timeval timeout;

        FD_ZERO(&fdset);
        FD_SET(SDLAUDIOHIDDEN->audio_fd, &fdset);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
#ifdef DEBUG_AUDIO
        fprintf(stderr, "Waiting for audio to get ready\n");
#endif
        if (select(SDLAUDIOHIDDEN->audio_fd + 1, NULL, &fdset, NULL, &timeout)
            <= 0) {
            const char *message =
                "Audio timeout - buggy audio driver? (disabled)";

            fprintf(stderr, "SDL: %s\n", message);
            this->enabled = 0;

            SDLAUDIOHIDDEN->audio_fd = -1;
#ifdef DEBUG_AUDIO
            fprintf(stderr, "Done disabling audio\n");
#endif
        }
#ifdef DEBUG_AUDIO
        fprintf(stderr, "Ready!\n");
#endif
    }
#endif
}

static void
BSDAUDIO_PlayDevice(_THIS)
{
    int written, p = 0;

    do {
        written = write(SDLAUDIOHIDDEN->audio_fd,
                        &SDLAUDIOHIDDEN->mixbuf[p], SDLAUDIOHIDDEN->mixlen - p);

        if (written > 0)
            p += written;
        if (written == -1 && errno != 0 && errno != EAGAIN && errno != EINTR) {

            perror("audio");
            break;
        }

        if (p < written
            || ((written < 0) && ((errno == 0) || (errno == EAGAIN)))) {
            SDL_Delay(1);
        }
    } while (p < written);

    if (SDLAUDIOHIDDEN->frame_ticks) {
        SDLAUDIOHIDDEN->next_frame += SDLAUDIOHIDDEN->frame_ticks;
    }

    if (written < 0) {
        this->enabled = 0;
    }
#ifdef DEBUG_AUDIO
    fprintf(stderr, "Wrote %d bytes of audio data\n", written);
#endif
}

static Uint8 *
BSDAUDIO_GetDeviceBuf(_THIS)
{
    return (SDLAUDIOHIDDEN->mixbuf);
}

static void
BSDAUDIO_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        SDL_FreeAudioMem(SDLAUDIOHIDDEN->mixbuf);
        SDLAUDIOHIDDEN->mixbuf = NULL;
        if (SDLAUDIOHIDDEN->audio_fd >= 0) {
            close(SDLAUDIOHIDDEN->audio_fd);
            SDLAUDIOHIDDEN->audio_fd = -1;
        }
        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
BSDAUDIO_OpenDevice(_THIS, const char *devname, int iscapture)
{
    const int flags = ((iscapture) ? OPEN_FLAGS_INPUT : OPEN_FLAGS_OUTPUT);
    SDL_AudioFormat format = 0;
    audio_info_t info;

    if (devname == NULL) {
        devname = SDL_GetAudioDeviceName(0, iscapture);
        if (devname == NULL) {
            return SDL_SetError("No such audio device");
        }
    }

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    SDLAUDIOHIDDEN->audio_fd = open(devname, flags, 0);
    if (SDLAUDIOHIDDEN->audio_fd < 0) {
        return SDL_SetError("Couldn't open %s: %s", devname, strerror(errno));
    }

    AUDIO_INITINFO(&info);

    SDL_CalculateAudioSpec(&this->spec);

    info.mode = AUMODE_PLAY;
    if (ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_SETINFO, &info) < 0) {
        BSDAUDIO_CloseDevice(this);
        return SDL_SetError("Couldn't put device into play mode");
    }

    AUDIO_INITINFO(&info);
    for (format = SDL_FirstAudioFormat(this->spec.format);
         format; format = SDL_NextAudioFormat()) {
        switch (format) {
        case AUDIO_U8:
            info.play.encoding = AUDIO_ENCODING_ULINEAR;
            info.play.precision = 8;
            break;
        case AUDIO_S8:
            info.play.encoding = AUDIO_ENCODING_SLINEAR;
            info.play.precision = 8;
            break;
        case AUDIO_S16LSB:
            info.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
            info.play.precision = 16;
            break;
        case AUDIO_S16MSB:
            info.play.encoding = AUDIO_ENCODING_SLINEAR_BE;
            info.play.precision = 16;
            break;
        case AUDIO_U16LSB:
            info.play.encoding = AUDIO_ENCODING_ULINEAR_LE;
            info.play.precision = 16;
            break;
        case AUDIO_U16MSB:
            info.play.encoding = AUDIO_ENCODING_ULINEAR_BE;
            info.play.precision = 16;
            break;
        default:
            continue;
        }

        if (ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_SETINFO, &info) == 0) {
            break;
        }
    }

    if (!format) {
        BSDAUDIO_CloseDevice(this);
        return SDL_SetError("No supported encoding for 0x%x", this->spec.format);
    }

    this->spec.format = format;

    AUDIO_INITINFO(&info);
    info.play.channels = this->spec.channels;
    if (ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_SETINFO, &info) == -1) {
        this->spec.channels = 1;
    }
    AUDIO_INITINFO(&info);
    info.play.sample_rate = this->spec.freq;
    info.blocksize = this->spec.size;
    info.hiwat = 5;
    info.lowat = 3;
    (void) ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_SETINFO, &info);
    (void) ioctl(SDLAUDIOHIDDEN->audio_fd, AUDIO_GETINFO, &info);
    this->spec.freq = info.play.sample_rate;

    SDLAUDIOHIDDEN->mixlen = this->spec.size;
    SDLAUDIOHIDDEN->mixbuf = (Uint8 *) SDL_AllocAudioMem(SDLAUDIOHIDDEN->mixlen);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        BSDAUDIO_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN->mixbuf, this->spec.silence, this->spec.size);

    BSDAUDIO_Status(this);

    return 0;
}

static int
BSDAUDIO_Init(SDL_AudioDriverImpl * impl)
{

    impl->DetectDevices = BSDAUDIO_DetectDevices;
    impl->OpenDevice = BSDAUDIO_OpenDevice;
    impl->PlayDevice = BSDAUDIO_PlayDevice;
    impl->WaitDevice = BSDAUDIO_WaitDevice;
    impl->GetDeviceBuf = BSDAUDIO_GetDeviceBuf;
    impl->CloseDevice = BSDAUDIO_CloseDevice;

    return 1;
}

AudioBootStrap BSD_AUDIO_bootstrap = {
    "bsd", "BSD audio", BSDAUDIO_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_COREAUDIO

#ifndef _SDL_coreaudio_h
#define _SDL_coreaudio_h

#if !defined(__IPHONEOS__)
#define MACOSX_COREAUDIO 1
#endif

#if MACOSX_COREAUDIO
#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h>
#else
#include <AudioToolbox/AudioToolbox.h>
#endif

#include <AudioUnit/AudioUnit.h>

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataCOREAUDIO*)this->hidden)
struct SDL_PrivateAudioDataCOREAUDIO
{
    AudioUnit audioUnit;
    int audioUnitOpened;
    void *buffer;
    UInt32 bufferOffset;
    UInt32 bufferSize;
#if MACOSX_COREAUDIO
    AudioDeviceID deviceID;
#endif
};

#endif

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#define DEBUG_COREAUDIO 0

static void COREAUDIO_CloseDevice(_THIS);

#define CHECK_RESULT(msg) \
    if (result != noErr) { \
        COREAUDIO_CloseDevice(this); \
        SDL_SetError("CoreAudio error (%s): %d", msg, (int) result); \
        return 0; \
    }

#if MACOSX_COREAUDIO
typedef void (*addDevFn)(const char *name, AudioDeviceID devId, void *data);

static void
addToDevList(const char *name, AudioDeviceID devId, void *data)
{
    SDL_AddAudioDevice addfn = (SDL_AddAudioDevice) data;
    addfn(name);
}

typedef struct
{
    const char *findname;
    AudioDeviceID devId;
    int found;
} FindDevIdData;

static void
findDevId(const char *name, AudioDeviceID devId, void *_data)
{
    FindDevIdData *data = (FindDevIdData *) _data;
    if (!data->found) {
        if (SDL_strcmp(name, data->findname) == 0) {
            data->found = 1;
            data->devId = devId;
        }
    }
}

static void
build_device_list(int iscapture, addDevFn addfn, void *addfndata)
{
    OSStatus result = noErr;
    UInt32 size = 0;
    AudioDeviceID *devs = NULL;
    UInt32 i = 0;
    UInt32 max = 0;

    AudioObjectPropertyAddress addr = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };

    result = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &addr,
                                            0, NULL, &size);
    if (result != kAudioHardwareNoError)
        return;

    devs = (AudioDeviceID *) alloca(size);
    if (devs == NULL)
        return;

    result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr,
                                        0, NULL, &size, devs);
    if (result != kAudioHardwareNoError)
        return;

    max = size / sizeof (AudioDeviceID);
    for (i = 0; i < max; i++) {
        CFStringRef cfstr = NULL;
        char *ptr = NULL;
        AudioDeviceID dev = devs[i];
        AudioBufferList *buflist = NULL;
        int usable = 0;
        CFIndex len = 0;

        addr.mScope = iscapture ? kAudioDevicePropertyScopeInput :
                        kAudioDevicePropertyScopeOutput;
        addr.mSelector = kAudioDevicePropertyStreamConfiguration;

        result = AudioObjectGetPropertyDataSize(dev, &addr, 0, NULL, &size);
        if (result != noErr)
            continue;

        buflist = (AudioBufferList *) SDL_malloc(size);
        if (buflist == NULL)
            continue;

        result = AudioObjectGetPropertyData(dev, &addr, 0, NULL,
                                            &size, buflist);

        if (result == noErr) {
            UInt32 j;
            for (j = 0; j < buflist->mNumberBuffers; j++) {
                if (buflist->mBuffers[j].mNumberChannels > 0) {
                    usable = 1;
                    break;
                }
            }
        }

        SDL_free(buflist);

        if (!usable)
            continue;

        addr.mSelector = kAudioObjectPropertyName;
        size = sizeof (CFStringRef);
        result = AudioObjectGetPropertyData(dev, &addr, 0, NULL, &size, &cfstr);
        if (result != kAudioHardwareNoError)
            continue;

        len = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfstr),
                                                kCFStringEncodingUTF8);

        ptr = (char *) SDL_malloc(len + 1);
        usable = ((ptr != NULL) &&
                  (CFStringGetCString
                   (cfstr, ptr, len + 1, kCFStringEncodingUTF8)));

        CFRelease(cfstr);

        if (usable) {
            len = strlen(ptr);

            while ((len > 0) && (ptr[len - 1] == ' ')) {
                len--;
            }
            usable = (len > 0);
        }

        if (usable) {
            ptr[len] = '\0';

#if DEBUG_COREAUDIO
            printf("COREAUDIO: Found %s device #%d: '%s' (devid %d)\n",
                   ((iscapture) ? "capture" : "output"),
                   (int) *devCount, ptr, (int) dev);
#endif
            addfn(ptr, dev, addfndata);
        }
        SDL_free(ptr);
    }
}

static void
COREAUDIO_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    build_device_list(iscapture, addToDevList, addfn);
}

static int
find_device_by_name(_THIS, const char *devname, int iscapture)
{
    AudioDeviceID devid = 0;
    OSStatus result = noErr;
    UInt32 size = 0;
    UInt32 alive = 0;
    pid_t pid = 0;

    AudioObjectPropertyAddress addr = {
        0,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };

    if (devname == NULL) {
        size = sizeof (AudioDeviceID);
        addr.mSelector =
            ((iscapture) ? kAudioHardwarePropertyDefaultInputDevice :
            kAudioHardwarePropertyDefaultOutputDevice);
        result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr,
                                            0, NULL, &size, &devid);
        CHECK_RESULT("AudioHardwareGetProperty (default device)");
    } else {
        FindDevIdData data;
        SDL_zero(data);
        data.findname = devname;
        build_device_list(iscapture, findDevId, &data);
        if (!data.found) {
            SDL_SetError("CoreAudio: No such audio device.");
            return 0;
        }
        devid = data.devId;
    }

    addr.mSelector = kAudioDevicePropertyDeviceIsAlive;
    addr.mScope = iscapture ? kAudioDevicePropertyScopeInput :
                    kAudioDevicePropertyScopeOutput;

    size = sizeof (alive);
    result = AudioObjectGetPropertyData(devid, &addr, 0, NULL, &size, &alive);
    CHECK_RESULT
        ("AudioDeviceGetProperty (kAudioDevicePropertyDeviceIsAlive)");

    if (!alive) {
        SDL_SetError("CoreAudio: requested device exists, but isn't alive.");
        return 0;
    }

    addr.mSelector = kAudioDevicePropertyHogMode;
    size = sizeof (pid);
    result = AudioObjectGetPropertyData(devid, &addr, 0, NULL, &size, &pid);

    if ((result == noErr) && (pid != -1)) {
        SDL_SetError("CoreAudio: requested device is being hogged.");
        return 0;
    }

    SDLAUDIOHIDDEN->deviceID = devid;
    return 1;
}
#endif

static OSStatus
outputCallback(void *inRefCon,
               AudioUnitRenderActionFlags * ioActionFlags,
               const AudioTimeStamp * inTimeStamp,
               UInt32 inBusNumber, UInt32 inNumberFrames,
               AudioBufferList * ioData)
{
    SDL_AudioDevice *this = (SDL_AudioDevice *) inRefCon;
    AudioBuffer *abuf;
    UInt32 remaining, len;
    void *ptr;
    UInt32 i;

    if (!this->enabled || this->paused) {
        for (i = 0; i < ioData->mNumberBuffers; i++) {
            abuf = &ioData->mBuffers[i];
            SDL_memset(abuf->mData, this->spec.silence, abuf->mDataByteSize);
        }
        return 0;
    }

    for (i = 0; i < ioData->mNumberBuffers; i++) {
        abuf = &ioData->mBuffers[i];
        remaining = abuf->mDataByteSize;
        ptr = abuf->mData;
        while (remaining > 0) {
            if (SDLAUDIOHIDDEN->bufferOffset >= SDLAUDIOHIDDEN->bufferSize) {

                SDL_LockMutex(this->mixer_lock);
                (*this->spec.callback)(this->spec.userdata,
                            SDLAUDIOHIDDEN->buffer, SDLAUDIOHIDDEN->bufferSize);
                SDL_UnlockMutex(this->mixer_lock);
                SDLAUDIOHIDDEN->bufferOffset = 0;
            }

            len = SDLAUDIOHIDDEN->bufferSize - SDLAUDIOHIDDEN->bufferOffset;
            if (len > remaining)
                len = remaining;
            SDL_memcpy(ptr, (char *)SDLAUDIOHIDDEN->buffer +
                       SDLAUDIOHIDDEN->bufferOffset, len);
            ptr = (char *)ptr + len;
            remaining -= len;
            SDLAUDIOHIDDEN->bufferOffset += len;
        }
    }

    return 0;
}

static OSStatus
inputCallback(void *inRefCon,
              AudioUnitRenderActionFlags * ioActionFlags,
              const AudioTimeStamp * inTimeStamp,
              UInt32 inBusNumber, UInt32 inNumberFrames,
              AudioBufferList * ioData)
{

    return noErr;
}

static void
COREAUDIO_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        if (SDLAUDIOHIDDEN->audioUnitOpened) {
            OSStatus result = noErr;
            AURenderCallbackStruct callback;
            const AudioUnitElement output_bus = 0;
            const AudioUnitElement input_bus = 1;
            const int iscapture = this->iscapture;
            const AudioUnitElement bus =
                ((iscapture) ? input_bus : output_bus);
            const AudioUnitScope scope =
                ((iscapture) ? kAudioUnitScope_Output :
                 kAudioUnitScope_Input);

            result = AudioOutputUnitStop(SDLAUDIOHIDDEN->audioUnit);

            SDL_memset(&callback, 0, sizeof(AURenderCallbackStruct));
            result = AudioUnitSetProperty(SDLAUDIOHIDDEN->audioUnit,
                                          kAudioUnitProperty_SetRenderCallback,
                                          scope, bus, &callback,
                                          sizeof(callback));

            #if MACOSX_COREAUDIO
            CloseComponent(SDLAUDIOHIDDEN->audioUnit);
            #else
            AudioComponentInstanceDispose(SDLAUDIOHIDDEN->audioUnit);
            #endif

            SDLAUDIOHIDDEN->audioUnitOpened = 0;
        }
        SDL_free(SDLAUDIOHIDDEN->buffer);
        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
prepare_audiounit(_THIS, const char *devname, int iscapture,
                  const AudioStreamBasicDescription * strdesc)
{
    OSStatus result = noErr;
    AURenderCallbackStruct callback;
#if MACOSX_COREAUDIO
    ComponentDescription desc;
    Component comp = NULL;
#else
    AudioComponentDescription desc;
    AudioComponent comp = NULL;
#endif
    const AudioUnitElement output_bus = 0;
    const AudioUnitElement input_bus = 1;
    const AudioUnitElement bus = ((iscapture) ? input_bus : output_bus);
    const AudioUnitScope scope = ((iscapture) ? kAudioUnitScope_Output :
                                  kAudioUnitScope_Input);

#if MACOSX_COREAUDIO
    if (!find_device_by_name(this, devname, iscapture)) {
        SDL_SetError("Couldn't find requested CoreAudio device");
        return 0;
    }
#endif

    SDL_zero(desc);
    desc.componentType = kAudioUnitType_Output;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;

#if MACOSX_COREAUDIO
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
    comp = FindNextComponent(NULL, &desc);
#else
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    comp = AudioComponentFindNext(NULL, &desc);
#endif

    if (comp == NULL) {
        SDL_SetError("Couldn't find requested CoreAudio component");
        return 0;
    }

#if MACOSX_COREAUDIO
    result = OpenAComponent(comp, &SDLAUDIOHIDDEN->audioUnit);
    CHECK_RESULT("OpenAComponent");
#else

    result = AudioComponentInstanceNew(comp, &SDLAUDIOHIDDEN->audioUnit);
    CHECK_RESULT("AudioComponentInstanceNew");
#endif

    SDLAUDIOHIDDEN->audioUnitOpened = 1;

#if MACOSX_COREAUDIO
    result = AudioUnitSetProperty(SDLAUDIOHIDDEN->audioUnit,
                                  kAudioOutputUnitProperty_CurrentDevice,
                                  kAudioUnitScope_Global, 0,
                                  &SDLAUDIOHIDDEN->deviceID,
                                  sizeof(AudioDeviceID));
    CHECK_RESULT
        ("AudioUnitSetProperty (kAudioOutputUnitProperty_CurrentDevice)");
#endif

    result = AudioUnitSetProperty(SDLAUDIOHIDDEN->audioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  scope, bus, strdesc, sizeof(*strdesc));
    CHECK_RESULT("AudioUnitSetProperty (kAudioUnitProperty_StreamFormat)");

    SDL_memset(&callback, 0, sizeof(AURenderCallbackStruct));
    callback.inputProc = ((iscapture) ? inputCallback : outputCallback);
    callback.inputProcRefCon = this;
    result = AudioUnitSetProperty(SDLAUDIOHIDDEN->audioUnit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  scope, bus, &callback, sizeof(callback));
    CHECK_RESULT
        ("AudioUnitSetProperty (kAudioUnitProperty_SetRenderCallback)");

    SDL_CalculateAudioSpec(&this->spec);

    SDLAUDIOHIDDEN->bufferOffset = SDLAUDIOHIDDEN->bufferSize = this->spec.size;
    SDLAUDIOHIDDEN->buffer = SDL_malloc(SDLAUDIOHIDDEN->bufferSize);

    result = AudioUnitInitialize(SDLAUDIOHIDDEN->audioUnit);
    CHECK_RESULT("AudioUnitInitialize");

    result = AudioOutputUnitStart(SDLAUDIOHIDDEN->audioUnit);
    CHECK_RESULT("AudioOutputUnitStart");

    return 1;
}

static int
COREAUDIO_OpenDevice(_THIS, const char *devname, int iscapture)
{
    AudioStreamBasicDescription strdesc;
    SDL_AudioFormat test_format = SDL_FirstAudioFormat(this->spec.format);
    int valid_datatype = 0;

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    SDL_memset(&strdesc, '\0', sizeof(AudioStreamBasicDescription));
    strdesc.mFormatID = kAudioFormatLinearPCM;
    strdesc.mFormatFlags = kLinearPCMFormatFlagIsPacked;
    strdesc.mChannelsPerFrame = this->spec.channels;
    strdesc.mSampleRate = this->spec.freq;
    strdesc.mFramesPerPacket = 1;

    while ((!valid_datatype) && (test_format)) {
        this->spec.format = test_format;

        switch (test_format) {
        case AUDIO_U8:
        case AUDIO_S8:
        case AUDIO_U16LSB:
        case AUDIO_S16LSB:
        case AUDIO_U16MSB:
        case AUDIO_S16MSB:
        case AUDIO_S32LSB:
        case AUDIO_S32MSB:
        case AUDIO_F32LSB:
        case AUDIO_F32MSB:
            valid_datatype = 1;
            strdesc.mBitsPerChannel = SDL_AUDIO_BITSIZE(this->spec.format);
            if (SDL_AUDIO_ISBIGENDIAN(this->spec.format))
                strdesc.mFormatFlags |= kLinearPCMFormatFlagIsBigEndian;

            if (SDL_AUDIO_ISFLOAT(this->spec.format))
                strdesc.mFormatFlags |= kLinearPCMFormatFlagIsFloat;
            else if (SDL_AUDIO_ISSIGNED(this->spec.format))
                strdesc.mFormatFlags |= kLinearPCMFormatFlagIsSignedInteger;
            break;
        }
    }

    if (!valid_datatype) {
        COREAUDIO_CloseDevice(this);
        return SDL_SetError("Unsupported audio format");
    }

    strdesc.mBytesPerFrame =
        strdesc.mBitsPerChannel * strdesc.mChannelsPerFrame / 8;
    strdesc.mBytesPerPacket =
        strdesc.mBytesPerFrame * strdesc.mFramesPerPacket;

    if (!prepare_audiounit(this, devname, iscapture, &strdesc)) {
        COREAUDIO_CloseDevice(this);
        return -1;
    }

    return 0;
}

static int
COREAUDIO_Init(SDL_AudioDriverImpl * impl)
{

    impl->OpenDevice = COREAUDIO_OpenDevice;
    impl->CloseDevice = COREAUDIO_CloseDevice;

#if MACOSX_COREAUDIO
    impl->DetectDevices = COREAUDIO_DetectDevices;
#else
    impl->OnlyHasDefaultOutputDevice = 1;

    AudioSessionInitialize(NULL, NULL, NULL, nil);
    UInt32 category = kAudioSessionCategory_AmbientSound;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(UInt32), &category);
#endif

    impl->ProvidesOwnCallbackThread = 1;

    return 1;
}

AudioBootStrap COREAUDIO_bootstrap = {
    "coreaudio", "CoreAudio", COREAUDIO_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_DSOUND

#ifndef _SDL_directsound_h
#define _SDL_directsound_h

#ifndef _directx_h
#define _directx_h

#include <mmsystem.h>
#ifndef WIN32
#define WIN32
#endif
#undef  WINNT

#ifndef FAR
#define FAR
#endif

#ifndef MAKE_HRESULT
#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT)(((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))))
#endif

#ifndef S_OK
#define S_OK        (HRESULT)0x00000000L
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(x)    ((HRESULT)(x) >= 0)
#endif
#ifndef FAILED
#define FAILED(x)   ((HRESULT)(x)<0)
#endif

#ifndef E_FAIL
#define E_FAIL      (HRESULT)0x80000008L
#endif
#ifndef E_NOINTERFACE
#define E_NOINTERFACE   (HRESULT)0x80004002L
#endif
#ifndef E_OUTOFMEMORY
#define E_OUTOFMEMORY   (HRESULT)0x8007000EL
#endif
#ifndef E_INVALIDARG
#define E_INVALIDARG    (HRESULT)0x80070057L
#endif
#ifndef E_NOTIMPL
#define E_NOTIMPL   (HRESULT)0x80004001L
#endif
#ifndef REGDB_E_CLASSNOTREG
#define REGDB_E_CLASSNOTREG (HRESULT)0x80040154L
#endif

#ifndef SEVERITY_ERROR
#define SEVERITY_ERROR  1
#endif

#ifndef FACILITY_WIN32
#define FACILITY_WIN32  7
#endif

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
#endif

#define DIRECTDRAW_VERSION  0x0700
#define DIRECTSOUND_VERSION 0x0800
#define DIRECTINPUT_VERSION 0x0500

#include <ddraw.h>
#define COM_NO_WINDOWS_H
#include <objbase.h>
#include <float.h>

#ifndef DIRECTSOUND_VERSION
#define DIRECTSOUND_VERSION 0x0800
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DSOUND_INCLUDED__
#define __DSOUND_INCLUDED__

#ifndef DX_SHARED_DEFINES

typedef float D3DVALUE, *LPD3DVALUE;

#ifndef D3DCOLOR_DEFINED
typedef DWORD D3DCOLOR;
#define D3DCOLOR_DEFINED
#endif

#ifndef LPD3DCOLOR_DEFINED
typedef DWORD *LPD3DCOLOR;
#define LPD3DCOLOR_DEFINED
#endif

#ifndef D3DVECTOR_DEFINED
typedef struct _D3DVECTOR {
    float x;
    float y;
    float z;
} D3DVECTOR;
#define D3DVECTOR_DEFINED
#endif

#ifndef LPD3DVECTOR_DEFINED
typedef D3DVECTOR *LPD3DVECTOR;
#define LPD3DVECTOR_DEFINED
#endif

#define DX_SHARED_DEFINES
#endif

#define _FACDS  0x878
#define MAKE_DSHRESULT(code)  MAKE_HRESULT(1, _FACDS, code)

DEFINE_GUID(CLSID_DirectSound, 0x47d4d946, 0x62e8, 0x11cf, 0x93, 0xbc, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0);

DEFINE_GUID(CLSID_DirectSound8, 0x3901cc3f, 0x84b5, 0x4fa4, 0xba, 0x35, 0xaa, 0x81, 0x72, 0xb8, 0xa0, 0x9b);

DEFINE_GUID(CLSID_DirectSoundCapture, 0xb0210780, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16);

DEFINE_GUID(CLSID_DirectSoundCapture8, 0xe4bcac13, 0x7f99, 0x4908, 0x9a, 0x8e, 0x74, 0xe3, 0xbf, 0x24, 0xb6, 0xe1);

DEFINE_GUID(CLSID_DirectSoundFullDuplex, 0xfea4300c, 0x7959, 0x4147, 0xb2, 0x6a, 0x23, 0x77, 0xb9, 0xe7, 0xa9, 0x1d);

DEFINE_GUID(DSDEVID_DefaultPlayback, 0xdef00000, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

DEFINE_GUID(DSDEVID_DefaultCapture, 0xdef00001, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

DEFINE_GUID(DSDEVID_DefaultVoicePlayback, 0xdef00002, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

DEFINE_GUID(DSDEVID_DefaultVoiceCapture, 0xdef00003, 0x9c6d, 0x47ed, 0xaa, 0xf1, 0x4d, 0xda, 0x8f, 0x2b, 0x5c, 0x03);

#ifdef __cplusplus
struct IDirectSound;
struct IDirectSoundBuffer;
struct IDirectSound3DListener;
struct IDirectSound3DBuffer;
struct IDirectSoundCapture;
struct IDirectSoundCaptureBuffer;
struct IDirectSoundNotify;
#endif

#if DIRECTSOUND_VERSION >= 0x0800

#ifdef __cplusplus
struct IDirectSound8;
struct IDirectSoundBuffer8;
struct IDirectSoundCaptureBuffer8;
struct IDirectSoundFXGargle;
struct IDirectSoundFXChorus;
struct IDirectSoundFXFlanger;
struct IDirectSoundFXEcho;
struct IDirectSoundFXDistortion;
struct IDirectSoundFXCompressor;
struct IDirectSoundFXParamEq;
struct IDirectSoundFXWavesReverb;
struct IDirectSoundFXI3DL2Reverb;
struct IDirectSoundCaptureFXAec;
struct IDirectSoundCaptureFXNoiseSuppress;
struct IDirectSoundFullDuplex;
#endif

#define IDirectSoundCapture8            IDirectSoundCapture
#define IDirectSound3DListener8         IDirectSound3DListener
#define IDirectSound3DBuffer8           IDirectSound3DBuffer
#define IDirectSoundNotify8             IDirectSoundNotify
#define IDirectSoundFXGargle8           IDirectSoundFXGargle
#define IDirectSoundFXChorus8           IDirectSoundFXChorus
#define IDirectSoundFXFlanger8          IDirectSoundFXFlanger
#define IDirectSoundFXEcho8             IDirectSoundFXEcho
#define IDirectSoundFXDistortion8       IDirectSoundFXDistortion
#define IDirectSoundFXCompressor8       IDirectSoundFXCompressor
#define IDirectSoundFXParamEq8          IDirectSoundFXParamEq
#define IDirectSoundFXWavesReverb8      IDirectSoundFXWavesReverb
#define IDirectSoundFXI3DL2Reverb8      IDirectSoundFXI3DL2Reverb
#define IDirectSoundCaptureFXAec8       IDirectSoundCaptureFXAec
#define IDirectSoundCaptureFXNoiseSuppress8 IDirectSoundCaptureFXNoiseSuppress
#define IDirectSoundFullDuplex8         IDirectSoundFullDuplex

#endif

typedef struct IDirectSound                 *LPDIRECTSOUND;
typedef struct IDirectSoundBuffer           *LPDIRECTSOUNDBUFFER;
typedef struct IDirectSound3DListener       *LPDIRECTSOUND3DLISTENER;
typedef struct IDirectSound3DBuffer         *LPDIRECTSOUND3DBUFFER;
typedef struct IDirectSoundCapture          *LPDIRECTSOUNDCAPTURE;
typedef struct IDirectSoundCaptureBuffer    *LPDIRECTSOUNDCAPTUREBUFFER;
typedef struct IDirectSoundNotify           *LPDIRECTSOUNDNOTIFY;

#if DIRECTSOUND_VERSION >= 0x0800
typedef struct IDirectSoundFXGargle         *LPDIRECTSOUNDFXGARGLE;
typedef struct IDirectSoundFXChorus         *LPDIRECTSOUNDFXCHORUS;
typedef struct IDirectSoundFXFlanger        *LPDIRECTSOUNDFXFLANGER;
typedef struct IDirectSoundFXEcho           *LPDIRECTSOUNDFXECHO;
typedef struct IDirectSoundFXDistortion     *LPDIRECTSOUNDFXDISTORTION;
typedef struct IDirectSoundFXCompressor     *LPDIRECTSOUNDFXCOMPRESSOR;
typedef struct IDirectSoundFXParamEq        *LPDIRECTSOUNDFXPARAMEQ;
typedef struct IDirectSoundFXWavesReverb    *LPDIRECTSOUNDFXWAVESREVERB;
typedef struct IDirectSoundFXI3DL2Reverb    *LPDIRECTSOUNDFXI3DL2REVERB;
typedef struct IDirectSoundCaptureFXAec     *LPDIRECTSOUNDCAPTUREFXAEC;
typedef struct IDirectSoundCaptureFXNoiseSuppress *LPDIRECTSOUNDCAPTUREFXNOISESUPPRESS;
typedef struct IDirectSoundFullDuplex       *LPDIRECTSOUNDFULLDUPLEX;

typedef struct IDirectSound8                *LPDIRECTSOUND8;
typedef struct IDirectSoundBuffer8          *LPDIRECTSOUNDBUFFER8;
typedef struct IDirectSound3DListener8      *LPDIRECTSOUND3DLISTENER8;
typedef struct IDirectSound3DBuffer8        *LPDIRECTSOUND3DBUFFER8;
typedef struct IDirectSoundCapture8         *LPDIRECTSOUNDCAPTURE8;
typedef struct IDirectSoundCaptureBuffer8   *LPDIRECTSOUNDCAPTUREBUFFER8;
typedef struct IDirectSoundNotify8          *LPDIRECTSOUNDNOTIFY8;
typedef struct IDirectSoundFXGargle8        *LPDIRECTSOUNDFXGARGLE8;
typedef struct IDirectSoundFXChorus8        *LPDIRECTSOUNDFXCHORUS8;
typedef struct IDirectSoundFXFlanger8       *LPDIRECTSOUNDFXFLANGER8;
typedef struct IDirectSoundFXEcho8          *LPDIRECTSOUNDFXECHO8;
typedef struct IDirectSoundFXDistortion8    *LPDIRECTSOUNDFXDISTORTION8;
typedef struct IDirectSoundFXCompressor8    *LPDIRECTSOUNDFXCOMPRESSOR8;
typedef struct IDirectSoundFXParamEq8       *LPDIRECTSOUNDFXPARAMEQ8;
typedef struct IDirectSoundFXWavesReverb8   *LPDIRECTSOUNDFXWAVESREVERB8;
typedef struct IDirectSoundFXI3DL2Reverb8   *LPDIRECTSOUNDFXI3DL2REVERB8;
typedef struct IDirectSoundCaptureFXAec8    *LPDIRECTSOUNDCAPTUREFXAEC8;
typedef struct IDirectSoundCaptureFXNoiseSuppress8 *LPDIRECTSOUNDCAPTUREFXNOISESUPPRESS8;
typedef struct IDirectSoundFullDuplex8      *LPDIRECTSOUNDFULLDUPLEX8;

#endif

#if DIRECTSOUND_VERSION >= 0x0800
#define IID_IDirectSoundCapture8            IID_IDirectSoundCapture
#define IID_IDirectSound3DListener8         IID_IDirectSound3DListener
#define IID_IDirectSound3DBuffer8           IID_IDirectSound3DBuffer
#define IID_IDirectSoundNotify8             IID_IDirectSoundNotify
#define IID_IDirectSoundFXGargle8           IID_IDirectSoundFXGargle
#define IID_IDirectSoundFXChorus8           IID_IDirectSoundFXChorus
#define IID_IDirectSoundFXFlanger8          IID_IDirectSoundFXFlanger
#define IID_IDirectSoundFXEcho8             IID_IDirectSoundFXEcho
#define IID_IDirectSoundFXDistortion8       IID_IDirectSoundFXDistortion
#define IID_IDirectSoundFXCompressor8       IID_IDirectSoundFXCompressor
#define IID_IDirectSoundFXParamEq8          IID_IDirectSoundFXParamEq
#define IID_IDirectSoundFXWavesReverb8      IID_IDirectSoundFXWavesReverb
#define IID_IDirectSoundFXI3DL2Reverb8      IID_IDirectSoundFXI3DL2Reverb
#define IID_IDirectSoundCaptureFXAec8       IID_IDirectSoundCaptureFXAec
#define IID_IDirectSoundCaptureFXNoiseSuppress8 IID_IDirectSoundCaptureFXNoiseSuppress
#define IID_IDirectSoundFullDuplex8         IID_IDirectSoundFullDuplex
#endif

#ifndef _LPCWAVEFORMATEX_DEFINED
#define _LPCWAVEFORMATEX_DEFINED
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;
#endif

#ifndef __LPCGUID_DEFINED__
#define __LPCGUID_DEFINED__
typedef const GUID *LPCGUID;
#endif

typedef LPDIRECTSOUND *LPLPDIRECTSOUND;
typedef LPDIRECTSOUNDBUFFER *LPLPDIRECTSOUNDBUFFER;
typedef LPDIRECTSOUND3DLISTENER *LPLPDIRECTSOUND3DLISTENER;
typedef LPDIRECTSOUND3DBUFFER *LPLPDIRECTSOUND3DBUFFER;
typedef LPDIRECTSOUNDCAPTURE *LPLPDIRECTSOUNDCAPTURE;
typedef LPDIRECTSOUNDCAPTUREBUFFER *LPLPDIRECTSOUNDCAPTUREBUFFER;
typedef LPDIRECTSOUNDNOTIFY *LPLPDIRECTSOUNDNOTIFY;

#if DIRECTSOUND_VERSION >= 0x0800
typedef LPDIRECTSOUND8 *LPLPDIRECTSOUND8;
typedef LPDIRECTSOUNDBUFFER8 *LPLPDIRECTSOUNDBUFFER8;
typedef LPDIRECTSOUNDCAPTURE8 *LPLPDIRECTSOUNDCAPTURE8;
typedef LPDIRECTSOUNDCAPTUREBUFFER8 *LPLPDIRECTSOUNDCAPTUREBUFFER8;
#endif

typedef struct _DSCAPS
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwMinSecondarySampleRate;
    DWORD           dwMaxSecondarySampleRate;
    DWORD           dwPrimaryBuffers;
    DWORD           dwMaxHwMixingAllBuffers;
    DWORD           dwMaxHwMixingStaticBuffers;
    DWORD           dwMaxHwMixingStreamingBuffers;
    DWORD           dwFreeHwMixingAllBuffers;
    DWORD           dwFreeHwMixingStaticBuffers;
    DWORD           dwFreeHwMixingStreamingBuffers;
    DWORD           dwMaxHw3DAllBuffers;
    DWORD           dwMaxHw3DStaticBuffers;
    DWORD           dwMaxHw3DStreamingBuffers;
    DWORD           dwFreeHw3DAllBuffers;
    DWORD           dwFreeHw3DStaticBuffers;
    DWORD           dwFreeHw3DStreamingBuffers;
    DWORD           dwTotalHwMemBytes;
    DWORD           dwFreeHwMemBytes;
    DWORD           dwMaxContigFreeHwMemBytes;
    DWORD           dwUnlockTransferRateHwBuffers;
    DWORD           dwPlayCpuOverheadSwBuffers;
    DWORD           dwReserved1;
    DWORD           dwReserved2;
} DSCAPS, *LPDSCAPS;

typedef const DSCAPS *LPCDSCAPS;

typedef struct _DSBCAPS
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwUnlockTransferRate;
    DWORD           dwPlayCpuOverhead;
} DSBCAPS, *LPDSBCAPS;

typedef const DSBCAPS *LPCDSBCAPS;

#if DIRECTSOUND_VERSION >= 0x0800

    typedef struct _DSEFFECTDESC
    {
        DWORD       dwSize;
        DWORD       dwFlags;
        GUID        guidDSFXClass;
        DWORD       dwReserved1;
        DWORD       dwReserved2;
    } DSEFFECTDESC, *LPDSEFFECTDESC;
    typedef const DSEFFECTDESC *LPCDSEFFECTDESC;

    #define DSFX_LOCHARDWARE    0x00000001
    #define DSFX_LOCSOFTWARE    0x00000002

    enum
    {
        DSFXR_PRESENT,
        DSFXR_LOCHARDWARE,
        DSFXR_LOCSOFTWARE,
        DSFXR_UNALLOCATED,
        DSFXR_FAILED,
        DSFXR_UNKNOWN,
        DSFXR_SENDLOOP
    };

    typedef struct _DSCEFFECTDESC
    {
        DWORD       dwSize;
        DWORD       dwFlags;
        GUID        guidDSCFXClass;
        GUID        guidDSCFXInstance;
        DWORD       dwReserved1;
        DWORD       dwReserved2;
    } DSCEFFECTDESC, *LPDSCEFFECTDESC;
    typedef const DSCEFFECTDESC *LPCDSCEFFECTDESC;

    #define DSCFX_LOCHARDWARE   0x00000001
    #define DSCFX_LOCSOFTWARE   0x00000002

    #define DSCFXR_LOCHARDWARE  0x00000010
    #define DSCFXR_LOCSOFTWARE  0x00000020
    #define DSCFXR_UNALLOCATED  0x00000040
    #define DSCFXR_FAILED       0x00000080
    #define DSCFXR_UNKNOWN      0x00000100

#endif

typedef struct _DSBUFFERDESC
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
#if DIRECTSOUND_VERSION >= 0x0700
    GUID            guid3DAlgorithm;
#endif
} DSBUFFERDESC, *LPDSBUFFERDESC;

typedef const DSBUFFERDESC *LPCDSBUFFERDESC;

typedef struct _DSBUFFERDESC1
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
} DSBUFFERDESC1, *LPDSBUFFERDESC1;

typedef const DSBUFFERDESC1 *LPCDSBUFFERDESC1;

typedef struct _DS3DBUFFER
{
    DWORD           dwSize;
    D3DVECTOR       vPosition;
    D3DVECTOR       vVelocity;
    DWORD           dwInsideConeAngle;
    DWORD           dwOutsideConeAngle;
    D3DVECTOR       vConeOrientation;
    LONG            lConeOutsideVolume;
    D3DVALUE        flMinDistance;
    D3DVALUE        flMaxDistance;
    DWORD           dwMode;
} DS3DBUFFER, *LPDS3DBUFFER;

typedef const DS3DBUFFER *LPCDS3DBUFFER;

typedef struct _DS3DLISTENER
{
    DWORD           dwSize;
    D3DVECTOR       vPosition;
    D3DVECTOR       vVelocity;
    D3DVECTOR       vOrientFront;
    D3DVECTOR       vOrientTop;
    D3DVALUE        flDistanceFactor;
    D3DVALUE        flRolloffFactor;
    D3DVALUE        flDopplerFactor;
} DS3DLISTENER, *LPDS3DLISTENER;

typedef const DS3DLISTENER *LPCDS3DLISTENER;

typedef struct _DSCCAPS
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwFormats;
    DWORD           dwChannels;
} DSCCAPS, *LPDSCCAPS;

typedef const DSCCAPS *LPCDSCCAPS;

typedef struct _DSCBUFFERDESC1
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
} DSCBUFFERDESC1, *LPDSCBUFFERDESC1;

typedef struct _DSCBUFFERDESC
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
#if DIRECTSOUND_VERSION >= 0x0800
    DWORD           dwFXCount;
    LPDSCEFFECTDESC lpDSCFXDesc;
#endif
} DSCBUFFERDESC, *LPDSCBUFFERDESC;

typedef const DSCBUFFERDESC *LPCDSCBUFFERDESC;

typedef struct _DSCBCAPS
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
} DSCBCAPS, *LPDSCBCAPS;

typedef const DSCBCAPS *LPCDSCBCAPS;

typedef struct _DSBPOSITIONNOTIFY
{
    DWORD           dwOffset;
    HANDLE          hEventNotify;
} DSBPOSITIONNOTIFY, *LPDSBPOSITIONNOTIFY;

typedef const DSBPOSITIONNOTIFY *LPCDSBPOSITIONNOTIFY;

typedef BOOL (CALLBACK *LPDSENUMCALLBACKA)(LPGUID, LPCSTR, LPCSTR, LPVOID);
typedef BOOL (CALLBACK *LPDSENUMCALLBACKW)(LPGUID, LPCWSTR, LPCWSTR, LPVOID);

extern HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
extern HRESULT WINAPI DirectSoundEnumerateA(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext);
extern HRESULT WINAPI DirectSoundEnumerateW(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext);

extern HRESULT WINAPI DirectSoundCaptureCreate(LPCGUID pcGuidDevice, LPDIRECTSOUNDCAPTURE *ppDSC, LPUNKNOWN pUnkOuter);
extern HRESULT WINAPI DirectSoundCaptureEnumerateA(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext);
extern HRESULT WINAPI DirectSoundCaptureEnumerateW(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext);

#if DIRECTSOUND_VERSION >= 0x0800
extern HRESULT WINAPI DirectSoundCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUND8 *ppDS8, LPUNKNOWN pUnkOuter);
extern HRESULT WINAPI DirectSoundCaptureCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUNDCAPTURE8 *ppDSC8, LPUNKNOWN pUnkOuter);
extern HRESULT WINAPI DirectSoundFullDuplexCreate(LPCGUID pcGuidCaptureDevice, LPCGUID pcGuidRenderDevice,
        LPCDSCBUFFERDESC pcDSCBufferDesc, LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd,
        DWORD dwLevel, LPDIRECTSOUNDFULLDUPLEX* ppDSFD, LPDIRECTSOUNDCAPTUREBUFFER8 *ppDSCBuffer8,
        LPDIRECTSOUNDBUFFER8 *ppDSBuffer8, LPUNKNOWN pUnkOuter);
#define DirectSoundFullDuplexCreate8 DirectSoundFullDuplexCreate

extern HRESULT WINAPI GetDeviceID(LPCGUID pGuidSrc, LPGUID pGuidDest);
#endif

#ifdef UNICODE
#define LPDSENUMCALLBACK            LPDSENUMCALLBACKW
#define DirectSoundEnumerate        DirectSoundEnumerateW
#define DirectSoundCaptureEnumerate DirectSoundCaptureEnumerateW
#else
#define LPDSENUMCALLBACK            LPDSENUMCALLBACKA
#define DirectSoundEnumerate        DirectSoundEnumerateA
#define DirectSoundCaptureEnumerate DirectSoundCaptureEnumerateA
#endif

#if !defined(__cplusplus) || defined(CINTERFACE)
#ifndef IUnknown_QueryInterface
#define IUnknown_QueryInterface(p,a,b)  (p)->lpVtbl->QueryInterface(p,a,b)
#endif
#ifndef IUnknown_AddRef
#define IUnknown_AddRef(p)              (p)->lpVtbl->AddRef(p)
#endif
#ifndef IUnknown_Release
#define IUnknown_Release(p)             (p)->lpVtbl->Release(p)
#endif
#else
#ifndef IUnknown_QueryInterface
#define IUnknown_QueryInterface(p,a,b)  (p)->QueryInterface(a,b)
#endif
#ifndef IUnknown_AddRef
#define IUnknown_AddRef(p)              (p)->AddRef()
#endif
#ifndef IUnknown_Release
#define IUnknown_Release(p)             (p)->Release()
#endif
#endif

#ifndef __IReferenceClock_INTERFACE_DEFINED__
#define __IReferenceClock_INTERFACE_DEFINED__

typedef LONGLONG REFERENCE_TIME;
typedef REFERENCE_TIME *LPREFERENCE_TIME;

DEFINE_GUID(IID_IReferenceClock, 0x56a86897, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);

#undef INTERFACE
#define INTERFACE IReferenceClock

DECLARE_INTERFACE_(IReferenceClock, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetTime)              (THIS_ REFERENCE_TIME *pTime) PURE;
    STDMETHOD(AdviseTime)           (THIS_ REFERENCE_TIME rtBaseTime, REFERENCE_TIME rtStreamTime,
                                           HANDLE hEvent, LPDWORD pdwAdviseCookie) PURE;
    STDMETHOD(AdvisePeriodic)       (THIS_ REFERENCE_TIME rtStartTime, REFERENCE_TIME rtPeriodTime,
                                           HANDLE hSemaphore, LPDWORD pdwAdviseCookie) PURE;
    STDMETHOD(Unadvise)             (THIS_ DWORD dwAdviseCookie) PURE;
};

#endif

#ifndef IReferenceClock_QueryInterface

#define IReferenceClock_QueryInterface(p,a,b)      IUnknown_QueryInterface(p,a,b)
#define IReferenceClock_AddRef(p)                  IUnknown_AddRef(p)
#define IReferenceClock_Release(p)                 IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IReferenceClock_GetTime(p,a)               (p)->lpVtbl->GetTime(p,a)
#define IReferenceClock_AdviseTime(p,a,b,c,d)      (p)->lpVtbl->AdviseTime(p,a,b,c,d)
#define IReferenceClock_AdvisePeriodic(p,a,b,c,d)  (p)->lpVtbl->AdvisePeriodic(p,a,b,c,d)
#define IReferenceClock_Unadvise(p,a)              (p)->lpVtbl->Unadvise(p,a)
#else
#define IReferenceClock_GetTime(p,a)               (p)->GetTime(a)
#define IReferenceClock_AdviseTime(p,a,b,c,d)      (p)->AdviseTime(a,b,c,d)
#define IReferenceClock_AdvisePeriodic(p,a,b,c,d)  (p)->AdvisePeriodic(a,b,c,d)
#define IReferenceClock_Unadvise(p,a)              (p)->Unadvise(a)
#endif

#endif

DEFINE_GUID(IID_IDirectSound, 0x279AFA83, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);

#undef INTERFACE
#define INTERFACE IDirectSound

DECLARE_INTERFACE_(IDirectSound, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(CreateSoundBuffer)    (THIS_ LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(GetCaps)              (THIS_ LPDSCAPS pDSCaps) PURE;
    STDMETHOD(DuplicateSoundBuffer) (THIS_ LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate) PURE;
    STDMETHOD(SetCooperativeLevel)  (THIS_ HWND hwnd, DWORD dwLevel) PURE;
    STDMETHOD(Compact)              (THIS) PURE;
    STDMETHOD(GetSpeakerConfig)     (THIS_ LPDWORD pdwSpeakerConfig) PURE;
    STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD dwSpeakerConfig) PURE;
    STDMETHOD(Initialize)           (THIS_ LPCGUID pcGuidDevice) PURE;
};

#define IDirectSound_QueryInterface(p,a,b)       IUnknown_QueryInterface(p,a,b)
#define IDirectSound_AddRef(p)                   IUnknown_AddRef(p)
#define IDirectSound_Release(p)                  IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSound_CreateSoundBuffer(p,a,b,c)  (p)->lpVtbl->CreateSoundBuffer(p,a,b,c)
#define IDirectSound_GetCaps(p,a)                (p)->lpVtbl->GetCaps(p,a)
#define IDirectSound_DuplicateSoundBuffer(p,a,b) (p)->lpVtbl->DuplicateSoundBuffer(p,a,b)
#define IDirectSound_SetCooperativeLevel(p,a,b)  (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectSound_Compact(p)                  (p)->lpVtbl->Compact(p)
#define IDirectSound_GetSpeakerConfig(p,a)       (p)->lpVtbl->GetSpeakerConfig(p,a)
#define IDirectSound_SetSpeakerConfig(p,b)       (p)->lpVtbl->SetSpeakerConfig(p,b)
#define IDirectSound_Initialize(p,a)             (p)->lpVtbl->Initialize(p,a)
#else
#define IDirectSound_CreateSoundBuffer(p,a,b,c)  (p)->CreateSoundBuffer(a,b,c)
#define IDirectSound_GetCaps(p,a)                (p)->GetCaps(a)
#define IDirectSound_DuplicateSoundBuffer(p,a,b) (p)->DuplicateSoundBuffer(a,b)
#define IDirectSound_SetCooperativeLevel(p,a,b)  (p)->SetCooperativeLevel(a,b)
#define IDirectSound_Compact(p)                  (p)->Compact()
#define IDirectSound_GetSpeakerConfig(p,a)       (p)->GetSpeakerConfig(a)
#define IDirectSound_SetSpeakerConfig(p,b)       (p)->SetSpeakerConfig(b)
#define IDirectSound_Initialize(p,a)             (p)->Initialize(a)
#endif

#if DIRECTSOUND_VERSION >= 0x0800

DEFINE_GUID(IID_IDirectSound8, 0xC50A7E93, 0xF395, 0x4834, 0x9E, 0xF6, 0x7F, 0xA9, 0x9D, 0xE5, 0x09, 0x66);

#undef INTERFACE
#define INTERFACE IDirectSound8

DECLARE_INTERFACE_(IDirectSound8, IDirectSound)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(CreateSoundBuffer)    (THIS_ LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(GetCaps)              (THIS_ LPDSCAPS pDSCaps) PURE;
    STDMETHOD(DuplicateSoundBuffer) (THIS_ LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate) PURE;
    STDMETHOD(SetCooperativeLevel)  (THIS_ HWND hwnd, DWORD dwLevel) PURE;
    STDMETHOD(Compact)              (THIS) PURE;
    STDMETHOD(GetSpeakerConfig)     (THIS_ LPDWORD pdwSpeakerConfig) PURE;
    STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD dwSpeakerConfig) PURE;
    STDMETHOD(Initialize)           (THIS_ LPCGUID pcGuidDevice) PURE;

    STDMETHOD(VerifyCertification)  (THIS_ LPDWORD pdwCertified) PURE;
};

#define IDirectSound8_QueryInterface(p,a,b)       IDirectSound_QueryInterface(p,a,b)
#define IDirectSound8_AddRef(p)                   IDirectSound_AddRef(p)
#define IDirectSound8_Release(p)                  IDirectSound_Release(p)
#define IDirectSound8_CreateSoundBuffer(p,a,b,c)  IDirectSound_CreateSoundBuffer(p,a,b,c)
#define IDirectSound8_GetCaps(p,a)                IDirectSound_GetCaps(p,a)
#define IDirectSound8_DuplicateSoundBuffer(p,a,b) IDirectSound_DuplicateSoundBuffer(p,a,b)
#define IDirectSound8_SetCooperativeLevel(p,a,b)  IDirectSound_SetCooperativeLevel(p,a,b)
#define IDirectSound8_Compact(p)                  IDirectSound_Compact(p)
#define IDirectSound8_GetSpeakerConfig(p,a)       IDirectSound_GetSpeakerConfig(p,a)
#define IDirectSound8_SetSpeakerConfig(p,a)       IDirectSound_SetSpeakerConfig(p,a)
#define IDirectSound8_Initialize(p,a)             IDirectSound_Initialize(p,a)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSound8_VerifyCertification(p,a)           (p)->lpVtbl->VerifyCertification(p,a)
#else
#define IDirectSound8_VerifyCertification(p,a)           (p)->VerifyCertification(a)
#endif

#endif

DEFINE_GUID(IID_IDirectSoundBuffer, 0x279AFA85, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);

#undef INTERFACE
#define INTERFACE IDirectSoundBuffer

DECLARE_INTERFACE_(IDirectSoundBuffer, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetCaps)              (THIS_ LPDSBCAPS pDSBufferCaps) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD pdwCurrentPlayCursor, LPDWORD pdwCurrentWriteCursor) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten) PURE;
    STDMETHOD(GetVolume)            (THIS_ LPLONG plVolume) PURE;
    STDMETHOD(GetPan)               (THIS_ LPLONG plPan) PURE;
    STDMETHOD(GetFrequency)         (THIS_ LPDWORD pdwFrequency) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD pdwStatus) PURE;
    STDMETHOD(Initialize)           (THIS_ LPDIRECTSOUND pDirectSound, LPCDSBUFFERDESC pcDSBufferDesc) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags) PURE;
    STDMETHOD(Play)                 (THIS_ DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags) PURE;
    STDMETHOD(SetCurrentPosition)   (THIS_ DWORD dwNewPosition) PURE;
    STDMETHOD(SetFormat)            (THIS_ LPCWAVEFORMATEX pcfxFormat) PURE;
    STDMETHOD(SetVolume)            (THIS_ LONG lVolume) PURE;
    STDMETHOD(SetPan)               (THIS_ LONG lPan) PURE;
    STDMETHOD(SetFrequency)         (THIS_ DWORD dwFrequency) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2) PURE;
    STDMETHOD(Restore)              (THIS) PURE;
};

#define IDirectSoundBuffer_QueryInterface(p,a,b)        IUnknown_QueryInterface(p,a,b)
#define IDirectSoundBuffer_AddRef(p)                    IUnknown_AddRef(p)
#define IDirectSoundBuffer_Release(p)                   IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundBuffer_GetCaps(p,a)                 (p)->lpVtbl->GetCaps(p,a)
#define IDirectSoundBuffer_GetCurrentPosition(p,a,b)    (p)->lpVtbl->GetCurrentPosition(p,a,b)
#define IDirectSoundBuffer_GetFormat(p,a,b,c)           (p)->lpVtbl->GetFormat(p,a,b,c)
#define IDirectSoundBuffer_GetVolume(p,a)               (p)->lpVtbl->GetVolume(p,a)
#define IDirectSoundBuffer_GetPan(p,a)                  (p)->lpVtbl->GetPan(p,a)
#define IDirectSoundBuffer_GetFrequency(p,a)            (p)->lpVtbl->GetFrequency(p,a)
#define IDirectSoundBuffer_GetStatus(p,a)               (p)->lpVtbl->GetStatus(p,a)
#define IDirectSoundBuffer_Initialize(p,a,b)            (p)->lpVtbl->Initialize(p,a,b)
#define IDirectSoundBuffer_Lock(p,a,b,c,d,e,f,g)        (p)->lpVtbl->Lock(p,a,b,c,d,e,f,g)
#define IDirectSoundBuffer_Play(p,a,b,c)                (p)->lpVtbl->Play(p,a,b,c)
#define IDirectSoundBuffer_SetCurrentPosition(p,a)      (p)->lpVtbl->SetCurrentPosition(p,a)
#define IDirectSoundBuffer_SetFormat(p,a)               (p)->lpVtbl->SetFormat(p,a)
#define IDirectSoundBuffer_SetVolume(p,a)               (p)->lpVtbl->SetVolume(p,a)
#define IDirectSoundBuffer_SetPan(p,a)                  (p)->lpVtbl->SetPan(p,a)
#define IDirectSoundBuffer_SetFrequency(p,a)            (p)->lpVtbl->SetFrequency(p,a)
#define IDirectSoundBuffer_Stop(p)                      (p)->lpVtbl->Stop(p)
#define IDirectSoundBuffer_Unlock(p,a,b,c,d)            (p)->lpVtbl->Unlock(p,a,b,c,d)
#define IDirectSoundBuffer_Restore(p)                   (p)->lpVtbl->Restore(p)
#else
#define IDirectSoundBuffer_GetCaps(p,a)                 (p)->GetCaps(a)
#define IDirectSoundBuffer_GetCurrentPosition(p,a,b)    (p)->GetCurrentPosition(a,b)
#define IDirectSoundBuffer_GetFormat(p,a,b,c)           (p)->GetFormat(a,b,c)
#define IDirectSoundBuffer_GetVolume(p,a)               (p)->GetVolume(a)
#define IDirectSoundBuffer_GetPan(p,a)                  (p)->GetPan(a)
#define IDirectSoundBuffer_GetFrequency(p,a)            (p)->GetFrequency(a)
#define IDirectSoundBuffer_GetStatus(p,a)               (p)->GetStatus(a)
#define IDirectSoundBuffer_Initialize(p,a,b)            (p)->Initialize(a,b)
#define IDirectSoundBuffer_Lock(p,a,b,c,d,e,f,g)        (p)->Lock(a,b,c,d,e,f,g)
#define IDirectSoundBuffer_Play(p,a,b,c)                (p)->Play(a,b,c)
#define IDirectSoundBuffer_SetCurrentPosition(p,a)      (p)->SetCurrentPosition(a)
#define IDirectSoundBuffer_SetFormat(p,a)               (p)->SetFormat(a)
#define IDirectSoundBuffer_SetVolume(p,a)               (p)->SetVolume(a)
#define IDirectSoundBuffer_SetPan(p,a)                  (p)->SetPan(a)
#define IDirectSoundBuffer_SetFrequency(p,a)            (p)->SetFrequency(a)
#define IDirectSoundBuffer_Stop(p)                      (p)->Stop()
#define IDirectSoundBuffer_Unlock(p,a,b,c,d)            (p)->Unlock(a,b,c,d)
#define IDirectSoundBuffer_Restore(p)                   (p)->Restore()
#endif

#if DIRECTSOUND_VERSION >= 0x0800

DEFINE_GUID(IID_IDirectSoundBuffer8, 0x6825a449, 0x7524, 0x4d82, 0x92, 0x0f, 0x50, 0xe3, 0x6a, 0xb3, 0xab, 0x1e);

#undef INTERFACE
#define INTERFACE IDirectSoundBuffer8

DECLARE_INTERFACE_(IDirectSoundBuffer8, IDirectSoundBuffer)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetCaps)              (THIS_ LPDSBCAPS pDSBufferCaps) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD pdwCurrentPlayCursor, LPDWORD pdwCurrentWriteCursor) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten) PURE;
    STDMETHOD(GetVolume)            (THIS_ LPLONG plVolume) PURE;
    STDMETHOD(GetPan)               (THIS_ LPLONG plPan) PURE;
    STDMETHOD(GetFrequency)         (THIS_ LPDWORD pdwFrequency) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD pdwStatus) PURE;
    STDMETHOD(Initialize)           (THIS_ LPDIRECTSOUND pDirectSound, LPCDSBUFFERDESC pcDSBufferDesc) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags) PURE;
    STDMETHOD(Play)                 (THIS_ DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags) PURE;
    STDMETHOD(SetCurrentPosition)   (THIS_ DWORD dwNewPosition) PURE;
    STDMETHOD(SetFormat)            (THIS_ LPCWAVEFORMATEX pcfxFormat) PURE;
    STDMETHOD(SetVolume)            (THIS_ LONG lVolume) PURE;
    STDMETHOD(SetPan)               (THIS_ LONG lPan) PURE;
    STDMETHOD(SetFrequency)         (THIS_ DWORD dwFrequency) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2) PURE;
    STDMETHOD(Restore)              (THIS) PURE;

    STDMETHOD(SetFX)                (THIS_ DWORD dwEffectsCount, LPDSEFFECTDESC pDSFXDesc, LPDWORD pdwResultCodes) PURE;
    STDMETHOD(AcquireResources)     (THIS_ DWORD dwFlags, DWORD dwEffectsCount, LPDWORD pdwResultCodes) PURE;
    STDMETHOD(GetObjectInPath)      (THIS_ REFGUID rguidObject, DWORD dwIndex, REFGUID rguidInterface, LPVOID *ppObject) PURE;
};

DEFINE_GUID(GUID_All_Objects, 0xaa114de5, 0xc262, 0x4169, 0xa1, 0xc8, 0x23, 0xd6, 0x98, 0xcc, 0x73, 0xb5);

#define IDirectSoundBuffer8_QueryInterface(p,a,b)           IUnknown_QueryInterface(p,a,b)
#define IDirectSoundBuffer8_AddRef(p)                       IUnknown_AddRef(p)
#define IDirectSoundBuffer8_Release(p)                      IUnknown_Release(p)

#define IDirectSoundBuffer8_GetCaps(p,a)                    IDirectSoundBuffer_GetCaps(p,a)
#define IDirectSoundBuffer8_GetCurrentPosition(p,a,b)       IDirectSoundBuffer_GetCurrentPosition(p,a,b)
#define IDirectSoundBuffer8_GetFormat(p,a,b,c)              IDirectSoundBuffer_GetFormat(p,a,b,c)
#define IDirectSoundBuffer8_GetVolume(p,a)                  IDirectSoundBuffer_GetVolume(p,a)
#define IDirectSoundBuffer8_GetPan(p,a)                     IDirectSoundBuffer_GetPan(p,a)
#define IDirectSoundBuffer8_GetFrequency(p,a)               IDirectSoundBuffer_GetFrequency(p,a)
#define IDirectSoundBuffer8_GetStatus(p,a)                  IDirectSoundBuffer_GetStatus(p,a)
#define IDirectSoundBuffer8_Initialize(p,a,b)               IDirectSoundBuffer_Initialize(p,a,b)
#define IDirectSoundBuffer8_Lock(p,a,b,c,d,e,f,g)           IDirectSoundBuffer_Lock(p,a,b,c,d,e,f,g)
#define IDirectSoundBuffer8_Play(p,a,b,c)                   IDirectSoundBuffer_Play(p,a,b,c)
#define IDirectSoundBuffer8_SetCurrentPosition(p,a)         IDirectSoundBuffer_SetCurrentPosition(p,a)
#define IDirectSoundBuffer8_SetFormat(p,a)                  IDirectSoundBuffer_SetFormat(p,a)
#define IDirectSoundBuffer8_SetVolume(p,a)                  IDirectSoundBuffer_SetVolume(p,a)
#define IDirectSoundBuffer8_SetPan(p,a)                     IDirectSoundBuffer_SetPan(p,a)
#define IDirectSoundBuffer8_SetFrequency(p,a)               IDirectSoundBuffer_SetFrequency(p,a)
#define IDirectSoundBuffer8_Stop(p)                         IDirectSoundBuffer_Stop(p)
#define IDirectSoundBuffer8_Unlock(p,a,b,c,d)               IDirectSoundBuffer_Unlock(p,a,b,c,d)
#define IDirectSoundBuffer8_Restore(p)                      IDirectSoundBuffer_Restore(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundBuffer8_SetFX(p,a,b,c)                  (p)->lpVtbl->SetFX(p,a,b,c)
#define IDirectSoundBuffer8_AcquireResources(p,a,b,c)       (p)->lpVtbl->AcquireResources(p,a,b,c)
#define IDirectSoundBuffer8_GetObjectInPath(p,a,b,c,d)      (p)->lpVtbl->GetObjectInPath(p,a,b,c,d)
#else
#define IDirectSoundBuffer8_SetFX(p,a,b,c)                  (p)->SetFX(a,b,c)
#define IDirectSoundBuffer8_AcquireResources(p,a,b,c)       (p)->AcquireResources(a,b,c)
#define IDirectSoundBuffer8_GetObjectInPath(p,a,b,c,d)      (p)->GetObjectInPath(a,b,c,d)
#endif

#endif

DEFINE_GUID(IID_IDirectSound3DListener, 0x279AFA84, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);

#undef INTERFACE
#define INTERFACE IDirectSound3DListener

DECLARE_INTERFACE_(IDirectSound3DListener, IUnknown)
{

    STDMETHOD(QueryInterface)           (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)            (THIS) PURE;
    STDMETHOD_(ULONG,Release)           (THIS) PURE;

    STDMETHOD(GetAllParameters)         (THIS_ LPDS3DLISTENER pListener) PURE;
    STDMETHOD(GetDistanceFactor)        (THIS_ D3DVALUE* pflDistanceFactor) PURE;
    STDMETHOD(GetDopplerFactor)         (THIS_ D3DVALUE* pflDopplerFactor) PURE;
    STDMETHOD(GetOrientation)           (THIS_ D3DVECTOR* pvOrientFront, D3DVECTOR* pvOrientTop) PURE;
    STDMETHOD(GetPosition)              (THIS_ D3DVECTOR* pvPosition) PURE;
    STDMETHOD(GetRolloffFactor)         (THIS_ D3DVALUE* pflRolloffFactor) PURE;
    STDMETHOD(GetVelocity)              (THIS_ D3DVECTOR* pvVelocity) PURE;
    STDMETHOD(SetAllParameters)         (THIS_ LPCDS3DLISTENER pcListener, DWORD dwApply) PURE;
    STDMETHOD(SetDistanceFactor)        (THIS_ D3DVALUE flDistanceFactor, DWORD dwApply) PURE;
    STDMETHOD(SetDopplerFactor)         (THIS_ D3DVALUE flDopplerFactor, DWORD dwApply) PURE;
    STDMETHOD(SetOrientation)           (THIS_ D3DVALUE xFront, D3DVALUE yFront, D3DVALUE zFront,
                                               D3DVALUE xTop, D3DVALUE yTop, D3DVALUE zTop, DWORD dwApply) PURE;
    STDMETHOD(SetPosition)              (THIS_ D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) PURE;
    STDMETHOD(SetRolloffFactor)         (THIS_ D3DVALUE flRolloffFactor, DWORD dwApply) PURE;
    STDMETHOD(SetVelocity)              (THIS_ D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) PURE;
    STDMETHOD(CommitDeferredSettings)   (THIS) PURE;
};

#define IDirectSound3DListener_QueryInterface(p,a,b)            IUnknown_QueryInterface(p,a,b)
#define IDirectSound3DListener_AddRef(p)                        IUnknown_AddRef(p)
#define IDirectSound3DListener_Release(p)                       IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSound3DListener_GetAllParameters(p,a)            (p)->lpVtbl->GetAllParameters(p,a)
#define IDirectSound3DListener_GetDistanceFactor(p,a)           (p)->lpVtbl->GetDistanceFactor(p,a)
#define IDirectSound3DListener_GetDopplerFactor(p,a)            (p)->lpVtbl->GetDopplerFactor(p,a)
#define IDirectSound3DListener_GetOrientation(p,a,b)            (p)->lpVtbl->GetOrientation(p,a,b)
#define IDirectSound3DListener_GetPosition(p,a)                 (p)->lpVtbl->GetPosition(p,a)
#define IDirectSound3DListener_GetRolloffFactor(p,a)            (p)->lpVtbl->GetRolloffFactor(p,a)
#define IDirectSound3DListener_GetVelocity(p,a)                 (p)->lpVtbl->GetVelocity(p,a)
#define IDirectSound3DListener_SetAllParameters(p,a,b)          (p)->lpVtbl->SetAllParameters(p,a,b)
#define IDirectSound3DListener_SetDistanceFactor(p,a,b)         (p)->lpVtbl->SetDistanceFactor(p,a,b)
#define IDirectSound3DListener_SetDopplerFactor(p,a,b)          (p)->lpVtbl->SetDopplerFactor(p,a,b)
#define IDirectSound3DListener_SetOrientation(p,a,b,c,d,e,f,g)  (p)->lpVtbl->SetOrientation(p,a,b,c,d,e,f,g)
#define IDirectSound3DListener_SetPosition(p,a,b,c,d)           (p)->lpVtbl->SetPosition(p,a,b,c,d)
#define IDirectSound3DListener_SetRolloffFactor(p,a,b)          (p)->lpVtbl->SetRolloffFactor(p,a,b)
#define IDirectSound3DListener_SetVelocity(p,a,b,c,d)           (p)->lpVtbl->SetVelocity(p,a,b,c,d)
#define IDirectSound3DListener_CommitDeferredSettings(p)        (p)->lpVtbl->CommitDeferredSettings(p)
#else
#define IDirectSound3DListener_GetAllParameters(p,a)            (p)->GetAllParameters(a)
#define IDirectSound3DListener_GetDistanceFactor(p,a)           (p)->GetDistanceFactor(a)
#define IDirectSound3DListener_GetDopplerFactor(p,a)            (p)->GetDopplerFactor(a)
#define IDirectSound3DListener_GetOrientation(p,a,b)            (p)->GetOrientation(a,b)
#define IDirectSound3DListener_GetPosition(p,a)                 (p)->GetPosition(a)
#define IDirectSound3DListener_GetRolloffFactor(p,a)            (p)->GetRolloffFactor(a)
#define IDirectSound3DListener_GetVelocity(p,a)                 (p)->GetVelocity(a)
#define IDirectSound3DListener_SetAllParameters(p,a,b)          (p)->SetAllParameters(a,b)
#define IDirectSound3DListener_SetDistanceFactor(p,a,b)         (p)->SetDistanceFactor(a,b)
#define IDirectSound3DListener_SetDopplerFactor(p,a,b)          (p)->SetDopplerFactor(a,b)
#define IDirectSound3DListener_SetOrientation(p,a,b,c,d,e,f,g)  (p)->SetOrientation(a,b,c,d,e,f,g)
#define IDirectSound3DListener_SetPosition(p,a,b,c,d)           (p)->SetPosition(a,b,c,d)
#define IDirectSound3DListener_SetRolloffFactor(p,a,b)          (p)->SetRolloffFactor(a,b)
#define IDirectSound3DListener_SetVelocity(p,a,b,c,d)           (p)->SetVelocity(a,b,c,d)
#define IDirectSound3DListener_CommitDeferredSettings(p)        (p)->CommitDeferredSettings()
#endif

DEFINE_GUID(IID_IDirectSound3DBuffer, 0x279AFA86, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);

#undef INTERFACE
#define INTERFACE IDirectSound3DBuffer

DECLARE_INTERFACE_(IDirectSound3DBuffer, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetAllParameters)     (THIS_ LPDS3DBUFFER pDs3dBuffer) PURE;
    STDMETHOD(GetConeAngles)        (THIS_ LPDWORD pdwInsideConeAngle, LPDWORD pdwOutsideConeAngle) PURE;
    STDMETHOD(GetConeOrientation)   (THIS_ D3DVECTOR* pvOrientation) PURE;
    STDMETHOD(GetConeOutsideVolume) (THIS_ LPLONG plConeOutsideVolume) PURE;
    STDMETHOD(GetMaxDistance)       (THIS_ D3DVALUE* pflMaxDistance) PURE;
    STDMETHOD(GetMinDistance)       (THIS_ D3DVALUE* pflMinDistance) PURE;
    STDMETHOD(GetMode)              (THIS_ LPDWORD pdwMode) PURE;
    STDMETHOD(GetPosition)          (THIS_ D3DVECTOR* pvPosition) PURE;
    STDMETHOD(GetVelocity)          (THIS_ D3DVECTOR* pvVelocity) PURE;
    STDMETHOD(SetAllParameters)     (THIS_ LPCDS3DBUFFER pcDs3dBuffer, DWORD dwApply) PURE;
    STDMETHOD(SetConeAngles)        (THIS_ DWORD dwInsideConeAngle, DWORD dwOutsideConeAngle, DWORD dwApply) PURE;
    STDMETHOD(SetConeOrientation)   (THIS_ D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) PURE;
    STDMETHOD(SetConeOutsideVolume) (THIS_ LONG lConeOutsideVolume, DWORD dwApply) PURE;
    STDMETHOD(SetMaxDistance)       (THIS_ D3DVALUE flMaxDistance, DWORD dwApply) PURE;
    STDMETHOD(SetMinDistance)       (THIS_ D3DVALUE flMinDistance, DWORD dwApply) PURE;
    STDMETHOD(SetMode)              (THIS_ DWORD dwMode, DWORD dwApply) PURE;
    STDMETHOD(SetPosition)          (THIS_ D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) PURE;
    STDMETHOD(SetVelocity)          (THIS_ D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) PURE;
};

#define IDirectSound3DBuffer_QueryInterface(p,a,b)          IUnknown_QueryInterface(p,a,b)
#define IDirectSound3DBuffer_AddRef(p)                      IUnknown_AddRef(p)
#define IDirectSound3DBuffer_Release(p)                     IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSound3DBuffer_GetAllParameters(p,a)          (p)->lpVtbl->GetAllParameters(p,a)
#define IDirectSound3DBuffer_GetConeAngles(p,a,b)           (p)->lpVtbl->GetConeAngles(p,a,b)
#define IDirectSound3DBuffer_GetConeOrientation(p,a)        (p)->lpVtbl->GetConeOrientation(p,a)
#define IDirectSound3DBuffer_GetConeOutsideVolume(p,a)      (p)->lpVtbl->GetConeOutsideVolume(p,a)
#define IDirectSound3DBuffer_GetPosition(p,a)               (p)->lpVtbl->GetPosition(p,a)
#define IDirectSound3DBuffer_GetMinDistance(p,a)            (p)->lpVtbl->GetMinDistance(p,a)
#define IDirectSound3DBuffer_GetMaxDistance(p,a)            (p)->lpVtbl->GetMaxDistance(p,a)
#define IDirectSound3DBuffer_GetMode(p,a)                   (p)->lpVtbl->GetMode(p,a)
#define IDirectSound3DBuffer_GetVelocity(p,a)               (p)->lpVtbl->GetVelocity(p,a)
#define IDirectSound3DBuffer_SetAllParameters(p,a,b)        (p)->lpVtbl->SetAllParameters(p,a,b)
#define IDirectSound3DBuffer_SetConeAngles(p,a,b,c)         (p)->lpVtbl->SetConeAngles(p,a,b,c)
#define IDirectSound3DBuffer_SetConeOrientation(p,a,b,c,d)  (p)->lpVtbl->SetConeOrientation(p,a,b,c,d)
#define IDirectSound3DBuffer_SetConeOutsideVolume(p,a,b)    (p)->lpVtbl->SetConeOutsideVolume(p,a,b)
#define IDirectSound3DBuffer_SetPosition(p,a,b,c,d)         (p)->lpVtbl->SetPosition(p,a,b,c,d)
#define IDirectSound3DBuffer_SetMinDistance(p,a,b)          (p)->lpVtbl->SetMinDistance(p,a,b)
#define IDirectSound3DBuffer_SetMaxDistance(p,a,b)          (p)->lpVtbl->SetMaxDistance(p,a,b)
#define IDirectSound3DBuffer_SetMode(p,a,b)                 (p)->lpVtbl->SetMode(p,a,b)
#define IDirectSound3DBuffer_SetVelocity(p,a,b,c,d)         (p)->lpVtbl->SetVelocity(p,a,b,c,d)
#else
#define IDirectSound3DBuffer_GetAllParameters(p,a)          (p)->GetAllParameters(a)
#define IDirectSound3DBuffer_GetConeAngles(p,a,b)           (p)->GetConeAngles(a,b)
#define IDirectSound3DBuffer_GetConeOrientation(p,a)        (p)->GetConeOrientation(a)
#define IDirectSound3DBuffer_GetConeOutsideVolume(p,a)      (p)->GetConeOutsideVolume(a)
#define IDirectSound3DBuffer_GetPosition(p,a)               (p)->GetPosition(a)
#define IDirectSound3DBuffer_GetMinDistance(p,a)            (p)->GetMinDistance(a)
#define IDirectSound3DBuffer_GetMaxDistance(p,a)            (p)->GetMaxDistance(a)
#define IDirectSound3DBuffer_GetMode(p,a)                   (p)->GetMode(a)
#define IDirectSound3DBuffer_GetVelocity(p,a)               (p)->GetVelocity(a)
#define IDirectSound3DBuffer_SetAllParameters(p,a,b)        (p)->SetAllParameters(a,b)
#define IDirectSound3DBuffer_SetConeAngles(p,a,b,c)         (p)->SetConeAngles(a,b,c)
#define IDirectSound3DBuffer_SetConeOrientation(p,a,b,c,d)  (p)->SetConeOrientation(a,b,c,d)
#define IDirectSound3DBuffer_SetConeOutsideVolume(p,a,b)    (p)->SetConeOutsideVolume(a,b)
#define IDirectSound3DBuffer_SetPosition(p,a,b,c,d)         (p)->SetPosition(a,b,c,d)
#define IDirectSound3DBuffer_SetMinDistance(p,a,b)          (p)->SetMinDistance(a,b)
#define IDirectSound3DBuffer_SetMaxDistance(p,a,b)          (p)->SetMaxDistance(a,b)
#define IDirectSound3DBuffer_SetMode(p,a,b)                 (p)->SetMode(a,b)
#define IDirectSound3DBuffer_SetVelocity(p,a,b,c,d)         (p)->SetVelocity(a,b,c,d)
#endif

DEFINE_GUID(IID_IDirectSoundCapture, 0xb0210781, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16);

#undef INTERFACE
#define INTERFACE IDirectSoundCapture

DECLARE_INTERFACE_(IDirectSoundCapture, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(CreateCaptureBuffer)  (THIS_ LPCDSCBUFFERDESC pcDSCBufferDesc, LPDIRECTSOUNDCAPTUREBUFFER *ppDSCBuffer, LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(GetCaps)              (THIS_ LPDSCCAPS pDSCCaps) PURE;
    STDMETHOD(Initialize)           (THIS_ LPCGUID pcGuidDevice) PURE;
};

#define IDirectSoundCapture_QueryInterface(p,a,b)           IUnknown_QueryInterface(p,a,b)
#define IDirectSoundCapture_AddRef(p)                       IUnknown_AddRef(p)
#define IDirectSoundCapture_Release(p)                      IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundCapture_CreateCaptureBuffer(p,a,b,c)    (p)->lpVtbl->CreateCaptureBuffer(p,a,b,c)
#define IDirectSoundCapture_GetCaps(p,a)                    (p)->lpVtbl->GetCaps(p,a)
#define IDirectSoundCapture_Initialize(p,a)                 (p)->lpVtbl->Initialize(p,a)
#else
#define IDirectSoundCapture_CreateCaptureBuffer(p,a,b,c)    (p)->CreateCaptureBuffer(a,b,c)
#define IDirectSoundCapture_GetCaps(p,a)                    (p)->GetCaps(a)
#define IDirectSoundCapture_Initialize(p,a)                 (p)->Initialize(a)
#endif

DEFINE_GUID(IID_IDirectSoundCaptureBuffer, 0xb0210782, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16);

#undef INTERFACE
#define INTERFACE IDirectSoundCaptureBuffer

DECLARE_INTERFACE_(IDirectSoundCaptureBuffer, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetCaps)              (THIS_ LPDSCBCAPS pDSCBCaps) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD pdwCapturePosition, LPDWORD pdwReadPosition) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD pdwStatus) PURE;
    STDMETHOD(Initialize)           (THIS_ LPDIRECTSOUNDCAPTURE pDirectSoundCapture, LPCDSCBUFFERDESC pcDSCBufferDesc) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags) PURE;
    STDMETHOD(Start)                (THIS_ DWORD dwFlags) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2) PURE;
};

#define IDirectSoundCaptureBuffer_QueryInterface(p,a,b)         IUnknown_QueryInterface(p,a,b)
#define IDirectSoundCaptureBuffer_AddRef(p)                     IUnknown_AddRef(p)
#define IDirectSoundCaptureBuffer_Release(p)                    IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundCaptureBuffer_GetCaps(p,a)                  (p)->lpVtbl->GetCaps(p,a)
#define IDirectSoundCaptureBuffer_GetCurrentPosition(p,a,b)     (p)->lpVtbl->GetCurrentPosition(p,a,b)
#define IDirectSoundCaptureBuffer_GetFormat(p,a,b,c)            (p)->lpVtbl->GetFormat(p,a,b,c)
#define IDirectSoundCaptureBuffer_GetStatus(p,a)                (p)->lpVtbl->GetStatus(p,a)
#define IDirectSoundCaptureBuffer_Initialize(p,a,b)             (p)->lpVtbl->Initialize(p,a,b)
#define IDirectSoundCaptureBuffer_Lock(p,a,b,c,d,e,f,g)         (p)->lpVtbl->Lock(p,a,b,c,d,e,f,g)
#define IDirectSoundCaptureBuffer_Start(p,a)                    (p)->lpVtbl->Start(p,a)
#define IDirectSoundCaptureBuffer_Stop(p)                       (p)->lpVtbl->Stop(p)
#define IDirectSoundCaptureBuffer_Unlock(p,a,b,c,d)             (p)->lpVtbl->Unlock(p,a,b,c,d)
#else
#define IDirectSoundCaptureBuffer_GetCaps(p,a)                  (p)->GetCaps(a)
#define IDirectSoundCaptureBuffer_GetCurrentPosition(p,a,b)     (p)->GetCurrentPosition(a,b)
#define IDirectSoundCaptureBuffer_GetFormat(p,a,b,c)            (p)->GetFormat(a,b,c)
#define IDirectSoundCaptureBuffer_GetStatus(p,a)                (p)->GetStatus(a)
#define IDirectSoundCaptureBuffer_Initialize(p,a,b)             (p)->Initialize(a,b)
#define IDirectSoundCaptureBuffer_Lock(p,a,b,c,d,e,f,g)         (p)->Lock(a,b,c,d,e,f,g)
#define IDirectSoundCaptureBuffer_Start(p,a)                    (p)->Start(a)
#define IDirectSoundCaptureBuffer_Stop(p)                       (p)->Stop()
#define IDirectSoundCaptureBuffer_Unlock(p,a,b,c,d)             (p)->Unlock(a,b,c,d)
#endif

#if DIRECTSOUND_VERSION >= 0x0800

DEFINE_GUID(IID_IDirectSoundCaptureBuffer8, 0x990df4, 0xdbb, 0x4872, 0x83, 0x3e, 0x6d, 0x30, 0x3e, 0x80, 0xae, 0xb6);

#undef INTERFACE
#define INTERFACE IDirectSoundCaptureBuffer8

DECLARE_INTERFACE_(IDirectSoundCaptureBuffer8, IDirectSoundCaptureBuffer)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(GetCaps)              (THIS_ LPDSCBCAPS pDSCBCaps) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD pdwCapturePosition, LPDWORD pdwReadPosition) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, LPDWORD pdwSizeWritten) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD pdwStatus) PURE;
    STDMETHOD(Initialize)           (THIS_ LPDIRECTSOUNDCAPTURE pDirectSoundCapture, LPCDSCBUFFERDESC pcDSCBufferDesc) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD dwOffset, DWORD dwBytes, LPVOID *ppvAudioPtr1, LPDWORD pdwAudioBytes1,
                                           LPVOID *ppvAudioPtr2, LPDWORD pdwAudioBytes2, DWORD dwFlags) PURE;
    STDMETHOD(Start)                (THIS_ DWORD dwFlags) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID pvAudioPtr1, DWORD dwAudioBytes1, LPVOID pvAudioPtr2, DWORD dwAudioBytes2) PURE;

    STDMETHOD(GetObjectInPath)      (THIS_ REFGUID rguidObject, DWORD dwIndex, REFGUID rguidInterface, LPVOID *ppObject) PURE;
    STDMETHOD(GetFXStatus)          (DWORD dwFXCount, LPDWORD pdwFXStatus) PURE;
};

#define IDirectSoundCaptureBuffer8_QueryInterface(p,a,b)            IUnknown_QueryInterface(p,a,b)
#define IDirectSoundCaptureBuffer8_AddRef(p)                        IUnknown_AddRef(p)
#define IDirectSoundCaptureBuffer8_Release(p)                       IUnknown_Release(p)

#define IDirectSoundCaptureBuffer8_GetCaps(p,a)                     IDirectSoundCaptureBuffer_GetCaps(p,a)
#define IDirectSoundCaptureBuffer8_GetCurrentPosition(p,a,b)        IDirectSoundCaptureBuffer_GetCurrentPosition(p,a,b)
#define IDirectSoundCaptureBuffer8_GetFormat(p,a,b,c)               IDirectSoundCaptureBuffer_GetFormat(p,a,b,c)
#define IDirectSoundCaptureBuffer8_GetStatus(p,a)                   IDirectSoundCaptureBuffer_GetStatus(p,a)
#define IDirectSoundCaptureBuffer8_Initialize(p,a,b)                IDirectSoundCaptureBuffer_Initialize(p,a,b)
#define IDirectSoundCaptureBuffer8_Lock(p,a,b,c,d,e,f,g)            IDirectSoundCaptureBuffer_Lock(p,a,b,c,d,e,f,g)
#define IDirectSoundCaptureBuffer8_Start(p,a)                       IDirectSoundCaptureBuffer_Start(p,a)
#define IDirectSoundCaptureBuffer8_Stop(p)                          IDirectSoundCaptureBuffer_Stop(p))
#define IDirectSoundCaptureBuffer8_Unlock(p,a,b,c,d)                IDirectSoundCaptureBuffer_Unlock(p,a,b,c,d)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundCaptureBuffer8_GetObjectInPath(p,a,b,c,d)       (p)->lpVtbl->GetObjectInPath(p,a,b,c,d)
#define IDirectSoundCaptureBuffer8_GetFXStatus(p,a,b)               (p)->lpVtbl->GetFXStatus(p,a,b)
#else
#define IDirectSoundCaptureBuffer8_GetObjectInPath(p,a,b,c,d)       (p)->GetObjectInPath(a,b,c,d)
#define IDirectSoundCaptureBuffer8_GetFXStatus(p,a,b)               (p)->GetFXStatus(a,b)
#endif

#endif

DEFINE_GUID(IID_IDirectSoundNotify, 0xb0210783, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16);

#undef INTERFACE
#define INTERFACE IDirectSoundNotify

DECLARE_INTERFACE_(IDirectSoundNotify, IUnknown)
{

    STDMETHOD(QueryInterface)           (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)            (THIS) PURE;
    STDMETHOD_(ULONG,Release)           (THIS) PURE;

    STDMETHOD(SetNotificationPositions) (THIS_ DWORD dwPositionNotifies, LPCDSBPOSITIONNOTIFY pcPositionNotifies) PURE;
};

#define IDirectSoundNotify_QueryInterface(p,a,b)            IUnknown_QueryInterface(p,a,b)
#define IDirectSoundNotify_AddRef(p)                        IUnknown_AddRef(p)
#define IDirectSoundNotify_Release(p)                       IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundNotify_SetNotificationPositions(p,a,b)  (p)->lpVtbl->SetNotificationPositions(p,a,b)
#else
#define IDirectSoundNotify_SetNotificationPositions(p,a,b)  (p)->SetNotificationPositions(a,b)
#endif

#ifndef _IKsPropertySet_
#define _IKsPropertySet_

#ifdef __cplusplus

struct IKsPropertySet;
#endif

typedef struct IKsPropertySet *LPKSPROPERTYSET;

#define KSPROPERTY_SUPPORT_GET  0x00000001
#define KSPROPERTY_SUPPORT_SET  0x00000002

DEFINE_GUID(IID_IKsPropertySet, 0x31efac30, 0x515c, 0x11d0, 0xa9, 0xaa, 0x00, 0xaa, 0x00, 0x61, 0xbe, 0x93);

#undef INTERFACE
#define INTERFACE IKsPropertySet

DECLARE_INTERFACE_(IKsPropertySet, IUnknown)
{

    STDMETHOD(QueryInterface)   (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    STDMETHOD(Get)              (THIS_ REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength,
                                       LPVOID pPropertyData, ULONG ulDataLength, PULONG pulBytesReturned) PURE;
    STDMETHOD(Set)              (THIS_ REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength,
                                       LPVOID pPropertyData, ULONG ulDataLength) PURE;
    STDMETHOD(QuerySupport)     (THIS_ REFGUID rguidPropSet, ULONG ulId, PULONG pulTypeSupport) PURE;
};

#define IKsPropertySet_QueryInterface(p,a,b)       IUnknown_QueryInterface(p,a,b)
#define IKsPropertySet_AddRef(p)                   IUnknown_AddRef(p)
#define IKsPropertySet_Release(p)                  IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IKsPropertySet_Get(p,a,b,c,d,e,f,g)        (p)->lpVtbl->Get(p,a,b,c,d,e,f,g)
#define IKsPropertySet_Set(p,a,b,c,d,e,f)          (p)->lpVtbl->Set(p,a,b,c,d,e,f)
#define IKsPropertySet_QuerySupport(p,a,b,c)       (p)->lpVtbl->QuerySupport(p,a,b,c)
#else
#define IKsPropertySet_Get(p,a,b,c,d,e,f,g)        (p)->Get(a,b,c,d,e,f,g)
#define IKsPropertySet_Set(p,a,b,c,d,e,f)          (p)->Set(a,b,c,d,e,f)
#define IKsPropertySet_QuerySupport(p,a,b,c)       (p)->QuerySupport(a,b,c)
#endif

#endif

#if DIRECTSOUND_VERSION >= 0x0800

DEFINE_GUID(IID_IDirectSoundFXGargle, 0xd616f352, 0xd622, 0x11ce, 0xaa, 0xc5, 0x00, 0x20, 0xaf, 0x0b, 0x99, 0xa3);

typedef struct _DSFXGargle
{
    DWORD       dwRateHz;
    DWORD       dwWaveShape;
} DSFXGargle, *LPDSFXGargle;

#define DSFXGARGLE_WAVE_TRIANGLE        0
#define DSFXGARGLE_WAVE_SQUARE          1

typedef const DSFXGargle *LPCDSFXGargle;

#define DSFXGARGLE_RATEHZ_MIN           1
#define DSFXGARGLE_RATEHZ_MAX           1000

#undef INTERFACE
#define INTERFACE IDirectSoundFXGargle

DECLARE_INTERFACE_(IDirectSoundFXGargle, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXGargle pcDsFxGargle) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXGargle pDsFxGargle) PURE;
};

#define IDirectSoundFXGargle_QueryInterface(p,a,b)          IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXGargle_AddRef(p)                      IUnknown_AddRef(p)
#define IDirectSoundFXGargle_Release(p)                     IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXGargle_SetAllParameters(p,a)          (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXGargle_GetAllParameters(p,a)          (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXGargle_SetAllParameters(p,a)          (p)->SetAllParameters(a)
#define IDirectSoundFXGargle_GetAllParameters(p,a)          (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXChorus, 0x880842e3, 0x145f, 0x43e6, 0xa9, 0x34, 0xa7, 0x18, 0x06, 0xe5, 0x05, 0x47);

typedef struct _DSFXChorus
{
    FLOAT       fWetDryMix;
    FLOAT       fDepth;
    FLOAT       fFeedback;
    FLOAT       fFrequency;
    LONG        lWaveform;
    FLOAT       fDelay;
    LONG        lPhase;
} DSFXChorus, *LPDSFXChorus;

typedef const DSFXChorus *LPCDSFXChorus;

#define DSFXCHORUS_WAVE_TRIANGLE        0
#define DSFXCHORUS_WAVE_SIN             1

#define DSFXCHORUS_WETDRYMIX_MIN        0.0f
#define DSFXCHORUS_WETDRYMIX_MAX        100.0f
#define DSFXCHORUS_DEPTH_MIN            0.0f
#define DSFXCHORUS_DEPTH_MAX            100.0f
#define DSFXCHORUS_FEEDBACK_MIN         -99.0f
#define DSFXCHORUS_FEEDBACK_MAX         99.0f
#define DSFXCHORUS_FREQUENCY_MIN        0.0f
#define DSFXCHORUS_FREQUENCY_MAX        10.0f
#define DSFXCHORUS_DELAY_MIN            0.0f
#define DSFXCHORUS_DELAY_MAX            20.0f
#define DSFXCHORUS_PHASE_MIN            0
#define DSFXCHORUS_PHASE_MAX            4

#define DSFXCHORUS_PHASE_NEG_180        0
#define DSFXCHORUS_PHASE_NEG_90         1
#define DSFXCHORUS_PHASE_ZERO           2
#define DSFXCHORUS_PHASE_90             3
#define DSFXCHORUS_PHASE_180            4

#undef INTERFACE
#define INTERFACE IDirectSoundFXChorus

DECLARE_INTERFACE_(IDirectSoundFXChorus, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXChorus pcDsFxChorus) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXChorus pDsFxChorus) PURE;
};

#define IDirectSoundFXChorus_QueryInterface(p,a,b)          IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXChorus_AddRef(p)                      IUnknown_AddRef(p)
#define IDirectSoundFXChorus_Release(p)                     IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXChorus_SetAllParameters(p,a)          (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXChorus_GetAllParameters(p,a)          (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXChorus_SetAllParameters(p,a)          (p)->SetAllParameters(a)
#define IDirectSoundFXChorus_GetAllParameters(p,a)          (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXFlanger, 0x903e9878, 0x2c92, 0x4072, 0x9b, 0x2c, 0xea, 0x68, 0xf5, 0x39, 0x67, 0x83);

typedef struct _DSFXFlanger
{
    FLOAT       fWetDryMix;
    FLOAT       fDepth;
    FLOAT       fFeedback;
    FLOAT       fFrequency;
    LONG        lWaveform;
    FLOAT       fDelay;
    LONG        lPhase;
} DSFXFlanger, *LPDSFXFlanger;

typedef const DSFXFlanger *LPCDSFXFlanger;

#define DSFXFLANGER_WAVE_TRIANGLE       0
#define DSFXFLANGER_WAVE_SIN            1

#define DSFXFLANGER_WETDRYMIX_MIN       0.0f
#define DSFXFLANGER_WETDRYMIX_MAX       100.0f
#define DSFXFLANGER_FREQUENCY_MIN       0.0f
#define DSFXFLANGER_FREQUENCY_MAX       10.0f
#define DSFXFLANGER_DEPTH_MIN           0.0f
#define DSFXFLANGER_DEPTH_MAX           100.0f
#define DSFXFLANGER_PHASE_MIN           0
#define DSFXFLANGER_PHASE_MAX           4
#define DSFXFLANGER_FEEDBACK_MIN        -99.0f
#define DSFXFLANGER_FEEDBACK_MAX        99.0f
#define DSFXFLANGER_DELAY_MIN           0.0f
#define DSFXFLANGER_DELAY_MAX           4.0f

#define DSFXFLANGER_PHASE_NEG_180       0
#define DSFXFLANGER_PHASE_NEG_90        1
#define DSFXFLANGER_PHASE_ZERO          2
#define DSFXFLANGER_PHASE_90            3
#define DSFXFLANGER_PHASE_180           4

#undef INTERFACE
#define INTERFACE IDirectSoundFXFlanger

DECLARE_INTERFACE_(IDirectSoundFXFlanger, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXFlanger pcDsFxFlanger) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXFlanger pDsFxFlanger) PURE;
};

#define IDirectSoundFXFlanger_QueryInterface(p,a,b)         IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXFlanger_AddRef(p)                     IUnknown_AddRef(p)
#define IDirectSoundFXFlanger_Release(p)                    IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXFlanger_SetAllParameters(p,a)         (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXFlanger_GetAllParameters(p,a)         (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXFlanger_SetAllParameters(p,a)         (p)->SetAllParameters(a)
#define IDirectSoundFXFlanger_GetAllParameters(p,a)         (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXEcho, 0x8bd28edf, 0x50db, 0x4e92, 0xa2, 0xbd, 0x44, 0x54, 0x88, 0xd1, 0xed, 0x42);

typedef struct _DSFXEcho
{
    FLOAT   fWetDryMix;
    FLOAT   fFeedback;
    FLOAT   fLeftDelay;
    FLOAT   fRightDelay;
    LONG    lPanDelay;
} DSFXEcho, *LPDSFXEcho;

typedef const DSFXEcho *LPCDSFXEcho;

#define DSFXECHO_WETDRYMIX_MIN      0.0f
#define DSFXECHO_WETDRYMIX_MAX      100.0f
#define DSFXECHO_FEEDBACK_MIN       0.0f
#define DSFXECHO_FEEDBACK_MAX       100.0f
#define DSFXECHO_LEFTDELAY_MIN      1.0f
#define DSFXECHO_LEFTDELAY_MAX      2000.0f
#define DSFXECHO_RIGHTDELAY_MIN     1.0f
#define DSFXECHO_RIGHTDELAY_MAX     2000.0f
#define DSFXECHO_PANDELAY_MIN       0
#define DSFXECHO_PANDELAY_MAX       1

#undef INTERFACE
#define INTERFACE IDirectSoundFXEcho

DECLARE_INTERFACE_(IDirectSoundFXEcho, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXEcho pcDsFxEcho) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXEcho pDsFxEcho) PURE;
};

#define IDirectSoundFXEcho_QueryInterface(p,a,b)            IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXEcho_AddRef(p)                        IUnknown_AddRef(p)
#define IDirectSoundFXEcho_Release(p)                       IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXEcho_SetAllParameters(p,a)            (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXEcho_GetAllParameters(p,a)            (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXEcho_SetAllParameters(p,a)            (p)->SetAllParameters(a)
#define IDirectSoundFXEcho_GetAllParameters(p,a)            (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXDistortion, 0x8ecf4326, 0x455f, 0x4d8b, 0xbd, 0xa9, 0x8d, 0x5d, 0x3e, 0x9e, 0x3e, 0x0b);

typedef struct _DSFXDistortion
{
    FLOAT   fGain;
    FLOAT   fEdge;
    FLOAT   fPostEQCenterFrequency;
    FLOAT   fPostEQBandwidth;
    FLOAT   fPreLowpassCutoff;
} DSFXDistortion, *LPDSFXDistortion;

typedef const DSFXDistortion *LPCDSFXDistortion;

#define DSFXDISTORTION_GAIN_MIN                     -60.0f
#define DSFXDISTORTION_GAIN_MAX                     0.0f
#define DSFXDISTORTION_EDGE_MIN                     0.0f
#define DSFXDISTORTION_EDGE_MAX                     100.0f
#define DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN    100.0f
#define DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX    8000.0f
#define DSFXDISTORTION_POSTEQBANDWIDTH_MIN          100.0f
#define DSFXDISTORTION_POSTEQBANDWIDTH_MAX          8000.0f
#define DSFXDISTORTION_PRELOWPASSCUTOFF_MIN         100.0f
#define DSFXDISTORTION_PRELOWPASSCUTOFF_MAX         8000.0f

#undef INTERFACE
#define INTERFACE IDirectSoundFXDistortion

DECLARE_INTERFACE_(IDirectSoundFXDistortion, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXDistortion pcDsFxDistortion) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXDistortion pDsFxDistortion) PURE;
};

#define IDirectSoundFXDistortion_QueryInterface(p,a,b)      IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXDistortion_AddRef(p)                  IUnknown_AddRef(p)
#define IDirectSoundFXDistortion_Release(p)                 IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXDistortion_SetAllParameters(p,a)      (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXDistortion_GetAllParameters(p,a)      (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXDistortion_SetAllParameters(p,a)      (p)->SetAllParameters(a)
#define IDirectSoundFXDistortion_GetAllParameters(p,a)      (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXCompressor, 0x4bbd1154, 0x62f6, 0x4e2c, 0xa1, 0x5c, 0xd3, 0xb6, 0xc4, 0x17, 0xf7, 0xa0);

typedef struct _DSFXCompressor
{
    FLOAT   fGain;
    FLOAT   fAttack;
    FLOAT   fRelease;
    FLOAT   fThreshold;
    FLOAT   fRatio;
    FLOAT   fPredelay;
} DSFXCompressor, *LPDSFXCompressor;

typedef const DSFXCompressor *LPCDSFXCompressor;

#define DSFXCOMPRESSOR_GAIN_MIN             -60.0f
#define DSFXCOMPRESSOR_GAIN_MAX             60.0f
#define DSFXCOMPRESSOR_ATTACK_MIN           0.01f
#define DSFXCOMPRESSOR_ATTACK_MAX           500.0f
#define DSFXCOMPRESSOR_RELEASE_MIN          50.0f
#define DSFXCOMPRESSOR_RELEASE_MAX          3000.0f
#define DSFXCOMPRESSOR_THRESHOLD_MIN        -60.0f
#define DSFXCOMPRESSOR_THRESHOLD_MAX        0.0f
#define DSFXCOMPRESSOR_RATIO_MIN            1.0f
#define DSFXCOMPRESSOR_RATIO_MAX            100.0f
#define DSFXCOMPRESSOR_PREDELAY_MIN         0.0f
#define DSFXCOMPRESSOR_PREDELAY_MAX         4.0f

#undef INTERFACE
#define INTERFACE IDirectSoundFXCompressor

DECLARE_INTERFACE_(IDirectSoundFXCompressor, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXCompressor pcDsFxCompressor) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXCompressor pDsFxCompressor) PURE;
};

#define IDirectSoundFXCompressor_QueryInterface(p,a,b)      IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXCompressor_AddRef(p)                  IUnknown_AddRef(p)
#define IDirectSoundFXCompressor_Release(p)                 IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXCompressor_SetAllParameters(p,a)      (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXCompressor_GetAllParameters(p,a)      (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXCompressor_SetAllParameters(p,a)      (p)->SetAllParameters(a)
#define IDirectSoundFXCompressor_GetAllParameters(p,a)      (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXParamEq, 0xc03ca9fe, 0xfe90, 0x4204, 0x80, 0x78, 0x82, 0x33, 0x4c, 0xd1, 0x77, 0xda);

typedef struct _DSFXParamEq
{
    FLOAT   fCenter;
    FLOAT   fBandwidth;
    FLOAT   fGain;
} DSFXParamEq, *LPDSFXParamEq;

typedef const DSFXParamEq *LPCDSFXParamEq;

#define DSFXPARAMEQ_CENTER_MIN      80.0f
#define DSFXPARAMEQ_CENTER_MAX      16000.0f
#define DSFXPARAMEQ_BANDWIDTH_MIN   1.0f
#define DSFXPARAMEQ_BANDWIDTH_MAX   36.0f
#define DSFXPARAMEQ_GAIN_MIN        -15.0f
#define DSFXPARAMEQ_GAIN_MAX        15.0f

#undef INTERFACE
#define INTERFACE IDirectSoundFXParamEq

DECLARE_INTERFACE_(IDirectSoundFXParamEq, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXParamEq pcDsFxParamEq) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXParamEq pDsFxParamEq) PURE;
};

#define IDirectSoundFXParamEq_QueryInterface(p,a,b)      IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXParamEq_AddRef(p)                  IUnknown_AddRef(p)
#define IDirectSoundFXParamEq_Release(p)                 IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXParamEq_SetAllParameters(p,a)      (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXParamEq_GetAllParameters(p,a)      (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXParamEq_SetAllParameters(p,a)      (p)->SetAllParameters(a)
#define IDirectSoundFXParamEq_GetAllParameters(p,a)      (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXI3DL2Reverb, 0x4b166a6a, 0x0d66, 0x43f3, 0x80, 0xe3, 0xee, 0x62, 0x80, 0xde, 0xe1, 0xa4);

typedef struct _DSFXI3DL2Reverb
{
    LONG    lRoom;
    LONG    lRoomHF;
    FLOAT   flRoomRolloffFactor;
    FLOAT   flDecayTime;
    FLOAT   flDecayHFRatio;
    LONG    lReflections;
    FLOAT   flReflectionsDelay;
    LONG    lReverb;
    FLOAT   flReverbDelay;
    FLOAT   flDiffusion;
    FLOAT   flDensity;
    FLOAT   flHFReference;
} DSFXI3DL2Reverb, *LPDSFXI3DL2Reverb;

typedef const DSFXI3DL2Reverb *LPCDSFXI3DL2Reverb;

#define DSFX_I3DL2REVERB_ROOM_MIN                   (-10000)
#define DSFX_I3DL2REVERB_ROOM_MAX                   0
#define DSFX_I3DL2REVERB_ROOM_DEFAULT               (-1000)

#define DSFX_I3DL2REVERB_ROOMHF_MIN                 (-10000)
#define DSFX_I3DL2REVERB_ROOMHF_MAX                 0
#define DSFX_I3DL2REVERB_ROOMHF_DEFAULT             (-100)

#define DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN      0.0f
#define DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX      10.0f
#define DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_DEFAULT  0.0f

#define DSFX_I3DL2REVERB_DECAYTIME_MIN              0.1f
#define DSFX_I3DL2REVERB_DECAYTIME_MAX              20.0f
#define DSFX_I3DL2REVERB_DECAYTIME_DEFAULT          1.49f

#define DSFX_I3DL2REVERB_DECAYHFRATIO_MIN           0.1f
#define DSFX_I3DL2REVERB_DECAYHFRATIO_MAX           2.0f
#define DSFX_I3DL2REVERB_DECAYHFRATIO_DEFAULT       0.83f

#define DSFX_I3DL2REVERB_REFLECTIONS_MIN            (-10000)
#define DSFX_I3DL2REVERB_REFLECTIONS_MAX            1000
#define DSFX_I3DL2REVERB_REFLECTIONS_DEFAULT        (-2602)

#define DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN       0.0f
#define DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX       0.3f
#define DSFX_I3DL2REVERB_REFLECTIONSDELAY_DEFAULT   0.007f

#define DSFX_I3DL2REVERB_REVERB_MIN                 (-10000)
#define DSFX_I3DL2REVERB_REVERB_MAX                 2000
#define DSFX_I3DL2REVERB_REVERB_DEFAULT             (200)

#define DSFX_I3DL2REVERB_REVERBDELAY_MIN            0.0f
#define DSFX_I3DL2REVERB_REVERBDELAY_MAX            0.1f
#define DSFX_I3DL2REVERB_REVERBDELAY_DEFAULT        0.011f

#define DSFX_I3DL2REVERB_DIFFUSION_MIN              0.0f
#define DSFX_I3DL2REVERB_DIFFUSION_MAX              100.0f
#define DSFX_I3DL2REVERB_DIFFUSION_DEFAULT          100.0f

#define DSFX_I3DL2REVERB_DENSITY_MIN                0.0f
#define DSFX_I3DL2REVERB_DENSITY_MAX                100.0f
#define DSFX_I3DL2REVERB_DENSITY_DEFAULT            100.0f

#define DSFX_I3DL2REVERB_HFREFERENCE_MIN            20.0f
#define DSFX_I3DL2REVERB_HFREFERENCE_MAX            20000.0f
#define DSFX_I3DL2REVERB_HFREFERENCE_DEFAULT        5000.0f

#define DSFX_I3DL2REVERB_QUALITY_MIN                0
#define DSFX_I3DL2REVERB_QUALITY_MAX                3
#define DSFX_I3DL2REVERB_QUALITY_DEFAULT            2

#undef INTERFACE
#define INTERFACE IDirectSoundFXI3DL2Reverb

DECLARE_INTERFACE_(IDirectSoundFXI3DL2Reverb, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXI3DL2Reverb pcDsFxI3DL2Reverb) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXI3DL2Reverb pDsFxI3DL2Reverb) PURE;
    STDMETHOD(SetPreset)            (THIS_ DWORD dwPreset) PURE;
    STDMETHOD(GetPreset)            (THIS_ LPDWORD pdwPreset) PURE;
    STDMETHOD(SetQuality)           (THIS_ LONG lQuality) PURE;
    STDMETHOD(GetQuality)           (THIS_ LONG *plQuality) PURE;
};

#define IDirectSoundFXI3DL2Reverb_QueryInterface(p,a,b)     IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXI3DL2Reverb_AddRef(p)                 IUnknown_AddRef(p)
#define IDirectSoundFXI3DL2Reverb_Release(p)                IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXI3DL2Reverb_SetAllParameters(p,a)     (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXI3DL2Reverb_GetAllParameters(p,a)     (p)->lpVtbl->GetAllParameters(p,a)
#define IDirectSoundFXI3DL2Reverb_SetPreset(p,a)            (p)->lpVtbl->SetPreset(p,a)
#define IDirectSoundFXI3DL2Reverb_GetPreset(p,a)            (p)->lpVtbl->GetPreset(p,a)
#else
#define IDirectSoundFXI3DL2Reverb_SetAllParameters(p,a)     (p)->SetAllParameters(a)
#define IDirectSoundFXI3DL2Reverb_GetAllParameters(p,a)     (p)->GetAllParameters(a)
#define IDirectSoundFXI3DL2Reverb_SetPreset(p,a)            (p)->SetPreset(a)
#define IDirectSoundFXI3DL2Reverb_GetPreset(p,a)            (p)->GetPreset(a)
#endif

DEFINE_GUID(IID_IDirectSoundFXWavesReverb,0x46858c3a,0x0dc6,0x45e3,0xb7,0x60,0xd4,0xee,0xf1,0x6c,0xb3,0x25);

typedef struct _DSFXWavesReverb
{
    FLOAT   fInGain;
    FLOAT   fReverbMix;
    FLOAT   fReverbTime;
    FLOAT   fHighFreqRTRatio;
} DSFXWavesReverb, *LPDSFXWavesReverb;

typedef const DSFXWavesReverb *LPCDSFXWavesReverb;

#define DSFX_WAVESREVERB_INGAIN_MIN                 -96.0f
#define DSFX_WAVESREVERB_INGAIN_MAX                 0.0f
#define DSFX_WAVESREVERB_INGAIN_DEFAULT             0.0f
#define DSFX_WAVESREVERB_REVERBMIX_MIN              -96.0f
#define DSFX_WAVESREVERB_REVERBMIX_MAX              0.0f
#define DSFX_WAVESREVERB_REVERBMIX_DEFAULT          0.0f
#define DSFX_WAVESREVERB_REVERBTIME_MIN             0.001f
#define DSFX_WAVESREVERB_REVERBTIME_MAX             3000.0f
#define DSFX_WAVESREVERB_REVERBTIME_DEFAULT         1000.0f
#define DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN        0.001f
#define DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX        0.999f
#define DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT    0.001f

#undef INTERFACE
#define INTERFACE IDirectSoundFXWavesReverb

DECLARE_INTERFACE_(IDirectSoundFXWavesReverb, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSFXWavesReverb pcDsFxWavesReverb) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSFXWavesReverb pDsFxWavesReverb) PURE;
};

#define IDirectSoundFXWavesReverb_QueryInterface(p,a,b)     IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFXWavesReverb_AddRef(p)                 IUnknown_AddRef(p)
#define IDirectSoundFXWavesReverb_Release(p)                IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFXWavesReverb_SetAllParameters(p,a)     (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundFXWavesReverb_GetAllParameters(p,a)     (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundFXWavesReverb_SetAllParameters(p,a)     (p)->SetAllParameters(a)
#define IDirectSoundFXWavesReverb_GetAllParameters(p,a)     (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundCaptureFXAec, 0x174d3eb9, 0x6696, 0x4fac, 0xa4, 0x6c, 0xa0, 0xac, 0x7b, 0xc9, 0xe2, 0xf);

typedef struct _DSCFXAec
{
    BOOL    fEnable;
    BOOL    fReset;
} DSCFXAec, *LPDSCFXAec;

typedef const DSCFXAec *LPCDSCFXAec;

#undef INTERFACE
#define INTERFACE IDirectSoundCaptureFXAec

DECLARE_INTERFACE_(IDirectSoundCaptureFXAec, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSCFXAec pDscFxAec) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSCFXAec pDscFxAec) PURE;
};

#define IDirectSoundCaptureFXAec_QueryInterface(p,a,b)     IUnknown_QueryInterface(p,a,b)
#define IDirectSoundCaptureFXAec_AddRef(p)                 IUnknown_AddRef(p)
#define IDirectSoundCaptureFXAec_Release(p)                IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundCaptureFXAec_SetAllParameters(p,a)     (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundCaptureFXAec_GetAllParameters(p,a)     (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundCaptureFXAec_SetAllParameters(p,a)     (p)->SetAllParameters(a)
#define IDirectSoundCaptureFXAec_GetAllParameters(p,a)     (p)->GetAllParameters(a)
#endif

DEFINE_GUID(IID_IDirectSoundCaptureFXNoiseSuppress, 0xed311e41, 0xfbae, 0x4175, 0x96, 0x25, 0xcd, 0x8, 0x54, 0xf6, 0x93, 0xca);

typedef struct _DSCFXNoiseSuppress
{
    BOOL    fEnable;
    BOOL    fReset;
} DSCFXNoiseSuppress, *LPDSCFXNoiseSuppress;

typedef const DSCFXNoiseSuppress *LPCDSCFXNoiseSuppress;

#undef INTERFACE
#define INTERFACE IDirectSoundCaptureFXNoiseSuppress

DECLARE_INTERFACE_(IDirectSoundCaptureFXNoiseSuppress, IUnknown)
{

    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    STDMETHOD(SetAllParameters)     (THIS_ LPCDSCFXNoiseSuppress pcDscFxNoiseSuppress) PURE;
    STDMETHOD(GetAllParameters)     (THIS_ LPDSCFXNoiseSuppress pDscFxNoiseSuppress) PURE;
};

#define IDirectSoundCaptureFXNoiseSuppress_QueryInterface(p,a,b)     IUnknown_QueryInterface(p,a,b)
#define IDirectSoundCaptureFXNoiseSuppress_AddRef(p)                 IUnknown_AddRef(p)
#define IDirectSoundCaptureFXNoiseSuppress_Release(p)                IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundCaptureFXNoiseSuppress_SetAllParameters(p,a)     (p)->lpVtbl->SetAllParameters(p,a)
#define IDirectSoundCaptureFXNoiseSuppress_GetAllParameters(p,a)     (p)->lpVtbl->GetAllParameters(p,a)
#else
#define IDirectSoundCaptureFXNoiseSuppress_SetAllParameters(p,a)     (p)->SetAllParameters(a)
#define IDirectSoundCaptureFXNoiseSuppress_GetAllParameters(p,a)     (p)->GetAllParameters(a)
#endif

#ifndef _IDirectSoundFullDuplex_
#define _IDirectSoundFullDuplex_

#ifdef __cplusplus

struct IDirectSoundFullDuplex;
#endif

typedef struct IDirectSoundFullDuplex *LPDIRECTSOUNDFULLDUPLEX;

DEFINE_GUID(IID_IDirectSoundFullDuplex, 0xedcb4c7a, 0xdaab, 0x4216, 0xa4, 0x2e, 0x6c, 0x50, 0x59, 0x6d, 0xdc, 0x1d);

#undef INTERFACE
#define INTERFACE IDirectSoundFullDuplex

DECLARE_INTERFACE_(IDirectSoundFullDuplex, IUnknown)
{

    STDMETHOD(QueryInterface)   (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    STDMETHOD(Initialize)     (THIS_ LPCGUID pCaptureGuid, LPCGUID pRenderGuid, LPCDSCBUFFERDESC lpDscBufferDesc, LPCDSBUFFERDESC lpDsBufferDesc, HWND hWnd, DWORD dwLevel, LPLPDIRECTSOUNDCAPTUREBUFFER8 lplpDirectSoundCaptureBuffer8, LPLPDIRECTSOUNDBUFFER8 lplpDirectSoundBuffer8) PURE;
};

#define IDirectSoundFullDuplex_QueryInterface(p,a,b)    IUnknown_QueryInterface(p,a,b)
#define IDirectSoundFullDuplex_AddRef(p)                IUnknown_AddRef(p)
#define IDirectSoundFullDuplex_Release(p)               IUnknown_Release(p)

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectSoundFullDuplex_Initialize(p,a,b,c,d,e,f,g,h)     (p)->lpVtbl->Initialize(p,a,b,c,d,e,f,g,h)
#else
#define IDirectSoundFullDuplex_Initialize(p,a,b,c,d,e,f,g,h)     (p)->Initialize(a,b,c,d,e,f,g,h)
#endif

#endif

#endif

#define DS_OK                           S_OK

#define DS_NO_VIRTUALIZATION            MAKE_HRESULT(0, _FACDS, 10)

#define DS_INCOMPLETE                   MAKE_HRESULT(0, _FACDS, 20)

#define DSERR_ALLOCATED                 MAKE_DSHRESULT(10)

#define DSERR_CONTROLUNAVAIL            MAKE_DSHRESULT(30)

#define DSERR_INVALIDPARAM              E_INVALIDARG

#define DSERR_INVALIDCALL               MAKE_DSHRESULT(50)

#define DSERR_GENERIC                   E_FAIL

#define DSERR_PRIOLEVELNEEDED           MAKE_DSHRESULT(70)

#define DSERR_OUTOFMEMORY               E_OUTOFMEMORY

#define DSERR_BADFORMAT                 MAKE_DSHRESULT(100)

#define DSERR_UNSUPPORTED               E_NOTIMPL

#define DSERR_NODRIVER                  MAKE_DSHRESULT(120)

#define DSERR_ALREADYINITIALIZED        MAKE_DSHRESULT(130)

#define DSERR_NOAGGREGATION             CLASS_E_NOAGGREGATION

#define DSERR_BUFFERLOST                MAKE_DSHRESULT(150)

#define DSERR_OTHERAPPHASPRIO           MAKE_DSHRESULT(160)

#define DSERR_UNINITIALIZED             MAKE_DSHRESULT(170)

#define DSERR_NOINTERFACE               E_NOINTERFACE

#define DSERR_ACCESSDENIED              E_ACCESSDENIED

#define DSERR_BUFFERTOOSMALL            MAKE_DSHRESULT(180)

#define DSERR_DS8_REQUIRED              MAKE_DSHRESULT(190)

#define DSERR_SENDLOOP                  MAKE_DSHRESULT(200)

#define DSERR_BADSENDBUFFERGUID         MAKE_DSHRESULT(210)

#define DSERR_OBJECTNOTFOUND            MAKE_DSHRESULT(4449)

#define DSCAPS_PRIMARYMONO          0x00000001
#define DSCAPS_PRIMARYSTEREO        0x00000002
#define DSCAPS_PRIMARY8BIT          0x00000004
#define DSCAPS_PRIMARY16BIT         0x00000008
#define DSCAPS_CONTINUOUSRATE       0x00000010
#define DSCAPS_EMULDRIVER           0x00000020
#define DSCAPS_CERTIFIED            0x00000040
#define DSCAPS_SECONDARYMONO        0x00000100
#define DSCAPS_SECONDARYSTEREO      0x00000200
#define DSCAPS_SECONDARY8BIT        0x00000400
#define DSCAPS_SECONDARY16BIT       0x00000800

#define DSSCL_NORMAL                0x00000001
#define DSSCL_PRIORITY              0x00000002
#define DSSCL_EXCLUSIVE             0x00000003
#define DSSCL_WRITEPRIMARY          0x00000004

#define DSSPEAKER_HEADPHONE         0x00000001
#define DSSPEAKER_MONO              0x00000002
#define DSSPEAKER_QUAD              0x00000003
#define DSSPEAKER_STEREO            0x00000004
#define DSSPEAKER_SURROUND          0x00000005
#define DSSPEAKER_5POINT1           0x00000006

#define DSSPEAKER_GEOMETRY_MIN      0x00000005
#define DSSPEAKER_GEOMETRY_NARROW   0x0000000A
#define DSSPEAKER_GEOMETRY_WIDE     0x00000014
#define DSSPEAKER_GEOMETRY_MAX      0x000000B4

#define DSSPEAKER_COMBINED(c, g)    ((DWORD)(((BYTE)(c)) | ((DWORD)((BYTE)(g))) << 16))
#define DSSPEAKER_CONFIG(a)         ((BYTE)(a))
#define DSSPEAKER_GEOMETRY(a)       ((BYTE)(((DWORD)(a) >> 16) & 0x00FF))

#define DSBCAPS_PRIMARYBUFFER       0x00000001
#define DSBCAPS_STATIC              0x00000002
#define DSBCAPS_LOCHARDWARE         0x00000004
#define DSBCAPS_LOCSOFTWARE         0x00000008
#define DSBCAPS_CTRL3D              0x00000010
#define DSBCAPS_CTRLFREQUENCY       0x00000020
#define DSBCAPS_CTRLPAN             0x00000040
#define DSBCAPS_CTRLVOLUME          0x00000080
#define DSBCAPS_CTRLPOSITIONNOTIFY  0x00000100
#define DSBCAPS_CTRLFX              0x00000200
#define DSBCAPS_STICKYFOCUS         0x00004000
#define DSBCAPS_GLOBALFOCUS         0x00008000
#define DSBCAPS_GETCURRENTPOSITION2 0x00010000
#define DSBCAPS_MUTE3DATMAXDISTANCE 0x00020000
#define DSBCAPS_LOCDEFER            0x00040000

#define DSBPLAY_LOOPING             0x00000001
#define DSBPLAY_LOCHARDWARE         0x00000002
#define DSBPLAY_LOCSOFTWARE         0x00000004
#define DSBPLAY_TERMINATEBY_TIME    0x00000008
#define DSBPLAY_TERMINATEBY_DISTANCE    0x000000010
#define DSBPLAY_TERMINATEBY_PRIORITY    0x000000020

#define DSBSTATUS_PLAYING           0x00000001
#define DSBSTATUS_BUFFERLOST        0x00000002
#define DSBSTATUS_LOOPING           0x00000004
#define DSBSTATUS_LOCHARDWARE       0x00000008
#define DSBSTATUS_LOCSOFTWARE       0x00000010
#define DSBSTATUS_TERMINATED        0x00000020

#define DSBLOCK_FROMWRITECURSOR     0x00000001
#define DSBLOCK_ENTIREBUFFER        0x00000002

#define DSBFREQUENCY_MIN            100
#define DSBFREQUENCY_MAX            100000
#define DSBFREQUENCY_ORIGINAL       0

#define DSBPAN_LEFT                 -10000
#define DSBPAN_CENTER               0
#define DSBPAN_RIGHT                10000

#define DSBVOLUME_MIN               -10000
#define DSBVOLUME_MAX               0

#define DSBSIZE_MIN                 4
#define DSBSIZE_MAX                 0x0FFFFFFF
#define DSBSIZE_FX_MIN              150

#define DS3DMODE_NORMAL             0x00000000
#define DS3DMODE_HEADRELATIVE       0x00000001
#define DS3DMODE_DISABLE            0x00000002

#define DS3D_IMMEDIATE              0x00000000
#define DS3D_DEFERRED               0x00000001

#define DS3D_MINDISTANCEFACTOR      FLT_MIN
#define DS3D_MAXDISTANCEFACTOR      FLT_MAX
#define DS3D_DEFAULTDISTANCEFACTOR  1.0f

#define DS3D_MINROLLOFFFACTOR       0.0f
#define DS3D_MAXROLLOFFFACTOR       10.0f
#define DS3D_DEFAULTROLLOFFFACTOR   1.0f

#define DS3D_MINDOPPLERFACTOR       0.0f
#define DS3D_MAXDOPPLERFACTOR       10.0f
#define DS3D_DEFAULTDOPPLERFACTOR   1.0f

#define DS3D_DEFAULTMINDISTANCE     1.0f
#define DS3D_DEFAULTMAXDISTANCE     1000000000.0f

#define DS3D_MINCONEANGLE           0
#define DS3D_MAXCONEANGLE           360
#define DS3D_DEFAULTCONEANGLE       360

#define DS3D_DEFAULTCONEOUTSIDEVOLUME DSBVOLUME_MAX

#define DSCCAPS_EMULDRIVER          DSCAPS_EMULDRIVER
#define DSCCAPS_CERTIFIED           DSCAPS_CERTIFIED

#define DSCBCAPS_WAVEMAPPED         0x80000000

#if DIRECTSOUND_VERSION >= 0x0800
#define DSCBCAPS_CTRLFX             0x00000200
#endif

#define DSCBLOCK_ENTIREBUFFER       0x00000001

#define DSCBSTATUS_CAPTURING        0x00000001
#define DSCBSTATUS_LOOPING          0x00000002

#define DSCBSTART_LOOPING           0x00000001

#define DSBPN_OFFSETSTOP            0xFFFFFFFF

#define DS_CERTIFIED                0x00000000
#define DS_UNCERTIFIED              0x00000001

#define DS_SYSTEM_RESOURCES_NO_HOST_RESOURCES  0x00000000
#define DS_SYSTEM_RESOURCES_ALL_HOST_RESOURCES 0x7FFFFFFF
#define DS_SYSTEM_RESOURCES_UNDEFINED          0x80000000

enum
{
    DSFX_I3DL2_MATERIAL_PRESET_SINGLEWINDOW,
    DSFX_I3DL2_MATERIAL_PRESET_DOUBLEWINDOW,
    DSFX_I3DL2_MATERIAL_PRESET_THINDOOR,
    DSFX_I3DL2_MATERIAL_PRESET_THICKDOOR,
    DSFX_I3DL2_MATERIAL_PRESET_WOODWALL,
    DSFX_I3DL2_MATERIAL_PRESET_BRICKWALL,
    DSFX_I3DL2_MATERIAL_PRESET_STONEWALL,
    DSFX_I3DL2_MATERIAL_PRESET_CURTAIN
};

#define I3DL2_MATERIAL_PRESET_SINGLEWINDOW    -2800,0.71f
#define I3DL2_MATERIAL_PRESET_DOUBLEWINDOW    -5000,0.40f
#define I3DL2_MATERIAL_PRESET_THINDOOR        -1800,0.66f
#define I3DL2_MATERIAL_PRESET_THICKDOOR       -4400,0.64f
#define I3DL2_MATERIAL_PRESET_WOODWALL        -4000,0.50f
#define I3DL2_MATERIAL_PRESET_BRICKWALL       -5000,0.60f
#define I3DL2_MATERIAL_PRESET_STONEWALL       -6000,0.68f
#define I3DL2_MATERIAL_PRESET_CURTAIN         -1200,0.15f

enum
{
    DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT,
    DSFX_I3DL2_ENVIRONMENT_PRESET_GENERIC,
    DSFX_I3DL2_ENVIRONMENT_PRESET_PADDEDCELL,
    DSFX_I3DL2_ENVIRONMENT_PRESET_ROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_BATHROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_LIVINGROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_STONEROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_AUDITORIUM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_CONCERTHALL,
    DSFX_I3DL2_ENVIRONMENT_PRESET_CAVE,
    DSFX_I3DL2_ENVIRONMENT_PRESET_ARENA,
    DSFX_I3DL2_ENVIRONMENT_PRESET_HANGAR,
    DSFX_I3DL2_ENVIRONMENT_PRESET_CARPETEDHALLWAY,
    DSFX_I3DL2_ENVIRONMENT_PRESET_HALLWAY,
    DSFX_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR,
    DSFX_I3DL2_ENVIRONMENT_PRESET_ALLEY,
    DSFX_I3DL2_ENVIRONMENT_PRESET_FOREST,
    DSFX_I3DL2_ENVIRONMENT_PRESET_CITY,
    DSFX_I3DL2_ENVIRONMENT_PRESET_MOUNTAINS,
    DSFX_I3DL2_ENVIRONMENT_PRESET_QUARRY,
    DSFX_I3DL2_ENVIRONMENT_PRESET_PLAIN,
    DSFX_I3DL2_ENVIRONMENT_PRESET_PARKINGLOT,
    DSFX_I3DL2_ENVIRONMENT_PRESET_SEWERPIPE,
    DSFX_I3DL2_ENVIRONMENT_PRESET_UNDERWATER,
    DSFX_I3DL2_ENVIRONMENT_PRESET_SMALLROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEROOM,
    DSFX_I3DL2_ENVIRONMENT_PRESET_MEDIUMHALL,
    DSFX_I3DL2_ENVIRONMENT_PRESET_LARGEHALL,
    DSFX_I3DL2_ENVIRONMENT_PRESET_PLATE
};

#define I3DL2_ENVIRONMENT_PRESET_DEFAULT         -1000, -100, 0.0f, 1.49f, 0.83f, -2602, 0.007f,   200, 0.011f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_GENERIC         -1000, -100, 0.0f, 1.49f, 0.83f, -2602, 0.007f,   200, 0.011f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_PADDEDCELL      -1000,-6000, 0.0f, 0.17f, 0.10f, -1204, 0.001f,   207, 0.002f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_ROOM            -1000, -454, 0.0f, 0.40f, 0.83f, -1646, 0.002f,    53, 0.003f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_BATHROOM        -1000,-1200, 0.0f, 1.49f, 0.54f,  -370, 0.007f,  1030, 0.011f, 100.0f,  60.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_LIVINGROOM      -1000,-6000, 0.0f, 0.50f, 0.10f, -1376, 0.003f, -1104, 0.004f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_STONEROOM       -1000, -300, 0.0f, 2.31f, 0.64f,  -711, 0.012f,    83, 0.017f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_AUDITORIUM      -1000, -476, 0.0f, 4.32f, 0.59f,  -789, 0.020f,  -289, 0.030f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_CONCERTHALL     -1000, -500, 0.0f, 3.92f, 0.70f, -1230, 0.020f,    -2, 0.029f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_CAVE            -1000,    0, 0.0f, 2.91f, 1.30f,  -602, 0.015f,  -302, 0.022f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_ARENA           -1000, -698, 0.0f, 7.24f, 0.33f, -1166, 0.020f,    16, 0.030f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_HANGAR          -1000,-1000, 0.0f,10.05f, 0.23f,  -602, 0.020f,   198, 0.030f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_CARPETEDHALLWAY -1000,-4000, 0.0f, 0.30f, 0.10f, -1831, 0.002f, -1630, 0.030f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_HALLWAY         -1000, -300, 0.0f, 1.49f, 0.59f, -1219, 0.007f,   441, 0.011f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR   -1000, -237, 0.0f, 2.70f, 0.79f, -1214, 0.013f,   395, 0.020f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_ALLEY           -1000, -270, 0.0f, 1.49f, 0.86f, -1204, 0.007f,    -4, 0.011f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_FOREST          -1000,-3300, 0.0f, 1.49f, 0.54f, -2560, 0.162f,  -613, 0.088f,  79.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_CITY            -1000, -800, 0.0f, 1.49f, 0.67f, -2273, 0.007f, -2217, 0.011f,  50.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_MOUNTAINS       -1000,-2500, 0.0f, 1.49f, 0.21f, -2780, 0.300f, -2014, 0.100f,  27.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_QUARRY          -1000,-1000, 0.0f, 1.49f, 0.83f,-10000, 0.061f,   500, 0.025f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_PLAIN           -1000,-2000, 0.0f, 1.49f, 0.50f, -2466, 0.179f, -2514, 0.100f,  21.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_PARKINGLOT      -1000,    0, 0.0f, 1.65f, 1.50f, -1363, 0.008f, -1153, 0.012f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_SEWERPIPE       -1000,-1000, 0.0f, 2.81f, 0.14f,   429, 0.014f,   648, 0.021f,  80.0f,  60.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_UNDERWATER      -1000,-4000, 0.0f, 1.49f, 0.10f,  -449, 0.007f,  1700, 0.011f, 100.0f, 100.0f, 5000.0f

#define I3DL2_ENVIRONMENT_PRESET_SMALLROOM       -1000, -600, 0.0f, 1.10f, 0.83f,  -400, 0.005f,   500, 0.010f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_MEDIUMROOM      -1000, -600, 0.0f, 1.30f, 0.83f, -1000, 0.010f,  -200, 0.020f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_LARGEROOM       -1000, -600, 0.0f, 1.50f, 0.83f, -1600, 0.020f, -1000, 0.040f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_MEDIUMHALL      -1000, -600, 0.0f, 1.80f, 0.70f, -1300, 0.015f,  -800, 0.030f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_LARGEHALL       -1000, -600, 0.0f, 1.80f, 0.70f, -2000, 0.030f, -1400, 0.060f, 100.0f, 100.0f, 5000.0f
#define I3DL2_ENVIRONMENT_PRESET_PLATE           -1000, -200, 0.0f, 1.30f, 0.90f,     0, 0.002f,     0, 0.010f, 100.0f,  75.0f, 5000.0f

#define DS3DALG_DEFAULT GUID_NULL

DEFINE_GUID(DS3DALG_NO_VIRTUALIZATION, 0xc241333f, 0x1c1b, 0x11d2, 0x94, 0xf5, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

DEFINE_GUID(DS3DALG_HRTF_FULL, 0xc2413340, 0x1c1b, 0x11d2, 0x94, 0xf5, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

DEFINE_GUID(DS3DALG_HRTF_LIGHT, 0xc2413342, 0x1c1b, 0x11d2, 0x94, 0xf5, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

#if DIRECTSOUND_VERSION >= 0x0800

DEFINE_GUID(GUID_DSFX_STANDARD_GARGLE, 0xdafd8210, 0x5711, 0x4b91, 0x9f, 0xe3, 0xf7, 0x5b, 0x7a, 0xe2, 0x79, 0xbf);

DEFINE_GUID(GUID_DSFX_STANDARD_CHORUS, 0xefe6629c, 0x81f7, 0x4281, 0xbd, 0x91, 0xc9, 0xd6, 0x04, 0xa9, 0x5a, 0xf6);

DEFINE_GUID(GUID_DSFX_STANDARD_FLANGER, 0xefca3d92, 0xdfd8, 0x4672, 0xa6, 0x03, 0x74, 0x20, 0x89, 0x4b, 0xad, 0x98);

DEFINE_GUID(GUID_DSFX_STANDARD_ECHO, 0xef3e932c, 0xd40b, 0x4f51, 0x8c, 0xcf, 0x3f, 0x98, 0xf1, 0xb2, 0x9d, 0x5d);

DEFINE_GUID(GUID_DSFX_STANDARD_DISTORTION, 0xef114c90, 0xcd1d, 0x484e, 0x96, 0xe5, 0x09, 0xcf, 0xaf, 0x91, 0x2a, 0x21);

DEFINE_GUID(GUID_DSFX_STANDARD_COMPRESSOR, 0xef011f79, 0x4000, 0x406d, 0x87, 0xaf, 0xbf, 0xfb, 0x3f, 0xc3, 0x9d, 0x57);

DEFINE_GUID(GUID_DSFX_STANDARD_PARAMEQ, 0x120ced89, 0x3bf4, 0x4173, 0xa1, 0x32, 0x3c, 0xb4, 0x06, 0xcf, 0x32, 0x31);

DEFINE_GUID(GUID_DSFX_STANDARD_I3DL2REVERB, 0xef985e71, 0xd5c7, 0x42d4, 0xba, 0x4d, 0x2d, 0x07, 0x3e, 0x2e, 0x96, 0xf4);

DEFINE_GUID(GUID_DSFX_WAVES_REVERB, 0x87fc0268, 0x9a55, 0x4360, 0x95, 0xaa, 0x00, 0x4a, 0x1d, 0x9d, 0xe2, 0x6c);

DEFINE_GUID(GUID_DSCFX_CLASS_AEC, 0xBF963D80L, 0xC559, 0x11D0, 0x8A, 0x2B, 0x00, 0xA0, 0xC9, 0x25, 0x5A, 0xC1);

DEFINE_GUID(GUID_DSCFX_MS_AEC, 0xcdebb919, 0x379a, 0x488a, 0x87, 0x65, 0xf5, 0x3c, 0xfd, 0x36, 0xde, 0x40);

DEFINE_GUID(GUID_DSCFX_SYSTEM_AEC, 0x1c22c56d, 0x9879, 0x4f5b, 0xa3, 0x89, 0x27, 0x99, 0x6d, 0xdc, 0x28, 0x10);

DEFINE_GUID(GUID_DSCFX_CLASS_NS, 0xe07f903f, 0x62fd, 0x4e60, 0x8c, 0xdd, 0xde, 0xa7, 0x23, 0x66, 0x65, 0xb5);

DEFINE_GUID(GUID_DSCFX_MS_NS, 0x11c5c73b, 0x66e9, 0x4ba1, 0xa0, 0xba, 0xe8, 0x14, 0xc6, 0xee, 0xd9, 0x2d);

DEFINE_GUID(GUID_DSCFX_SYSTEM_NS, 0x5ab0882e, 0x7274, 0x4516, 0x87, 0x7d, 0x4e, 0xee, 0x99, 0xba, 0x4f, 0xd0);

#endif

#endif

#ifdef __cplusplus
};
#endif
#ifndef __DINPUT_INCLUDED__
#define __DINPUT_INCLUDED__

#ifndef DIJ_RINGZERO

#ifdef _WIN32
#define COM_NO_WINDOWS_H
#include <objbase.h>
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DIRECTINPUT_HEADER_VERSION  0x0800
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
#endif

#ifndef DIJ_RINGZERO

DEFINE_GUID(CLSID_DirectInput,       0x25E609E0,0xB259,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(CLSID_DirectInputDevice, 0x25E609E1,0xB259,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(CLSID_DirectInput8,      0x25E609E4,0xB259,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(CLSID_DirectInputDevice8,0x25E609E5,0xB259,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(IID_IDirectInputA,     0x89521360,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputW,     0x89521361,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput2A,    0x5944E662,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput2W,    0x5944E663,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput7A,    0x9A4CB684,0x236D,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInput7W,    0x9A4CB685,0x236D,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInput8A,    0xBF798030,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);
DEFINE_GUID(IID_IDirectInput8W,    0xBF798031,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);
DEFINE_GUID(IID_IDirectInputDeviceA, 0x5944E680,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDeviceW, 0x5944E681,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice2A,0x5944E682,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice2W,0x5944E683,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice7A,0x57D7C6BC,0x2356,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInputDevice7W,0x57D7C6BD,0x2356,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInputDevice8A,0x54D41080,0xDC15,0x4833,0xA4,0x1B,0x74,0x8F,0x73,0xA3,0x81,0x79);
DEFINE_GUID(IID_IDirectInputDevice8W,0x54D41081,0xDC15,0x4833,0xA4,0x1B,0x74,0x8F,0x73,0xA3,0x81,0x79);
DEFINE_GUID(IID_IDirectInputEffect,  0xE7E1F7C0,0x88D2,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);

DEFINE_GUID(GUID_XAxis,   0xA36D02E0,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_YAxis,   0xA36D02E1,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_ZAxis,   0xA36D02E2,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RxAxis,  0xA36D02F4,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RyAxis,  0xA36D02F5,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RzAxis,  0xA36D02E3,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Slider,  0xA36D02E4,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_Button,  0xA36D02F0,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Key,     0x55728220,0xD33C,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_POV,     0xA36D02F2,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_Unknown, 0xA36D02F3,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_SysMouse,   0x6F1D2B60,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_SysKeyboard,0x6F1D2B61,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Joystick   ,0x6F1D2B70,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_SysMouseEm, 0x6F1D2B80,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_SysMouseEm2,0x6F1D2B81,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_SysKeyboardEm, 0x6F1D2B82,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_SysKeyboardEm2,0x6F1D2B83,0xD5A0,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_ConstantForce, 0x13541C20,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_RampForce,     0x13541C21,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Square,        0x13541C22,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Sine,          0x13541C23,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Triangle,      0x13541C24,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_SawtoothUp,    0x13541C25,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_SawtoothDown,  0x13541C26,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Spring,        0x13541C27,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Damper,        0x13541C28,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Inertia,       0x13541C29,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_Friction,      0x13541C2A,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(GUID_CustomForce,   0x13541C2B,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);

#endif

#if(DIRECTINPUT_VERSION >= 0x0500)

#define DIEFT_ALL                   0x00000000

#define DIEFT_CONSTANTFORCE         0x00000001
#define DIEFT_RAMPFORCE             0x00000002
#define DIEFT_PERIODIC              0x00000003
#define DIEFT_CONDITION             0x00000004
#define DIEFT_CUSTOMFORCE           0x00000005
#define DIEFT_HARDWARE              0x000000FF
#define DIEFT_FFATTACK              0x00000200
#define DIEFT_FFFADE                0x00000400
#define DIEFT_SATURATION            0x00000800
#define DIEFT_POSNEGCOEFFICIENTS    0x00001000
#define DIEFT_POSNEGSATURATION      0x00002000
#define DIEFT_DEADBAND              0x00004000
#define DIEFT_STARTDELAY            0x00008000
#define DIEFT_GETTYPE(n)            LOBYTE(n)

#define DI_DEGREES                  100
#define DI_FFNOMINALMAX             10000
#define DI_SECONDS                  1000000

typedef struct DICONSTANTFORCE {
    LONG  lMagnitude;
} DICONSTANTFORCE, *LPDICONSTANTFORCE;
typedef const DICONSTANTFORCE *LPCDICONSTANTFORCE;

typedef struct DIRAMPFORCE {
    LONG  lStart;
    LONG  lEnd;
} DIRAMPFORCE, *LPDIRAMPFORCE;
typedef const DIRAMPFORCE *LPCDIRAMPFORCE;

typedef struct DIPERIODIC {
    DWORD dwMagnitude;
    LONG  lOffset;
    DWORD dwPhase;
    DWORD dwPeriod;
} DIPERIODIC, *LPDIPERIODIC;
typedef const DIPERIODIC *LPCDIPERIODIC;

typedef struct DICONDITION {
    LONG  lOffset;
    LONG  lPositiveCoefficient;
    LONG  lNegativeCoefficient;
    DWORD dwPositiveSaturation;
    DWORD dwNegativeSaturation;
    LONG  lDeadBand;
} DICONDITION, *LPDICONDITION;
typedef const DICONDITION *LPCDICONDITION;

typedef struct DICUSTOMFORCE {
    DWORD cChannels;
    DWORD dwSamplePeriod;
    DWORD cSamples;
    LPLONG rglForceData;
} DICUSTOMFORCE, *LPDICUSTOMFORCE;
typedef const DICUSTOMFORCE *LPCDICUSTOMFORCE;

typedef struct DIENVELOPE {
    DWORD dwSize;
    DWORD dwAttackLevel;
    DWORD dwAttackTime;
    DWORD dwFadeLevel;
    DWORD dwFadeTime;
} DIENVELOPE, *LPDIENVELOPE;
typedef const DIENVELOPE *LPCDIENVELOPE;

typedef struct DIEFFECT_DX5 {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDuration;
    DWORD dwSamplePeriod;
    DWORD dwGain;
    DWORD dwTriggerButton;
    DWORD dwTriggerRepeatInterval;
    DWORD cAxes;
    LPDWORD rgdwAxes;
    LPLONG rglDirection;
    LPDIENVELOPE lpEnvelope;
    DWORD cbTypeSpecificParams;
    LPVOID lpvTypeSpecificParams;
} DIEFFECT_DX5, *LPDIEFFECT_DX5;
typedef const DIEFFECT_DX5 *LPCDIEFFECT_DX5;

typedef struct DIEFFECT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDuration;
    DWORD dwSamplePeriod;
    DWORD dwGain;
    DWORD dwTriggerButton;
    DWORD dwTriggerRepeatInterval;
    DWORD cAxes;
    LPDWORD rgdwAxes;
    LPLONG rglDirection;
    LPDIENVELOPE lpEnvelope;
    DWORD cbTypeSpecificParams;
    LPVOID lpvTypeSpecificParams;
#if(DIRECTINPUT_VERSION >= 0x0600)
    DWORD  dwStartDelay;
#endif
} DIEFFECT, *LPDIEFFECT;
typedef DIEFFECT DIEFFECT_DX6;
typedef LPDIEFFECT LPDIEFFECT_DX6;
typedef const DIEFFECT *LPCDIEFFECT;

#if(DIRECTINPUT_VERSION >= 0x0700)
#ifndef DIJ_RINGZERO
typedef struct DIFILEEFFECT{
    DWORD       dwSize;
    GUID        GuidEffect;
    LPCDIEFFECT lpDiEffect;
    CHAR        szFriendlyName[MAX_PATH];
}DIFILEEFFECT, *LPDIFILEEFFECT;
typedef const DIFILEEFFECT *LPCDIFILEEFFECT;
typedef BOOL (FAR PASCAL * LPDIENUMEFFECTSINFILECALLBACK)(LPCDIFILEEFFECT , LPVOID);
#endif
#endif

#define DIEFF_OBJECTIDS             0x00000001
#define DIEFF_OBJECTOFFSETS         0x00000002
#define DIEFF_CARTESIAN             0x00000010
#define DIEFF_POLAR                 0x00000020
#define DIEFF_SPHERICAL             0x00000040

#define DIEP_DURATION               0x00000001
#define DIEP_SAMPLEPERIOD           0x00000002
#define DIEP_GAIN                   0x00000004
#define DIEP_TRIGGERBUTTON          0x00000008
#define DIEP_TRIGGERREPEATINTERVAL  0x00000010
#define DIEP_AXES                   0x00000020
#define DIEP_DIRECTION              0x00000040
#define DIEP_ENVELOPE               0x00000080
#define DIEP_TYPESPECIFICPARAMS     0x00000100
#if(DIRECTINPUT_VERSION >= 0x0600)
#define DIEP_STARTDELAY             0x00000200
#define DIEP_ALLPARAMS_DX5          0x000001FF
#define DIEP_ALLPARAMS              0x000003FF
#else
#define DIEP_ALLPARAMS              0x000001FF
#endif
#define DIEP_START                  0x20000000
#define DIEP_NORESTART              0x40000000
#define DIEP_NODOWNLOAD             0x80000000
#define DIEB_NOTRIGGER              0xFFFFFFFF

#define DIES_SOLO                   0x00000001
#define DIES_NODOWNLOAD             0x80000000

#define DIEGES_PLAYING              0x00000001
#define DIEGES_EMULATED             0x00000002

typedef struct DIEFFESCAPE {
    DWORD   dwSize;
    DWORD   dwCommand;
    LPVOID  lpvInBuffer;
    DWORD   cbInBuffer;
    LPVOID  lpvOutBuffer;
    DWORD   cbOutBuffer;
} DIEFFESCAPE, *LPDIEFFESCAPE;

#ifndef DIJ_RINGZERO

#undef INTERFACE
#define INTERFACE IDirectInputEffect

DECLARE_INTERFACE_(IDirectInputEffect, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
    STDMETHOD(GetEffectGuid)(THIS_ LPGUID) PURE;
    STDMETHOD(GetParameters)(THIS_ LPDIEFFECT,DWORD) PURE;
    STDMETHOD(SetParameters)(THIS_ LPCDIEFFECT,DWORD) PURE;
    STDMETHOD(Start)(THIS_ DWORD,DWORD) PURE;
    STDMETHOD(Stop)(THIS) PURE;
    STDMETHOD(GetEffectStatus)(THIS_ LPDWORD) PURE;
    STDMETHOD(Download)(THIS) PURE;
    STDMETHOD(Unload)(THIS) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
};

typedef struct IDirectInputEffect *LPDIRECTINPUTEFFECT;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInputEffect_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInputEffect_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInputEffect_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInputEffect_Initialize(p,a,b,c) (p)->lpVtbl->Initialize(p,a,b,c)
#define IDirectInputEffect_GetEffectGuid(p,a) (p)->lpVtbl->GetEffectGuid(p,a)
#define IDirectInputEffect_GetParameters(p,a,b) (p)->lpVtbl->GetParameters(p,a,b)
#define IDirectInputEffect_SetParameters(p,a,b) (p)->lpVtbl->SetParameters(p,a,b)
#define IDirectInputEffect_Start(p,a,b) (p)->lpVtbl->Start(p,a,b)
#define IDirectInputEffect_Stop(p) (p)->lpVtbl->Stop(p)
#define IDirectInputEffect_GetEffectStatus(p,a) (p)->lpVtbl->GetEffectStatus(p,a)
#define IDirectInputEffect_Download(p) (p)->lpVtbl->Download(p)
#define IDirectInputEffect_Unload(p) (p)->lpVtbl->Unload(p)
#define IDirectInputEffect_Escape(p,a) (p)->lpVtbl->Escape(p,a)
#else
#define IDirectInputEffect_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInputEffect_AddRef(p) (p)->AddRef()
#define IDirectInputEffect_Release(p) (p)->Release()
#define IDirectInputEffect_Initialize(p,a,b,c) (p)->Initialize(a,b,c)
#define IDirectInputEffect_GetEffectGuid(p,a) (p)->GetEffectGuid(a)
#define IDirectInputEffect_GetParameters(p,a,b) (p)->GetParameters(a,b)
#define IDirectInputEffect_SetParameters(p,a,b) (p)->SetParameters(a,b)
#define IDirectInputEffect_Start(p,a,b) (p)->Start(a,b)
#define IDirectInputEffect_Stop(p) (p)->Stop()
#define IDirectInputEffect_GetEffectStatus(p,a) (p)->GetEffectStatus(a)
#define IDirectInputEffect_Download(p) (p)->Download()
#define IDirectInputEffect_Unload(p) (p)->Unload()
#define IDirectInputEffect_Escape(p,a) (p)->Escape(a)
#endif

#endif

#endif

#if DIRECTINPUT_VERSION <= 0x700
#define DIDEVTYPE_DEVICE        1
#define DIDEVTYPE_MOUSE         2
#define DIDEVTYPE_KEYBOARD      3
#define DIDEVTYPE_JOYSTICK      4

#else
#define DI8DEVCLASS_ALL             0
#define DI8DEVCLASS_DEVICE          1
#define DI8DEVCLASS_POINTER         2
#define DI8DEVCLASS_KEYBOARD        3
#define DI8DEVCLASS_GAMECTRL        4

#define DI8DEVTYPE_DEVICE           0x11
#define DI8DEVTYPE_MOUSE            0x12
#define DI8DEVTYPE_KEYBOARD         0x13
#define DI8DEVTYPE_JOYSTICK         0x14
#define DI8DEVTYPE_GAMEPAD          0x15
#define DI8DEVTYPE_DRIVING          0x16
#define DI8DEVTYPE_FLIGHT           0x17
#define DI8DEVTYPE_1STPERSON        0x18
#define DI8DEVTYPE_DEVICECTRL       0x19
#define DI8DEVTYPE_SCREENPOINTER    0x1A
#define DI8DEVTYPE_REMOTE           0x1B
#define DI8DEVTYPE_SUPPLEMENTAL     0x1C
#endif

#define DIDEVTYPE_HID           0x00010000

#if DIRECTINPUT_VERSION <= 0x700
#define DIDEVTYPEMOUSE_UNKNOWN          1
#define DIDEVTYPEMOUSE_TRADITIONAL      2
#define DIDEVTYPEMOUSE_FINGERSTICK      3
#define DIDEVTYPEMOUSE_TOUCHPAD         4
#define DIDEVTYPEMOUSE_TRACKBALL        5

#define DIDEVTYPEKEYBOARD_UNKNOWN       0
#define DIDEVTYPEKEYBOARD_PCXT          1
#define DIDEVTYPEKEYBOARD_OLIVETTI      2
#define DIDEVTYPEKEYBOARD_PCAT          3
#define DIDEVTYPEKEYBOARD_PCENH         4
#define DIDEVTYPEKEYBOARD_NOKIA1050     5
#define DIDEVTYPEKEYBOARD_NOKIA9140     6
#define DIDEVTYPEKEYBOARD_NEC98         7
#define DIDEVTYPEKEYBOARD_NEC98LAPTOP   8
#define DIDEVTYPEKEYBOARD_NEC98106      9
#define DIDEVTYPEKEYBOARD_JAPAN106     10
#define DIDEVTYPEKEYBOARD_JAPANAX      11
#define DIDEVTYPEKEYBOARD_J3100        12

#define DIDEVTYPEJOYSTICK_UNKNOWN       1
#define DIDEVTYPEJOYSTICK_TRADITIONAL   2
#define DIDEVTYPEJOYSTICK_FLIGHTSTICK   3
#define DIDEVTYPEJOYSTICK_GAMEPAD       4
#define DIDEVTYPEJOYSTICK_RUDDER        5
#define DIDEVTYPEJOYSTICK_WHEEL         6
#define DIDEVTYPEJOYSTICK_HEADTRACKER   7

#else
#define DI8DEVTYPEMOUSE_UNKNOWN                     1
#define DI8DEVTYPEMOUSE_TRADITIONAL                 2
#define DI8DEVTYPEMOUSE_FINGERSTICK                 3
#define DI8DEVTYPEMOUSE_TOUCHPAD                    4
#define DI8DEVTYPEMOUSE_TRACKBALL                   5
#define DI8DEVTYPEMOUSE_ABSOLUTE                    6

#define DI8DEVTYPEKEYBOARD_UNKNOWN                  0
#define DI8DEVTYPEKEYBOARD_PCXT                     1
#define DI8DEVTYPEKEYBOARD_OLIVETTI                 2
#define DI8DEVTYPEKEYBOARD_PCAT                     3
#define DI8DEVTYPEKEYBOARD_PCENH                    4
#define DI8DEVTYPEKEYBOARD_NOKIA1050                5
#define DI8DEVTYPEKEYBOARD_NOKIA9140                6
#define DI8DEVTYPEKEYBOARD_NEC98                    7
#define DI8DEVTYPEKEYBOARD_NEC98LAPTOP              8
#define DI8DEVTYPEKEYBOARD_NEC98106                 9
#define DI8DEVTYPEKEYBOARD_JAPAN106                10
#define DI8DEVTYPEKEYBOARD_JAPANAX                 11
#define DI8DEVTYPEKEYBOARD_J3100                   12

#define DI8DEVTYPE_LIMITEDGAMESUBTYPE               1

#define DI8DEVTYPEJOYSTICK_LIMITED                  DI8DEVTYPE_LIMITEDGAMESUBTYPE
#define DI8DEVTYPEJOYSTICK_STANDARD                 2

#define DI8DEVTYPEGAMEPAD_LIMITED                   DI8DEVTYPE_LIMITEDGAMESUBTYPE
#define DI8DEVTYPEGAMEPAD_STANDARD                  2
#define DI8DEVTYPEGAMEPAD_TILT                      3

#define DI8DEVTYPEDRIVING_LIMITED                   DI8DEVTYPE_LIMITEDGAMESUBTYPE
#define DI8DEVTYPEDRIVING_COMBINEDPEDALS            2
#define DI8DEVTYPEDRIVING_DUALPEDALS                3
#define DI8DEVTYPEDRIVING_THREEPEDALS               4
#define DI8DEVTYPEDRIVING_HANDHELD                  5

#define DI8DEVTYPEFLIGHT_LIMITED                    DI8DEVTYPE_LIMITEDGAMESUBTYPE
#define DI8DEVTYPEFLIGHT_STICK                      2
#define DI8DEVTYPEFLIGHT_YOKE                       3
#define DI8DEVTYPEFLIGHT_RC                         4

#define DI8DEVTYPE1STPERSON_LIMITED                 DI8DEVTYPE_LIMITEDGAMESUBTYPE
#define DI8DEVTYPE1STPERSON_UNKNOWN                 2
#define DI8DEVTYPE1STPERSON_SIXDOF                  3
#define DI8DEVTYPE1STPERSON_SHOOTER                 4

#define DI8DEVTYPESCREENPTR_UNKNOWN                 2
#define DI8DEVTYPESCREENPTR_LIGHTGUN                3
#define DI8DEVTYPESCREENPTR_LIGHTPEN                4
#define DI8DEVTYPESCREENPTR_TOUCH                   5

#define DI8DEVTYPEREMOTE_UNKNOWN                    2

#define DI8DEVTYPEDEVICECTRL_UNKNOWN                2
#define DI8DEVTYPEDEVICECTRL_COMMSSELECTION         3
#define DI8DEVTYPEDEVICECTRL_COMMSSELECTION_HARDWIRED 4

#define DI8DEVTYPESUPPLEMENTAL_UNKNOWN              2
#define DI8DEVTYPESUPPLEMENTAL_2NDHANDCONTROLLER    3
#define DI8DEVTYPESUPPLEMENTAL_HEADTRACKER          4
#define DI8DEVTYPESUPPLEMENTAL_HANDTRACKER          5
#define DI8DEVTYPESUPPLEMENTAL_SHIFTSTICKGATE       6
#define DI8DEVTYPESUPPLEMENTAL_SHIFTER              7
#define DI8DEVTYPESUPPLEMENTAL_THROTTLE             8
#define DI8DEVTYPESUPPLEMENTAL_SPLITTHROTTLE        9
#define DI8DEVTYPESUPPLEMENTAL_COMBINEDPEDALS      10
#define DI8DEVTYPESUPPLEMENTAL_DUALPEDALS          11
#define DI8DEVTYPESUPPLEMENTAL_THREEPEDALS         12
#define DI8DEVTYPESUPPLEMENTAL_RUDDERPEDALS        13
#endif

#define GET_DIDEVICE_TYPE(dwDevType)    LOBYTE(dwDevType)
#define GET_DIDEVICE_SUBTYPE(dwDevType) HIBYTE(dwDevType)

#if(DIRECTINPUT_VERSION >= 0x0500)

typedef struct DIDEVCAPS_DX3 {
    DWORD   dwSize;
    DWORD   dwFlags;
    DWORD   dwDevType;
    DWORD   dwAxes;
    DWORD   dwButtons;
    DWORD   dwPOVs;
} DIDEVCAPS_DX3, *LPDIDEVCAPS_DX3;
#endif

typedef struct DIDEVCAPS {
    DWORD   dwSize;
    DWORD   dwFlags;
    DWORD   dwDevType;
    DWORD   dwAxes;
    DWORD   dwButtons;
    DWORD   dwPOVs;
#if(DIRECTINPUT_VERSION >= 0x0500)
    DWORD   dwFFSamplePeriod;
    DWORD   dwFFMinTimeResolution;
    DWORD   dwFirmwareRevision;
    DWORD   dwHardwareRevision;
    DWORD   dwFFDriverVersion;
#endif
} DIDEVCAPS, *LPDIDEVCAPS;

#define DIDC_ATTACHED           0x00000001
#define DIDC_POLLEDDEVICE       0x00000002
#define DIDC_EMULATED           0x00000004
#define DIDC_POLLEDDATAFORMAT   0x00000008
#if(DIRECTINPUT_VERSION >= 0x0500)
#define DIDC_FORCEFEEDBACK      0x00000100
#define DIDC_FFATTACK           0x00000200
#define DIDC_FFFADE             0x00000400
#define DIDC_SATURATION         0x00000800
#define DIDC_POSNEGCOEFFICIENTS 0x00001000
#define DIDC_POSNEGSATURATION   0x00002000
#define DIDC_DEADBAND           0x00004000
#endif
#define DIDC_STARTDELAY         0x00008000
#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIDC_ALIAS              0x00010000
#define DIDC_PHANTOM            0x00020000
#endif
#if(DIRECTINPUT_VERSION >= 0x0800)
#define DIDC_HIDDEN             0x00040000
#endif

#define DIDFT_ALL           0x00000000

#define DIDFT_RELAXIS       0x00000001
#define DIDFT_ABSAXIS       0x00000002
#define DIDFT_AXIS          0x00000003

#define DIDFT_PSHBUTTON     0x00000004
#define DIDFT_TGLBUTTON     0x00000008
#define DIDFT_BUTTON        0x0000000C

#define DIDFT_POV           0x00000010
#define DIDFT_COLLECTION    0x00000040
#define DIDFT_NODATA        0x00000080

#define DIDFT_ANYINSTANCE   0x00FFFF00
#define DIDFT_INSTANCEMASK  DIDFT_ANYINSTANCE
#define DIDFT_MAKEINSTANCE(n) ((WORD)(n) << 8)
#define DIDFT_GETTYPE(n)     LOBYTE(n)
#define DIDFT_GETINSTANCE(n) LOWORD((n) >> 8)
#define DIDFT_FFACTUATOR        0x01000000
#define DIDFT_FFEFFECTTRIGGER   0x02000000
#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIDFT_OUTPUT            0x10000000
#define DIDFT_VENDORDEFINED     0x04000000
#define DIDFT_ALIAS             0x08000000
#endif

#define DIDFT_ENUMCOLLECTION(n) ((WORD)(n) << 8)
#define DIDFT_NOCOLLECTION      0x00FFFF00

#ifndef DIJ_RINGZERO

typedef struct _DIOBJECTDATAFORMAT {
    const GUID *pguid;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
} DIOBJECTDATAFORMAT, *LPDIOBJECTDATAFORMAT;
typedef const DIOBJECTDATAFORMAT *LPCDIOBJECTDATAFORMAT;

typedef struct _DIDATAFORMAT {
    DWORD   dwSize;
    DWORD   dwObjSize;
    DWORD   dwFlags;
    DWORD   dwDataSize;
    DWORD   dwNumObjs;
    LPDIOBJECTDATAFORMAT rgodf;
} DIDATAFORMAT, *LPDIDATAFORMAT;
typedef const DIDATAFORMAT *LPCDIDATAFORMAT;

#define DIDF_ABSAXIS            0x00000001
#define DIDF_RELAXIS            0x00000002

#ifdef __cplusplus
extern "C" {
#endif
extern const DIDATAFORMAT c_dfDIMouse;

#if(DIRECTINPUT_VERSION >= 0x0700)
extern const DIDATAFORMAT c_dfDIMouse2;
#endif

extern const DIDATAFORMAT c_dfDIKeyboard;

#if(DIRECTINPUT_VERSION >= 0x0500)
extern const DIDATAFORMAT c_dfDIJoystick;
extern const DIDATAFORMAT c_dfDIJoystick2;
#endif

#ifdef __cplusplus
};
#endif

#if DIRECTINPUT_VERSION > 0x0700

typedef struct _DIACTIONA {
                UINT_PTR    uAppData;
                DWORD       dwSemantic;
    OPTIONAL    DWORD       dwFlags;
    OPTIONAL    union {
                    LPCSTR      lptszActionName;
                    UINT        uResIdString;
                };
    OPTIONAL    GUID        guidInstance;
    OPTIONAL    DWORD       dwObjID;
    OPTIONAL    DWORD       dwHow;
} DIACTIONA, *LPDIACTIONA ;
typedef struct _DIACTIONW {
                UINT_PTR    uAppData;
                DWORD       dwSemantic;
    OPTIONAL    DWORD       dwFlags;
    OPTIONAL    union {
                    LPCWSTR     lptszActionName;
                    UINT        uResIdString;
                };
    OPTIONAL    GUID        guidInstance;
    OPTIONAL    DWORD       dwObjID;
    OPTIONAL    DWORD       dwHow;
} DIACTIONW, *LPDIACTIONW ;
#ifdef UNICODE
typedef DIACTIONW DIACTION;
typedef LPDIACTIONW LPDIACTION;
#else
typedef DIACTIONA DIACTION;
typedef LPDIACTIONA LPDIACTION;
#endif

typedef const DIACTIONA *LPCDIACTIONA;
typedef const DIACTIONW *LPCDIACTIONW;
#ifdef UNICODE
typedef DIACTIONW DIACTION;
typedef LPCDIACTIONW LPCDIACTION;
#else
typedef DIACTIONA DIACTION;
typedef LPCDIACTIONA LPCDIACTION;
#endif
typedef const DIACTION *LPCDIACTION;

#define DIA_FORCEFEEDBACK       0x00000001
#define DIA_APPMAPPED           0x00000002
#define DIA_APPNOMAP            0x00000004
#define DIA_NORANGE             0x00000008
#define DIA_APPFIXED            0x00000010

#define DIAH_UNMAPPED           0x00000000
#define DIAH_USERCONFIG         0x00000001
#define DIAH_APPREQUESTED       0x00000002
#define DIAH_HWAPP              0x00000004
#define DIAH_HWDEFAULT          0x00000008
#define DIAH_DEFAULT            0x00000020
#define DIAH_ERROR              0x80000000

typedef struct _DIACTIONFORMATA {
                DWORD       dwSize;
                DWORD       dwActionSize;
                DWORD       dwDataSize;
                DWORD       dwNumActions;
                LPDIACTIONA rgoAction;
                GUID        guidActionMap;
                DWORD       dwGenre;
                DWORD       dwBufferSize;
    OPTIONAL    LONG        lAxisMin;
    OPTIONAL    LONG        lAxisMax;
    OPTIONAL    HINSTANCE   hInstString;
                FILETIME    ftTimeStamp;
                DWORD       dwCRC;
                CHAR        tszActionMap[MAX_PATH];
} DIACTIONFORMATA, *LPDIACTIONFORMATA;
typedef struct _DIACTIONFORMATW {
                DWORD       dwSize;
                DWORD       dwActionSize;
                DWORD       dwDataSize;
                DWORD       dwNumActions;
                LPDIACTIONW rgoAction;
                GUID        guidActionMap;
                DWORD       dwGenre;
                DWORD       dwBufferSize;
    OPTIONAL    LONG        lAxisMin;
    OPTIONAL    LONG        lAxisMax;
    OPTIONAL    HINSTANCE   hInstString;
                FILETIME    ftTimeStamp;
                DWORD       dwCRC;
                WCHAR       tszActionMap[MAX_PATH];
} DIACTIONFORMATW, *LPDIACTIONFORMATW;
#ifdef UNICODE
typedef DIACTIONFORMATW DIACTIONFORMAT;
typedef LPDIACTIONFORMATW LPDIACTIONFORMAT;
#else
typedef DIACTIONFORMATA DIACTIONFORMAT;
typedef LPDIACTIONFORMATA LPDIACTIONFORMAT;
#endif
typedef const DIACTIONFORMATA *LPCDIACTIONFORMATA;
typedef const DIACTIONFORMATW *LPCDIACTIONFORMATW;
#ifdef UNICODE
typedef DIACTIONFORMATW DIACTIONFORMAT;
typedef LPCDIACTIONFORMATW LPCDIACTIONFORMAT;
#else
typedef DIACTIONFORMATA DIACTIONFORMAT;
typedef LPCDIACTIONFORMATA LPCDIACTIONFORMAT;
#endif
typedef const DIACTIONFORMAT *LPCDIACTIONFORMAT;

#define DIAFTS_NEWDEVICELOW     0xFFFFFFFF
#define DIAFTS_NEWDEVICEHIGH    0xFFFFFFFF
#define DIAFTS_UNUSEDDEVICELOW  0x00000000
#define DIAFTS_UNUSEDDEVICEHIGH 0x00000000

#define DIDBAM_DEFAULT          0x00000000
#define DIDBAM_PRESERVE         0x00000001
#define DIDBAM_INITIALIZE       0x00000002
#define DIDBAM_HWDEFAULTS       0x00000004

#define DIDSAM_DEFAULT          0x00000000
#define DIDSAM_NOUSER           0x00000001
#define DIDSAM_FORCESAVE        0x00000002

#define DICD_DEFAULT            0x00000000
#define DICD_EDIT               0x00000001

#ifndef D3DCOLOR_DEFINED
typedef DWORD D3DCOLOR;
#define D3DCOLOR_DEFINED
#endif

typedef struct _DICOLORSET{
    DWORD dwSize;
    D3DCOLOR cTextFore;
    D3DCOLOR cTextHighlight;
    D3DCOLOR cCalloutLine;
    D3DCOLOR cCalloutHighlight;
    D3DCOLOR cBorder;
    D3DCOLOR cControlFill;
    D3DCOLOR cHighlightFill;
    D3DCOLOR cAreaFill;
} DICOLORSET, *LPDICOLORSET;
typedef const DICOLORSET *LPCDICOLORSET;

typedef struct _DICONFIGUREDEVICESPARAMSA{
     DWORD             dwSize;
     DWORD             dwcUsers;
     LPSTR             lptszUserNames;
     DWORD             dwcFormats;
     LPDIACTIONFORMATA lprgFormats;
     HWND              hwnd;
     DICOLORSET        dics;
     IUnknown FAR *    lpUnkDDSTarget;
} DICONFIGUREDEVICESPARAMSA, *LPDICONFIGUREDEVICESPARAMSA;
typedef struct _DICONFIGUREDEVICESPARAMSW{
     DWORD             dwSize;
     DWORD             dwcUsers;
     LPWSTR            lptszUserNames;
     DWORD             dwcFormats;
     LPDIACTIONFORMATW lprgFormats;
     HWND              hwnd;
     DICOLORSET        dics;
     IUnknown FAR *    lpUnkDDSTarget;
} DICONFIGUREDEVICESPARAMSW, *LPDICONFIGUREDEVICESPARAMSW;
#ifdef UNICODE
typedef DICONFIGUREDEVICESPARAMSW DICONFIGUREDEVICESPARAMS;
typedef LPDICONFIGUREDEVICESPARAMSW LPDICONFIGUREDEVICESPARAMS;
#else
typedef DICONFIGUREDEVICESPARAMSA DICONFIGUREDEVICESPARAMS;
typedef LPDICONFIGUREDEVICESPARAMSA LPDICONFIGUREDEVICESPARAMS;
#endif
typedef const DICONFIGUREDEVICESPARAMSA *LPCDICONFIGUREDEVICESPARAMSA;
typedef const DICONFIGUREDEVICESPARAMSW *LPCDICONFIGUREDEVICESPARAMSW;
#ifdef UNICODE
typedef DICONFIGUREDEVICESPARAMSW DICONFIGUREDEVICESPARAMS;
typedef LPCDICONFIGUREDEVICESPARAMSW LPCDICONFIGUREDEVICESPARAMS;
#else
typedef DICONFIGUREDEVICESPARAMSA DICONFIGUREDEVICESPARAMS;
typedef LPCDICONFIGUREDEVICESPARAMSA LPCDICONFIGUREDEVICESPARAMS;
#endif
typedef const DICONFIGUREDEVICESPARAMS *LPCDICONFIGUREDEVICESPARAMS;

#define DIDIFT_CONFIGURATION    0x00000001
#define DIDIFT_OVERLAY          0x00000002

#define DIDAL_CENTERED      0x00000000
#define DIDAL_LEFTALIGNED   0x00000001
#define DIDAL_RIGHTALIGNED  0x00000002
#define DIDAL_MIDDLE        0x00000000
#define DIDAL_TOPALIGNED    0x00000004
#define DIDAL_BOTTOMALIGNED 0x00000008

typedef struct _DIDEVICEIMAGEINFOA {
    CHAR        tszImagePath[MAX_PATH];
    DWORD       dwFlags;

    DWORD       dwViewID;
    RECT        rcOverlay;
    DWORD       dwObjID;
    DWORD       dwcValidPts;
    POINT       rgptCalloutLine[5];
    RECT        rcCalloutRect;
    DWORD       dwTextAlign;
} DIDEVICEIMAGEINFOA, *LPDIDEVICEIMAGEINFOA;
typedef struct _DIDEVICEIMAGEINFOW {
    WCHAR       tszImagePath[MAX_PATH];
    DWORD       dwFlags;

    DWORD       dwViewID;
    RECT        rcOverlay;
    DWORD       dwObjID;
    DWORD       dwcValidPts;
    POINT       rgptCalloutLine[5];
    RECT        rcCalloutRect;
    DWORD       dwTextAlign;
} DIDEVICEIMAGEINFOW, *LPDIDEVICEIMAGEINFOW;
#ifdef UNICODE
typedef DIDEVICEIMAGEINFOW DIDEVICEIMAGEINFO;
typedef LPDIDEVICEIMAGEINFOW LPDIDEVICEIMAGEINFO;
#else
typedef DIDEVICEIMAGEINFOA DIDEVICEIMAGEINFO;
typedef LPDIDEVICEIMAGEINFOA LPDIDEVICEIMAGEINFO;
#endif
typedef const DIDEVICEIMAGEINFOA *LPCDIDEVICEIMAGEINFOA;
typedef const DIDEVICEIMAGEINFOW *LPCDIDEVICEIMAGEINFOW;
#ifdef UNICODE
typedef DIDEVICEIMAGEINFOW DIDEVICEIMAGEINFO;
typedef LPCDIDEVICEIMAGEINFOW LPCDIDEVICEIMAGEINFO;
#else
typedef DIDEVICEIMAGEINFOA DIDEVICEIMAGEINFO;
typedef LPCDIDEVICEIMAGEINFOA LPCDIDEVICEIMAGEINFO;
#endif
typedef const DIDEVICEIMAGEINFO *LPCDIDEVICEIMAGEINFO;

typedef struct _DIDEVICEIMAGEINFOHEADERA {
    DWORD       dwSize;
    DWORD       dwSizeImageInfo;
    DWORD       dwcViews;
    DWORD       dwcButtons;
    DWORD       dwcAxes;
    DWORD       dwcPOVs;
    DWORD       dwBufferSize;
    DWORD       dwBufferUsed;
    LPDIDEVICEIMAGEINFOA lprgImageInfoArray;
} DIDEVICEIMAGEINFOHEADERA, *LPDIDEVICEIMAGEINFOHEADERA;
typedef struct _DIDEVICEIMAGEINFOHEADERW {
    DWORD       dwSize;
    DWORD       dwSizeImageInfo;
    DWORD       dwcViews;
    DWORD       dwcButtons;
    DWORD       dwcAxes;
    DWORD       dwcPOVs;
    DWORD       dwBufferSize;
    DWORD       dwBufferUsed;
    LPDIDEVICEIMAGEINFOW lprgImageInfoArray;
} DIDEVICEIMAGEINFOHEADERW, *LPDIDEVICEIMAGEINFOHEADERW;
#ifdef UNICODE
typedef DIDEVICEIMAGEINFOHEADERW DIDEVICEIMAGEINFOHEADER;
typedef LPDIDEVICEIMAGEINFOHEADERW LPDIDEVICEIMAGEINFOHEADER;
#else
typedef DIDEVICEIMAGEINFOHEADERA DIDEVICEIMAGEINFOHEADER;
typedef LPDIDEVICEIMAGEINFOHEADERA LPDIDEVICEIMAGEINFOHEADER;
#endif
typedef const DIDEVICEIMAGEINFOHEADERA *LPCDIDEVICEIMAGEINFOHEADERA;
typedef const DIDEVICEIMAGEINFOHEADERW *LPCDIDEVICEIMAGEINFOHEADERW;
#ifdef UNICODE
typedef DIDEVICEIMAGEINFOHEADERW DIDEVICEIMAGEINFOHEADER;
typedef LPCDIDEVICEIMAGEINFOHEADERW LPCDIDEVICEIMAGEINFOHEADER;
#else
typedef DIDEVICEIMAGEINFOHEADERA DIDEVICEIMAGEINFOHEADER;
typedef LPCDIDEVICEIMAGEINFOHEADERA LPCDIDEVICEIMAGEINFOHEADER;
#endif
typedef const DIDEVICEIMAGEINFOHEADER *LPCDIDEVICEIMAGEINFOHEADER;

#endif

#if(DIRECTINPUT_VERSION >= 0x0500)

typedef struct DIDEVICEOBJECTINSTANCE_DX3A {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    CHAR    tszName[MAX_PATH];
} DIDEVICEOBJECTINSTANCE_DX3A, *LPDIDEVICEOBJECTINSTANCE_DX3A;
typedef struct DIDEVICEOBJECTINSTANCE_DX3W {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    WCHAR   tszName[MAX_PATH];
} DIDEVICEOBJECTINSTANCE_DX3W, *LPDIDEVICEOBJECTINSTANCE_DX3W;
#ifdef UNICODE
typedef DIDEVICEOBJECTINSTANCE_DX3W DIDEVICEOBJECTINSTANCE_DX3;
typedef LPDIDEVICEOBJECTINSTANCE_DX3W LPDIDEVICEOBJECTINSTANCE_DX3;
#else
typedef DIDEVICEOBJECTINSTANCE_DX3A DIDEVICEOBJECTINSTANCE_DX3;
typedef LPDIDEVICEOBJECTINSTANCE_DX3A LPDIDEVICEOBJECTINSTANCE_DX3;
#endif
typedef const DIDEVICEOBJECTINSTANCE_DX3A *LPCDIDEVICEOBJECTINSTANCE_DX3A;
typedef const DIDEVICEOBJECTINSTANCE_DX3W *LPCDIDEVICEOBJECTINSTANCE_DX3W;
typedef const DIDEVICEOBJECTINSTANCE_DX3  *LPCDIDEVICEOBJECTINSTANCE_DX3;
#endif

typedef struct DIDEVICEOBJECTINSTANCEA {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    CHAR    tszName[MAX_PATH];
#if(DIRECTINPUT_VERSION >= 0x0500)
    DWORD   dwFFMaxForce;
    DWORD   dwFFForceResolution;
    WORD    wCollectionNumber;
    WORD    wDesignatorIndex;
    WORD    wUsagePage;
    WORD    wUsage;
    DWORD   dwDimension;
    WORD    wExponent;
    WORD    wReportId;
#endif
} DIDEVICEOBJECTINSTANCEA, *LPDIDEVICEOBJECTINSTANCEA;
typedef struct DIDEVICEOBJECTINSTANCEW {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    WCHAR   tszName[MAX_PATH];
#if(DIRECTINPUT_VERSION >= 0x0500)
    DWORD   dwFFMaxForce;
    DWORD   dwFFForceResolution;
    WORD    wCollectionNumber;
    WORD    wDesignatorIndex;
    WORD    wUsagePage;
    WORD    wUsage;
    DWORD   dwDimension;
    WORD    wExponent;
    WORD    wReportId;
#endif
} DIDEVICEOBJECTINSTANCEW, *LPDIDEVICEOBJECTINSTANCEW;
#ifdef UNICODE
typedef DIDEVICEOBJECTINSTANCEW DIDEVICEOBJECTINSTANCE;
typedef LPDIDEVICEOBJECTINSTANCEW LPDIDEVICEOBJECTINSTANCE;
#else
typedef DIDEVICEOBJECTINSTANCEA DIDEVICEOBJECTINSTANCE;
typedef LPDIDEVICEOBJECTINSTANCEA LPDIDEVICEOBJECTINSTANCE;
#endif
typedef const DIDEVICEOBJECTINSTANCEA *LPCDIDEVICEOBJECTINSTANCEA;
typedef const DIDEVICEOBJECTINSTANCEW *LPCDIDEVICEOBJECTINSTANCEW;
typedef const DIDEVICEOBJECTINSTANCE  *LPCDIDEVICEOBJECTINSTANCE;

typedef BOOL (FAR PASCAL * LPDIENUMDEVICEOBJECTSCALLBACKA)(LPCDIDEVICEOBJECTINSTANCEA, LPVOID);
typedef BOOL (FAR PASCAL * LPDIENUMDEVICEOBJECTSCALLBACKW)(LPCDIDEVICEOBJECTINSTANCEW, LPVOID);
#ifdef UNICODE
#define LPDIENUMDEVICEOBJECTSCALLBACK  LPDIENUMDEVICEOBJECTSCALLBACKW
#else
#define LPDIENUMDEVICEOBJECTSCALLBACK  LPDIENUMDEVICEOBJECTSCALLBACKA
#endif

#if(DIRECTINPUT_VERSION >= 0x0500)
#define DIDOI_FFACTUATOR        0x00000001
#define DIDOI_FFEFFECTTRIGGER   0x00000002
#define DIDOI_POLLED            0x00008000
#define DIDOI_ASPECTPOSITION    0x00000100
#define DIDOI_ASPECTVELOCITY    0x00000200
#define DIDOI_ASPECTACCEL       0x00000300
#define DIDOI_ASPECTFORCE       0x00000400
#define DIDOI_ASPECTMASK        0x00000F00
#endif
#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIDOI_GUIDISUSAGE       0x00010000
#endif

typedef struct DIPROPHEADER {
    DWORD   dwSize;
    DWORD   dwHeaderSize;
    DWORD   dwObj;
    DWORD   dwHow;
} DIPROPHEADER, *LPDIPROPHEADER;
typedef const DIPROPHEADER *LPCDIPROPHEADER;

#define DIPH_DEVICE             0
#define DIPH_BYOFFSET           1
#define DIPH_BYID               2
#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIPH_BYUSAGE            3

#define DIMAKEUSAGEDWORD(UsagePage, Usage) \
                                (DWORD)MAKELONG(Usage, UsagePage)
#endif

typedef struct DIPROPDWORD {
    DIPROPHEADER diph;
    DWORD   dwData;
} DIPROPDWORD, *LPDIPROPDWORD;
typedef const DIPROPDWORD *LPCDIPROPDWORD;

#if(DIRECTINPUT_VERSION >= 0x0800)
typedef struct DIPROPPOINTER {
    DIPROPHEADER diph;
    UINT_PTR uData;
} DIPROPPOINTER, *LPDIPROPPOINTER;
typedef const DIPROPPOINTER *LPCDIPROPPOINTER;
#endif

typedef struct DIPROPRANGE {
    DIPROPHEADER diph;
    LONG    lMin;
    LONG    lMax;
} DIPROPRANGE, *LPDIPROPRANGE;
typedef const DIPROPRANGE *LPCDIPROPRANGE;

#define DIPROPRANGE_NOMIN       ((LONG)0x80000000)
#define DIPROPRANGE_NOMAX       ((LONG)0x7FFFFFFF)

#if(DIRECTINPUT_VERSION >= 0x050a)
typedef struct DIPROPCAL {
    DIPROPHEADER diph;
    LONG    lMin;
    LONG    lCenter;
    LONG    lMax;
} DIPROPCAL, *LPDIPROPCAL;
typedef const DIPROPCAL *LPCDIPROPCAL;

typedef struct DIPROPGUIDANDPATH {
    DIPROPHEADER diph;
    GUID    guidClass;
    WCHAR   wszPath[MAX_PATH];
} DIPROPGUIDANDPATH, *LPDIPROPGUIDANDPATH;
typedef const DIPROPGUIDANDPATH *LPCDIPROPGUIDANDPATH;

typedef struct DIPROPSTRING {
    DIPROPHEADER diph;
    WCHAR   wsz[MAX_PATH];
} DIPROPSTRING, *LPDIPROPSTRING;
typedef const DIPROPSTRING *LPCDIPROPSTRING;

#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
#define MAXCPOINTSNUM          8

typedef struct _CPOINT
{
    LONG  lP;
    DWORD dwLog;
} CPOINT, *PCPOINT;

typedef struct DIPROPCPOINTS {
    DIPROPHEADER diph;
    DWORD  dwCPointsNum;
    CPOINT cp[MAXCPOINTSNUM];
} DIPROPCPOINTS, *LPDIPROPCPOINTS;
typedef const DIPROPCPOINTS *LPCDIPROPCPOINTS;
#endif

#ifdef __cplusplus
#define MAKEDIPROP(prop)    (*(const GUID *)(prop))
#else
#define MAKEDIPROP(prop)    ((REFGUID)(prop))
#endif

#define DIPROP_BUFFERSIZE       MAKEDIPROP(1)

#define DIPROP_AXISMODE         MAKEDIPROP(2)

#define DIPROPAXISMODE_ABS      0
#define DIPROPAXISMODE_REL      1

#define DIPROP_GRANULARITY      MAKEDIPROP(3)

#define DIPROP_RANGE            MAKEDIPROP(4)

#define DIPROP_DEADZONE         MAKEDIPROP(5)

#define DIPROP_SATURATION       MAKEDIPROP(6)

#define DIPROP_FFGAIN           MAKEDIPROP(7)

#define DIPROP_FFLOAD           MAKEDIPROP(8)

#define DIPROP_AUTOCENTER       MAKEDIPROP(9)

#define DIPROPAUTOCENTER_OFF    0
#define DIPROPAUTOCENTER_ON     1

#define DIPROP_CALIBRATIONMODE  MAKEDIPROP(10)

#define DIPROPCALIBRATIONMODE_COOKED    0
#define DIPROPCALIBRATIONMODE_RAW       1

#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIPROP_CALIBRATION      MAKEDIPROP(11)

#define DIPROP_GUIDANDPATH      MAKEDIPROP(12)

#define DIPROP_INSTANCENAME     MAKEDIPROP(13)

#define DIPROP_PRODUCTNAME      MAKEDIPROP(14)
#endif

#if(DIRECTINPUT_VERSION >= 0x05b2)
#define DIPROP_JOYSTICKID       MAKEDIPROP(15)

#define DIPROP_GETPORTDISPLAYNAME       MAKEDIPROP(16)

#endif

#if(DIRECTINPUT_VERSION >= 0x0700)
#define DIPROP_PHYSICALRANGE            MAKEDIPROP(18)

#define DIPROP_LOGICALRANGE             MAKEDIPROP(19)
#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
#define DIPROP_KEYNAME                     MAKEDIPROP(20)

#define DIPROP_CPOINTS                 MAKEDIPROP(21)

#define DIPROP_APPDATA       MAKEDIPROP(22)

#define DIPROP_SCANCODE      MAKEDIPROP(23)

#define DIPROP_VIDPID           MAKEDIPROP(24)

#define DIPROP_USERNAME         MAKEDIPROP(25)

#define DIPROP_TYPENAME         MAKEDIPROP(26)
#endif

typedef struct DIDEVICEOBJECTDATA_DX3 {
    DWORD       dwOfs;
    DWORD       dwData;
    DWORD       dwTimeStamp;
    DWORD       dwSequence;
} DIDEVICEOBJECTDATA_DX3, *LPDIDEVICEOBJECTDATA_DX3;
typedef const DIDEVICEOBJECTDATA_DX3 *LPCDIDEVICEOBJECTDATA_DX;

typedef struct DIDEVICEOBJECTDATA {
    DWORD       dwOfs;
    DWORD       dwData;
    DWORD       dwTimeStamp;
    DWORD       dwSequence;
#if(DIRECTINPUT_VERSION >= 0x0800)
    UINT_PTR    uAppData;
#endif
} DIDEVICEOBJECTDATA, *LPDIDEVICEOBJECTDATA;
typedef const DIDEVICEOBJECTDATA *LPCDIDEVICEOBJECTDATA;

#define DIGDD_PEEK          0x00000001

#define DISEQUENCE_COMPARE(dwSequence1, cmp, dwSequence2) \
                        ((int)((dwSequence1) - (dwSequence2)) cmp 0)
#define DISCL_EXCLUSIVE     0x00000001
#define DISCL_NONEXCLUSIVE  0x00000002
#define DISCL_FOREGROUND    0x00000004
#define DISCL_BACKGROUND    0x00000008
#define DISCL_NOWINKEY      0x00000010

#if(DIRECTINPUT_VERSION >= 0x0500)

typedef struct DIDEVICEINSTANCE_DX3A {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    CHAR    tszInstanceName[MAX_PATH];
    CHAR    tszProductName[MAX_PATH];
} DIDEVICEINSTANCE_DX3A, *LPDIDEVICEINSTANCE_DX3A;
typedef struct DIDEVICEINSTANCE_DX3W {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    WCHAR   tszInstanceName[MAX_PATH];
    WCHAR   tszProductName[MAX_PATH];
} DIDEVICEINSTANCE_DX3W, *LPDIDEVICEINSTANCE_DX3W;
#ifdef UNICODE
typedef DIDEVICEINSTANCE_DX3W DIDEVICEINSTANCE_DX3;
typedef LPDIDEVICEINSTANCE_DX3W LPDIDEVICEINSTANCE_DX3;
#else
typedef DIDEVICEINSTANCE_DX3A DIDEVICEINSTANCE_DX3;
typedef LPDIDEVICEINSTANCE_DX3A LPDIDEVICEINSTANCE_DX3;
#endif
typedef const DIDEVICEINSTANCE_DX3A *LPCDIDEVICEINSTANCE_DX3A;
typedef const DIDEVICEINSTANCE_DX3W *LPCDIDEVICEINSTANCE_DX3W;
typedef const DIDEVICEINSTANCE_DX3  *LPCDIDEVICEINSTANCE_DX3;
#endif

typedef struct DIDEVICEINSTANCEA {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    CHAR    tszInstanceName[MAX_PATH];
    CHAR    tszProductName[MAX_PATH];
#if(DIRECTINPUT_VERSION >= 0x0500)
    GUID    guidFFDriver;
    WORD    wUsagePage;
    WORD    wUsage;
#endif
} DIDEVICEINSTANCEA, *LPDIDEVICEINSTANCEA;
typedef struct DIDEVICEINSTANCEW {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    WCHAR   tszInstanceName[MAX_PATH];
    WCHAR   tszProductName[MAX_PATH];
#if(DIRECTINPUT_VERSION >= 0x0500)
    GUID    guidFFDriver;
    WORD    wUsagePage;
    WORD    wUsage;
#endif
} DIDEVICEINSTANCEW, *LPDIDEVICEINSTANCEW;
#ifdef UNICODE
typedef DIDEVICEINSTANCEW DIDEVICEINSTANCE;
typedef LPDIDEVICEINSTANCEW LPDIDEVICEINSTANCE;
#else
typedef DIDEVICEINSTANCEA DIDEVICEINSTANCE;
typedef LPDIDEVICEINSTANCEA LPDIDEVICEINSTANCE;
#endif

typedef const DIDEVICEINSTANCEA *LPCDIDEVICEINSTANCEA;
typedef const DIDEVICEINSTANCEW *LPCDIDEVICEINSTANCEW;
#ifdef UNICODE
typedef DIDEVICEINSTANCEW DIDEVICEINSTANCE;
typedef LPCDIDEVICEINSTANCEW LPCDIDEVICEINSTANCE;
#else
typedef DIDEVICEINSTANCEA DIDEVICEINSTANCE;
typedef LPCDIDEVICEINSTANCEA LPCDIDEVICEINSTANCE;
#endif
typedef const DIDEVICEINSTANCE  *LPCDIDEVICEINSTANCE;

#undef INTERFACE
#define INTERFACE IDirectInputDeviceW

DECLARE_INTERFACE_(IDirectInputDeviceW, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEW,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEW) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
};

typedef struct IDirectInputDeviceW *LPDIRECTINPUTDEVICEW;

#undef INTERFACE
#define INTERFACE IDirectInputDeviceA

DECLARE_INTERFACE_(IDirectInputDeviceA, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
};

typedef struct IDirectInputDeviceA *LPDIRECTINPUTDEVICEA;

#ifdef UNICODE
#define IID_IDirectInputDevice IID_IDirectInputDeviceW
#define IDirectInputDevice IDirectInputDeviceW
#define IDirectInputDeviceVtbl IDirectInputDeviceWVtbl
#else
#define IID_IDirectInputDevice IID_IDirectInputDeviceA
#define IDirectInputDevice IDirectInputDeviceA
#define IDirectInputDeviceVtbl IDirectInputDeviceAVtbl
#endif
typedef struct IDirectInputDevice *LPDIRECTINPUTDEVICE;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInputDevice_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInputDevice_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInputDevice_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInputDevice_GetCapabilities(p,a) (p)->lpVtbl->GetCapabilities(p,a)
#define IDirectInputDevice_EnumObjects(p,a,b,c) (p)->lpVtbl->EnumObjects(p,a,b,c)
#define IDirectInputDevice_GetProperty(p,a,b) (p)->lpVtbl->GetProperty(p,a,b)
#define IDirectInputDevice_SetProperty(p,a,b) (p)->lpVtbl->SetProperty(p,a,b)
#define IDirectInputDevice_Acquire(p) (p)->lpVtbl->Acquire(p)
#define IDirectInputDevice_Unacquire(p) (p)->lpVtbl->Unacquire(p)
#define IDirectInputDevice_GetDeviceState(p,a,b) (p)->lpVtbl->GetDeviceState(p,a,b)
#define IDirectInputDevice_GetDeviceData(p,a,b,c,d) (p)->lpVtbl->GetDeviceData(p,a,b,c,d)
#define IDirectInputDevice_SetDataFormat(p,a) (p)->lpVtbl->SetDataFormat(p,a)
#define IDirectInputDevice_SetEventNotification(p,a) (p)->lpVtbl->SetEventNotification(p,a)
#define IDirectInputDevice_SetCooperativeLevel(p,a,b) (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectInputDevice_GetObjectInfo(p,a,b,c) (p)->lpVtbl->GetObjectInfo(p,a,b,c)
#define IDirectInputDevice_GetDeviceInfo(p,a) (p)->lpVtbl->GetDeviceInfo(p,a)
#define IDirectInputDevice_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInputDevice_Initialize(p,a,b,c) (p)->lpVtbl->Initialize(p,a,b,c)
#else
#define IDirectInputDevice_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInputDevice_AddRef(p) (p)->AddRef()
#define IDirectInputDevice_Release(p) (p)->Release()
#define IDirectInputDevice_GetCapabilities(p,a) (p)->GetCapabilities(a)
#define IDirectInputDevice_EnumObjects(p,a,b,c) (p)->EnumObjects(a,b,c)
#define IDirectInputDevice_GetProperty(p,a,b) (p)->GetProperty(a,b)
#define IDirectInputDevice_SetProperty(p,a,b) (p)->SetProperty(a,b)
#define IDirectInputDevice_Acquire(p) (p)->Acquire()
#define IDirectInputDevice_Unacquire(p) (p)->Unacquire()
#define IDirectInputDevice_GetDeviceState(p,a,b) (p)->GetDeviceState(a,b)
#define IDirectInputDevice_GetDeviceData(p,a,b,c,d) (p)->GetDeviceData(a,b,c,d)
#define IDirectInputDevice_SetDataFormat(p,a) (p)->SetDataFormat(a)
#define IDirectInputDevice_SetEventNotification(p,a) (p)->SetEventNotification(a)
#define IDirectInputDevice_SetCooperativeLevel(p,a,b) (p)->SetCooperativeLevel(a,b)
#define IDirectInputDevice_GetObjectInfo(p,a,b,c) (p)->GetObjectInfo(a,b,c)
#define IDirectInputDevice_GetDeviceInfo(p,a) (p)->GetDeviceInfo(a)
#define IDirectInputDevice_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInputDevice_Initialize(p,a,b,c) (p)->Initialize(a,b,c)
#endif

#endif

#if(DIRECTINPUT_VERSION >= 0x0500)

#define DISFFC_RESET            0x00000001
#define DISFFC_STOPALL          0x00000002
#define DISFFC_PAUSE            0x00000004
#define DISFFC_CONTINUE         0x00000008
#define DISFFC_SETACTUATORSON   0x00000010
#define DISFFC_SETACTUATORSOFF  0x00000020

#define DIGFFS_EMPTY            0x00000001
#define DIGFFS_STOPPED          0x00000002
#define DIGFFS_PAUSED           0x00000004
#define DIGFFS_ACTUATORSON      0x00000010
#define DIGFFS_ACTUATORSOFF     0x00000020
#define DIGFFS_POWERON          0x00000040
#define DIGFFS_POWEROFF         0x00000080
#define DIGFFS_SAFETYSWITCHON   0x00000100
#define DIGFFS_SAFETYSWITCHOFF  0x00000200
#define DIGFFS_USERFFSWITCHON   0x00000400
#define DIGFFS_USERFFSWITCHOFF  0x00000800
#define DIGFFS_DEVICELOST       0x80000000

#ifndef DIJ_RINGZERO

typedef struct DIEFFECTINFOA {
    DWORD   dwSize;
    GUID    guid;
    DWORD   dwEffType;
    DWORD   dwStaticParams;
    DWORD   dwDynamicParams;
    CHAR    tszName[MAX_PATH];
} DIEFFECTINFOA, *LPDIEFFECTINFOA;
typedef struct DIEFFECTINFOW {
    DWORD   dwSize;
    GUID    guid;
    DWORD   dwEffType;
    DWORD   dwStaticParams;
    DWORD   dwDynamicParams;
    WCHAR   tszName[MAX_PATH];
} DIEFFECTINFOW, *LPDIEFFECTINFOW;
#ifdef UNICODE
typedef DIEFFECTINFOW DIEFFECTINFO;
typedef LPDIEFFECTINFOW LPDIEFFECTINFO;
#else
typedef DIEFFECTINFOA DIEFFECTINFO;
typedef LPDIEFFECTINFOA LPDIEFFECTINFO;
#endif
typedef const DIEFFECTINFOA *LPCDIEFFECTINFOA;
typedef const DIEFFECTINFOW *LPCDIEFFECTINFOW;
typedef const DIEFFECTINFO  *LPCDIEFFECTINFO;

#define DISDD_CONTINUE          0x00000001

typedef BOOL (FAR PASCAL * LPDIENUMEFFECTSCALLBACKA)(LPCDIEFFECTINFOA, LPVOID);
typedef BOOL (FAR PASCAL * LPDIENUMEFFECTSCALLBACKW)(LPCDIEFFECTINFOW, LPVOID);
#ifdef UNICODE
#define LPDIENUMEFFECTSCALLBACK  LPDIENUMEFFECTSCALLBACKW
#else
#define LPDIENUMEFFECTSCALLBACK  LPDIENUMEFFECTSCALLBACKA
#endif
typedef BOOL (FAR PASCAL * LPDIENUMCREATEDEFFECTOBJECTSCALLBACK)(LPDIRECTINPUTEFFECT, LPVOID);

#undef INTERFACE
#define INTERFACE IDirectInputDevice2W

DECLARE_INTERFACE_(IDirectInputDevice2W, IDirectInputDeviceW)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEW,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEW) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;

    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOW,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
};

typedef struct IDirectInputDevice2W *LPDIRECTINPUTDEVICE2W;

#undef INTERFACE
#define INTERFACE IDirectInputDevice2A

DECLARE_INTERFACE_(IDirectInputDevice2A, IDirectInputDeviceA)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;

    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOA,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
};

typedef struct IDirectInputDevice2A *LPDIRECTINPUTDEVICE2A;

#ifdef UNICODE
#define IID_IDirectInputDevice2 IID_IDirectInputDevice2W
#define IDirectInputDevice2 IDirectInputDevice2W
#define IDirectInputDevice2Vtbl IDirectInputDevice2WVtbl
#else
#define IID_IDirectInputDevice2 IID_IDirectInputDevice2A
#define IDirectInputDevice2 IDirectInputDevice2A
#define IDirectInputDevice2Vtbl IDirectInputDevice2AVtbl
#endif
typedef struct IDirectInputDevice2 *LPDIRECTINPUTDEVICE2;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInputDevice2_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInputDevice2_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInputDevice2_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInputDevice2_GetCapabilities(p,a) (p)->lpVtbl->GetCapabilities(p,a)
#define IDirectInputDevice2_EnumObjects(p,a,b,c) (p)->lpVtbl->EnumObjects(p,a,b,c)
#define IDirectInputDevice2_GetProperty(p,a,b) (p)->lpVtbl->GetProperty(p,a,b)
#define IDirectInputDevice2_SetProperty(p,a,b) (p)->lpVtbl->SetProperty(p,a,b)
#define IDirectInputDevice2_Acquire(p) (p)->lpVtbl->Acquire(p)
#define IDirectInputDevice2_Unacquire(p) (p)->lpVtbl->Unacquire(p)
#define IDirectInputDevice2_GetDeviceState(p,a,b) (p)->lpVtbl->GetDeviceState(p,a,b)
#define IDirectInputDevice2_GetDeviceData(p,a,b,c,d) (p)->lpVtbl->GetDeviceData(p,a,b,c,d)
#define IDirectInputDevice2_SetDataFormat(p,a) (p)->lpVtbl->SetDataFormat(p,a)
#define IDirectInputDevice2_SetEventNotification(p,a) (p)->lpVtbl->SetEventNotification(p,a)
#define IDirectInputDevice2_SetCooperativeLevel(p,a,b) (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectInputDevice2_GetObjectInfo(p,a,b,c) (p)->lpVtbl->GetObjectInfo(p,a,b,c)
#define IDirectInputDevice2_GetDeviceInfo(p,a) (p)->lpVtbl->GetDeviceInfo(p,a)
#define IDirectInputDevice2_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInputDevice2_Initialize(p,a,b,c) (p)->lpVtbl->Initialize(p,a,b,c)
#define IDirectInputDevice2_CreateEffect(p,a,b,c,d) (p)->lpVtbl->CreateEffect(p,a,b,c,d)
#define IDirectInputDevice2_EnumEffects(p,a,b,c) (p)->lpVtbl->EnumEffects(p,a,b,c)
#define IDirectInputDevice2_GetEffectInfo(p,a,b) (p)->lpVtbl->GetEffectInfo(p,a,b)
#define IDirectInputDevice2_GetForceFeedbackState(p,a) (p)->lpVtbl->GetForceFeedbackState(p,a)
#define IDirectInputDevice2_SendForceFeedbackCommand(p,a) (p)->lpVtbl->SendForceFeedbackCommand(p,a)
#define IDirectInputDevice2_EnumCreatedEffectObjects(p,a,b,c) (p)->lpVtbl->EnumCreatedEffectObjects(p,a,b,c)
#define IDirectInputDevice2_Escape(p,a) (p)->lpVtbl->Escape(p,a)
#define IDirectInputDevice2_Poll(p) (p)->lpVtbl->Poll(p)
#define IDirectInputDevice2_SendDeviceData(p,a,b,c,d) (p)->lpVtbl->SendDeviceData(p,a,b,c,d)
#else
#define IDirectInputDevice2_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInputDevice2_AddRef(p) (p)->AddRef()
#define IDirectInputDevice2_Release(p) (p)->Release()
#define IDirectInputDevice2_GetCapabilities(p,a) (p)->GetCapabilities(a)
#define IDirectInputDevice2_EnumObjects(p,a,b,c) (p)->EnumObjects(a,b,c)
#define IDirectInputDevice2_GetProperty(p,a,b) (p)->GetProperty(a,b)
#define IDirectInputDevice2_SetProperty(p,a,b) (p)->SetProperty(a,b)
#define IDirectInputDevice2_Acquire(p) (p)->Acquire()
#define IDirectInputDevice2_Unacquire(p) (p)->Unacquire()
#define IDirectInputDevice2_GetDeviceState(p,a,b) (p)->GetDeviceState(a,b)
#define IDirectInputDevice2_GetDeviceData(p,a,b,c,d) (p)->GetDeviceData(a,b,c,d)
#define IDirectInputDevice2_SetDataFormat(p,a) (p)->SetDataFormat(a)
#define IDirectInputDevice2_SetEventNotification(p,a) (p)->SetEventNotification(a)
#define IDirectInputDevice2_SetCooperativeLevel(p,a,b) (p)->SetCooperativeLevel(a,b)
#define IDirectInputDevice2_GetObjectInfo(p,a,b,c) (p)->GetObjectInfo(a,b,c)
#define IDirectInputDevice2_GetDeviceInfo(p,a) (p)->GetDeviceInfo(a)
#define IDirectInputDevice2_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInputDevice2_Initialize(p,a,b,c) (p)->Initialize(a,b,c)
#define IDirectInputDevice2_CreateEffect(p,a,b,c,d) (p)->CreateEffect(a,b,c,d)
#define IDirectInputDevice2_EnumEffects(p,a,b,c) (p)->EnumEffects(a,b,c)
#define IDirectInputDevice2_GetEffectInfo(p,a,b) (p)->GetEffectInfo(a,b)
#define IDirectInputDevice2_GetForceFeedbackState(p,a) (p)->GetForceFeedbackState(a)
#define IDirectInputDevice2_SendForceFeedbackCommand(p,a) (p)->SendForceFeedbackCommand(a)
#define IDirectInputDevice2_EnumCreatedEffectObjects(p,a,b,c) (p)->EnumCreatedEffectObjects(a,b,c)
#define IDirectInputDevice2_Escape(p,a) (p)->Escape(a)
#define IDirectInputDevice2_Poll(p) (p)->Poll()
#define IDirectInputDevice2_SendDeviceData(p,a,b,c,d) (p)->SendDeviceData(a,b,c,d)
#endif

#endif

#endif

#if(DIRECTINPUT_VERSION >= 0x0700)
#define DIFEF_DEFAULT               0x00000000
#define DIFEF_INCLUDENONSTANDARD    0x00000001
#define DIFEF_MODIFYIFNEEDED            0x00000010

#ifndef DIJ_RINGZERO

#undef INTERFACE
#define INTERFACE IDirectInputDevice7W

DECLARE_INTERFACE_(IDirectInputDevice7W, IDirectInputDevice2W)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEW,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEW) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOW,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;

    STDMETHOD(EnumEffectsInFile)(THIS_ LPCWSTR,LPDIENUMEFFECTSINFILECALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(WriteEffectToFile)(THIS_ LPCWSTR,DWORD,LPDIFILEEFFECT,DWORD) PURE;
};

typedef struct IDirectInputDevice7W *LPDIRECTINPUTDEVICE7W;

#undef INTERFACE
#define INTERFACE IDirectInputDevice7A

DECLARE_INTERFACE_(IDirectInputDevice7A, IDirectInputDevice2A)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOA,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;

    STDMETHOD(EnumEffectsInFile)(THIS_ LPCSTR,LPDIENUMEFFECTSINFILECALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(WriteEffectToFile)(THIS_ LPCSTR,DWORD,LPDIFILEEFFECT,DWORD) PURE;
};

typedef struct IDirectInputDevice7A *LPDIRECTINPUTDEVICE7A;

#ifdef UNICODE
#define IID_IDirectInputDevice7 IID_IDirectInputDevice7W
#define IDirectInputDevice7 IDirectInputDevice7W
#define IDirectInputDevice7Vtbl IDirectInputDevice7WVtbl
#else
#define IID_IDirectInputDevice7 IID_IDirectInputDevice7A
#define IDirectInputDevice7 IDirectInputDevice7A
#define IDirectInputDevice7Vtbl IDirectInputDevice7AVtbl
#endif
typedef struct IDirectInputDevice7 *LPDIRECTINPUTDEVICE7;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInputDevice7_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInputDevice7_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInputDevice7_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInputDevice7_GetCapabilities(p,a) (p)->lpVtbl->GetCapabilities(p,a)
#define IDirectInputDevice7_EnumObjects(p,a,b,c) (p)->lpVtbl->EnumObjects(p,a,b,c)
#define IDirectInputDevice7_GetProperty(p,a,b) (p)->lpVtbl->GetProperty(p,a,b)
#define IDirectInputDevice7_SetProperty(p,a,b) (p)->lpVtbl->SetProperty(p,a,b)
#define IDirectInputDevice7_Acquire(p) (p)->lpVtbl->Acquire(p)
#define IDirectInputDevice7_Unacquire(p) (p)->lpVtbl->Unacquire(p)
#define IDirectInputDevice7_GetDeviceState(p,a,b) (p)->lpVtbl->GetDeviceState(p,a,b)
#define IDirectInputDevice7_GetDeviceData(p,a,b,c,d) (p)->lpVtbl->GetDeviceData(p,a,b,c,d)
#define IDirectInputDevice7_SetDataFormat(p,a) (p)->lpVtbl->SetDataFormat(p,a)
#define IDirectInputDevice7_SetEventNotification(p,a) (p)->lpVtbl->SetEventNotification(p,a)
#define IDirectInputDevice7_SetCooperativeLevel(p,a,b) (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectInputDevice7_GetObjectInfo(p,a,b,c) (p)->lpVtbl->GetObjectInfo(p,a,b,c)
#define IDirectInputDevice7_GetDeviceInfo(p,a) (p)->lpVtbl->GetDeviceInfo(p,a)
#define IDirectInputDevice7_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInputDevice7_Initialize(p,a,b,c) (p)->lpVtbl->Initialize(p,a,b,c)
#define IDirectInputDevice7_CreateEffect(p,a,b,c,d) (p)->lpVtbl->CreateEffect(p,a,b,c,d)
#define IDirectInputDevice7_EnumEffects(p,a,b,c) (p)->lpVtbl->EnumEffects(p,a,b,c)
#define IDirectInputDevice7_GetEffectInfo(p,a,b) (p)->lpVtbl->GetEffectInfo(p,a,b)
#define IDirectInputDevice7_GetForceFeedbackState(p,a) (p)->lpVtbl->GetForceFeedbackState(p,a)
#define IDirectInputDevice7_SendForceFeedbackCommand(p,a) (p)->lpVtbl->SendForceFeedbackCommand(p,a)
#define IDirectInputDevice7_EnumCreatedEffectObjects(p,a,b,c) (p)->lpVtbl->EnumCreatedEffectObjects(p,a,b,c)
#define IDirectInputDevice7_Escape(p,a) (p)->lpVtbl->Escape(p,a)
#define IDirectInputDevice7_Poll(p) (p)->lpVtbl->Poll(p)
#define IDirectInputDevice7_SendDeviceData(p,a,b,c,d) (p)->lpVtbl->SendDeviceData(p,a,b,c,d)
#define IDirectInputDevice7_EnumEffectsInFile(p,a,b,c,d) (p)->lpVtbl->EnumEffectsInFile(p,a,b,c,d)
#define IDirectInputDevice7_WriteEffectToFile(p,a,b,c,d) (p)->lpVtbl->WriteEffectToFile(p,a,b,c,d)
#else
#define IDirectInputDevice7_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInputDevice7_AddRef(p) (p)->AddRef()
#define IDirectInputDevice7_Release(p) (p)->Release()
#define IDirectInputDevice7_GetCapabilities(p,a) (p)->GetCapabilities(a)
#define IDirectInputDevice7_EnumObjects(p,a,b,c) (p)->EnumObjects(a,b,c)
#define IDirectInputDevice7_GetProperty(p,a,b) (p)->GetProperty(a,b)
#define IDirectInputDevice7_SetProperty(p,a,b) (p)->SetProperty(a,b)
#define IDirectInputDevice7_Acquire(p) (p)->Acquire()
#define IDirectInputDevice7_Unacquire(p) (p)->Unacquire()
#define IDirectInputDevice7_GetDeviceState(p,a,b) (p)->GetDeviceState(a,b)
#define IDirectInputDevice7_GetDeviceData(p,a,b,c,d) (p)->GetDeviceData(a,b,c,d)
#define IDirectInputDevice7_SetDataFormat(p,a) (p)->SetDataFormat(a)
#define IDirectInputDevice7_SetEventNotification(p,a) (p)->SetEventNotification(a)
#define IDirectInputDevice7_SetCooperativeLevel(p,a,b) (p)->SetCooperativeLevel(a,b)
#define IDirectInputDevice7_GetObjectInfo(p,a,b,c) (p)->GetObjectInfo(a,b,c)
#define IDirectInputDevice7_GetDeviceInfo(p,a) (p)->GetDeviceInfo(a)
#define IDirectInputDevice7_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInputDevice7_Initialize(p,a,b,c) (p)->Initialize(a,b,c)
#define IDirectInputDevice7_CreateEffect(p,a,b,c,d) (p)->CreateEffect(a,b,c,d)
#define IDirectInputDevice7_EnumEffects(p,a,b,c) (p)->EnumEffects(a,b,c)
#define IDirectInputDevice7_GetEffectInfo(p,a,b) (p)->GetEffectInfo(a,b)
#define IDirectInputDevice7_GetForceFeedbackState(p,a) (p)->GetForceFeedbackState(a)
#define IDirectInputDevice7_SendForceFeedbackCommand(p,a) (p)->SendForceFeedbackCommand(a)
#define IDirectInputDevice7_EnumCreatedEffectObjects(p,a,b,c) (p)->EnumCreatedEffectObjects(a,b,c)
#define IDirectInputDevice7_Escape(p,a) (p)->Escape(a)
#define IDirectInputDevice7_Poll(p) (p)->Poll()
#define IDirectInputDevice7_SendDeviceData(p,a,b,c,d) (p)->SendDeviceData(a,b,c,d)
#define IDirectInputDevice7_EnumEffectsInFile(p,a,b,c,d) (p)->EnumEffectsInFile(a,b,c,d)
#define IDirectInputDevice7_WriteEffectToFile(p,a,b,c,d) (p)->WriteEffectToFile(a,b,c,d)
#endif

#endif

#endif

#if(DIRECTINPUT_VERSION >= 0x0800)

#ifndef DIJ_RINGZERO

#undef INTERFACE
#define INTERFACE IDirectInputDevice8W

DECLARE_INTERFACE_(IDirectInputDevice8W, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEW,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEW) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOW,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(EnumEffectsInFile)(THIS_ LPCWSTR,LPDIENUMEFFECTSINFILECALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(WriteEffectToFile)(THIS_ LPCWSTR,DWORD,LPDIFILEEFFECT,DWORD) PURE;
    STDMETHOD(BuildActionMap)(THIS_ LPDIACTIONFORMATW,LPCWSTR,DWORD) PURE;
    STDMETHOD(SetActionMap)(THIS_ LPDIACTIONFORMATW,LPCWSTR,DWORD) PURE;
    STDMETHOD(GetImageInfo)(THIS_ LPDIDEVICEIMAGEINFOHEADERW) PURE;
};

typedef struct IDirectInputDevice8W *LPDIRECTINPUTDEVICE8W;

#undef INTERFACE
#define INTERFACE IDirectInputDevice8A

DECLARE_INTERFACE_(IDirectInputDevice8A, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) PURE;
    STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetProperty)(THIS_ REFGUID,LPDIPROPHEADER) PURE;
    STDMETHOD(SetProperty)(THIS_ REFGUID,LPCDIPROPHEADER) PURE;
    STDMETHOD(Acquire)(THIS) PURE;
    STDMETHOD(Unacquire)(THIS) PURE;
    STDMETHOD(GetDeviceState)(THIS_ DWORD,LPVOID) PURE;
    STDMETHOD(GetDeviceData)(THIS_ DWORD,LPDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) PURE;
    STDMETHOD(SetEventNotification)(THIS_ HANDLE) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA,DWORD,DWORD) PURE;
    STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD,REFGUID) PURE;
    STDMETHOD(CreateEffect)(THIS_ REFGUID,LPCDIEFFECT,LPDIRECTINPUTEFFECT *,LPUNKNOWN) PURE;
    STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOA,REFGUID) PURE;
    STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) PURE;
    STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) PURE;
    STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) PURE;
    STDMETHOD(Poll)(THIS) PURE;
    STDMETHOD(SendDeviceData)(THIS_ DWORD,LPCDIDEVICEOBJECTDATA,LPDWORD,DWORD) PURE;
    STDMETHOD(EnumEffectsInFile)(THIS_ LPCSTR,LPDIENUMEFFECTSINFILECALLBACK,LPVOID,DWORD) PURE;
    STDMETHOD(WriteEffectToFile)(THIS_ LPCSTR,DWORD,LPDIFILEEFFECT,DWORD) PURE;
    STDMETHOD(BuildActionMap)(THIS_ LPDIACTIONFORMATA,LPCSTR,DWORD) PURE;
    STDMETHOD(SetActionMap)(THIS_ LPDIACTIONFORMATA,LPCSTR,DWORD) PURE;
    STDMETHOD(GetImageInfo)(THIS_ LPDIDEVICEIMAGEINFOHEADERA) PURE;
};

typedef struct IDirectInputDevice8A *LPDIRECTINPUTDEVICE8A;

#ifdef UNICODE
#define IID_IDirectInputDevice8 IID_IDirectInputDevice8W
#define IDirectInputDevice8 IDirectInputDevice8W
#define IDirectInputDevice8Vtbl IDirectInputDevice8WVtbl
#else
#define IID_IDirectInputDevice8 IID_IDirectInputDevice8A
#define IDirectInputDevice8 IDirectInputDevice8A
#define IDirectInputDevice8Vtbl IDirectInputDevice8AVtbl
#endif
typedef struct IDirectInputDevice8 *LPDIRECTINPUTDEVICE8;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInputDevice8_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInputDevice8_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInputDevice8_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInputDevice8_GetCapabilities(p,a) (p)->lpVtbl->GetCapabilities(p,a)
#define IDirectInputDevice8_EnumObjects(p,a,b,c) (p)->lpVtbl->EnumObjects(p,a,b,c)
#define IDirectInputDevice8_GetProperty(p,a,b) (p)->lpVtbl->GetProperty(p,a,b)
#define IDirectInputDevice8_SetProperty(p,a,b) (p)->lpVtbl->SetProperty(p,a,b)
#define IDirectInputDevice8_Acquire(p) (p)->lpVtbl->Acquire(p)
#define IDirectInputDevice8_Unacquire(p) (p)->lpVtbl->Unacquire(p)
#define IDirectInputDevice8_GetDeviceState(p,a,b) (p)->lpVtbl->GetDeviceState(p,a,b)
#define IDirectInputDevice8_GetDeviceData(p,a,b,c,d) (p)->lpVtbl->GetDeviceData(p,a,b,c,d)
#define IDirectInputDevice8_SetDataFormat(p,a) (p)->lpVtbl->SetDataFormat(p,a)
#define IDirectInputDevice8_SetEventNotification(p,a) (p)->lpVtbl->SetEventNotification(p,a)
#define IDirectInputDevice8_SetCooperativeLevel(p,a,b) (p)->lpVtbl->SetCooperativeLevel(p,a,b)
#define IDirectInputDevice8_GetObjectInfo(p,a,b,c) (p)->lpVtbl->GetObjectInfo(p,a,b,c)
#define IDirectInputDevice8_GetDeviceInfo(p,a) (p)->lpVtbl->GetDeviceInfo(p,a)
#define IDirectInputDevice8_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInputDevice8_Initialize(p,a,b,c) (p)->lpVtbl->Initialize(p,a,b,c)
#define IDirectInputDevice8_CreateEffect(p,a,b,c,d) (p)->lpVtbl->CreateEffect(p,a,b,c,d)
#define IDirectInputDevice8_EnumEffects(p,a,b,c) (p)->lpVtbl->EnumEffects(p,a,b,c)
#define IDirectInputDevice8_GetEffectInfo(p,a,b) (p)->lpVtbl->GetEffectInfo(p,a,b)
#define IDirectInputDevice8_GetForceFeedbackState(p,a) (p)->lpVtbl->GetForceFeedbackState(p,a)
#define IDirectInputDevice8_SendForceFeedbackCommand(p,a) (p)->lpVtbl->SendForceFeedbackCommand(p,a)
#define IDirectInputDevice8_EnumCreatedEffectObjects(p,a,b,c) (p)->lpVtbl->EnumCreatedEffectObjects(p,a,b,c)
#define IDirectInputDevice8_Escape(p,a) (p)->lpVtbl->Escape(p,a)
#define IDirectInputDevice8_Poll(p) (p)->lpVtbl->Poll(p)
#define IDirectInputDevice8_SendDeviceData(p,a,b,c,d) (p)->lpVtbl->SendDeviceData(p,a,b,c,d)
#define IDirectInputDevice8_EnumEffectsInFile(p,a,b,c,d) (p)->lpVtbl->EnumEffectsInFile(p,a,b,c,d)
#define IDirectInputDevice8_WriteEffectToFile(p,a,b,c,d) (p)->lpVtbl->WriteEffectToFile(p,a,b,c,d)
#define IDirectInputDevice8_BuildActionMap(p,a,b,c) (p)->lpVtbl->BuildActionMap(p,a,b,c)
#define IDirectInputDevice8_SetActionMap(p,a,b,c) (p)->lpVtbl->SetActionMap(p,a,b,c)
#define IDirectInputDevice8_GetImageInfo(p,a) (p)->lpVtbl->GetImageInfo(p,a)
#else
#define IDirectInputDevice8_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInputDevice8_AddRef(p) (p)->AddRef()
#define IDirectInputDevice8_Release(p) (p)->Release()
#define IDirectInputDevice8_GetCapabilities(p,a) (p)->GetCapabilities(a)
#define IDirectInputDevice8_EnumObjects(p,a,b,c) (p)->EnumObjects(a,b,c)
#define IDirectInputDevice8_GetProperty(p,a,b) (p)->GetProperty(a,b)
#define IDirectInputDevice8_SetProperty(p,a,b) (p)->SetProperty(a,b)
#define IDirectInputDevice8_Acquire(p) (p)->Acquire()
#define IDirectInputDevice8_Unacquire(p) (p)->Unacquire()
#define IDirectInputDevice8_GetDeviceState(p,a,b) (p)->GetDeviceState(a,b)
#define IDirectInputDevice8_GetDeviceData(p,a,b,c,d) (p)->GetDeviceData(a,b,c,d)
#define IDirectInputDevice8_SetDataFormat(p,a) (p)->SetDataFormat(a)
#define IDirectInputDevice8_SetEventNotification(p,a) (p)->SetEventNotification(a)
#define IDirectInputDevice8_SetCooperativeLevel(p,a,b) (p)->SetCooperativeLevel(a,b)
#define IDirectInputDevice8_GetObjectInfo(p,a,b,c) (p)->GetObjectInfo(a,b,c)
#define IDirectInputDevice8_GetDeviceInfo(p,a) (p)->GetDeviceInfo(a)
#define IDirectInputDevice8_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInputDevice8_Initialize(p,a,b,c) (p)->Initialize(a,b,c)
#define IDirectInputDevice8_CreateEffect(p,a,b,c,d) (p)->CreateEffect(a,b,c,d)
#define IDirectInputDevice8_EnumEffects(p,a,b,c) (p)->EnumEffects(a,b,c)
#define IDirectInputDevice8_GetEffectInfo(p,a,b) (p)->GetEffectInfo(a,b)
#define IDirectInputDevice8_GetForceFeedbackState(p,a) (p)->GetForceFeedbackState(a)
#define IDirectInputDevice8_SendForceFeedbackCommand(p,a) (p)->SendForceFeedbackCommand(a)
#define IDirectInputDevice8_EnumCreatedEffectObjects(p,a,b,c) (p)->EnumCreatedEffectObjects(a,b,c)
#define IDirectInputDevice8_Escape(p,a) (p)->Escape(a)
#define IDirectInputDevice8_Poll(p) (p)->Poll()
#define IDirectInputDevice8_SendDeviceData(p,a,b,c,d) (p)->SendDeviceData(a,b,c,d)
#define IDirectInputDevice8_EnumEffectsInFile(p,a,b,c,d) (p)->EnumEffectsInFile(a,b,c,d)
#define IDirectInputDevice8_WriteEffectToFile(p,a,b,c,d) (p)->WriteEffectToFile(a,b,c,d)
#define IDirectInputDevice8_BuildActionMap(p,a,b,c) (p)->BuildActionMap(a,b,c)
#define IDirectInputDevice8_SetActionMap(p,a,b,c) (p)->SetActionMap(a,b,c)
#define IDirectInputDevice8_GetImageInfo(p,a) (p)->GetImageInfo(a)
#endif

#endif

#endif

#ifndef DIJ_RINGZERO

typedef struct _DIMOUSESTATE {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    BYTE    rgbButtons[4];
} DIMOUSESTATE, *LPDIMOUSESTATE;

#if DIRECTINPUT_VERSION >= 0x0700
typedef struct _DIMOUSESTATE2 {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    BYTE    rgbButtons[8];
} DIMOUSESTATE2, *LPDIMOUSESTATE2;
#endif

#define DIMOFS_X        FIELD_OFFSET(DIMOUSESTATE, lX)
#define DIMOFS_Y        FIELD_OFFSET(DIMOUSESTATE, lY)
#define DIMOFS_Z        FIELD_OFFSET(DIMOUSESTATE, lZ)
#define DIMOFS_BUTTON0 (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + 0)
#define DIMOFS_BUTTON1 (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + 1)
#define DIMOFS_BUTTON2 (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + 2)
#define DIMOFS_BUTTON3 (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + 3)
#if (DIRECTINPUT_VERSION >= 0x0700)
#define DIMOFS_BUTTON4 (FIELD_OFFSET(DIMOUSESTATE2, rgbButtons) + 4)
#define DIMOFS_BUTTON5 (FIELD_OFFSET(DIMOUSESTATE2, rgbButtons) + 5)
#define DIMOFS_BUTTON6 (FIELD_OFFSET(DIMOUSESTATE2, rgbButtons) + 6)
#define DIMOFS_BUTTON7 (FIELD_OFFSET(DIMOUSESTATE2, rgbButtons) + 7)
#endif
#endif

#ifndef DIJ_RINGZERO

#define DIK_ESCAPE          0x01
#define DIK_1               0x02
#define DIK_2               0x03
#define DIK_3               0x04
#define DIK_4               0x05
#define DIK_5               0x06
#define DIK_6               0x07
#define DIK_7               0x08
#define DIK_8               0x09
#define DIK_9               0x0A
#define DIK_0               0x0B
#define DIK_MINUS           0x0C
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34
#define DIK_SLASH           0x35
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37
#define DIK_LMENU           0x38
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_NUMLOCK         0x45
#define DIK_SCROLL          0x46
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_SUBTRACT        0x4A
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_ADD             0x4E
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53
#define DIK_OEM_102         0x56
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_F13             0x64
#define DIK_F14             0x65
#define DIK_F15             0x66
#define DIK_KANA            0x70
#define DIK_ABNT_C1         0x73
#define DIK_CONVERT         0x79
#define DIK_NOCONVERT       0x7B
#define DIK_YEN             0x7D
#define DIK_ABNT_C2         0x7E
#define DIK_NUMPADEQUALS    0x8D
#define DIK_PREVTRACK       0x90
#define DIK_AT              0x91
#define DIK_COLON           0x92
#define DIK_UNDERLINE       0x93
#define DIK_KANJI           0x94
#define DIK_STOP            0x95
#define DIK_AX              0x96
#define DIK_UNLABELED       0x97
#define DIK_NEXTTRACK       0x99
#define DIK_NUMPADENTER     0x9C
#define DIK_RCONTROL        0x9D
#define DIK_MUTE            0xA0
#define DIK_CALCULATOR      0xA1
#define DIK_PLAYPAUSE       0xA2
#define DIK_MEDIASTOP       0xA4
#define DIK_VOLUMEDOWN      0xAE
#define DIK_VOLUMEUP        0xB0
#define DIK_WEBHOME         0xB2
#define DIK_NUMPADCOMMA     0xB3
#define DIK_DIVIDE          0xB5
#define DIK_SYSRQ           0xB7
#define DIK_RMENU           0xB8
#define DIK_PAUSE           0xC5
#define DIK_HOME            0xC7
#define DIK_UP              0xC8
#define DIK_PRIOR           0xC9
#define DIK_LEFT            0xCB
#define DIK_RIGHT           0xCD
#define DIK_END             0xCF
#define DIK_DOWN            0xD0
#define DIK_NEXT            0xD1
#define DIK_INSERT          0xD2
#define DIK_DELETE          0xD3
#define DIK_LWIN            0xDB
#define DIK_RWIN            0xDC
#define DIK_APPS            0xDD
#define DIK_POWER           0xDE
#define DIK_SLEEP           0xDF
#define DIK_WAKE            0xE3
#define DIK_WEBSEARCH       0xE5
#define DIK_WEBFAVORITES    0xE6
#define DIK_WEBREFRESH      0xE7
#define DIK_WEBSTOP         0xE8
#define DIK_WEBFORWARD      0xE9
#define DIK_WEBBACK         0xEA
#define DIK_MYCOMPUTER      0xEB
#define DIK_MAIL            0xEC
#define DIK_MEDIASELECT     0xED

#define DIK_BACKSPACE       DIK_BACK
#define DIK_NUMPADSTAR      DIK_MULTIPLY
#define DIK_LALT            DIK_LMENU
#define DIK_CAPSLOCK        DIK_CAPITAL
#define DIK_NUMPADMINUS     DIK_SUBTRACT
#define DIK_NUMPADPLUS      DIK_ADD
#define DIK_NUMPADPERIOD    DIK_DECIMAL
#define DIK_NUMPADSLASH     DIK_DIVIDE
#define DIK_RALT            DIK_RMENU
#define DIK_UPARROW         DIK_UP
#define DIK_PGUP            DIK_PRIOR
#define DIK_LEFTARROW       DIK_LEFT
#define DIK_RIGHTARROW      DIK_RIGHT
#define DIK_DOWNARROW       DIK_DOWN
#define DIK_PGDN            DIK_NEXT

#define DIK_CIRCUMFLEX      DIK_PREVTRACK

#endif

#ifndef DIJ_RINGZERO

typedef struct DIJOYSTATE {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    LONG    lRx;
    LONG    lRy;
    LONG    lRz;
    LONG    rglSlider[2];
    DWORD   rgdwPOV[4];
    BYTE    rgbButtons[32];
} DIJOYSTATE, *LPDIJOYSTATE;

typedef struct DIJOYSTATE2 {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    LONG    lRx;
    LONG    lRy;
    LONG    lRz;
    LONG    rglSlider[2];
    DWORD   rgdwPOV[4];
    BYTE    rgbButtons[128];
    LONG    lVX;
    LONG    lVY;
    LONG    lVZ;
    LONG    lVRx;
    LONG    lVRy;
    LONG    lVRz;
    LONG    rglVSlider[2];
    LONG    lAX;
    LONG    lAY;
    LONG    lAZ;
    LONG    lARx;
    LONG    lARy;
    LONG    lARz;
    LONG    rglASlider[2];
    LONG    lFX;
    LONG    lFY;
    LONG    lFZ;
    LONG    lFRx;
    LONG    lFRy;
    LONG    lFRz;
    LONG    rglFSlider[2];
} DIJOYSTATE2, *LPDIJOYSTATE2;

#define DIJOFS_X            FIELD_OFFSET(DIJOYSTATE, lX)
#define DIJOFS_Y            FIELD_OFFSET(DIJOYSTATE, lY)
#define DIJOFS_Z            FIELD_OFFSET(DIJOYSTATE, lZ)
#define DIJOFS_RX           FIELD_OFFSET(DIJOYSTATE, lRx)
#define DIJOFS_RY           FIELD_OFFSET(DIJOYSTATE, lRy)
#define DIJOFS_RZ           FIELD_OFFSET(DIJOYSTATE, lRz)
#define DIJOFS_SLIDER(n)   (FIELD_OFFSET(DIJOYSTATE, rglSlider) + \
                                                        (n) * sizeof(LONG))
#define DIJOFS_POV(n)      (FIELD_OFFSET(DIJOYSTATE, rgdwPOV) + \
                                                        (n) * sizeof(DWORD))
#define DIJOFS_BUTTON(n)   (FIELD_OFFSET(DIJOYSTATE, rgbButtons) + (n))
#define DIJOFS_BUTTON0      DIJOFS_BUTTON(0)
#define DIJOFS_BUTTON1      DIJOFS_BUTTON(1)
#define DIJOFS_BUTTON2      DIJOFS_BUTTON(2)
#define DIJOFS_BUTTON3      DIJOFS_BUTTON(3)
#define DIJOFS_BUTTON4      DIJOFS_BUTTON(4)
#define DIJOFS_BUTTON5      DIJOFS_BUTTON(5)
#define DIJOFS_BUTTON6      DIJOFS_BUTTON(6)
#define DIJOFS_BUTTON7      DIJOFS_BUTTON(7)
#define DIJOFS_BUTTON8      DIJOFS_BUTTON(8)
#define DIJOFS_BUTTON9      DIJOFS_BUTTON(9)
#define DIJOFS_BUTTON10     DIJOFS_BUTTON(10)
#define DIJOFS_BUTTON11     DIJOFS_BUTTON(11)
#define DIJOFS_BUTTON12     DIJOFS_BUTTON(12)
#define DIJOFS_BUTTON13     DIJOFS_BUTTON(13)
#define DIJOFS_BUTTON14     DIJOFS_BUTTON(14)
#define DIJOFS_BUTTON15     DIJOFS_BUTTON(15)
#define DIJOFS_BUTTON16     DIJOFS_BUTTON(16)
#define DIJOFS_BUTTON17     DIJOFS_BUTTON(17)
#define DIJOFS_BUTTON18     DIJOFS_BUTTON(18)
#define DIJOFS_BUTTON19     DIJOFS_BUTTON(19)
#define DIJOFS_BUTTON20     DIJOFS_BUTTON(20)
#define DIJOFS_BUTTON21     DIJOFS_BUTTON(21)
#define DIJOFS_BUTTON22     DIJOFS_BUTTON(22)
#define DIJOFS_BUTTON23     DIJOFS_BUTTON(23)
#define DIJOFS_BUTTON24     DIJOFS_BUTTON(24)
#define DIJOFS_BUTTON25     DIJOFS_BUTTON(25)
#define DIJOFS_BUTTON26     DIJOFS_BUTTON(26)
#define DIJOFS_BUTTON27     DIJOFS_BUTTON(27)
#define DIJOFS_BUTTON28     DIJOFS_BUTTON(28)
#define DIJOFS_BUTTON29     DIJOFS_BUTTON(29)
#define DIJOFS_BUTTON30     DIJOFS_BUTTON(30)
#define DIJOFS_BUTTON31     DIJOFS_BUTTON(31)

#endif

#ifndef DIJ_RINGZERO

#define DIENUM_STOP             0
#define DIENUM_CONTINUE         1

typedef BOOL (FAR PASCAL * LPDIENUMDEVICESCALLBACKA)(LPCDIDEVICEINSTANCEA, LPVOID);
typedef BOOL (FAR PASCAL * LPDIENUMDEVICESCALLBACKW)(LPCDIDEVICEINSTANCEW, LPVOID);
#ifdef UNICODE
#define LPDIENUMDEVICESCALLBACK  LPDIENUMDEVICESCALLBACKW
#else
#define LPDIENUMDEVICESCALLBACK  LPDIENUMDEVICESCALLBACKA
#endif
typedef BOOL (FAR PASCAL * LPDICONFIGUREDEVICESCALLBACK)(IUnknown FAR *, LPVOID);

#define DIEDFL_ALLDEVICES       0x00000000
#define DIEDFL_ATTACHEDONLY     0x00000001
#if(DIRECTINPUT_VERSION >= 0x0500)
#define DIEDFL_FORCEFEEDBACK    0x00000100
#endif
#if(DIRECTINPUT_VERSION >= 0x050a)
#define DIEDFL_INCLUDEALIASES   0x00010000
#define DIEDFL_INCLUDEPHANTOMS  0x00020000
#endif
#if(DIRECTINPUT_VERSION >= 0x0800)
#define DIEDFL_INCLUDEHIDDEN    0x00040000
#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
typedef BOOL (FAR PASCAL * LPDIENUMDEVICESBYSEMANTICSCBA)(LPCDIDEVICEINSTANCEA, LPDIRECTINPUTDEVICE8A, DWORD, DWORD, LPVOID);
typedef BOOL (FAR PASCAL * LPDIENUMDEVICESBYSEMANTICSCBW)(LPCDIDEVICEINSTANCEW, LPDIRECTINPUTDEVICE8W, DWORD, DWORD, LPVOID);
#ifdef UNICODE
#define LPDIENUMDEVICESBYSEMANTICSCB  LPDIENUMDEVICESBYSEMANTICSCBW
#else
#define LPDIENUMDEVICESBYSEMANTICSCB  LPDIENUMDEVICESBYSEMANTICSCBA
#endif
#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
#define DIEDBS_MAPPEDPRI1         0x00000001
#define DIEDBS_MAPPEDPRI2         0x00000002
#define DIEDBS_RECENTDEVICE       0x00000010
#define DIEDBS_NEWDEVICE          0x00000020
#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
#define DIEDBSFL_ATTACHEDONLY       0x00000000
#define DIEDBSFL_THISUSER           0x00000010
#define DIEDBSFL_FORCEFEEDBACK      DIEDFL_FORCEFEEDBACK
#define DIEDBSFL_AVAILABLEDEVICES   0x00001000
#define DIEDBSFL_MULTIMICEKEYBOARDS 0x00002000
#define DIEDBSFL_NONGAMINGDEVICES   0x00004000
#define DIEDBSFL_VALID              0x00007110
#endif

#undef INTERFACE
#define INTERFACE IDirectInputW

DECLARE_INTERFACE_(IDirectInputW, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEW *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
};

typedef struct IDirectInputW *LPDIRECTINPUTW;

#undef INTERFACE
#define INTERFACE IDirectInputA

DECLARE_INTERFACE_(IDirectInputA, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEA *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
};

typedef struct IDirectInputA *LPDIRECTINPUTA;

#ifdef UNICODE
#define IID_IDirectInput IID_IDirectInputW
#define IDirectInput IDirectInputW
#define IDirectInputVtbl IDirectInputWVtbl
#else
#define IID_IDirectInput IID_IDirectInputA
#define IDirectInput IDirectInputA
#define IDirectInputVtbl IDirectInputAVtbl
#endif
typedef struct IDirectInput *LPDIRECTINPUT;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInput_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInput_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInput_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInput_CreateDevice(p,a,b,c) (p)->lpVtbl->CreateDevice(p,a,b,c)
#define IDirectInput_EnumDevices(p,a,b,c,d) (p)->lpVtbl->EnumDevices(p,a,b,c,d)
#define IDirectInput_GetDeviceStatus(p,a) (p)->lpVtbl->GetDeviceStatus(p,a)
#define IDirectInput_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInput_Initialize(p,a,b) (p)->lpVtbl->Initialize(p,a,b)
#else
#define IDirectInput_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInput_AddRef(p) (p)->AddRef()
#define IDirectInput_Release(p) (p)->Release()
#define IDirectInput_CreateDevice(p,a,b,c) (p)->CreateDevice(a,b,c)
#define IDirectInput_EnumDevices(p,a,b,c,d) (p)->EnumDevices(a,b,c,d)
#define IDirectInput_GetDeviceStatus(p,a) (p)->GetDeviceStatus(a)
#define IDirectInput_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInput_Initialize(p,a,b) (p)->Initialize(a,b)
#endif

#undef INTERFACE
#define INTERFACE IDirectInput2W

DECLARE_INTERFACE_(IDirectInput2W, IDirectInputW)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEW *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;

    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCWSTR,LPGUID) PURE;
};

typedef struct IDirectInput2W *LPDIRECTINPUT2W;

#undef INTERFACE
#define INTERFACE IDirectInput2A

DECLARE_INTERFACE_(IDirectInput2A, IDirectInputA)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEA *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;

    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCSTR,LPGUID) PURE;
};

typedef struct IDirectInput2A *LPDIRECTINPUT2A;

#ifdef UNICODE
#define IID_IDirectInput2 IID_IDirectInput2W
#define IDirectInput2 IDirectInput2W
#define IDirectInput2Vtbl IDirectInput2WVtbl
#else
#define IID_IDirectInput2 IID_IDirectInput2A
#define IDirectInput2 IDirectInput2A
#define IDirectInput2Vtbl IDirectInput2AVtbl
#endif
typedef struct IDirectInput2 *LPDIRECTINPUT2;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInput2_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInput2_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInput2_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInput2_CreateDevice(p,a,b,c) (p)->lpVtbl->CreateDevice(p,a,b,c)
#define IDirectInput2_EnumDevices(p,a,b,c,d) (p)->lpVtbl->EnumDevices(p,a,b,c,d)
#define IDirectInput2_GetDeviceStatus(p,a) (p)->lpVtbl->GetDeviceStatus(p,a)
#define IDirectInput2_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInput2_Initialize(p,a,b) (p)->lpVtbl->Initialize(p,a,b)
#define IDirectInput2_FindDevice(p,a,b,c) (p)->lpVtbl->FindDevice(p,a,b,c)
#else
#define IDirectInput2_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInput2_AddRef(p) (p)->AddRef()
#define IDirectInput2_Release(p) (p)->Release()
#define IDirectInput2_CreateDevice(p,a,b,c) (p)->CreateDevice(a,b,c)
#define IDirectInput2_EnumDevices(p,a,b,c,d) (p)->EnumDevices(a,b,c,d)
#define IDirectInput2_GetDeviceStatus(p,a) (p)->GetDeviceStatus(a)
#define IDirectInput2_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInput2_Initialize(p,a,b) (p)->Initialize(a,b)
#define IDirectInput2_FindDevice(p,a,b,c) (p)->FindDevice(a,b,c)
#endif

#undef INTERFACE
#define INTERFACE IDirectInput7W

DECLARE_INTERFACE_(IDirectInput7W, IDirectInput2W)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEW *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCWSTR,LPGUID) PURE;

    STDMETHOD(CreateDeviceEx)(THIS_ REFGUID,REFIID,LPVOID *,LPUNKNOWN) PURE;
};

typedef struct IDirectInput7W *LPDIRECTINPUT7W;

#undef INTERFACE
#define INTERFACE IDirectInput7A

DECLARE_INTERFACE_(IDirectInput7A, IDirectInput2A)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEA *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCSTR,LPGUID) PURE;

    STDMETHOD(CreateDeviceEx)(THIS_ REFGUID,REFIID,LPVOID *,LPUNKNOWN) PURE;
};

typedef struct IDirectInput7A *LPDIRECTINPUT7A;

#ifdef UNICODE
#define IID_IDirectInput7 IID_IDirectInput7W
#define IDirectInput7 IDirectInput7W
#define IDirectInput7Vtbl IDirectInput7WVtbl
#else
#define IID_IDirectInput7 IID_IDirectInput7A
#define IDirectInput7 IDirectInput7A
#define IDirectInput7Vtbl IDirectInput7AVtbl
#endif
typedef struct IDirectInput7 *LPDIRECTINPUT7;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInput7_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInput7_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInput7_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInput7_CreateDevice(p,a,b,c) (p)->lpVtbl->CreateDevice(p,a,b,c)
#define IDirectInput7_EnumDevices(p,a,b,c,d) (p)->lpVtbl->EnumDevices(p,a,b,c,d)
#define IDirectInput7_GetDeviceStatus(p,a) (p)->lpVtbl->GetDeviceStatus(p,a)
#define IDirectInput7_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInput7_Initialize(p,a,b) (p)->lpVtbl->Initialize(p,a,b)
#define IDirectInput7_FindDevice(p,a,b,c) (p)->lpVtbl->FindDevice(p,a,b,c)
#define IDirectInput7_CreateDeviceEx(p,a,b,c,d) (p)->lpVtbl->CreateDeviceEx(p,a,b,c,d)
#else
#define IDirectInput7_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInput7_AddRef(p) (p)->AddRef()
#define IDirectInput7_Release(p) (p)->Release()
#define IDirectInput7_CreateDevice(p,a,b,c) (p)->CreateDevice(a,b,c)
#define IDirectInput7_EnumDevices(p,a,b,c,d) (p)->EnumDevices(a,b,c,d)
#define IDirectInput7_GetDeviceStatus(p,a) (p)->GetDeviceStatus(a)
#define IDirectInput7_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInput7_Initialize(p,a,b) (p)->Initialize(a,b)
#define IDirectInput7_FindDevice(p,a,b,c) (p)->FindDevice(a,b,c)
#define IDirectInput7_CreateDeviceEx(p,a,b,c,d) (p)->CreateDeviceEx(a,b,c,d)
#endif

#if(DIRECTINPUT_VERSION >= 0x0800)
#undef INTERFACE
#define INTERFACE IDirectInput8W

DECLARE_INTERFACE_(IDirectInput8W, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICE8W *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKW,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCWSTR,LPGUID) PURE;
    STDMETHOD(EnumDevicesBySemantics)(THIS_ LPCWSTR,LPDIACTIONFORMATW,LPDIENUMDEVICESBYSEMANTICSCBW,LPVOID,DWORD) PURE;
    STDMETHOD(ConfigureDevices)(THIS_ LPDICONFIGUREDEVICESCALLBACK,LPDICONFIGUREDEVICESPARAMSW,DWORD,LPVOID) PURE;
};

typedef struct IDirectInput8W *LPDIRECTINPUT8W;

#undef INTERFACE
#define INTERFACE IDirectInput8A

DECLARE_INTERFACE_(IDirectInput8A, IUnknown)
{

    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICE8A *,LPUNKNOWN) PURE;
    STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD) PURE;
    STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
    STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
    STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;
    STDMETHOD(FindDevice)(THIS_ REFGUID,LPCSTR,LPGUID) PURE;
    STDMETHOD(EnumDevicesBySemantics)(THIS_ LPCSTR,LPDIACTIONFORMATA,LPDIENUMDEVICESBYSEMANTICSCBA,LPVOID,DWORD) PURE;
    STDMETHOD(ConfigureDevices)(THIS_ LPDICONFIGUREDEVICESCALLBACK,LPDICONFIGUREDEVICESPARAMSA,DWORD,LPVOID) PURE;
};

typedef struct IDirectInput8A *LPDIRECTINPUT8A;

#ifdef UNICODE
#define IID_IDirectInput8 IID_IDirectInput8W
#define IDirectInput8 IDirectInput8W
#define IDirectInput8Vtbl IDirectInput8WVtbl
#else
#define IID_IDirectInput8 IID_IDirectInput8A
#define IDirectInput8 IDirectInput8A
#define IDirectInput8Vtbl IDirectInput8AVtbl
#endif
typedef struct IDirectInput8 *LPDIRECTINPUT8;

#if !defined(__cplusplus) || defined(CINTERFACE)
#define IDirectInput8_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
#define IDirectInput8_AddRef(p) (p)->lpVtbl->AddRef(p)
#define IDirectInput8_Release(p) (p)->lpVtbl->Release(p)
#define IDirectInput8_CreateDevice(p,a,b,c) (p)->lpVtbl->CreateDevice(p,a,b,c)
#define IDirectInput8_EnumDevices(p,a,b,c,d) (p)->lpVtbl->EnumDevices(p,a,b,c,d)
#define IDirectInput8_GetDeviceStatus(p,a) (p)->lpVtbl->GetDeviceStatus(p,a)
#define IDirectInput8_RunControlPanel(p,a,b) (p)->lpVtbl->RunControlPanel(p,a,b)
#define IDirectInput8_Initialize(p,a,b) (p)->lpVtbl->Initialize(p,a,b)
#define IDirectInput8_FindDevice(p,a,b,c) (p)->lpVtbl->FindDevice(p,a,b,c)
#define IDirectInput8_EnumDevicesBySemantics(p,a,b,c,d,e) (p)->lpVtbl->EnumDevicesBySemantics(p,a,b,c,d,e)
#define IDirectInput8_ConfigureDevices(p,a,b,c,d) (p)->lpVtbl->ConfigureDevices(p,a,b,c,d)
#else
#define IDirectInput8_QueryInterface(p,a,b) (p)->QueryInterface(a,b)
#define IDirectInput8_AddRef(p) (p)->AddRef()
#define IDirectInput8_Release(p) (p)->Release()
#define IDirectInput8_CreateDevice(p,a,b,c) (p)->CreateDevice(a,b,c)
#define IDirectInput8_EnumDevices(p,a,b,c,d) (p)->EnumDevices(a,b,c,d)
#define IDirectInput8_GetDeviceStatus(p,a) (p)->GetDeviceStatus(a)
#define IDirectInput8_RunControlPanel(p,a,b) (p)->RunControlPanel(a,b)
#define IDirectInput8_Initialize(p,a,b) (p)->Initialize(a,b)
#define IDirectInput8_FindDevice(p,a,b,c) (p)->FindDevice(a,b,c)
#define IDirectInput8_EnumDevicesBySemantics(p,a,b,c,d,e) (p)->EnumDevicesBySemantics(a,b,c,d,e)
#define IDirectInput8_ConfigureDevices(p,a,b,c,d) (p)->ConfigureDevices(a,b,c,d)
#endif
#endif

#if DIRECTINPUT_VERSION > 0x0700

extern HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

#else
extern HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter);
extern HRESULT WINAPI DirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTW *ppDI, LPUNKNOWN punkOuter);
#ifdef UNICODE
#define DirectInputCreate  DirectInputCreateW
#else
#define DirectInputCreate  DirectInputCreateA
#endif

extern HRESULT WINAPI DirectInputCreateEx(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

#endif

#endif

#define DI_OK                           S_OK

#define DI_NOTATTACHED                  S_FALSE

#define DI_BUFFEROVERFLOW               S_FALSE

#define DI_PROPNOEFFECT                 S_FALSE

#define DI_NOEFFECT                     S_FALSE

#define DI_POLLEDDEVICE                 ((HRESULT)0x00000002L)

#define DI_DOWNLOADSKIPPED              ((HRESULT)0x00000003L)

#define DI_EFFECTRESTARTED              ((HRESULT)0x00000004L)

#define DI_TRUNCATED                    ((HRESULT)0x00000008L)

#define DI_SETTINGSNOTSAVED				((HRESULT)0x0000000BL)

#define DI_TRUNCATEDANDRESTARTED        ((HRESULT)0x0000000CL)

#define DI_WRITEPROTECT                 ((HRESULT)0x00000013L)

#define DIERR_OLDDIRECTINPUTVERSION     \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_OLD_WIN_VERSION)

#define DIERR_BETADIRECTINPUTVERSION    \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_RMODE_APP)

#define DIERR_BADDRIVERVER              \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_BAD_DRIVER_LEVEL)

#define DIERR_DEVICENOTREG              REGDB_E_CLASSNOTREG

#define DIERR_NOTFOUND                  \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_FILE_NOT_FOUND)

#define DIERR_OBJECTNOTFOUND            \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_FILE_NOT_FOUND)

#define DIERR_INVALIDPARAM              E_INVALIDARG

#define DIERR_NOINTERFACE               E_NOINTERFACE

#define DIERR_GENERIC                   E_FAIL

#define DIERR_OUTOFMEMORY               E_OUTOFMEMORY

#define DIERR_UNSUPPORTED               E_NOTIMPL

#define DIERR_NOTINITIALIZED            \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_NOT_READY)

#define DIERR_ALREADYINITIALIZED        \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_ALREADY_INITIALIZED)

#define DIERR_NOAGGREGATION             CLASS_E_NOAGGREGATION

#define DIERR_OTHERAPPHASPRIO           E_ACCESSDENIED

#define DIERR_INPUTLOST                 \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_READ_FAULT)

#define DIERR_ACQUIRED                  \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_BUSY)

#define DIERR_NOTACQUIRED               \
    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_INVALID_ACCESS)

#define DIERR_READONLY                  E_ACCESSDENIED

#define DIERR_HANDLEEXISTS              E_ACCESSDENIED

#ifndef E_PENDING
#define E_PENDING                       0x8000000AL
#endif

#define DIERR_INSUFFICIENTPRIVS         0x80040200L

#define DIERR_DEVICEFULL                0x80040201L

#define DIERR_MOREDATA                  0x80040202L

#define DIERR_NOTDOWNLOADED             0x80040203L

#define DIERR_HASEFFECTS                0x80040204L

#define DIERR_NOTEXCLUSIVEACQUIRED      0x80040205L

#define DIERR_INCOMPLETEEFFECT          0x80040206L

#define DIERR_NOTBUFFERED               0x80040207L

#define DIERR_EFFECTPLAYING             0x80040208L

#define DIERR_UNPLUGGED                 0x80040209L

#define DIERR_REPORTFULL                0x8004020AL

#define DIERR_MAPFILEFAIL               0x8004020BL

#define DIKEYBOARD_ESCAPE                       0x81000401
#define DIKEYBOARD_1                            0x81000402
#define DIKEYBOARD_2                            0x81000403
#define DIKEYBOARD_3                            0x81000404
#define DIKEYBOARD_4                            0x81000405
#define DIKEYBOARD_5                            0x81000406
#define DIKEYBOARD_6                            0x81000407
#define DIKEYBOARD_7                            0x81000408
#define DIKEYBOARD_8                            0x81000409
#define DIKEYBOARD_9                            0x8100040A
#define DIKEYBOARD_0                            0x8100040B
#define DIKEYBOARD_MINUS                        0x8100040C
#define DIKEYBOARD_EQUALS                       0x8100040D
#define DIKEYBOARD_BACK                         0x8100040E
#define DIKEYBOARD_TAB                          0x8100040F
#define DIKEYBOARD_Q                            0x81000410
#define DIKEYBOARD_W                            0x81000411
#define DIKEYBOARD_E                            0x81000412
#define DIKEYBOARD_R                            0x81000413
#define DIKEYBOARD_T                            0x81000414
#define DIKEYBOARD_Y                            0x81000415
#define DIKEYBOARD_U                            0x81000416
#define DIKEYBOARD_I                            0x81000417
#define DIKEYBOARD_O                            0x81000418
#define DIKEYBOARD_P                            0x81000419
#define DIKEYBOARD_LBRACKET                     0x8100041A
#define DIKEYBOARD_RBRACKET                     0x8100041B
#define DIKEYBOARD_RETURN                       0x8100041C
#define DIKEYBOARD_LCONTROL                     0x8100041D
#define DIKEYBOARD_A                            0x8100041E
#define DIKEYBOARD_S                            0x8100041F
#define DIKEYBOARD_D                            0x81000420
#define DIKEYBOARD_F                            0x81000421
#define DIKEYBOARD_G                            0x81000422
#define DIKEYBOARD_H                            0x81000423
#define DIKEYBOARD_J                            0x81000424
#define DIKEYBOARD_K                            0x81000425
#define DIKEYBOARD_L                            0x81000426
#define DIKEYBOARD_SEMICOLON                    0x81000427
#define DIKEYBOARD_APOSTROPHE                   0x81000428
#define DIKEYBOARD_GRAVE                        0x81000429
#define DIKEYBOARD_LSHIFT                       0x8100042A
#define DIKEYBOARD_BACKSLASH                    0x8100042B
#define DIKEYBOARD_Z                            0x8100042C
#define DIKEYBOARD_X                            0x8100042D
#define DIKEYBOARD_C                            0x8100042E
#define DIKEYBOARD_V                            0x8100042F
#define DIKEYBOARD_B                            0x81000430
#define DIKEYBOARD_N                            0x81000431
#define DIKEYBOARD_M                            0x81000432
#define DIKEYBOARD_COMMA                        0x81000433
#define DIKEYBOARD_PERIOD                       0x81000434
#define DIKEYBOARD_SLASH                        0x81000435
#define DIKEYBOARD_RSHIFT                       0x81000436
#define DIKEYBOARD_MULTIPLY                     0x81000437
#define DIKEYBOARD_LMENU                        0x81000438
#define DIKEYBOARD_SPACE                        0x81000439
#define DIKEYBOARD_CAPITAL                      0x8100043A
#define DIKEYBOARD_F1                           0x8100043B
#define DIKEYBOARD_F2                           0x8100043C
#define DIKEYBOARD_F3                           0x8100043D
#define DIKEYBOARD_F4                           0x8100043E
#define DIKEYBOARD_F5                           0x8100043F
#define DIKEYBOARD_F6                           0x81000440
#define DIKEYBOARD_F7                           0x81000441
#define DIKEYBOARD_F8                           0x81000442
#define DIKEYBOARD_F9                           0x81000443
#define DIKEYBOARD_F10                          0x81000444
#define DIKEYBOARD_NUMLOCK                      0x81000445
#define DIKEYBOARD_SCROLL                       0x81000446
#define DIKEYBOARD_NUMPAD7                      0x81000447
#define DIKEYBOARD_NUMPAD8                      0x81000448
#define DIKEYBOARD_NUMPAD9                      0x81000449
#define DIKEYBOARD_SUBTRACT                     0x8100044A
#define DIKEYBOARD_NUMPAD4                      0x8100044B
#define DIKEYBOARD_NUMPAD5                      0x8100044C
#define DIKEYBOARD_NUMPAD6                      0x8100044D
#define DIKEYBOARD_ADD                          0x8100044E
#define DIKEYBOARD_NUMPAD1                      0x8100044F
#define DIKEYBOARD_NUMPAD2                      0x81000450
#define DIKEYBOARD_NUMPAD3                      0x81000451
#define DIKEYBOARD_NUMPAD0                      0x81000452
#define DIKEYBOARD_DECIMAL                      0x81000453
#define DIKEYBOARD_OEM_102                      0x81000456
#define DIKEYBOARD_F11                          0x81000457
#define DIKEYBOARD_F12                          0x81000458
#define DIKEYBOARD_F13                          0x81000464
#define DIKEYBOARD_F14                          0x81000465
#define DIKEYBOARD_F15                          0x81000466
#define DIKEYBOARD_KANA                         0x81000470
#define DIKEYBOARD_ABNT_C1                      0x81000473
#define DIKEYBOARD_CONVERT                      0x81000479
#define DIKEYBOARD_NOCONVERT                    0x8100047B
#define DIKEYBOARD_YEN                          0x8100047D
#define DIKEYBOARD_ABNT_C2                      0x8100047E
#define DIKEYBOARD_NUMPADEQUALS                 0x8100048D
#define DIKEYBOARD_PREVTRACK                    0x81000490
#define DIKEYBOARD_AT                           0x81000491
#define DIKEYBOARD_COLON                        0x81000492
#define DIKEYBOARD_UNDERLINE                    0x81000493
#define DIKEYBOARD_KANJI                        0x81000494
#define DIKEYBOARD_STOP                         0x81000495
#define DIKEYBOARD_AX                           0x81000496
#define DIKEYBOARD_UNLABELED                    0x81000497
#define DIKEYBOARD_NEXTTRACK                    0x81000499
#define DIKEYBOARD_NUMPADENTER                  0x8100049C
#define DIKEYBOARD_RCONTROL                     0x8100049D
#define DIKEYBOARD_MUTE                         0x810004A0
#define DIKEYBOARD_CALCULATOR                   0x810004A1
#define DIKEYBOARD_PLAYPAUSE                    0x810004A2
#define DIKEYBOARD_MEDIASTOP                    0x810004A4
#define DIKEYBOARD_VOLUMEDOWN                   0x810004AE
#define DIKEYBOARD_VOLUMEUP                     0x810004B0
#define DIKEYBOARD_WEBHOME                      0x810004B2
#define DIKEYBOARD_NUMPADCOMMA                  0x810004B3
#define DIKEYBOARD_DIVIDE                       0x810004B5
#define DIKEYBOARD_SYSRQ                        0x810004B7
#define DIKEYBOARD_RMENU                        0x810004B8
#define DIKEYBOARD_PAUSE                        0x810004C5
#define DIKEYBOARD_HOME                         0x810004C7
#define DIKEYBOARD_UP                           0x810004C8
#define DIKEYBOARD_PRIOR                        0x810004C9
#define DIKEYBOARD_LEFT                         0x810004CB
#define DIKEYBOARD_RIGHT                        0x810004CD
#define DIKEYBOARD_END                          0x810004CF
#define DIKEYBOARD_DOWN                         0x810004D0
#define DIKEYBOARD_NEXT                         0x810004D1
#define DIKEYBOARD_INSERT                       0x810004D2
#define DIKEYBOARD_DELETE                       0x810004D3
#define DIKEYBOARD_LWIN                         0x810004DB
#define DIKEYBOARD_RWIN                         0x810004DC
#define DIKEYBOARD_APPS                         0x810004DD
#define DIKEYBOARD_POWER                        0x810004DE
#define DIKEYBOARD_SLEEP                        0x810004DF
#define DIKEYBOARD_WAKE                         0x810004E3
#define DIKEYBOARD_WEBSEARCH                    0x810004E5
#define DIKEYBOARD_WEBFAVORITES                 0x810004E6
#define DIKEYBOARD_WEBREFRESH                   0x810004E7
#define DIKEYBOARD_WEBSTOP                      0x810004E8
#define DIKEYBOARD_WEBFORWARD                   0x810004E9
#define DIKEYBOARD_WEBBACK                      0x810004EA
#define DIKEYBOARD_MYCOMPUTER                   0x810004EB
#define DIKEYBOARD_MAIL                         0x810004EC
#define DIKEYBOARD_MEDIASELECT                  0x810004ED

#define DIMOUSE_XAXISAB                         (0x82000200 |DIMOFS_X )
#define DIMOUSE_YAXISAB                         (0x82000200 |DIMOFS_Y )
#define DIMOUSE_XAXIS                           (0x82000300 |DIMOFS_X )
#define DIMOUSE_YAXIS                           (0x82000300 |DIMOFS_Y )
#define DIMOUSE_WHEEL                           (0x82000300 |DIMOFS_Z )
#define DIMOUSE_BUTTON0                         (0x82000400 |DIMOFS_BUTTON0)
#define DIMOUSE_BUTTON1                         (0x82000400 |DIMOFS_BUTTON1)
#define DIMOUSE_BUTTON2                         (0x82000400 |DIMOFS_BUTTON2)
#define DIMOUSE_BUTTON3                         (0x82000400 |DIMOFS_BUTTON3)
#define DIMOUSE_BUTTON4                         (0x82000400 |DIMOFS_BUTTON4)
#define DIMOUSE_BUTTON5                         (0x82000400 |DIMOFS_BUTTON5)
#define DIMOUSE_BUTTON6                         (0x82000400 |DIMOFS_BUTTON6)
#define DIMOUSE_BUTTON7                         (0x82000400 |DIMOFS_BUTTON7)

#define DIVOICE_CHANNEL1                        0x83000401
#define DIVOICE_CHANNEL2                        0x83000402
#define DIVOICE_CHANNEL3                        0x83000403
#define DIVOICE_CHANNEL4                        0x83000404
#define DIVOICE_CHANNEL5                        0x83000405
#define DIVOICE_CHANNEL6                        0x83000406
#define DIVOICE_CHANNEL7                        0x83000407
#define DIVOICE_CHANNEL8                        0x83000408
#define DIVOICE_TEAM                            0x83000409
#define DIVOICE_ALL                             0x8300040A
#define DIVOICE_RECORDMUTE                      0x8300040B
#define DIVOICE_PLAYBACKMUTE                    0x8300040C
#define DIVOICE_TRANSMIT                        0x8300040D

#define DIVOICE_VOICECOMMAND                    0x83000410

#define DIVIRTUAL_DRIVING_RACE                  0x01000000
#define DIAXIS_DRIVINGR_STEER                   0x01008A01
#define DIAXIS_DRIVINGR_ACCELERATE              0x01039202
#define DIAXIS_DRIVINGR_BRAKE                   0x01041203
#define DIBUTTON_DRIVINGR_SHIFTUP               0x01000C01
#define DIBUTTON_DRIVINGR_SHIFTDOWN             0x01000C02
#define DIBUTTON_DRIVINGR_VIEW                  0x01001C03
#define DIBUTTON_DRIVINGR_MENU                  0x010004FD

#define DIAXIS_DRIVINGR_ACCEL_AND_BRAKE         0x01014A04
#define DIHATSWITCH_DRIVINGR_GLANCE             0x01004601
#define DIBUTTON_DRIVINGR_BRAKE                 0x01004C04
#define DIBUTTON_DRIVINGR_DASHBOARD             0x01004405
#define DIBUTTON_DRIVINGR_AIDS                  0x01004406
#define DIBUTTON_DRIVINGR_MAP                   0x01004407
#define DIBUTTON_DRIVINGR_BOOST                 0x01004408
#define DIBUTTON_DRIVINGR_PIT                   0x01004409
#define DIBUTTON_DRIVINGR_ACCELERATE_LINK       0x0103D4E0
#define DIBUTTON_DRIVINGR_STEER_LEFT_LINK       0x0100CCE4
#define DIBUTTON_DRIVINGR_STEER_RIGHT_LINK      0x0100CCEC
#define DIBUTTON_DRIVINGR_GLANCE_LEFT_LINK      0x0107C4E4
#define DIBUTTON_DRIVINGR_GLANCE_RIGHT_LINK     0x0107C4EC
#define DIBUTTON_DRIVINGR_DEVICE                0x010044FE
#define DIBUTTON_DRIVINGR_PAUSE                 0x010044FC

#define DIVIRTUAL_DRIVING_COMBAT                0x02000000
#define DIAXIS_DRIVINGC_STEER                   0x02008A01
#define DIAXIS_DRIVINGC_ACCELERATE              0x02039202
#define DIAXIS_DRIVINGC_BRAKE                   0x02041203
#define DIBUTTON_DRIVINGC_FIRE                  0x02000C01
#define DIBUTTON_DRIVINGC_WEAPONS               0x02000C02
#define DIBUTTON_DRIVINGC_TARGET                0x02000C03
#define DIBUTTON_DRIVINGC_MENU                  0x020004FD

#define DIAXIS_DRIVINGC_ACCEL_AND_BRAKE         0x02014A04
#define DIHATSWITCH_DRIVINGC_GLANCE             0x02004601
#define DIBUTTON_DRIVINGC_SHIFTUP               0x02004C04
#define DIBUTTON_DRIVINGC_SHIFTDOWN             0x02004C05
#define DIBUTTON_DRIVINGC_DASHBOARD             0x02004406
#define DIBUTTON_DRIVINGC_AIDS                  0x02004407
#define DIBUTTON_DRIVINGC_BRAKE                 0x02004C08
#define DIBUTTON_DRIVINGC_FIRESECONDARY         0x02004C09
#define DIBUTTON_DRIVINGC_ACCELERATE_LINK       0x0203D4E0
#define DIBUTTON_DRIVINGC_STEER_LEFT_LINK       0x0200CCE4
#define DIBUTTON_DRIVINGC_STEER_RIGHT_LINK      0x0200CCEC
#define DIBUTTON_DRIVINGC_GLANCE_LEFT_LINK      0x0207C4E4
#define DIBUTTON_DRIVINGC_GLANCE_RIGHT_LINK     0x0207C4EC
#define DIBUTTON_DRIVINGC_DEVICE                0x020044FE
#define DIBUTTON_DRIVINGC_PAUSE                 0x020044FC

#define DIVIRTUAL_DRIVING_TANK                  0x03000000
#define DIAXIS_DRIVINGT_STEER                   0x03008A01
#define DIAXIS_DRIVINGT_BARREL                  0x03010202
#define DIAXIS_DRIVINGT_ACCELERATE              0x03039203
#define DIAXIS_DRIVINGT_ROTATE                  0x03020204
#define DIBUTTON_DRIVINGT_FIRE                  0x03000C01
#define DIBUTTON_DRIVINGT_WEAPONS               0x03000C02
#define DIBUTTON_DRIVINGT_TARGET                0x03000C03
#define DIBUTTON_DRIVINGT_MENU                  0x030004FD

#define DIHATSWITCH_DRIVINGT_GLANCE             0x03004601
#define DIAXIS_DRIVINGT_BRAKE                   0x03045205
#define DIAXIS_DRIVINGT_ACCEL_AND_BRAKE         0x03014A06
#define DIBUTTON_DRIVINGT_VIEW                  0x03005C04
#define DIBUTTON_DRIVINGT_DASHBOARD             0x03005C05
#define DIBUTTON_DRIVINGT_BRAKE                 0x03004C06
#define DIBUTTON_DRIVINGT_FIRESECONDARY         0x03004C07
#define DIBUTTON_DRIVINGT_ACCELERATE_LINK       0x0303D4E0
#define DIBUTTON_DRIVINGT_STEER_LEFT_LINK       0x0300CCE4
#define DIBUTTON_DRIVINGT_STEER_RIGHT_LINK      0x0300CCEC
#define DIBUTTON_DRIVINGT_BARREL_UP_LINK        0x030144E0
#define DIBUTTON_DRIVINGT_BARREL_DOWN_LINK      0x030144E8
#define DIBUTTON_DRIVINGT_ROTATE_LEFT_LINK      0x030244E4
#define DIBUTTON_DRIVINGT_ROTATE_RIGHT_LINK     0x030244EC
#define DIBUTTON_DRIVINGT_GLANCE_LEFT_LINK      0x0307C4E4
#define DIBUTTON_DRIVINGT_GLANCE_RIGHT_LINK     0x0307C4EC
#define DIBUTTON_DRIVINGT_DEVICE                0x030044FE
#define DIBUTTON_DRIVINGT_PAUSE                 0x030044FC

#define DIVIRTUAL_FLYING_CIVILIAN               0x04000000
#define DIAXIS_FLYINGC_BANK                     0x04008A01
#define DIAXIS_FLYINGC_PITCH                    0x04010A02
#define DIAXIS_FLYINGC_THROTTLE                 0x04039203
#define DIBUTTON_FLYINGC_VIEW                   0x04002401
#define DIBUTTON_FLYINGC_DISPLAY                0x04002402
#define DIBUTTON_FLYINGC_GEAR                   0x04002C03
#define DIBUTTON_FLYINGC_MENU                   0x040004FD

#define DIHATSWITCH_FLYINGC_GLANCE              0x04004601
#define DIAXIS_FLYINGC_BRAKE                    0x04046A04
#define DIAXIS_FLYINGC_RUDDER                   0x04025205
#define DIAXIS_FLYINGC_FLAPS                    0x04055A06
#define DIBUTTON_FLYINGC_FLAPSUP                0x04006404
#define DIBUTTON_FLYINGC_FLAPSDOWN              0x04006405
#define DIBUTTON_FLYINGC_BRAKE_LINK             0x04046CE0
#define DIBUTTON_FLYINGC_FASTER_LINK            0x0403D4E0
#define DIBUTTON_FLYINGC_SLOWER_LINK            0x0403D4E8
#define DIBUTTON_FLYINGC_GLANCE_LEFT_LINK       0x0407C4E4
#define DIBUTTON_FLYINGC_GLANCE_RIGHT_LINK      0x0407C4EC
#define DIBUTTON_FLYINGC_GLANCE_UP_LINK         0x0407C4E0
#define DIBUTTON_FLYINGC_GLANCE_DOWN_LINK       0x0407C4E8
#define DIBUTTON_FLYINGC_DEVICE                 0x040044FE
#define DIBUTTON_FLYINGC_PAUSE                  0x040044FC

#define DIVIRTUAL_FLYING_MILITARY               0x05000000
#define DIAXIS_FLYINGM_BANK                     0x05008A01
#define DIAXIS_FLYINGM_PITCH                    0x05010A02
#define DIAXIS_FLYINGM_THROTTLE                 0x05039203
#define DIBUTTON_FLYINGM_FIRE                   0x05000C01
#define DIBUTTON_FLYINGM_WEAPONS                0x05000C02
#define DIBUTTON_FLYINGM_TARGET                 0x05000C03
#define DIBUTTON_FLYINGM_MENU                   0x050004FD

#define DIHATSWITCH_FLYINGM_GLANCE              0x05004601
#define DIBUTTON_FLYINGM_COUNTER                0x05005C04
#define DIAXIS_FLYINGM_RUDDER                   0x05024A04
#define DIAXIS_FLYINGM_BRAKE                    0x05046205
#define DIBUTTON_FLYINGM_VIEW                   0x05006405
#define DIBUTTON_FLYINGM_DISPLAY                0x05006406
#define DIAXIS_FLYINGM_FLAPS                    0x05055206
#define DIBUTTON_FLYINGM_FLAPSUP                0x05005407
#define DIBUTTON_FLYINGM_FLAPSDOWN              0x05005408
#define DIBUTTON_FLYINGM_FIRESECONDARY          0x05004C09
#define DIBUTTON_FLYINGM_GEAR                   0x0500640A
#define DIBUTTON_FLYINGM_BRAKE_LINK             0x050464E0
#define DIBUTTON_FLYINGM_FASTER_LINK            0x0503D4E0
#define DIBUTTON_FLYINGM_SLOWER_LINK            0x0503D4E8
#define DIBUTTON_FLYINGM_GLANCE_LEFT_LINK       0x0507C4E4
#define DIBUTTON_FLYINGM_GLANCE_RIGHT_LINK      0x0507C4EC
#define DIBUTTON_FLYINGM_GLANCE_UP_LINK         0x0507C4E0
#define DIBUTTON_FLYINGM_GLANCE_DOWN_LINK       0x0507C4E8
#define DIBUTTON_FLYINGM_DEVICE                 0x050044FE
#define DIBUTTON_FLYINGM_PAUSE                  0x050044FC

#define DIVIRTUAL_FLYING_HELICOPTER             0x06000000
#define DIAXIS_FLYINGH_BANK                     0x06008A01
#define DIAXIS_FLYINGH_PITCH                    0x06010A02
#define DIAXIS_FLYINGH_COLLECTIVE               0x06018A03
#define DIBUTTON_FLYINGH_FIRE                   0x06001401
#define DIBUTTON_FLYINGH_WEAPONS                0x06001402
#define DIBUTTON_FLYINGH_TARGET                 0x06001403
#define DIBUTTON_FLYINGH_MENU                   0x060004FD

#define DIHATSWITCH_FLYINGH_GLANCE              0x06004601
#define DIAXIS_FLYINGH_TORQUE                   0x06025A04
#define DIAXIS_FLYINGH_THROTTLE                 0x0603DA05
#define DIBUTTON_FLYINGH_COUNTER                0x06005404
#define DIBUTTON_FLYINGH_VIEW                   0x06006405
#define DIBUTTON_FLYINGH_GEAR                   0x06006406
#define DIBUTTON_FLYINGH_FIRESECONDARY          0x06004C07
#define DIBUTTON_FLYINGH_FASTER_LINK            0x0603DCE0
#define DIBUTTON_FLYINGH_SLOWER_LINK            0x0603DCE8
#define DIBUTTON_FLYINGH_GLANCE_LEFT_LINK       0x0607C4E4
#define DIBUTTON_FLYINGH_GLANCE_RIGHT_LINK      0x0607C4EC
#define DIBUTTON_FLYINGH_GLANCE_UP_LINK         0x0607C4E0
#define DIBUTTON_FLYINGH_GLANCE_DOWN_LINK       0x0607C4E8
#define DIBUTTON_FLYINGH_DEVICE                 0x060044FE
#define DIBUTTON_FLYINGH_PAUSE                  0x060044FC

#define DIVIRTUAL_SPACESIM                      0x07000000
#define DIAXIS_SPACESIM_LATERAL                 0x07008201
#define DIAXIS_SPACESIM_MOVE                    0x07010202
#define DIAXIS_SPACESIM_THROTTLE                0x07038203
#define DIBUTTON_SPACESIM_FIRE                  0x07000401
#define DIBUTTON_SPACESIM_WEAPONS               0x07000402
#define DIBUTTON_SPACESIM_TARGET                0x07000403
#define DIBUTTON_SPACESIM_MENU                  0x070004FD

#define DIHATSWITCH_SPACESIM_GLANCE             0x07004601
#define DIAXIS_SPACESIM_CLIMB                   0x0701C204
#define DIAXIS_SPACESIM_ROTATE                  0x07024205
#define DIBUTTON_SPACESIM_VIEW                  0x07004404
#define DIBUTTON_SPACESIM_DISPLAY               0x07004405
#define DIBUTTON_SPACESIM_RAISE                 0x07004406
#define DIBUTTON_SPACESIM_LOWER                 0x07004407
#define DIBUTTON_SPACESIM_GEAR                  0x07004408
#define DIBUTTON_SPACESIM_FIRESECONDARY         0x07004409
#define DIBUTTON_SPACESIM_LEFT_LINK             0x0700C4E4
#define DIBUTTON_SPACESIM_RIGHT_LINK            0x0700C4EC
#define DIBUTTON_SPACESIM_FORWARD_LINK          0x070144E0
#define DIBUTTON_SPACESIM_BACKWARD_LINK         0x070144E8
#define DIBUTTON_SPACESIM_FASTER_LINK           0x0703C4E0
#define DIBUTTON_SPACESIM_SLOWER_LINK           0x0703C4E8
#define DIBUTTON_SPACESIM_TURN_LEFT_LINK        0x070244E4
#define DIBUTTON_SPACESIM_TURN_RIGHT_LINK       0x070244EC
#define DIBUTTON_SPACESIM_GLANCE_LEFT_LINK      0x0707C4E4
#define DIBUTTON_SPACESIM_GLANCE_RIGHT_LINK     0x0707C4EC
#define DIBUTTON_SPACESIM_GLANCE_UP_LINK        0x0707C4E0
#define DIBUTTON_SPACESIM_GLANCE_DOWN_LINK      0x0707C4E8
#define DIBUTTON_SPACESIM_DEVICE                0x070044FE
#define DIBUTTON_SPACESIM_PAUSE                 0x070044FC

#define DIVIRTUAL_FIGHTING_HAND2HAND            0x08000000
#define DIAXIS_FIGHTINGH_LATERAL                0x08008201
#define DIAXIS_FIGHTINGH_MOVE                   0x08010202
#define DIBUTTON_FIGHTINGH_PUNCH                0x08000401
#define DIBUTTON_FIGHTINGH_KICK                 0x08000402
#define DIBUTTON_FIGHTINGH_BLOCK                0x08000403
#define DIBUTTON_FIGHTINGH_CROUCH               0x08000404
#define DIBUTTON_FIGHTINGH_JUMP                 0x08000405
#define DIBUTTON_FIGHTINGH_SPECIAL1             0x08000406
#define DIBUTTON_FIGHTINGH_SPECIAL2             0x08000407
#define DIBUTTON_FIGHTINGH_MENU                 0x080004FD

#define DIBUTTON_FIGHTINGH_SELECT               0x08004408
#define DIHATSWITCH_FIGHTINGH_SLIDE             0x08004601
#define DIBUTTON_FIGHTINGH_DISPLAY              0x08004409
#define DIAXIS_FIGHTINGH_ROTATE                 0x08024203
#define DIBUTTON_FIGHTINGH_DODGE                0x0800440A
#define DIBUTTON_FIGHTINGH_LEFT_LINK            0x0800C4E4
#define DIBUTTON_FIGHTINGH_RIGHT_LINK           0x0800C4EC
#define DIBUTTON_FIGHTINGH_FORWARD_LINK         0x080144E0
#define DIBUTTON_FIGHTINGH_BACKWARD_LINK        0x080144E8
#define DIBUTTON_FIGHTINGH_DEVICE               0x080044FE
#define DIBUTTON_FIGHTINGH_PAUSE                0x080044FC

#define DIVIRTUAL_FIGHTING_FPS                  0x09000000
#define DIAXIS_FPS_ROTATE                       0x09008201
#define DIAXIS_FPS_MOVE                         0x09010202
#define DIBUTTON_FPS_FIRE                       0x09000401
#define DIBUTTON_FPS_WEAPONS                    0x09000402
#define DIBUTTON_FPS_APPLY                      0x09000403
#define DIBUTTON_FPS_SELECT                     0x09000404
#define DIBUTTON_FPS_CROUCH                     0x09000405
#define DIBUTTON_FPS_JUMP                       0x09000406
#define DIAXIS_FPS_LOOKUPDOWN                   0x09018203
#define DIBUTTON_FPS_STRAFE                     0x09000407
#define DIBUTTON_FPS_MENU                       0x090004FD

#define DIHATSWITCH_FPS_GLANCE                  0x09004601
#define DIBUTTON_FPS_DISPLAY                    0x09004408
#define DIAXIS_FPS_SIDESTEP                     0x09024204
#define DIBUTTON_FPS_DODGE                      0x09004409
#define DIBUTTON_FPS_GLANCEL                    0x0900440A
#define DIBUTTON_FPS_GLANCER                    0x0900440B
#define DIBUTTON_FPS_FIRESECONDARY              0x0900440C
#define DIBUTTON_FPS_ROTATE_LEFT_LINK           0x0900C4E4
#define DIBUTTON_FPS_ROTATE_RIGHT_LINK          0x0900C4EC
#define DIBUTTON_FPS_FORWARD_LINK               0x090144E0
#define DIBUTTON_FPS_BACKWARD_LINK              0x090144E8
#define DIBUTTON_FPS_GLANCE_UP_LINK             0x0901C4E0
#define DIBUTTON_FPS_GLANCE_DOWN_LINK           0x0901C4E8
#define DIBUTTON_FPS_DEVICE                     0x090044FE
#define DIBUTTON_FPS_PAUSE                      0x090044FC

#define DIVIRTUAL_FIGHTING_THIRDPERSON          0x0A000000
#define DIAXIS_TPS_TURN                         0x0A020201
#define DIAXIS_TPS_MOVE                         0x0A010202
#define DIBUTTON_TPS_RUN                        0x0A000401
#define DIBUTTON_TPS_ACTION                     0x0A000402
#define DIBUTTON_TPS_SELECT                     0x0A000403
#define DIBUTTON_TPS_USE                        0x0A000404
#define DIBUTTON_TPS_JUMP                       0x0A000405
#define DIBUTTON_TPS_MENU                       0x0A0004FD

#define DIHATSWITCH_TPS_GLANCE                  0x0A004601
#define DIBUTTON_TPS_VIEW                       0x0A004406
#define DIBUTTON_TPS_STEPLEFT                   0x0A004407
#define DIBUTTON_TPS_STEPRIGHT                  0x0A004408
#define DIAXIS_TPS_STEP                         0x0A00C203
#define DIBUTTON_TPS_DODGE                      0x0A004409
#define DIBUTTON_TPS_INVENTORY                  0x0A00440A
#define DIBUTTON_TPS_TURN_LEFT_LINK             0x0A0244E4
#define DIBUTTON_TPS_TURN_RIGHT_LINK            0x0A0244EC
#define DIBUTTON_TPS_FORWARD_LINK               0x0A0144E0
#define DIBUTTON_TPS_BACKWARD_LINK              0x0A0144E8
#define DIBUTTON_TPS_GLANCE_UP_LINK             0x0A07C4E0
#define DIBUTTON_TPS_GLANCE_DOWN_LINK           0x0A07C4E8
#define DIBUTTON_TPS_GLANCE_LEFT_LINK           0x0A07C4E4
#define DIBUTTON_TPS_GLANCE_RIGHT_LINK          0x0A07C4EC
#define DIBUTTON_TPS_DEVICE                     0x0A0044FE
#define DIBUTTON_TPS_PAUSE                      0x0A0044FC

#define DIVIRTUAL_STRATEGY_ROLEPLAYING          0x0B000000
#define DIAXIS_STRATEGYR_LATERAL                0x0B008201
#define DIAXIS_STRATEGYR_MOVE                   0x0B010202
#define DIBUTTON_STRATEGYR_GET                  0x0B000401
#define DIBUTTON_STRATEGYR_APPLY                0x0B000402
#define DIBUTTON_STRATEGYR_SELECT               0x0B000403
#define DIBUTTON_STRATEGYR_ATTACK               0x0B000404
#define DIBUTTON_STRATEGYR_CAST                 0x0B000405
#define DIBUTTON_STRATEGYR_CROUCH               0x0B000406
#define DIBUTTON_STRATEGYR_JUMP                 0x0B000407
#define DIBUTTON_STRATEGYR_MENU                 0x0B0004FD

#define DIHATSWITCH_STRATEGYR_GLANCE            0x0B004601
#define DIBUTTON_STRATEGYR_MAP                  0x0B004408
#define DIBUTTON_STRATEGYR_DISPLAY              0x0B004409
#define DIAXIS_STRATEGYR_ROTATE                 0x0B024203
#define DIBUTTON_STRATEGYR_LEFT_LINK            0x0B00C4E4
#define DIBUTTON_STRATEGYR_RIGHT_LINK           0x0B00C4EC
#define DIBUTTON_STRATEGYR_FORWARD_LINK         0x0B0144E0
#define DIBUTTON_STRATEGYR_BACK_LINK            0x0B0144E8
#define DIBUTTON_STRATEGYR_ROTATE_LEFT_LINK     0x0B0244E4
#define DIBUTTON_STRATEGYR_ROTATE_RIGHT_LINK    0x0B0244EC
#define DIBUTTON_STRATEGYR_DEVICE               0x0B0044FE
#define DIBUTTON_STRATEGYR_PAUSE                0x0B0044FC

#define DIVIRTUAL_STRATEGY_TURN                 0x0C000000
#define DIAXIS_STRATEGYT_LATERAL                0x0C008201
#define DIAXIS_STRATEGYT_MOVE                   0x0C010202
#define DIBUTTON_STRATEGYT_SELECT               0x0C000401
#define DIBUTTON_STRATEGYT_INSTRUCT             0x0C000402
#define DIBUTTON_STRATEGYT_APPLY                0x0C000403
#define DIBUTTON_STRATEGYT_TEAM                 0x0C000404
#define DIBUTTON_STRATEGYT_TURN                 0x0C000405
#define DIBUTTON_STRATEGYT_MENU                 0x0C0004FD

#define DIBUTTON_STRATEGYT_ZOOM                 0x0C004406
#define DIBUTTON_STRATEGYT_MAP                  0x0C004407
#define DIBUTTON_STRATEGYT_DISPLAY              0x0C004408
#define DIBUTTON_STRATEGYT_LEFT_LINK            0x0C00C4E4
#define DIBUTTON_STRATEGYT_RIGHT_LINK           0x0C00C4EC
#define DIBUTTON_STRATEGYT_FORWARD_LINK         0x0C0144E0
#define DIBUTTON_STRATEGYT_BACK_LINK            0x0C0144E8
#define DIBUTTON_STRATEGYT_DEVICE               0x0C0044FE
#define DIBUTTON_STRATEGYT_PAUSE                0x0C0044FC

#define DIVIRTUAL_SPORTS_HUNTING                0x0D000000
#define DIAXIS_HUNTING_LATERAL                  0x0D008201
#define DIAXIS_HUNTING_MOVE                     0x0D010202
#define DIBUTTON_HUNTING_FIRE                   0x0D000401
#define DIBUTTON_HUNTING_AIM                    0x0D000402
#define DIBUTTON_HUNTING_WEAPON                 0x0D000403
#define DIBUTTON_HUNTING_BINOCULAR              0x0D000404
#define DIBUTTON_HUNTING_CALL                   0x0D000405
#define DIBUTTON_HUNTING_MAP                    0x0D000406
#define DIBUTTON_HUNTING_SPECIAL                0x0D000407
#define DIBUTTON_HUNTING_MENU                   0x0D0004FD

#define DIHATSWITCH_HUNTING_GLANCE              0x0D004601
#define DIBUTTON_HUNTING_DISPLAY                0x0D004408
#define DIAXIS_HUNTING_ROTATE                   0x0D024203
#define DIBUTTON_HUNTING_CROUCH                 0x0D004409
#define DIBUTTON_HUNTING_JUMP                   0x0D00440A
#define DIBUTTON_HUNTING_FIRESECONDARY          0x0D00440B
#define DIBUTTON_HUNTING_LEFT_LINK              0x0D00C4E4
#define DIBUTTON_HUNTING_RIGHT_LINK             0x0D00C4EC
#define DIBUTTON_HUNTING_FORWARD_LINK           0x0D0144E0
#define DIBUTTON_HUNTING_BACK_LINK              0x0D0144E8
#define DIBUTTON_HUNTING_ROTATE_LEFT_LINK       0x0D0244E4
#define DIBUTTON_HUNTING_ROTATE_RIGHT_LINK      0x0D0244EC
#define DIBUTTON_HUNTING_DEVICE                 0x0D0044FE
#define DIBUTTON_HUNTING_PAUSE                  0x0D0044FC

#define DIVIRTUAL_SPORTS_FISHING                0x0E000000
#define DIAXIS_FISHING_LATERAL                  0x0E008201
#define DIAXIS_FISHING_MOVE                     0x0E010202
#define DIBUTTON_FISHING_CAST                   0x0E000401
#define DIBUTTON_FISHING_TYPE                   0x0E000402
#define DIBUTTON_FISHING_BINOCULAR              0x0E000403
#define DIBUTTON_FISHING_BAIT                   0x0E000404
#define DIBUTTON_FISHING_MAP                    0x0E000405
#define DIBUTTON_FISHING_MENU                   0x0E0004FD

#define DIHATSWITCH_FISHING_GLANCE              0x0E004601
#define DIBUTTON_FISHING_DISPLAY                0x0E004406
#define DIAXIS_FISHING_ROTATE                   0x0E024203
#define DIBUTTON_FISHING_CROUCH                 0x0E004407
#define DIBUTTON_FISHING_JUMP                   0x0E004408
#define DIBUTTON_FISHING_LEFT_LINK              0x0E00C4E4
#define DIBUTTON_FISHING_RIGHT_LINK             0x0E00C4EC
#define DIBUTTON_FISHING_FORWARD_LINK           0x0E0144E0
#define DIBUTTON_FISHING_BACK_LINK              0x0E0144E8
#define DIBUTTON_FISHING_ROTATE_LEFT_LINK       0x0E0244E4
#define DIBUTTON_FISHING_ROTATE_RIGHT_LINK      0x0E0244EC
#define DIBUTTON_FISHING_DEVICE                 0x0E0044FE
#define DIBUTTON_FISHING_PAUSE                  0x0E0044FC

#define DIVIRTUAL_SPORTS_BASEBALL_BAT           0x0F000000
#define DIAXIS_BASEBALLB_LATERAL                0x0F008201
#define DIAXIS_BASEBALLB_MOVE                   0x0F010202
#define DIBUTTON_BASEBALLB_SELECT               0x0F000401
#define DIBUTTON_BASEBALLB_NORMAL               0x0F000402
#define DIBUTTON_BASEBALLB_POWER                0x0F000403
#define DIBUTTON_BASEBALLB_BUNT                 0x0F000404
#define DIBUTTON_BASEBALLB_STEAL                0x0F000405
#define DIBUTTON_BASEBALLB_BURST                0x0F000406
#define DIBUTTON_BASEBALLB_SLIDE                0x0F000407
#define DIBUTTON_BASEBALLB_CONTACT              0x0F000408
#define DIBUTTON_BASEBALLB_MENU                 0x0F0004FD

#define DIBUTTON_BASEBALLB_NOSTEAL              0x0F004409
#define DIBUTTON_BASEBALLB_BOX                  0x0F00440A
#define DIBUTTON_BASEBALLB_LEFT_LINK            0x0F00C4E4
#define DIBUTTON_BASEBALLB_RIGHT_LINK           0x0F00C4EC
#define DIBUTTON_BASEBALLB_FORWARD_LINK         0x0F0144E0
#define DIBUTTON_BASEBALLB_BACK_LINK            0x0F0144E8
#define DIBUTTON_BASEBALLB_DEVICE               0x0F0044FE
#define DIBUTTON_BASEBALLB_PAUSE                0x0F0044FC

#define DIVIRTUAL_SPORTS_BASEBALL_PITCH         0x10000000
#define DIAXIS_BASEBALLP_LATERAL                0x10008201
#define DIAXIS_BASEBALLP_MOVE                   0x10010202
#define DIBUTTON_BASEBALLP_SELECT               0x10000401
#define DIBUTTON_BASEBALLP_PITCH                0x10000402
#define DIBUTTON_BASEBALLP_BASE                 0x10000403
#define DIBUTTON_BASEBALLP_THROW                0x10000404
#define DIBUTTON_BASEBALLP_FAKE                 0x10000405
#define DIBUTTON_BASEBALLP_MENU                 0x100004FD

#define DIBUTTON_BASEBALLP_WALK                 0x10004406
#define DIBUTTON_BASEBALLP_LOOK                 0x10004407
#define DIBUTTON_BASEBALLP_LEFT_LINK            0x1000C4E4
#define DIBUTTON_BASEBALLP_RIGHT_LINK           0x1000C4EC
#define DIBUTTON_BASEBALLP_FORWARD_LINK         0x100144E0
#define DIBUTTON_BASEBALLP_BACK_LINK            0x100144E8
#define DIBUTTON_BASEBALLP_DEVICE               0x100044FE
#define DIBUTTON_BASEBALLP_PAUSE                0x100044FC

#define DIVIRTUAL_SPORTS_BASEBALL_FIELD         0x11000000
#define DIAXIS_BASEBALLF_LATERAL                0x11008201
#define DIAXIS_BASEBALLF_MOVE                   0x11010202
#define DIBUTTON_BASEBALLF_NEAREST              0x11000401
#define DIBUTTON_BASEBALLF_THROW1               0x11000402
#define DIBUTTON_BASEBALLF_THROW2               0x11000403
#define DIBUTTON_BASEBALLF_BURST                0x11000404
#define DIBUTTON_BASEBALLF_JUMP                 0x11000405
#define DIBUTTON_BASEBALLF_DIVE                 0x11000406
#define DIBUTTON_BASEBALLF_MENU                 0x110004FD

#define DIBUTTON_BASEBALLF_SHIFTIN              0x11004407
#define DIBUTTON_BASEBALLF_SHIFTOUT             0x11004408
#define DIBUTTON_BASEBALLF_AIM_LEFT_LINK        0x1100C4E4
#define DIBUTTON_BASEBALLF_AIM_RIGHT_LINK       0x1100C4EC
#define DIBUTTON_BASEBALLF_FORWARD_LINK         0x110144E0
#define DIBUTTON_BASEBALLF_BACK_LINK            0x110144E8
#define DIBUTTON_BASEBALLF_DEVICE               0x110044FE
#define DIBUTTON_BASEBALLF_PAUSE                0x110044FC

#define DIVIRTUAL_SPORTS_BASKETBALL_OFFENSE     0x12000000
#define DIAXIS_BBALLO_LATERAL                   0x12008201
#define DIAXIS_BBALLO_MOVE                      0x12010202
#define DIBUTTON_BBALLO_SHOOT                   0x12000401
#define DIBUTTON_BBALLO_DUNK                    0x12000402
#define DIBUTTON_BBALLO_PASS                    0x12000403
#define DIBUTTON_BBALLO_FAKE                    0x12000404
#define DIBUTTON_BBALLO_SPECIAL                 0x12000405
#define DIBUTTON_BBALLO_PLAYER                  0x12000406
#define DIBUTTON_BBALLO_BURST                   0x12000407
#define DIBUTTON_BBALLO_CALL                    0x12000408
#define DIBUTTON_BBALLO_MENU                    0x120004FD

#define DIHATSWITCH_BBALLO_GLANCE               0x12004601
#define DIBUTTON_BBALLO_SCREEN                  0x12004409
#define DIBUTTON_BBALLO_PLAY                    0x1200440A
#define DIBUTTON_BBALLO_JAB                     0x1200440B
#define DIBUTTON_BBALLO_POST                    0x1200440C
#define DIBUTTON_BBALLO_TIMEOUT                 0x1200440D
#define DIBUTTON_BBALLO_SUBSTITUTE              0x1200440E
#define DIBUTTON_BBALLO_LEFT_LINK               0x1200C4E4
#define DIBUTTON_BBALLO_RIGHT_LINK              0x1200C4EC
#define DIBUTTON_BBALLO_FORWARD_LINK            0x120144E0
#define DIBUTTON_BBALLO_BACK_LINK               0x120144E8
#define DIBUTTON_BBALLO_DEVICE                  0x120044FE
#define DIBUTTON_BBALLO_PAUSE                   0x120044FC

#define DIVIRTUAL_SPORTS_BASKETBALL_DEFENSE     0x13000000
#define DIAXIS_BBALLD_LATERAL                   0x13008201
#define DIAXIS_BBALLD_MOVE                      0x13010202
#define DIBUTTON_BBALLD_JUMP                    0x13000401
#define DIBUTTON_BBALLD_STEAL                   0x13000402
#define DIBUTTON_BBALLD_FAKE                    0x13000403
#define DIBUTTON_BBALLD_SPECIAL                 0x13000404
#define DIBUTTON_BBALLD_PLAYER                  0x13000405
#define DIBUTTON_BBALLD_BURST                   0x13000406
#define DIBUTTON_BBALLD_PLAY                    0x13000407
#define DIBUTTON_BBALLD_MENU                    0x130004FD

#define DIHATSWITCH_BBALLD_GLANCE               0x13004601
#define DIBUTTON_BBALLD_TIMEOUT                 0x13004408
#define DIBUTTON_BBALLD_SUBSTITUTE              0x13004409
#define DIBUTTON_BBALLD_LEFT_LINK               0x1300C4E4
#define DIBUTTON_BBALLD_RIGHT_LINK              0x1300C4EC
#define DIBUTTON_BBALLD_FORWARD_LINK            0x130144E0
#define DIBUTTON_BBALLD_BACK_LINK               0x130144E8
#define DIBUTTON_BBALLD_DEVICE                  0x130044FE
#define DIBUTTON_BBALLD_PAUSE                   0x130044FC

#define DIVIRTUAL_SPORTS_FOOTBALL_FIELD         0x14000000
#define DIBUTTON_FOOTBALLP_PLAY                 0x14000401
#define DIBUTTON_FOOTBALLP_SELECT               0x14000402
#define DIBUTTON_FOOTBALLP_HELP                 0x14000403
#define DIBUTTON_FOOTBALLP_MENU                 0x140004FD

#define DIBUTTON_FOOTBALLP_DEVICE               0x140044FE
#define DIBUTTON_FOOTBALLP_PAUSE                0x140044FC

#define DIVIRTUAL_SPORTS_FOOTBALL_QBCK          0x15000000
#define DIAXIS_FOOTBALLQ_LATERAL                0x15008201
#define DIAXIS_FOOTBALLQ_MOVE                   0x15010202
#define DIBUTTON_FOOTBALLQ_SELECT               0x15000401
#define DIBUTTON_FOOTBALLQ_SNAP                 0x15000402
#define DIBUTTON_FOOTBALLQ_JUMP                 0x15000403
#define DIBUTTON_FOOTBALLQ_SLIDE                0x15000404
#define DIBUTTON_FOOTBALLQ_PASS                 0x15000405
#define DIBUTTON_FOOTBALLQ_FAKE                 0x15000406
#define DIBUTTON_FOOTBALLQ_MENU                 0x150004FD

#define DIBUTTON_FOOTBALLQ_FAKESNAP             0x15004407
#define DIBUTTON_FOOTBALLQ_MOTION               0x15004408
#define DIBUTTON_FOOTBALLQ_AUDIBLE              0x15004409
#define DIBUTTON_FOOTBALLQ_LEFT_LINK            0x1500C4E4
#define DIBUTTON_FOOTBALLQ_RIGHT_LINK           0x1500C4EC
#define DIBUTTON_FOOTBALLQ_FORWARD_LINK         0x150144E0
#define DIBUTTON_FOOTBALLQ_BACK_LINK            0x150144E8
#define DIBUTTON_FOOTBALLQ_DEVICE               0x150044FE
#define DIBUTTON_FOOTBALLQ_PAUSE                0x150044FC

#define DIVIRTUAL_SPORTS_FOOTBALL_OFFENSE       0x16000000
#define DIAXIS_FOOTBALLO_LATERAL                0x16008201
#define DIAXIS_FOOTBALLO_MOVE                   0x16010202
#define DIBUTTON_FOOTBALLO_JUMP                 0x16000401
#define DIBUTTON_FOOTBALLO_LEFTARM              0x16000402
#define DIBUTTON_FOOTBALLO_RIGHTARM             0x16000403
#define DIBUTTON_FOOTBALLO_THROW                0x16000404
#define DIBUTTON_FOOTBALLO_SPIN                 0x16000405
#define DIBUTTON_FOOTBALLO_MENU                 0x160004FD

#define DIBUTTON_FOOTBALLO_JUKE                 0x16004406
#define DIBUTTON_FOOTBALLO_SHOULDER             0x16004407
#define DIBUTTON_FOOTBALLO_TURBO                0x16004408
#define DIBUTTON_FOOTBALLO_DIVE                 0x16004409
#define DIBUTTON_FOOTBALLO_ZOOM                 0x1600440A
#define DIBUTTON_FOOTBALLO_SUBSTITUTE           0x1600440B
#define DIBUTTON_FOOTBALLO_LEFT_LINK            0x1600C4E4
#define DIBUTTON_FOOTBALLO_RIGHT_LINK           0x1600C4EC
#define DIBUTTON_FOOTBALLO_FORWARD_LINK         0x160144E0
#define DIBUTTON_FOOTBALLO_BACK_LINK            0x160144E8
#define DIBUTTON_FOOTBALLO_DEVICE               0x160044FE
#define DIBUTTON_FOOTBALLO_PAUSE                0x160044FC

#define DIVIRTUAL_SPORTS_FOOTBALL_DEFENSE       0x17000000
#define DIAXIS_FOOTBALLD_LATERAL                0x17008201
#define DIAXIS_FOOTBALLD_MOVE                   0x17010202
#define DIBUTTON_FOOTBALLD_PLAY                 0x17000401
#define DIBUTTON_FOOTBALLD_SELECT               0x17000402
#define DIBUTTON_FOOTBALLD_JUMP                 0x17000403
#define DIBUTTON_FOOTBALLD_TACKLE               0x17000404
#define DIBUTTON_FOOTBALLD_FAKE                 0x17000405
#define DIBUTTON_FOOTBALLD_SUPERTACKLE          0x17000406
#define DIBUTTON_FOOTBALLD_MENU                 0x170004FD

#define DIBUTTON_FOOTBALLD_SPIN                 0x17004407
#define DIBUTTON_FOOTBALLD_SWIM                 0x17004408
#define DIBUTTON_FOOTBALLD_BULLRUSH             0x17004409
#define DIBUTTON_FOOTBALLD_RIP                  0x1700440A
#define DIBUTTON_FOOTBALLD_AUDIBLE              0x1700440B
#define DIBUTTON_FOOTBALLD_ZOOM                 0x1700440C
#define DIBUTTON_FOOTBALLD_SUBSTITUTE           0x1700440D
#define DIBUTTON_FOOTBALLD_LEFT_LINK            0x1700C4E4
#define DIBUTTON_FOOTBALLD_RIGHT_LINK           0x1700C4EC
#define DIBUTTON_FOOTBALLD_FORWARD_LINK         0x170144E0
#define DIBUTTON_FOOTBALLD_BACK_LINK            0x170144E8
#define DIBUTTON_FOOTBALLD_DEVICE               0x170044FE
#define DIBUTTON_FOOTBALLD_PAUSE                0x170044FC

#define DIVIRTUAL_SPORTS_GOLF                   0x18000000
#define DIAXIS_GOLF_LATERAL                     0x18008201
#define DIAXIS_GOLF_MOVE                        0x18010202
#define DIBUTTON_GOLF_SWING                     0x18000401
#define DIBUTTON_GOLF_SELECT                    0x18000402
#define DIBUTTON_GOLF_UP                        0x18000403
#define DIBUTTON_GOLF_DOWN                      0x18000404
#define DIBUTTON_GOLF_TERRAIN                   0x18000405
#define DIBUTTON_GOLF_FLYBY                     0x18000406
#define DIBUTTON_GOLF_MENU                      0x180004FD

#define DIHATSWITCH_GOLF_SCROLL                 0x18004601
#define DIBUTTON_GOLF_ZOOM                      0x18004407
#define DIBUTTON_GOLF_TIMEOUT                   0x18004408
#define DIBUTTON_GOLF_SUBSTITUTE                0x18004409
#define DIBUTTON_GOLF_LEFT_LINK                 0x1800C4E4
#define DIBUTTON_GOLF_RIGHT_LINK                0x1800C4EC
#define DIBUTTON_GOLF_FORWARD_LINK              0x180144E0
#define DIBUTTON_GOLF_BACK_LINK                 0x180144E8
#define DIBUTTON_GOLF_DEVICE                    0x180044FE
#define DIBUTTON_GOLF_PAUSE                     0x180044FC

#define DIVIRTUAL_SPORTS_HOCKEY_OFFENSE         0x19000000
#define DIAXIS_HOCKEYO_LATERAL                  0x19008201
#define DIAXIS_HOCKEYO_MOVE                     0x19010202
#define DIBUTTON_HOCKEYO_SHOOT                  0x19000401
#define DIBUTTON_HOCKEYO_PASS                   0x19000402
#define DIBUTTON_HOCKEYO_BURST                  0x19000403
#define DIBUTTON_HOCKEYO_SPECIAL                0x19000404
#define DIBUTTON_HOCKEYO_FAKE                   0x19000405
#define DIBUTTON_HOCKEYO_MENU                   0x190004FD

#define DIHATSWITCH_HOCKEYO_SCROLL              0x19004601
#define DIBUTTON_HOCKEYO_ZOOM                   0x19004406
#define DIBUTTON_HOCKEYO_STRATEGY               0x19004407
#define DIBUTTON_HOCKEYO_TIMEOUT                0x19004408
#define DIBUTTON_HOCKEYO_SUBSTITUTE             0x19004409
#define DIBUTTON_HOCKEYO_LEFT_LINK              0x1900C4E4
#define DIBUTTON_HOCKEYO_RIGHT_LINK             0x1900C4EC
#define DIBUTTON_HOCKEYO_FORWARD_LINK           0x190144E0
#define DIBUTTON_HOCKEYO_BACK_LINK              0x190144E8
#define DIBUTTON_HOCKEYO_DEVICE                 0x190044FE
#define DIBUTTON_HOCKEYO_PAUSE                  0x190044FC

#define DIVIRTUAL_SPORTS_HOCKEY_DEFENSE         0x1A000000
#define DIAXIS_HOCKEYD_LATERAL                  0x1A008201
#define DIAXIS_HOCKEYD_MOVE                     0x1A010202
#define DIBUTTON_HOCKEYD_PLAYER                 0x1A000401
#define DIBUTTON_HOCKEYD_STEAL                  0x1A000402
#define DIBUTTON_HOCKEYD_BURST                  0x1A000403
#define DIBUTTON_HOCKEYD_BLOCK                  0x1A000404
#define DIBUTTON_HOCKEYD_FAKE                   0x1A000405
#define DIBUTTON_HOCKEYD_MENU                   0x1A0004FD

#define DIHATSWITCH_HOCKEYD_SCROLL              0x1A004601
#define DIBUTTON_HOCKEYD_ZOOM                   0x1A004406
#define DIBUTTON_HOCKEYD_STRATEGY               0x1A004407
#define DIBUTTON_HOCKEYD_TIMEOUT                0x1A004408
#define DIBUTTON_HOCKEYD_SUBSTITUTE             0x1A004409
#define DIBUTTON_HOCKEYD_LEFT_LINK              0x1A00C4E4
#define DIBUTTON_HOCKEYD_RIGHT_LINK             0x1A00C4EC
#define DIBUTTON_HOCKEYD_FORWARD_LINK           0x1A0144E0
#define DIBUTTON_HOCKEYD_BACK_LINK              0x1A0144E8
#define DIBUTTON_HOCKEYD_DEVICE                 0x1A0044FE
#define DIBUTTON_HOCKEYD_PAUSE                  0x1A0044FC

#define DIVIRTUAL_SPORTS_HOCKEY_GOALIE          0x1B000000
#define DIAXIS_HOCKEYG_LATERAL                  0x1B008201
#define DIAXIS_HOCKEYG_MOVE                     0x1B010202
#define DIBUTTON_HOCKEYG_PASS                   0x1B000401
#define DIBUTTON_HOCKEYG_POKE                   0x1B000402
#define DIBUTTON_HOCKEYG_STEAL                  0x1B000403
#define DIBUTTON_HOCKEYG_BLOCK                  0x1B000404
#define DIBUTTON_HOCKEYG_MENU                   0x1B0004FD

#define DIHATSWITCH_HOCKEYG_SCROLL              0x1B004601
#define DIBUTTON_HOCKEYG_ZOOM                   0x1B004405
#define DIBUTTON_HOCKEYG_STRATEGY               0x1B004406
#define DIBUTTON_HOCKEYG_TIMEOUT                0x1B004407
#define DIBUTTON_HOCKEYG_SUBSTITUTE             0x1B004408
#define DIBUTTON_HOCKEYG_LEFT_LINK              0x1B00C4E4
#define DIBUTTON_HOCKEYG_RIGHT_LINK             0x1B00C4EC
#define DIBUTTON_HOCKEYG_FORWARD_LINK           0x1B0144E0
#define DIBUTTON_HOCKEYG_BACK_LINK              0x1B0144E8
#define DIBUTTON_HOCKEYG_DEVICE                 0x1B0044FE
#define DIBUTTON_HOCKEYG_PAUSE                  0x1B0044FC

#define DIVIRTUAL_SPORTS_BIKING_MOUNTAIN        0x1C000000
#define DIAXIS_BIKINGM_TURN                     0x1C008201
#define DIAXIS_BIKINGM_PEDAL                    0x1C010202
#define DIBUTTON_BIKINGM_JUMP                   0x1C000401
#define DIBUTTON_BIKINGM_CAMERA                 0x1C000402
#define DIBUTTON_BIKINGM_SPECIAL1               0x1C000403
#define DIBUTTON_BIKINGM_SELECT                 0x1C000404
#define DIBUTTON_BIKINGM_SPECIAL2               0x1C000405
#define DIBUTTON_BIKINGM_MENU                   0x1C0004FD

#define DIHATSWITCH_BIKINGM_SCROLL              0x1C004601
#define DIBUTTON_BIKINGM_ZOOM                   0x1C004406
#define DIAXIS_BIKINGM_BRAKE                    0x1C044203
#define DIBUTTON_BIKINGM_LEFT_LINK              0x1C00C4E4
#define DIBUTTON_BIKINGM_RIGHT_LINK             0x1C00C4EC
#define DIBUTTON_BIKINGM_FASTER_LINK            0x1C0144E0
#define DIBUTTON_BIKINGM_SLOWER_LINK            0x1C0144E8
#define DIBUTTON_BIKINGM_BRAKE_BUTTON_LINK      0x1C0444E8
#define DIBUTTON_BIKINGM_DEVICE                 0x1C0044FE
#define DIBUTTON_BIKINGM_PAUSE                  0x1C0044FC

#define DIVIRTUAL_SPORTS_SKIING                 0x1D000000
#define DIAXIS_SKIING_TURN                      0x1D008201
#define DIAXIS_SKIING_SPEED                     0x1D010202
#define DIBUTTON_SKIING_JUMP                    0x1D000401
#define DIBUTTON_SKIING_CROUCH                  0x1D000402
#define DIBUTTON_SKIING_CAMERA                  0x1D000403
#define DIBUTTON_SKIING_SPECIAL1                0x1D000404
#define DIBUTTON_SKIING_SELECT                  0x1D000405
#define DIBUTTON_SKIING_SPECIAL2                0x1D000406
#define DIBUTTON_SKIING_MENU                    0x1D0004FD

#define DIHATSWITCH_SKIING_GLANCE               0x1D004601
#define DIBUTTON_SKIING_ZOOM                    0x1D004407
#define DIBUTTON_SKIING_LEFT_LINK               0x1D00C4E4
#define DIBUTTON_SKIING_RIGHT_LINK              0x1D00C4EC
#define DIBUTTON_SKIING_FASTER_LINK             0x1D0144E0
#define DIBUTTON_SKIING_SLOWER_LINK             0x1D0144E8
#define DIBUTTON_SKIING_DEVICE                  0x1D0044FE
#define DIBUTTON_SKIING_PAUSE                   0x1D0044FC

#define DIVIRTUAL_SPORTS_SOCCER_OFFENSE         0x1E000000
#define DIAXIS_SOCCERO_LATERAL                  0x1E008201
#define DIAXIS_SOCCERO_MOVE                     0x1E010202
#define DIAXIS_SOCCERO_BEND                     0x1E018203
#define DIBUTTON_SOCCERO_SHOOT                  0x1E000401
#define DIBUTTON_SOCCERO_PASS                   0x1E000402
#define DIBUTTON_SOCCERO_FAKE                   0x1E000403
#define DIBUTTON_SOCCERO_PLAYER                 0x1E000404
#define DIBUTTON_SOCCERO_SPECIAL1               0x1E000405
#define DIBUTTON_SOCCERO_SELECT                 0x1E000406
#define DIBUTTON_SOCCERO_MENU                   0x1E0004FD

#define DIHATSWITCH_SOCCERO_GLANCE              0x1E004601
#define DIBUTTON_SOCCERO_SUBSTITUTE             0x1E004407
#define DIBUTTON_SOCCERO_SHOOTLOW               0x1E004408
#define DIBUTTON_SOCCERO_SHOOTHIGH              0x1E004409
#define DIBUTTON_SOCCERO_PASSTHRU               0x1E00440A
#define DIBUTTON_SOCCERO_SPRINT                 0x1E00440B
#define DIBUTTON_SOCCERO_CONTROL                0x1E00440C
#define DIBUTTON_SOCCERO_HEAD                   0x1E00440D
#define DIBUTTON_SOCCERO_LEFT_LINK              0x1E00C4E4
#define DIBUTTON_SOCCERO_RIGHT_LINK             0x1E00C4EC
#define DIBUTTON_SOCCERO_FORWARD_LINK           0x1E0144E0
#define DIBUTTON_SOCCERO_BACK_LINK              0x1E0144E8
#define DIBUTTON_SOCCERO_DEVICE                 0x1E0044FE
#define DIBUTTON_SOCCERO_PAUSE                  0x1E0044FC

#define DIVIRTUAL_SPORTS_SOCCER_DEFENSE         0x1F000000
#define DIAXIS_SOCCERD_LATERAL                  0x1F008201
#define DIAXIS_SOCCERD_MOVE                     0x1F010202
#define DIBUTTON_SOCCERD_BLOCK                  0x1F000401
#define DIBUTTON_SOCCERD_STEAL                  0x1F000402
#define DIBUTTON_SOCCERD_FAKE                   0x1F000403
#define DIBUTTON_SOCCERD_PLAYER                 0x1F000404
#define DIBUTTON_SOCCERD_SPECIAL                0x1F000405
#define DIBUTTON_SOCCERD_SELECT                 0x1F000406
#define DIBUTTON_SOCCERD_SLIDE                  0x1F000407
#define DIBUTTON_SOCCERD_MENU                   0x1F0004FD

#define DIHATSWITCH_SOCCERD_GLANCE              0x1F004601
#define DIBUTTON_SOCCERD_FOUL                   0x1F004408
#define DIBUTTON_SOCCERD_HEAD                   0x1F004409
#define DIBUTTON_SOCCERD_CLEAR                  0x1F00440A
#define DIBUTTON_SOCCERD_GOALIECHARGE           0x1F00440B
#define DIBUTTON_SOCCERD_SUBSTITUTE             0x1F00440C
#define DIBUTTON_SOCCERD_LEFT_LINK              0x1F00C4E4
#define DIBUTTON_SOCCERD_RIGHT_LINK             0x1F00C4EC
#define DIBUTTON_SOCCERD_FORWARD_LINK           0x1F0144E0
#define DIBUTTON_SOCCERD_BACK_LINK              0x1F0144E8
#define DIBUTTON_SOCCERD_DEVICE                 0x1F0044FE
#define DIBUTTON_SOCCERD_PAUSE                  0x1F0044FC

#define DIVIRTUAL_SPORTS_RACQUET                0x20000000
#define DIAXIS_RACQUET_LATERAL                  0x20008201
#define DIAXIS_RACQUET_MOVE                     0x20010202
#define DIBUTTON_RACQUET_SWING                  0x20000401
#define DIBUTTON_RACQUET_BACKSWING              0x20000402
#define DIBUTTON_RACQUET_SMASH                  0x20000403
#define DIBUTTON_RACQUET_SPECIAL                0x20000404
#define DIBUTTON_RACQUET_SELECT                 0x20000405
#define DIBUTTON_RACQUET_MENU                   0x200004FD

#define DIHATSWITCH_RACQUET_GLANCE              0x20004601
#define DIBUTTON_RACQUET_TIMEOUT                0x20004406
#define DIBUTTON_RACQUET_SUBSTITUTE             0x20004407
#define DIBUTTON_RACQUET_LEFT_LINK              0x2000C4E4
#define DIBUTTON_RACQUET_RIGHT_LINK             0x2000C4EC
#define DIBUTTON_RACQUET_FORWARD_LINK           0x200144E0
#define DIBUTTON_RACQUET_BACK_LINK              0x200144E8
#define DIBUTTON_RACQUET_DEVICE                 0x200044FE
#define DIBUTTON_RACQUET_PAUSE                  0x200044FC

#define DIVIRTUAL_ARCADE_SIDE2SIDE              0x21000000
#define DIAXIS_ARCADES_LATERAL                  0x21008201
#define DIAXIS_ARCADES_MOVE                     0x21010202
#define DIBUTTON_ARCADES_THROW                  0x21000401
#define DIBUTTON_ARCADES_CARRY                  0x21000402
#define DIBUTTON_ARCADES_ATTACK                 0x21000403
#define DIBUTTON_ARCADES_SPECIAL                0x21000404
#define DIBUTTON_ARCADES_SELECT                 0x21000405
#define DIBUTTON_ARCADES_MENU                   0x210004FD

#define DIHATSWITCH_ARCADES_VIEW                0x21004601
#define DIBUTTON_ARCADES_LEFT_LINK              0x2100C4E4
#define DIBUTTON_ARCADES_RIGHT_LINK             0x2100C4EC
#define DIBUTTON_ARCADES_FORWARD_LINK           0x210144E0
#define DIBUTTON_ARCADES_BACK_LINK              0x210144E8
#define DIBUTTON_ARCADES_VIEW_UP_LINK           0x2107C4E0
#define DIBUTTON_ARCADES_VIEW_DOWN_LINK         0x2107C4E8
#define DIBUTTON_ARCADES_VIEW_LEFT_LINK         0x2107C4E4
#define DIBUTTON_ARCADES_VIEW_RIGHT_LINK        0x2107C4EC
#define DIBUTTON_ARCADES_DEVICE                 0x210044FE
#define DIBUTTON_ARCADES_PAUSE                  0x210044FC

#define DIVIRTUAL_ARCADE_PLATFORM               0x22000000
#define DIAXIS_ARCADEP_LATERAL                  0x22008201
#define DIAXIS_ARCADEP_MOVE                     0x22010202
#define DIBUTTON_ARCADEP_JUMP                   0x22000401
#define DIBUTTON_ARCADEP_FIRE                   0x22000402
#define DIBUTTON_ARCADEP_CROUCH                 0x22000403
#define DIBUTTON_ARCADEP_SPECIAL                0x22000404
#define DIBUTTON_ARCADEP_SELECT                 0x22000405
#define DIBUTTON_ARCADEP_MENU                   0x220004FD

#define DIHATSWITCH_ARCADEP_VIEW                0x22004601
#define DIBUTTON_ARCADEP_FIRESECONDARY          0x22004406
#define DIBUTTON_ARCADEP_LEFT_LINK              0x2200C4E4
#define DIBUTTON_ARCADEP_RIGHT_LINK             0x2200C4EC
#define DIBUTTON_ARCADEP_FORWARD_LINK           0x220144E0
#define DIBUTTON_ARCADEP_BACK_LINK              0x220144E8
#define DIBUTTON_ARCADEP_VIEW_UP_LINK           0x2207C4E0
#define DIBUTTON_ARCADEP_VIEW_DOWN_LINK         0x2207C4E8
#define DIBUTTON_ARCADEP_VIEW_LEFT_LINK         0x2207C4E4
#define DIBUTTON_ARCADEP_VIEW_RIGHT_LINK        0x2207C4EC
#define DIBUTTON_ARCADEP_DEVICE                 0x220044FE
#define DIBUTTON_ARCADEP_PAUSE                  0x220044FC

#define DIVIRTUAL_CAD_2DCONTROL                 0x23000000
#define DIAXIS_2DCONTROL_LATERAL                0x23008201
#define DIAXIS_2DCONTROL_MOVE                   0x23010202
#define DIAXIS_2DCONTROL_INOUT                  0x23018203
#define DIBUTTON_2DCONTROL_SELECT               0x23000401
#define DIBUTTON_2DCONTROL_SPECIAL1             0x23000402
#define DIBUTTON_2DCONTROL_SPECIAL              0x23000403
#define DIBUTTON_2DCONTROL_SPECIAL2             0x23000404
#define DIBUTTON_2DCONTROL_MENU                 0x230004FD

#define DIHATSWITCH_2DCONTROL_HATSWITCH         0x23004601
#define DIAXIS_2DCONTROL_ROTATEZ                0x23024204
#define DIBUTTON_2DCONTROL_DISPLAY              0x23004405
#define DIBUTTON_2DCONTROL_DEVICE               0x230044FE
#define DIBUTTON_2DCONTROL_PAUSE                0x230044FC

#define DIVIRTUAL_CAD_3DCONTROL                 0x24000000
#define DIAXIS_3DCONTROL_LATERAL                0x24008201
#define DIAXIS_3DCONTROL_MOVE                   0x24010202
#define DIAXIS_3DCONTROL_INOUT                  0x24018203
#define DIBUTTON_3DCONTROL_SELECT               0x24000401
#define DIBUTTON_3DCONTROL_SPECIAL1             0x24000402
#define DIBUTTON_3DCONTROL_SPECIAL              0x24000403
#define DIBUTTON_3DCONTROL_SPECIAL2             0x24000404
#define DIBUTTON_3DCONTROL_MENU                 0x240004FD

#define DIHATSWITCH_3DCONTROL_HATSWITCH         0x24004601
#define DIAXIS_3DCONTROL_ROTATEX                0x24034204
#define DIAXIS_3DCONTROL_ROTATEY                0x2402C205
#define DIAXIS_3DCONTROL_ROTATEZ                0x24024206
#define DIBUTTON_3DCONTROL_DISPLAY              0x24004405
#define DIBUTTON_3DCONTROL_DEVICE               0x240044FE
#define DIBUTTON_3DCONTROL_PAUSE                0x240044FC

#define DIVIRTUAL_CAD_FLYBY                     0x25000000
#define DIAXIS_CADF_LATERAL                     0x25008201
#define DIAXIS_CADF_MOVE                        0x25010202
#define DIAXIS_CADF_INOUT                       0x25018203
#define DIBUTTON_CADF_SELECT                    0x25000401
#define DIBUTTON_CADF_SPECIAL1                  0x25000402
#define DIBUTTON_CADF_SPECIAL                   0x25000403
#define DIBUTTON_CADF_SPECIAL2                  0x25000404
#define DIBUTTON_CADF_MENU                      0x250004FD

#define DIHATSWITCH_CADF_HATSWITCH              0x25004601
#define DIAXIS_CADF_ROTATEX                     0x25034204
#define DIAXIS_CADF_ROTATEY                     0x2502C205
#define DIAXIS_CADF_ROTATEZ                     0x25024206
#define DIBUTTON_CADF_DISPLAY                   0x25004405
#define DIBUTTON_CADF_DEVICE                    0x250044FE
#define DIBUTTON_CADF_PAUSE                     0x250044FC

#define DIVIRTUAL_CAD_MODEL                     0x26000000
#define DIAXIS_CADM_LATERAL                     0x26008201
#define DIAXIS_CADM_MOVE                        0x26010202
#define DIAXIS_CADM_INOUT                       0x26018203
#define DIBUTTON_CADM_SELECT                    0x26000401
#define DIBUTTON_CADM_SPECIAL1                  0x26000402
#define DIBUTTON_CADM_SPECIAL                   0x26000403
#define DIBUTTON_CADM_SPECIAL2                  0x26000404
#define DIBUTTON_CADM_MENU                      0x260004FD

#define DIHATSWITCH_CADM_HATSWITCH              0x26004601
#define DIAXIS_CADM_ROTATEX                     0x26034204
#define DIAXIS_CADM_ROTATEY                     0x2602C205
#define DIAXIS_CADM_ROTATEZ                     0x26024206
#define DIBUTTON_CADM_DISPLAY                   0x26004405
#define DIBUTTON_CADM_DEVICE                    0x260044FE
#define DIBUTTON_CADM_PAUSE                     0x260044FC

#define DIVIRTUAL_REMOTE_CONTROL                0x27000000
#define DIAXIS_REMOTE_SLIDER                    0x27050201
#define DIBUTTON_REMOTE_MUTE                    0x27000401
#define DIBUTTON_REMOTE_SELECT                  0x27000402
#define DIBUTTON_REMOTE_PLAY                    0x27002403
#define DIBUTTON_REMOTE_CUE                     0x27002404
#define DIBUTTON_REMOTE_REVIEW                  0x27002405
#define DIBUTTON_REMOTE_CHANGE                  0x27002406
#define DIBUTTON_REMOTE_RECORD                  0x27002407
#define DIBUTTON_REMOTE_MENU                    0x270004FD

#define DIAXIS_REMOTE_SLIDER2                   0x27054202
#define DIBUTTON_REMOTE_TV                      0x27005C08
#define DIBUTTON_REMOTE_CABLE                   0x27005C09
#define DIBUTTON_REMOTE_CD                      0x27005C0A
#define DIBUTTON_REMOTE_VCR                     0x27005C0B
#define DIBUTTON_REMOTE_TUNER                   0x27005C0C
#define DIBUTTON_REMOTE_DVD                     0x27005C0D
#define DIBUTTON_REMOTE_ADJUST                  0x27005C0E
#define DIBUTTON_REMOTE_DIGIT0                  0x2700540F
#define DIBUTTON_REMOTE_DIGIT1                  0x27005410
#define DIBUTTON_REMOTE_DIGIT2                  0x27005411
#define DIBUTTON_REMOTE_DIGIT3                  0x27005412
#define DIBUTTON_REMOTE_DIGIT4                  0x27005413
#define DIBUTTON_REMOTE_DIGIT5                  0x27005414
#define DIBUTTON_REMOTE_DIGIT6                  0x27005415
#define DIBUTTON_REMOTE_DIGIT7                  0x27005416
#define DIBUTTON_REMOTE_DIGIT8                  0x27005417
#define DIBUTTON_REMOTE_DIGIT9                  0x27005418
#define DIBUTTON_REMOTE_DEVICE                  0x270044FE
#define DIBUTTON_REMOTE_PAUSE                   0x270044FC

#define DIVIRTUAL_BROWSER_CONTROL               0x28000000
#define DIAXIS_BROWSER_LATERAL                  0x28008201
#define DIAXIS_BROWSER_MOVE                     0x28010202
#define DIBUTTON_BROWSER_SELECT                 0x28000401
#define DIAXIS_BROWSER_VIEW                     0x28018203
#define DIBUTTON_BROWSER_REFRESH                0x28000402
#define DIBUTTON_BROWSER_MENU                   0x280004FD

#define DIBUTTON_BROWSER_SEARCH                 0x28004403
#define DIBUTTON_BROWSER_STOP                   0x28004404
#define DIBUTTON_BROWSER_HOME                   0x28004405
#define DIBUTTON_BROWSER_FAVORITES              0x28004406
#define DIBUTTON_BROWSER_NEXT                   0x28004407
#define DIBUTTON_BROWSER_PREVIOUS               0x28004408
#define DIBUTTON_BROWSER_HISTORY                0x28004409
#define DIBUTTON_BROWSER_PRINT                  0x2800440A
#define DIBUTTON_BROWSER_DEVICE                 0x280044FE
#define DIBUTTON_BROWSER_PAUSE                  0x280044FC

#define DIVIRTUAL_DRIVING_MECHA                 0x29000000
#define DIAXIS_MECHA_STEER                      0x29008201
#define DIAXIS_MECHA_TORSO                      0x29010202
#define DIAXIS_MECHA_ROTATE                     0x29020203
#define DIAXIS_MECHA_THROTTLE                   0x29038204
#define DIBUTTON_MECHA_FIRE                     0x29000401
#define DIBUTTON_MECHA_WEAPONS                  0x29000402
#define DIBUTTON_MECHA_TARGET                   0x29000403
#define DIBUTTON_MECHA_REVERSE                  0x29000404
#define DIBUTTON_MECHA_ZOOM                     0x29000405
#define DIBUTTON_MECHA_JUMP                     0x29000406
#define DIBUTTON_MECHA_MENU                     0x290004FD

#define DIBUTTON_MECHA_CENTER                   0x29004407
#define DIHATSWITCH_MECHA_GLANCE                0x29004601
#define DIBUTTON_MECHA_VIEW                     0x29004408
#define DIBUTTON_MECHA_FIRESECONDARY            0x29004409
#define DIBUTTON_MECHA_LEFT_LINK                0x2900C4E4
#define DIBUTTON_MECHA_RIGHT_LINK               0x2900C4EC
#define DIBUTTON_MECHA_FORWARD_LINK             0x290144E0
#define DIBUTTON_MECHA_BACK_LINK                0x290144E8
#define DIBUTTON_MECHA_ROTATE_LEFT_LINK         0x290244E4
#define DIBUTTON_MECHA_ROTATE_RIGHT_LINK        0x290244EC
#define DIBUTTON_MECHA_FASTER_LINK              0x2903C4E0
#define DIBUTTON_MECHA_SLOWER_LINK              0x2903C4E8
#define DIBUTTON_MECHA_DEVICE                   0x290044FE
#define DIBUTTON_MECHA_PAUSE                    0x290044FC

#define DIAXIS_ANY_X_1                          0xFF00C201
#define DIAXIS_ANY_X_2                          0xFF00C202
#define DIAXIS_ANY_Y_1                          0xFF014201
#define DIAXIS_ANY_Y_2                          0xFF014202
#define DIAXIS_ANY_Z_1                          0xFF01C201
#define DIAXIS_ANY_Z_2                          0xFF01C202
#define DIAXIS_ANY_R_1                          0xFF024201
#define DIAXIS_ANY_R_2                          0xFF024202
#define DIAXIS_ANY_U_1                          0xFF02C201
#define DIAXIS_ANY_U_2                          0xFF02C202
#define DIAXIS_ANY_V_1                          0xFF034201
#define DIAXIS_ANY_V_2                          0xFF034202
#define DIAXIS_ANY_A_1                          0xFF03C201
#define DIAXIS_ANY_A_2                          0xFF03C202
#define DIAXIS_ANY_B_1                          0xFF044201
#define DIAXIS_ANY_B_2                          0xFF044202
#define DIAXIS_ANY_C_1                          0xFF04C201
#define DIAXIS_ANY_C_2                          0xFF04C202
#define DIAXIS_ANY_S_1                          0xFF054201
#define DIAXIS_ANY_S_2                          0xFF054202

#define DIAXIS_ANY_1                            0xFF004201
#define DIAXIS_ANY_2                            0xFF004202
#define DIAXIS_ANY_3                            0xFF004203
#define DIAXIS_ANY_4                            0xFF004204

#define DIPOV_ANY_1                             0xFF004601
#define DIPOV_ANY_2                             0xFF004602
#define DIPOV_ANY_3                             0xFF004603
#define DIPOV_ANY_4                             0xFF004604

#define DIBUTTON_ANY(instance)                  ( 0xFF004400 | instance )

#ifdef __cplusplus
};
#endif

#endif

#ifdef _INC_MMSYSTEM
#ifndef MMNOJOY

#ifndef __VJOYDX_INCLUDED__
#define __VJOYDX_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#define JOY_PASSDRIVERDATA          0x10000000l

WINMMAPI MMRESULT WINAPI joyConfigChanged( DWORD dwFlags );

#define JOY_HWS_ISHEADTRACKER       0x02000000l

#define JOY_HWS_ISGAMEPORTDRIVER    0x04000000l

#define JOY_HWS_ISANALOGPORTDRIVER  0x08000000l

#define JOY_HWS_AUTOLOAD            0x10000000l

#define JOY_HWS_NODEVNODE           0x20000000l

#define JOY_HWS_ISGAMEPORTBUS       0x80000000l
#define JOY_HWS_GAMEPORTBUSBUSY     0x00000001l

#define JOY_US_VOLATILE             0x00000008L

#ifdef __cplusplus
};
#endif

#endif

#endif
#endif

#ifndef DIJ_RINGZERO

#ifdef _INC_MMDDK
#ifndef MMNOJOYDEV

#ifndef __VJOYDXD_INCLUDED__
#define __VJOYDXD_INCLUDED__

#define JOY_OEMPOLL_PASSDRIVERDATA  7

#endif

#endif
#endif

#endif

#endif

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataDSOUND*)this->hidden)
struct SDL_PrivateAudioDataDSOUND
{
    LPDIRECTSOUND sound;
    LPDIRECTSOUNDBUFFER mixbuf;
    int num_buffers;
    int mixlen;
    DWORD lastchunk;
    Uint8 *locked_buf;
};

#endif

#ifndef WAVE_FORMAT_IEEE_FLOAT
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#endif

static void* DSoundDLL = NULL;
typedef HRESULT(WINAPI*fnDirectSoundCreate8)(LPGUID,LPDIRECTSOUND*,LPUNKNOWN);
typedef HRESULT(WINAPI*fnDirectSoundEnumerateW)(LPDSENUMCALLBACKW, LPVOID);
typedef HRESULT(WINAPI*fnDirectSoundCaptureEnumerateW)(LPDSENUMCALLBACKW,LPVOID);
static fnDirectSoundCreate8 pDirectSoundCreate8 = NULL;
static fnDirectSoundEnumerateW pDirectSoundEnumerateW = NULL;
static fnDirectSoundCaptureEnumerateW pDirectSoundCaptureEnumerateW = NULL;

static void
DSOUND_Unload(void)
{
    pDirectSoundCreate8 = NULL;
    pDirectSoundEnumerateW = NULL;
    pDirectSoundCaptureEnumerateW = NULL;

    if (DSoundDLL != NULL) {
        SDL_UnloadObject(DSoundDLL);
        DSoundDLL = NULL;
    }
}

static int
DSOUND_Load(void)
{
    int loaded = 0;

    DSOUND_Unload();

    DSoundDLL = SDL_LoadObject("DSOUND.DLL");
    if (DSoundDLL == NULL) {
        SDL_SetError("DirectSound: failed to load DSOUND.DLL");
    } else {

        #define DSOUNDLOAD(f) { \
            p##f = (fn##f) SDL_LoadFunction(DSoundDLL, #f); \
            if (!p##f) loaded = 0; \
        }
        loaded = 1;
        DSOUNDLOAD(DirectSoundCreate8);
        DSOUNDLOAD(DirectSoundEnumerateW);
        DSOUNDLOAD(DirectSoundCaptureEnumerateW);
        #undef DSOUNDLOAD

        if (!loaded) {
            SDL_SetError("DirectSound: System doesn't appear to have DX8.");
        }
    }

    if (!loaded) {
        DSOUND_Unload();
    }

    return loaded;
}

static int
SetDSerror(const char *function, int code)
{
    static const char *error;
    static char errbuf[1024];

    errbuf[0] = 0;
    switch (code) {
    case E_NOINTERFACE:
        error = "Unsupported interface -- Is DirectX 8.0 or later installed?";
        break;
    case DSERR_ALLOCATED:
        error = "Audio device in use";
        break;
    case DSERR_BADFORMAT:
        error = "Unsupported audio format";
        break;
    case DSERR_BUFFERLOST:
        error = "Mixing buffer was lost";
        break;
    case DSERR_CONTROLUNAVAIL:
        error = "Control requested is not available";
        break;
    case DSERR_INVALIDCALL:
        error = "Invalid call for the current state";
        break;
    case DSERR_INVALIDPARAM:
        error = "Invalid parameter";
        break;
    case DSERR_NODRIVER:
        error = "No audio device found";
        break;
    case DSERR_OUTOFMEMORY:
        error = "Out of memory";
        break;
    case DSERR_PRIOLEVELNEEDED:
        error = "Caller doesn't have priority";
        break;
    case DSERR_UNSUPPORTED:
        error = "Function not supported";
        break;
    default:
        SDL_snprintf(errbuf, SDL_arraysize(errbuf),
                     "%s: Unknown DirectSound error: 0x%x", function, code);
        break;
    }
    if (!errbuf[0]) {
        SDL_snprintf(errbuf, SDL_arraysize(errbuf), "%s: %s", function,
                     error);
    }
    return SDL_SetError("%s", errbuf);
}

static BOOL CALLBACK
FindAllDevs(LPGUID guid, LPCWSTR desc, LPCWSTR module, LPVOID data)
{
    SDL_AddAudioDevice addfn = (SDL_AddAudioDevice) data;
    if (guid != NULL) {
        char *str = WIN_StringToUTF8(desc);
        if (str != NULL) {
            addfn(str);
            SDL_free(str);
        }
    }
    return TRUE;
}

static void
DSOUND_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    if (iscapture) {
        pDirectSoundCaptureEnumerateW(FindAllDevs, addfn);
    } else {
        pDirectSoundEnumerateW(FindAllDevs, addfn);
    }
}

static void
DSOUND_WaitDevice(_THIS)
{
    DWORD status = 0;
    DWORD cursor = 0;
    DWORD junk = 0;
    HRESULT result = DS_OK;

    result = IDirectSoundBuffer_GetCurrentPosition(SDLAUDIOHIDDEN->mixbuf,
                                                   &junk, &cursor);
    if (result != DS_OK) {
        if (result == DSERR_BUFFERLOST) {
            IDirectSoundBuffer_Restore(SDLAUDIOHIDDEN->mixbuf);
        }
#ifdef DEBUG_SOUND
        SetDSerror("DirectSound GetCurrentPosition", result);
#endif
        return;
    }

    while ((cursor / SDLAUDIOHIDDEN->mixlen) == SDLAUDIOHIDDEN->lastchunk) {

        SDL_Delay(1);

        IDirectSoundBuffer_GetStatus(SDLAUDIOHIDDEN->mixbuf, &status);
        if ((status & DSBSTATUS_BUFFERLOST)) {
            IDirectSoundBuffer_Restore(SDLAUDIOHIDDEN->mixbuf);
            IDirectSoundBuffer_GetStatus(SDLAUDIOHIDDEN->mixbuf, &status);
            if ((status & DSBSTATUS_BUFFERLOST)) {
                break;
            }
        }
        if (!(status & DSBSTATUS_PLAYING)) {
            result = IDirectSoundBuffer_Play(SDLAUDIOHIDDEN->mixbuf, 0, 0,
                                             DSBPLAY_LOOPING);
            if (result == DS_OK) {
                continue;
            }
#ifdef DEBUG_SOUND
            SetDSerror("DirectSound Play", result);
#endif
            return;
        }

        result = IDirectSoundBuffer_GetCurrentPosition(SDLAUDIOHIDDEN->mixbuf,
                                                       &junk, &cursor);
        if (result != DS_OK) {
            SetDSerror("DirectSound GetCurrentPosition", result);
            return;
        }
    }
}

static void
DSOUND_PlayDevice(_THIS)
{

    if (SDLAUDIOHIDDEN->locked_buf) {
        IDirectSoundBuffer_Unlock(SDLAUDIOHIDDEN->mixbuf,
                                  SDLAUDIOHIDDEN->locked_buf,
                                  SDLAUDIOHIDDEN->mixlen, NULL, 0);
    }

}

static Uint8 *
DSOUND_GetDeviceBuf(_THIS)
{
    DWORD cursor = 0;
    DWORD junk = 0;
    HRESULT result = DS_OK;
    DWORD rawlen = 0;

    SDLAUDIOHIDDEN->locked_buf = NULL;
    result = IDirectSoundBuffer_GetCurrentPosition(SDLAUDIOHIDDEN->mixbuf,
                                                   &junk, &cursor);
    if (result == DSERR_BUFFERLOST) {
        IDirectSoundBuffer_Restore(SDLAUDIOHIDDEN->mixbuf);
        result = IDirectSoundBuffer_GetCurrentPosition(SDLAUDIOHIDDEN->mixbuf,
                                                       &junk, &cursor);
    }
    if (result != DS_OK) {
        SetDSerror("DirectSound GetCurrentPosition", result);
        return (NULL);
    }
    cursor /= SDLAUDIOHIDDEN->mixlen;
#ifdef DEBUG_SOUND

    {
        DWORD spot = cursor;
        if (spot < SDLAUDIOHIDDEN->lastchunk) {
            spot += SDLAUDIOHIDDEN->num_buffers;
        }
        if (spot > SDLAUDIOHIDDEN->lastchunk + 1) {
            fprintf(stderr, "Audio dropout, missed %d fragments\n",
                    (spot - (SDLAUDIOHIDDEN->lastchunk + 1)));
        }
    }
#endif
    SDLAUDIOHIDDEN->lastchunk = cursor;
    cursor = (cursor + 1) % SDLAUDIOHIDDEN->num_buffers;
    cursor *= SDLAUDIOHIDDEN->mixlen;

    result = IDirectSoundBuffer_Lock(SDLAUDIOHIDDEN->mixbuf, cursor,
                                     SDLAUDIOHIDDEN->mixlen,
                                     (LPVOID *) & SDLAUDIOHIDDEN->locked_buf,
                                     &rawlen, NULL, &junk, 0);
    if (result == DSERR_BUFFERLOST) {
        IDirectSoundBuffer_Restore(SDLAUDIOHIDDEN->mixbuf);
        result = IDirectSoundBuffer_Lock(SDLAUDIOHIDDEN->mixbuf, cursor,
                                         SDLAUDIOHIDDEN->mixlen,
                                         (LPVOID *) & 
                                         SDLAUDIOHIDDEN->locked_buf, &rawlen, NULL,
                                         &junk, 0);
    }
    if (result != DS_OK) {
        SetDSerror("DirectSound Lock", result);
        return (NULL);
    }
    return (SDLAUDIOHIDDEN->locked_buf);
}

static void
DSOUND_WaitDone(_THIS)
{
    Uint8 *stream = DSOUND_GetDeviceBuf(this);

    if (stream != NULL) {
        SDL_memset(stream, this->spec.silence, SDLAUDIOHIDDEN->mixlen);
        DSOUND_PlayDevice(this);
    }
    DSOUND_WaitDevice(this);

    IDirectSoundBuffer_Stop(SDLAUDIOHIDDEN->mixbuf);
}

static void
DSOUND_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        if (SDLAUDIOHIDDEN->sound != NULL) {
            if (SDLAUDIOHIDDEN->mixbuf != NULL) {

                IDirectSoundBuffer_Release(SDLAUDIOHIDDEN->mixbuf);
                SDLAUDIOHIDDEN->mixbuf = NULL;
            }
            IDirectSound_Release(SDLAUDIOHIDDEN->sound);
            SDLAUDIOHIDDEN->sound = NULL;
        }

        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
CreateSecondary(_THIS, HWND focus)
{
    LPDIRECTSOUND sndObj = SDLAUDIOHIDDEN->sound;
    LPDIRECTSOUNDBUFFER *sndbuf = &SDLAUDIOHIDDEN->mixbuf;
    Uint32 chunksize = this->spec.size;
    const int numchunks = 8;
    HRESULT result = DS_OK;
    DSBUFFERDESC format;
    LPVOID pvAudioPtr1, pvAudioPtr2;
    DWORD dwAudioBytes1, dwAudioBytes2;
    WAVEFORMATEX wfmt;

    SDL_zero(wfmt);

    if (SDL_AUDIO_ISFLOAT(this->spec.format)) {
        wfmt.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    } else {
        wfmt.wFormatTag = WAVE_FORMAT_PCM;
    }

    wfmt.wBitsPerSample = SDL_AUDIO_BITSIZE(this->spec.format);
    wfmt.nChannels = this->spec.channels;
    wfmt.nSamplesPerSec = this->spec.freq;
    wfmt.nBlockAlign = wfmt.nChannels * (wfmt.wBitsPerSample / 8);
    wfmt.nAvgBytesPerSec = wfmt.nSamplesPerSec * wfmt.nBlockAlign;

    SDL_CalculateAudioSpec(&this->spec);

    if (focus) {
        result = IDirectSound_SetCooperativeLevel(sndObj,
                                                  focus, DSSCL_PRIORITY);
    } else {
        result = IDirectSound_SetCooperativeLevel(sndObj,
                                                  GetDesktopWindow(),
                                                  DSSCL_NORMAL);
    }
    if (result != DS_OK) {
        return SetDSerror("DirectSound SetCooperativeLevel", result);
    }

    SDL_zero(format);
    format.dwSize = sizeof(format);
    format.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
    if (!focus) {
        format.dwFlags |= DSBCAPS_GLOBALFOCUS;
    } else {
        format.dwFlags |= DSBCAPS_STICKYFOCUS;
    }
    format.dwBufferBytes = numchunks * chunksize;
    if ((format.dwBufferBytes < DSBSIZE_MIN) ||
        (format.dwBufferBytes > DSBSIZE_MAX)) {
        return SDL_SetError("Sound buffer size must be between %d and %d",
                            DSBSIZE_MIN / numchunks, DSBSIZE_MAX / numchunks);
    }
    format.dwReserved = 0;
    format.lpwfxFormat = &wfmt;
    result = IDirectSound_CreateSoundBuffer(sndObj, &format, sndbuf, NULL);
    if (result != DS_OK) {
        return SetDSerror("DirectSound CreateSoundBuffer", result);
    }
    IDirectSoundBuffer_SetFormat(*sndbuf, &wfmt);

    result = IDirectSoundBuffer_Lock(*sndbuf, 0, format.dwBufferBytes,
                                     (LPVOID *) & pvAudioPtr1, &dwAudioBytes1,
                                     (LPVOID *) & pvAudioPtr2, &dwAudioBytes2,
                                     DSBLOCK_ENTIREBUFFER);
    if (result == DS_OK) {
        SDL_memset(pvAudioPtr1, this->spec.silence, dwAudioBytes1);
        IDirectSoundBuffer_Unlock(*sndbuf,
                                  (LPVOID) pvAudioPtr1, dwAudioBytes1,
                                  (LPVOID) pvAudioPtr2, dwAudioBytes2);
    }

    return (numchunks);
}

typedef struct FindDevGUIDData
{
    const char *devname;
    GUID guid;
    int found;
} FindDevGUIDData;

static BOOL CALLBACK
FindDevGUID(LPGUID guid, LPCWSTR desc, LPCWSTR module, LPVOID _data)
{
    if (guid != NULL) {
        FindDevGUIDData *data = (FindDevGUIDData *) _data;
        char *str = WIN_StringToUTF8(desc);
        const int match = (SDL_strcmp(str, data->devname) == 0);
        SDL_free(str);
        if (match) {
            data->found = 1;
            SDL_memcpy(&data->guid, guid, sizeof (data->guid));
            return FALSE;
        }
    }
    return TRUE;
}

static int
DSOUND_OpenDevice(_THIS, const char *devname, int iscapture)
{
    HRESULT result;
    SDL_bool valid_format = SDL_FALSE;
    SDL_bool tried_format = SDL_FALSE;
    SDL_AudioFormat test_format = SDL_FirstAudioFormat(this->spec.format);
    FindDevGUIDData devguid;
    LPGUID guid = NULL;

    if (devname != NULL) {
        devguid.found = 0;
        devguid.devname = devname;
        if (iscapture)
            pDirectSoundCaptureEnumerateW(FindDevGUID, &devguid);
        else
            pDirectSoundEnumerateW(FindDevGUID, &devguid);

        if (!devguid.found) {
            return SDL_SetError("DirectSound: Requested device not found");
        }
        guid = &devguid.guid;
    }

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    result = pDirectSoundCreate8(guid, &SDLAUDIOHIDDEN->sound, NULL);
    if (result != DS_OK) {
        DSOUND_CloseDevice(this);
        return SetDSerror("DirectSoundCreate", result);
    }

    while ((!valid_format) && (test_format)) {
        switch (test_format) {
        case AUDIO_U8:
        case AUDIO_S16:
        case AUDIO_S32:
        case AUDIO_F32:
            tried_format = SDL_TRUE;
            this->spec.format = test_format;
            SDLAUDIOHIDDEN->num_buffers = CreateSecondary(this, NULL);
            if (SDLAUDIOHIDDEN->num_buffers > 0) {
                valid_format = SDL_TRUE;
            }
            break;
        }
        test_format = SDL_NextAudioFormat();
    }

    if (!valid_format) {
        DSOUND_CloseDevice(this);
        if (tried_format) {
            return -1;
        }
        return SDL_SetError("DirectSound: Unsupported audio format");
    }

    SDLAUDIOHIDDEN->mixlen = this->spec.size;

    return 0;
}

static void
DSOUND_Deinitialize(void)
{
    DSOUND_Unload();
}

static int
DSOUND_Init(SDL_AudioDriverImpl * impl)
{
    if (!DSOUND_Load()) {
        return 0;
    }

    impl->DetectDevices = DSOUND_DetectDevices;
    impl->OpenDevice = DSOUND_OpenDevice;
    impl->PlayDevice = DSOUND_PlayDevice;
    impl->WaitDevice = DSOUND_WaitDevice;
    impl->WaitDone = DSOUND_WaitDone;
    impl->GetDeviceBuf = DSOUND_GetDeviceBuf;
    impl->CloseDevice = DSOUND_CloseDevice;
    impl->Deinitialize = DSOUND_Deinitialize;

    return 1;
}

AudioBootStrap DSOUND_bootstrap = {
    "directsound", "DirectSound", DSOUND_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_OSS

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#if SDL_AUDIO_DRIVER_OSS_SOUNDCARD_H

#include <soundcard.h>
#else

#include <sys/soundcard.h>
#endif

#define SDL_AllocAudioMem   SDL_malloc
#define SDL_FreeAudioMem    SDL_free
#ifdef USE_BLOCKING_WRITES
#define OPEN_FLAGS_OUTPUT O_WRONLY
#define OPEN_FLAGS_INPUT O_RDONLY
#else
#define OPEN_FLAGS_OUTPUT (O_WRONLY|O_NONBLOCK)
#define OPEN_FLAGS_INPUT (O_RDONLY|O_NONBLOCK)
#endif

void SDL_EnumUnixAudioDevices(int iscapture, int classic,
                              int (*test) (int fd), SDL_AddAudioDevice addfn);
#ifndef _SDL_dspaudio_h
#define _SDL_dspaudio_h

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataDSP*)this->hidden)
struct SDL_PrivateAudioDataDSP
{

    int audio_fd;

    Uint8 *mixbuf;
    int mixlen;
};
#define FUDGE_TICKS 10

#endif

static void
DSP_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    SDL_EnumUnixAudioDevices(iscapture, 0, NULL, addfn);
}

static void
DSP_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        SDL_FreeAudioMem(SDLAUDIOHIDDEN->mixbuf);
        SDLAUDIOHIDDEN->mixbuf = NULL;
        if (SDLAUDIOHIDDEN->audio_fd >= 0) {
            close(SDLAUDIOHIDDEN->audio_fd);
            SDLAUDIOHIDDEN->audio_fd = -1;
        }
        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
DSP_OpenDevice(_THIS, const char *devname, int iscapture)
{
    const int flags = ((iscapture) ? OPEN_FLAGS_INPUT : OPEN_FLAGS_OUTPUT);
    int format;
    int value;
    int frag_spec;
    SDL_AudioFormat test_format;

    if (devname == NULL) {
        devname = SDL_GetAudioDeviceName(0, iscapture);
        if (devname == NULL) {
            return SDL_SetError("No such audio device");
        }
    }

    if (this->spec.channels > 8)
        this->spec.channels = 8;
    else if (this->spec.channels > 4)
        this->spec.channels = 4;
    else if (this->spec.channels > 2)
        this->spec.channels = 2;

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    SDLAUDIOHIDDEN->audio_fd = open(devname, flags, 0);
    if (SDLAUDIOHIDDEN->audio_fd < 0) {
        DSP_CloseDevice(this);
        return SDL_SetError("Couldn't open %s: %s", devname, strerror(errno));
    }
    SDLAUDIOHIDDEN->mixbuf = NULL;

    {
        long ctlflags;
        ctlflags = fcntl(SDLAUDIOHIDDEN->audio_fd, F_GETFL);
        ctlflags &= ~O_NONBLOCK;
        if (fcntl(SDLAUDIOHIDDEN->audio_fd, F_SETFL, ctlflags) < 0) {
            DSP_CloseDevice(this);
            return SDL_SetError("Couldn't set audio blocking mode");
        }
    }

    if (ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_GETFMTS, &value) < 0) {
        perror("SNDCTL_DSP_GETFMTS");
        DSP_CloseDevice(this);
        return SDL_SetError("Couldn't get audio format list");
    }

    format = 0;
    for (test_format = SDL_FirstAudioFormat(this->spec.format);
         !format && test_format;) {
#ifdef DEBUG_AUDIO
        fprintf(stderr, "Trying format 0x%4.4x\n", test_format);
#endif
        switch (test_format) {
        case AUDIO_U8:
            if (value & AFMT_U8) {
                format = AFMT_U8;
            }
            break;
        case AUDIO_S16LSB:
            if (value & AFMT_S16_LE) {
                format = AFMT_S16_LE;
            }
            break;
        case AUDIO_S16MSB:
            if (value & AFMT_S16_BE) {
                format = AFMT_S16_BE;
            }
            break;
#if 0

        case AUDIO_S8:
            if (value & AFMT_S8) {
                format = AFMT_S8;
            }
            break;
        case AUDIO_U16LSB:
            if (value & AFMT_U16_LE) {
                format = AFMT_U16_LE;
            }
            break;
        case AUDIO_U16MSB:
            if (value & AFMT_U16_BE) {
                format = AFMT_U16_BE;
            }
            break;
#endif
        default:
            format = 0;
            break;
        }
        if (!format) {
            test_format = SDL_NextAudioFormat();
        }
    }
    if (format == 0) {
        DSP_CloseDevice(this);
        return SDL_SetError("Couldn't find any hardware audio formats");
    }
    this->spec.format = test_format;

    value = format;
    if ((ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_SETFMT, &value) < 0) ||
        (value != format)) {
        perror("SNDCTL_DSP_SETFMT");
        DSP_CloseDevice(this);
        return SDL_SetError("Couldn't set audio format");
    }

    value = this->spec.channels;
    if (ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_CHANNELS, &value) < 0) {
        perror("SNDCTL_DSP_CHANNELS");
        DSP_CloseDevice(this);
        return SDL_SetError("Cannot set the number of channels");
    }
    this->spec.channels = value;

    value = this->spec.freq;
    if (ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_SPEED, &value) < 0) {
        perror("SNDCTL_DSP_SPEED");
        DSP_CloseDevice(this);
        return SDL_SetError("Couldn't set audio frequency");
    }
    this->spec.freq = value;

    SDL_CalculateAudioSpec(&this->spec);

    for (frag_spec = 0; (0x01U << frag_spec) < this->spec.size; ++frag_spec);
    if ((0x01U << frag_spec) != this->spec.size) {
        DSP_CloseDevice(this);
        return SDL_SetError("Fragment size must be a power of two");
    }
    frag_spec |= 0x00020000;

#ifdef DEBUG_AUDIO
    fprintf(stderr, "Requesting %d fragments of size %d\n",
            (frag_spec >> 16), 1 << (frag_spec & 0xFFFF));
#endif
    if (ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_SETFRAGMENT, &frag_spec) < 0) {
        perror("SNDCTL_DSP_SETFRAGMENT");
    }
#ifdef DEBUG_AUDIO
    {
        audio_buf_info info;
        ioctl(SDLAUDIOHIDDEN->audio_fd, SNDCTL_DSP_GETOSPACE, &info);
        fprintf(stderr, "fragments = %d\n", info.fragments);
        fprintf(stderr, "fragstotal = %d\n", info.fragstotal);
        fprintf(stderr, "fragsize = %d\n", info.fragsize);
        fprintf(stderr, "bytes = %d\n", info.bytes);
    }
#endif

    SDLAUDIOHIDDEN->mixlen = this->spec.size;
    SDLAUDIOHIDDEN->mixbuf = (Uint8 *) SDL_AllocAudioMem(SDLAUDIOHIDDEN->mixlen);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        DSP_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN->mixbuf, this->spec.silence, this->spec.size);

    return 0;
}

static void
DSP_PlayDevice(_THIS)
{
    const Uint8 *mixbuf = SDLAUDIOHIDDEN->mixbuf;
    const int mixlen = SDLAUDIOHIDDEN->mixlen;
    if (write(SDLAUDIOHIDDEN->audio_fd, mixbuf, mixlen) == -1) {
        perror("Audio write");
        this->enabled = 0;
    }
#ifdef DEBUG_AUDIO
    fprintf(stderr, "Wrote %d bytes of audio data\n", mixlen);
#endif
}

static Uint8 *
DSP_GetDeviceBuf(_THIS)
{
    return (SDLAUDIOHIDDEN->mixbuf);
}

static int
DSP_Init(SDL_AudioDriverImpl * impl)
{

    impl->DetectDevices = DSP_DetectDevices;
    impl->OpenDevice = DSP_OpenDevice;
    impl->PlayDevice = DSP_PlayDevice;
    impl->GetDeviceBuf = DSP_GetDeviceBuf;
    impl->CloseDevice = DSP_CloseDevice;

    return 1;
}

AudioBootStrap DSP_bootstrap = {
    "dsp", "OSS /dev/dsp standard audio", DSP_Init, 0
};

#endif
#define SDL_AllocAudioMem   SDL_malloc
#define SDL_FreeAudioMem    SDL_free

#undef _THIS
#define _THIS SDL_AudioDevice *_this

static SDL_AudioDriver current_audio;
static SDL_AudioDevice *open_devices[16];

#define DEFAULT_OUTPUT_DEVNAME "System audio output device"
#define DEFAULT_INPUT_DEVNAME "System audio capture device"

extern AudioBootStrap BSD_AUDIO_bootstrap;
extern AudioBootStrap DSP_bootstrap;
extern AudioBootStrap ALSA_bootstrap;
extern AudioBootStrap PULSEAUDIO_bootstrap;
extern AudioBootStrap QSAAUDIO_bootstrap;
extern AudioBootStrap SUNAUDIO_bootstrap;
extern AudioBootStrap ARTS_bootstrap;
extern AudioBootStrap ESD_bootstrap;
extern AudioBootStrap NAS_bootstrap;
extern AudioBootStrap XAUDIO2_bootstrap;
extern AudioBootStrap DSOUND_bootstrap;
extern AudioBootStrap WINMM_bootstrap;
extern AudioBootStrap PAUDIO_bootstrap;
extern AudioBootStrap HAIKUAUDIO_bootstrap;
extern AudioBootStrap COREAUDIO_bootstrap;
extern AudioBootStrap SNDMGR_bootstrap;
extern AudioBootStrap DISKAUD_bootstrap;
extern AudioBootStrap DUMMYAUD_bootstrap;
extern AudioBootStrap DCAUD_bootstrap;
extern AudioBootStrap DART_bootstrap;
extern AudioBootStrap NDSAUD_bootstrap;
extern AudioBootStrap FUSIONSOUND_bootstrap;
extern AudioBootStrap ANDROIDAUD_bootstrap;
extern AudioBootStrap PSPAUD_bootstrap;
extern AudioBootStrap SNDIO_bootstrap;

static const AudioBootStrap *const bootstrap[] = {
#if SDL_AUDIO_DRIVER_PULSEAUDIO
    &PULSEAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_ALSA
    &ALSA_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_SNDIO
    &SNDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_BSD
    &BSD_AUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_OSS
    &DSP_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_QSA
    &QSAAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_SUNAUDIO
    &SUNAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_ARTS
    &ARTS_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_ESD
    &ESD_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_NAS
    &NAS_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_XAUDIO2
    &XAUDIO2_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_DSOUND
    &DSOUND_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_WINMM
    &WINMM_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_PAUDIO
    &PAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_HAIKU
    &HAIKUAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_COREAUDIO
    &COREAUDIO_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_DISK
    &DISKAUD_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_DUMMY
    &DUMMYAUD_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_FUSIONSOUND
    &FUSIONSOUND_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_ANDROID
    &ANDROIDAUD_bootstrap,
#endif
#if SDL_AUDIO_DRIVER_PSP
    &PSPAUD_bootstrap,
#endif
    NULL
};

static SDL_AudioDevice *
get_audio_device(SDL_AudioDeviceID id)
{
    id--;
    if ((id >= SDL_arraysize(open_devices)) || (open_devices[id] == NULL)) {
        SDL_SetError("Invalid audio device ID");
        return NULL;
    }

    return open_devices[id];
}

static void
SDL_AudioDetectDevices_Default(int iscapture, SDL_AddAudioDevice addfn)
{
}

static void
SDL_AudioThreadInit_Default(_THIS)
{
}

static void
SDL_AudioWaitDevice_Default(_THIS)
{
}

static void
SDL_AudioPlayDevice_Default(_THIS)
{
}

static Uint8 *
SDL_AudioGetDeviceBuf_Default(_THIS)
{
    return NULL;
}

static void
SDL_AudioWaitDone_Default(_THIS)
{
}

static void
SDL_AudioCloseDevice_Default(_THIS)
{
}

static void
SDL_AudioDeinitialize_Default(void)
{
}

static int
SDL_AudioOpenDevice_Default(_THIS, const char *devname, int iscapture)
{
    return -1;
}

static void
SDL_AudioLockDevice_Default(SDL_AudioDevice * device)
{
    if (device->thread && (SDL_ThreadID() == device->threadid)) {
        return;
    }
    SDL_LockMutex(device->mixer_lock);
}

static void
SDL_AudioUnlockDevice_Default(SDL_AudioDevice * device)
{
    if (device->thread && (SDL_ThreadID() == device->threadid)) {
        return;
    }
    SDL_UnlockMutex(device->mixer_lock);
}

static void
finalize_audio_entry_points(void)
{

#define FILL_STUB(x) \
        if (current_audio.impl.x == NULL) { \
            current_audio.impl.x = SDL_Audio##x##_Default; \
        }
    FILL_STUB(DetectDevices);
    FILL_STUB(OpenDevice);
    FILL_STUB(ThreadInit);
    FILL_STUB(WaitDevice);
    FILL_STUB(PlayDevice);
    FILL_STUB(GetDeviceBuf);
    FILL_STUB(WaitDone);
    FILL_STUB(CloseDevice);
    FILL_STUB(LockDevice);
    FILL_STUB(UnlockDevice);
    FILL_STUB(Deinitialize);
#undef FILL_STUB
}

static void
SDL_StreamWrite(SDL_AudioStreamer * stream, Uint8 * buf, int length)
{
    int i;

    for (i = 0; i < length; ++i) {
        stream->buffer[stream->write_pos] = buf[i];
        ++stream->write_pos;
    }
}

static void
SDL_StreamRead(SDL_AudioStreamer * stream, Uint8 * buf, int length)
{
    int i;

    for (i = 0; i < length; ++i) {
        buf[i] = stream->buffer[stream->read_pos];
        ++stream->read_pos;
    }
}

static int
SDL_StreamLength(SDL_AudioStreamer * stream)
{
    return (stream->write_pos - stream->read_pos) % stream->max_len;
}

#if 0
static int
SDL_StreamInit(SDL_AudioStreamer * stream, int max_len, Uint8 silence)
{

    stream->buffer = (Uint8 *) SDL_malloc(max_len);
    if (stream->buffer == NULL) {
        return -1;
    }

    stream->max_len = max_len;
    stream->read_pos = 0;
    stream->write_pos = 0;

    SDL_memset(stream->buffer, silence, max_len);

    return 0;
}
#endif

static void
SDL_StreamDeinit(SDL_AudioStreamer * stream)
{
    SDL_free(stream->buffer);
}

#if defined(ANDROID)
#include <android/log.h>
#endif

int SDLCALL
SDL_RunAudio(void *devicep)
{
    SDL_AudioDevice *device = (SDL_AudioDevice *) devicep;
    Uint8 *stream;
    int stream_len;
    void *udata;
    void (SDLCALL * fill) (void *userdata, Uint8 * stream, int len);
    Uint32 delay;

    Uint8 *istream;
    int istream_len = 0;

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

    device->threadid = SDL_ThreadID();
    current_audio.impl.ThreadInit(device);

    fill = device->spec.callback;
    udata = device->spec.userdata;

    device->use_streamer = 0;

    if (device->convert.needed) {
#if 0

        if (device->convert.len_mult != 1 || device->convert.len_div != 1) {

            stream_max_len = 2 * device->spec.size;
            if (device->convert.len_mult > device->convert.len_div) {
                stream_max_len *= device->convert.len_mult;
                stream_max_len /= device->convert.len_div;
            }
            if (SDL_StreamInit(&device->streamer, stream_max_len, silence) <
                0)
                return -1;
            device->use_streamer = 1;

            istream_len =
                device->spec.size * device->convert.len_div /
                device->convert.len_mult;
        }
#endif
        stream_len = device->convert.len;
    } else {
        stream_len = device->spec.size;
    }

    delay = ((device->spec.samples * 1000) / device->spec.freq);

    if (device->use_streamer == 1) {

        while (device->enabled) {

            if (device->paused) {
                SDL_Delay(delay);
                continue;
            }

            if (SDL_StreamLength(&device->streamer) < stream_len) {

                if (device->convert.needed) {
                    if (device->convert.buf) {
                        istream = device->convert.buf;
                    } else {
                        continue;
                    }
                } else {

                    istream = current_audio.impl.GetDeviceBuf(device);
                    if (istream == NULL) {
                        istream = device->fake_stream;
                    }
                }

                SDL_LockMutex(device->mixer_lock);
                (*fill) (udata, istream, istream_len);
                SDL_UnlockMutex(device->mixer_lock);

                if (device->convert.needed) {
                    SDL_ConvertAudio(&device->convert);
                    if (istream == NULL) {
                        istream = device->fake_stream;
                    }

                    SDL_StreamWrite(&device->streamer, device->convert.buf,
                                    device->convert.len_cvt);
                } else {
                    SDL_StreamWrite(&device->streamer, istream, istream_len);
                }
            }

            if (SDL_StreamLength(&device->streamer) >= stream_len) {

                if (device->convert.needed) {
                    if (device->convert.buf) {
                        stream = device->convert.buf;
                    } else {
                        continue;
                    }
                } else {
                    stream = current_audio.impl.GetDeviceBuf(device);
                    if (stream == NULL) {
                        stream = device->fake_stream;
                    }
                }

                SDL_StreamRead(&device->streamer, stream, stream_len);

                if (stream != device->fake_stream) {
                    current_audio.impl.PlayDevice(device);

                    current_audio.impl.WaitDevice(device);
                } else {
                    SDL_Delay(delay);
                }
            }

        }
    } else {

        const int silence = (int) device->spec.silence;

        while (device->enabled) {

            if (device->convert.needed) {
                if (device->convert.buf) {
                    stream = device->convert.buf;
                } else {
                    continue;
                }
            } else {
                stream = current_audio.impl.GetDeviceBuf(device);
                if (stream == NULL) {
                    stream = device->fake_stream;
                }
            }

            SDL_LockMutex(device->mixer_lock);
            if (device->paused) {
                SDL_memset(stream, silence, stream_len);
            } else {
                (*fill) (udata, stream, stream_len);
            }
            SDL_UnlockMutex(device->mixer_lock);

            if (device->convert.needed) {
                SDL_ConvertAudio(&device->convert);
                stream = current_audio.impl.GetDeviceBuf(device);
                if (stream == NULL) {
                    stream = device->fake_stream;
                }
                SDL_memcpy(stream, device->convert.buf,
                           device->convert.len_cvt);
            }

            if (stream != device->fake_stream) {
                current_audio.impl.PlayDevice(device);

                current_audio.impl.WaitDevice(device);
            } else {
                SDL_Delay(delay);
            }
        }
    }

    current_audio.impl.WaitDone(device);

    if (device->use_streamer == 1)
        SDL_StreamDeinit(&device->streamer);

    return (0);
}

static SDL_AudioFormat
SDL_ParseAudioFormat(const char *string)
{
#define CHECK_FMT_STRING(x) if (SDL_strcmp(string, #x) == 0) return AUDIO_##x
    CHECK_FMT_STRING(U8);
    CHECK_FMT_STRING(S8);
    CHECK_FMT_STRING(U16LSB);
    CHECK_FMT_STRING(S16LSB);
    CHECK_FMT_STRING(U16MSB);
    CHECK_FMT_STRING(S16MSB);
    CHECK_FMT_STRING(U16SYS);
    CHECK_FMT_STRING(S16SYS);
    CHECK_FMT_STRING(U16);
    CHECK_FMT_STRING(S16);
    CHECK_FMT_STRING(S32LSB);
    CHECK_FMT_STRING(S32MSB);
    CHECK_FMT_STRING(S32SYS);
    CHECK_FMT_STRING(S32);
    CHECK_FMT_STRING(F32LSB);
    CHECK_FMT_STRING(F32MSB);
    CHECK_FMT_STRING(F32SYS);
    CHECK_FMT_STRING(F32);
#undef CHECK_FMT_STRING
    return 0;
}

int
SDL_GetNumAudioDrivers(void)
{
    return (SDL_arraysize(bootstrap) - 1);
}

const char *
SDL_GetAudioDriver(int index)
{
    if (index >= 0 && index < SDL_GetNumAudioDrivers()) {
        return (bootstrap[index]->name);
    }
    return (NULL);
}

int
SDL_AudioInit(const char *driver_name)
{
    int i = 0;
    int initialized = 0;
    int tried_to_init = 0;

    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_AudioQuit();
    }

    SDL_memset(&current_audio, '\0', sizeof(current_audio));
    SDL_memset(open_devices, '\0', sizeof(open_devices));

    if (driver_name == NULL) {
        driver_name = SDL_getenv("SDL_AUDIODRIVER");
    }

    for (i = 0; (!initialized) && (bootstrap[i]); ++i) {

        const AudioBootStrap *backend = bootstrap[i];
        if ((driver_name && (SDL_strncasecmp(backend->name, driver_name, SDL_strlen(driver_name)) != 0)) ||
            (!driver_name && backend->demand_only)) {
            continue;
        }

        tried_to_init = 1;
        SDL_memset(&current_audio, 0, sizeof(current_audio));
        current_audio.name = backend->name;
        current_audio.desc = backend->desc;
        initialized = backend->init(&current_audio.impl);
    }

    if (!initialized) {

        if (!tried_to_init) {
            if (driver_name) {
                SDL_SetError("Audio target '%s' not available", driver_name);
            } else {
                SDL_SetError("No available audio device");
            }
        }

        SDL_memset(&current_audio, 0, sizeof(current_audio));
        return (-1);
    }

    finalize_audio_entry_points();

    return (0);
}

const char *
SDL_GetCurrentAudioDriver()
{
    return current_audio.name;
}

static void
free_device_list(char ***devices, int *devCount)
{
    int i = *devCount;
    if ((i > 0) && (*devices != NULL)) {
        while (i--) {
            SDL_free((*devices)[i]);
        }
    }

    SDL_free(*devices);

    *devices = NULL;
    *devCount = 0;
}

static
void SDL_AddCaptureAudioDevice(const char *_name)
{
    char *name = NULL;
    void *ptr = SDL_realloc(current_audio.inputDevices,
                          (current_audio.inputDeviceCount+1) * sizeof(char*));
    if (ptr == NULL) {
        return;
    }

    current_audio.inputDevices = (char **) ptr;
    name = SDL_strdup(_name);
    current_audio.inputDevices[current_audio.inputDeviceCount++] = name;
}

static
void SDL_AddOutputAudioDevice(const char *_name)
{
    char *name = NULL;
    void *ptr = SDL_realloc(current_audio.outputDevices,
                          (current_audio.outputDeviceCount+1) * sizeof(char*));
    if (ptr == NULL) {
        return;
    }

    current_audio.outputDevices = (char **) ptr;
    name = SDL_strdup(_name);
    current_audio.outputDevices[current_audio.outputDeviceCount++] = name;
}

int
SDL_GetNumAudioDevices(int iscapture)
{
    int retval = 0;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        return -1;
    }

    if ((iscapture) && (!current_audio.impl.HasCaptureSupport)) {
        return 0;
    }

    if ((iscapture) && (current_audio.impl.OnlyHasDefaultInputDevice)) {
        return 1;
    }

    if ((!iscapture) && (current_audio.impl.OnlyHasDefaultOutputDevice)) {
        return 1;
    }

    if (iscapture) {
        free_device_list(&current_audio.inputDevices,
                         &current_audio.inputDeviceCount);
        current_audio.impl.DetectDevices(iscapture, SDL_AddCaptureAudioDevice);
        retval = current_audio.inputDeviceCount;
    } else {
        free_device_list(&current_audio.outputDevices,
                         &current_audio.outputDeviceCount);
        current_audio.impl.DetectDevices(iscapture, SDL_AddOutputAudioDevice);
        retval = current_audio.outputDeviceCount;
    }

    return retval;
}

const char *
SDL_GetAudioDeviceName(int index, int iscapture)
{
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_SetError("Audio subsystem is not initialized");
        return NULL;
    }

    if ((iscapture) && (!current_audio.impl.HasCaptureSupport)) {
        SDL_SetError("No capture support");
        return NULL;
    }

    if (index < 0) {
        goto no_such_device;
    }

    if ((iscapture) && (current_audio.impl.OnlyHasDefaultInputDevice)) {
        if (index > 0) {
            goto no_such_device;
        }
        return DEFAULT_INPUT_DEVNAME;
    }

    if ((!iscapture) && (current_audio.impl.OnlyHasDefaultOutputDevice)) {
        if (index > 0) {
            goto no_such_device;
        }
        return DEFAULT_OUTPUT_DEVNAME;
    }

    if (iscapture) {
        if (index >= current_audio.inputDeviceCount) {
            goto no_such_device;
        }
        return current_audio.inputDevices[index];
    } else {
        if (index >= current_audio.outputDeviceCount) {
            goto no_such_device;
        }
        return current_audio.outputDevices[index];
    }

no_such_device:
    SDL_SetError("No such device");
    return NULL;
}

static void
close_audio_device(SDL_AudioDevice * device)
{
    device->enabled = 0;
    if (device->thread != NULL) {
        SDL_WaitThread(device->thread, NULL);
    }
    if (device->mixer_lock != NULL) {
        SDL_DestroyMutex(device->mixer_lock);
    }
    SDL_FreeAudioMem(device->fake_stream);
    if (device->convert.needed) {
        SDL_FreeAudioMem(device->convert.buf);
    }
    if (device->opened) {
        current_audio.impl.CloseDevice(device);
        device->opened = 0;
    }
    SDL_FreeAudioMem(device);
}

static int
prepare_audiospec(const SDL_AudioSpec * orig, SDL_AudioSpec * prepared)
{
    SDL_memcpy(prepared, orig, sizeof(SDL_AudioSpec));

    if (orig->callback == NULL) {
        SDL_SetError("SDL_OpenAudio() passed a NULL callback");
        return 0;
    }

    if (orig->freq == 0) {
        const char *env = SDL_getenv("SDL_AUDIO_FREQUENCY");
        if ((!env) || ((prepared->freq = SDL_atoi(env)) == 0)) {
            prepared->freq = 22050;
        }
    }

    if (orig->format == 0) {
        const char *env = SDL_getenv("SDL_AUDIO_FORMAT");
        if ((!env) || ((prepared->format = SDL_ParseAudioFormat(env)) == 0)) {
            prepared->format = AUDIO_S16;
        }
    }

    switch (orig->channels) {
    case 0:{
            const char *env = SDL_getenv("SDL_AUDIO_CHANNELS");
            if ((!env) || ((prepared->channels = (Uint8) SDL_atoi(env)) == 0)) {
                prepared->channels = 2;
            }
            break;
        }
    case 1:
    case 2:
    case 4:
    case 6:
        break;
    default:
        SDL_SetError("Unsupported number of audio channels.");
        return 0;
    }

    if (orig->samples == 0) {
        const char *env = SDL_getenv("SDL_AUDIO_SAMPLES");
        if ((!env) || ((prepared->samples = (Uint16) SDL_atoi(env)) == 0)) {

            const int samples = (prepared->freq / 1000) * 46;
            int power2 = 1;
            while (power2 < samples) {
                power2 *= 2;
            }
            prepared->samples = power2;
        }
    }

    SDL_CalculateAudioSpec(prepared);

    return 1;
}

static SDL_AudioDeviceID
open_audio_device(const char *devname, int iscapture,
                  const SDL_AudioSpec * desired, SDL_AudioSpec * obtained,
                  int allowed_changes, int min_id)
{
    SDL_AudioDeviceID id = 0;
    SDL_AudioSpec _obtained;
    SDL_AudioDevice *device;
    SDL_bool build_cvt;
    int i = 0;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_SetError("Audio subsystem is not initialized");
        return 0;
    }

    if ((iscapture) && (!current_audio.impl.HasCaptureSupport)) {
        SDL_SetError("No capture support");
        return 0;
    }

    if (!obtained) {
        obtained = &_obtained;
    }
    if (!prepare_audiospec(desired, obtained)) {
        return 0;
    }

    if (devname == NULL) {
        devname = SDL_getenv("SDL_AUDIO_DEVICE_NAME");
    }

    if ((iscapture) && (current_audio.impl.OnlyHasDefaultInputDevice)) {
        if ((devname) && (SDL_strcmp(devname, DEFAULT_INPUT_DEVNAME) != 0)) {
            SDL_SetError("No such device");
            return 0;
        }
        devname = NULL;

        for (i = 0; i < SDL_arraysize(open_devices); i++) {
            if ((open_devices[i]) && (open_devices[i]->iscapture)) {
                SDL_SetError("Audio device already open");
                return 0;
            }
        }
    }

    if ((!iscapture) && (current_audio.impl.OnlyHasDefaultOutputDevice)) {
        if ((devname) && (SDL_strcmp(devname, DEFAULT_OUTPUT_DEVNAME) != 0)) {
            SDL_SetError("No such device");
            return 0;
        }
        devname = NULL;

        for (i = 0; i < SDL_arraysize(open_devices); i++) {
            if ((open_devices[i]) && (!open_devices[i]->iscapture)) {
                SDL_SetError("Audio device already open");
                return 0;
            }
        }
    }

    device = (SDL_AudioDevice *) SDL_AllocAudioMem(sizeof(SDL_AudioDevice));
    if (device == NULL) {
        SDL_OutOfMemory();
        return 0;
    }
    SDL_memset(device, '\0', sizeof(SDL_AudioDevice));
    device->spec = *obtained;
    device->enabled = 1;
    device->paused = 1;
    device->iscapture = iscapture;

    if (!current_audio.impl.SkipMixerLock) {
        device->mixer_lock = SDL_CreateMutex();
        if (device->mixer_lock == NULL) {
            close_audio_device(device);
            SDL_SetError("Couldn't create mixer lock");
            return 0;
        }
    }

    if ( ((iscapture) && (current_audio.inputDevices == NULL)) ||
         ((!iscapture) && (current_audio.outputDevices == NULL)) )
        SDL_GetNumAudioDevices(iscapture);

    if (current_audio.impl.OpenDevice(device, devname, iscapture) < 0) {
        close_audio_device(device);
        return 0;
    }
    device->opened = 1;

    device->fake_stream = (Uint8 *)SDL_AllocAudioMem(device->spec.size);
    if (device->fake_stream == NULL) {
        close_audio_device(device);
        SDL_OutOfMemory();
        return 0;
    }

    build_cvt = SDL_FALSE;
    if (obtained->freq != device->spec.freq) {
        if (allowed_changes & SDL_AUDIO_ALLOW_FREQUENCY_CHANGE) {
            obtained->freq = device->spec.freq;
        } else {
            build_cvt = SDL_TRUE;
        }
    }
    if (obtained->format != device->spec.format) {
        if (allowed_changes & SDL_AUDIO_ALLOW_FORMAT_CHANGE) {
            obtained->format = device->spec.format;
        } else {
            build_cvt = SDL_TRUE;
        }
    }
    if (obtained->channels != device->spec.channels) {
        if (allowed_changes & SDL_AUDIO_ALLOW_CHANNELS_CHANGE) {
            obtained->channels = device->spec.channels;
        } else {
            build_cvt = SDL_TRUE;
        }
    }

    if (device->spec.samples != obtained->samples) {
        obtained->samples = device->spec.samples;
        SDL_CalculateAudioSpec(obtained);
    }

    if (build_cvt) {

        if (SDL_BuildAudioCVT(&device->convert,
                              obtained->format, obtained->channels,
                              obtained->freq,
                              device->spec.format, device->spec.channels,
                              device->spec.freq) < 0) {
            close_audio_device(device);
            return 0;
        }
        if (device->convert.needed) {
            device->convert.len = (int) (((double) device->spec.size) /
                                         device->convert.len_ratio);

            device->convert.buf =
                (Uint8 *) SDL_AllocAudioMem(device->convert.len *
                                            device->convert.len_mult);
            if (device->convert.buf == NULL) {
                close_audio_device(device);
                SDL_OutOfMemory();
                return 0;
            }
        }
    }

    for (id = min_id - 1; id < SDL_arraysize(open_devices); id++) {
        if (open_devices[id] == NULL) {
            open_devices[id] = device;
            break;
        }
    }

    if (id == SDL_arraysize(open_devices)) {
        SDL_SetError("Too many open audio devices");
        close_audio_device(device);
        return 0;
    }

    if (!current_audio.impl.ProvidesOwnCallbackThread) {

        char name[64];
        SDL_snprintf(name, sizeof (name), "SDLAudioDev%d", (int) (id + 1));

#if defined(__WIN32__) && !defined(HAVE_LIBC)
#undef SDL_CreateThread
#if SDL_DYNAMIC_API
        device->thread = SDL_CreateThread_REAL(SDL_RunAudio, name, device, NULL, NULL);
#else
        device->thread = SDL_CreateThread(SDL_RunAudio, name, device, NULL, NULL);
#endif
#else
        device->thread = SDL_CreateThread(SDL_RunAudio, name, device);
#endif
        if (device->thread == NULL) {
            SDL_CloseAudioDevice(id + 1);
            SDL_SetError("Couldn't create audio thread");
            return 0;
        }
    }

    return id + 1;
}

int
SDL_OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained)
{
    SDL_AudioDeviceID id = 0;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            return (-1);
        }
    }

    if (open_devices[0] != NULL) {
        SDL_SetError("Audio device is already opened");
        return (-1);
    }

    if (obtained) {
        id = open_audio_device(NULL, 0, desired, obtained,
                               SDL_AUDIO_ALLOW_ANY_CHANGE, 1);
    } else {
        id = open_audio_device(NULL, 0, desired, desired, 0, 1);
    }

    SDL_assert((id == 0) || (id == 1));
    return ((id == 0) ? -1 : 0);
}

SDL_AudioDeviceID
SDL_OpenAudioDevice(const char *device, int iscapture,
                    const SDL_AudioSpec * desired, SDL_AudioSpec * obtained,
                    int allowed_changes)
{
    return open_audio_device(device, iscapture, desired, obtained,
                             allowed_changes, 2);
}

SDL_AudioStatus
SDL_GetAudioDeviceStatus(SDL_AudioDeviceID devid)
{
    SDL_AudioDevice *device = get_audio_device(devid);
    SDL_AudioStatus status = SDL_AUDIO_STOPPED;
    if (device && device->enabled) {
        if (device->paused) {
            status = SDL_AUDIO_PAUSED;
        } else {
            status = SDL_AUDIO_PLAYING;
        }
    }
    return (status);
}

SDL_AudioStatus
SDL_GetAudioStatus(void)
{
    return SDL_GetAudioDeviceStatus(1);
}

void
SDL_PauseAudioDevice(SDL_AudioDeviceID devid, int pause_on)
{
    SDL_AudioDevice *device = get_audio_device(devid);
    if (device) {
        current_audio.impl.LockDevice(device);
        device->paused = pause_on;
        current_audio.impl.UnlockDevice(device);
    }
}

void
SDL_PauseAudio(int pause_on)
{
    SDL_PauseAudioDevice(1, pause_on);
}

void
SDL_LockAudioDevice(SDL_AudioDeviceID devid)
{

    SDL_AudioDevice *device = get_audio_device(devid);
    if (device) {
        current_audio.impl.LockDevice(device);
    }
}

void
SDL_LockAudio(void)
{
    SDL_LockAudioDevice(1);
}

void
SDL_UnlockAudioDevice(SDL_AudioDeviceID devid)
{

    SDL_AudioDevice *device = get_audio_device(devid);
    if (device) {
        current_audio.impl.UnlockDevice(device);
    }
}

void
SDL_UnlockAudio(void)
{
    SDL_UnlockAudioDevice(1);
}

void
SDL_CloseAudioDevice(SDL_AudioDeviceID devid)
{
    SDL_AudioDevice *device = get_audio_device(devid);
    if (device) {
        close_audio_device(device);
        open_devices[devid - 1] = NULL;
    }
}

void
SDL_CloseAudio(void)
{
    SDL_CloseAudioDevice(1);
}

void
SDL_AudioQuit(void)
{
    SDL_AudioDeviceID i;

    if (!current_audio.name) {
        return;
    }

    for (i = 0; i < SDL_arraysize(open_devices); i++) {
        if (open_devices[i] != NULL) {
            SDL_CloseAudioDevice(i+1);
        }
    }

    current_audio.impl.Deinitialize();
    free_device_list(&current_audio.outputDevices,
                     &current_audio.outputDeviceCount);
    free_device_list(&current_audio.inputDevices,
                     &current_audio.inputDeviceCount);
    SDL_memset(&current_audio, '\0', sizeof(current_audio));
    SDL_memset(open_devices, '\0', sizeof(open_devices));
}

#define NUM_FORMATS 10
static int format_idx;
static int format_idx_sub;
static SDL_AudioFormat format_list[NUM_FORMATS][NUM_FORMATS] = {
    {AUDIO_U8, AUDIO_S8, AUDIO_S16LSB, AUDIO_S16MSB, AUDIO_U16LSB,
     AUDIO_U16MSB, AUDIO_S32LSB, AUDIO_S32MSB, AUDIO_F32LSB, AUDIO_F32MSB},
    {AUDIO_S8, AUDIO_U8, AUDIO_S16LSB, AUDIO_S16MSB, AUDIO_U16LSB,
     AUDIO_U16MSB, AUDIO_S32LSB, AUDIO_S32MSB, AUDIO_F32LSB, AUDIO_F32MSB},
    {AUDIO_S16LSB, AUDIO_S16MSB, AUDIO_U16LSB, AUDIO_U16MSB, AUDIO_S32LSB,
     AUDIO_S32MSB, AUDIO_F32LSB, AUDIO_F32MSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_S16MSB, AUDIO_S16LSB, AUDIO_U16MSB, AUDIO_U16LSB, AUDIO_S32MSB,
     AUDIO_S32LSB, AUDIO_F32MSB, AUDIO_F32LSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_U16LSB, AUDIO_U16MSB, AUDIO_S16LSB, AUDIO_S16MSB, AUDIO_S32LSB,
     AUDIO_S32MSB, AUDIO_F32LSB, AUDIO_F32MSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_U16MSB, AUDIO_U16LSB, AUDIO_S16MSB, AUDIO_S16LSB, AUDIO_S32MSB,
     AUDIO_S32LSB, AUDIO_F32MSB, AUDIO_F32LSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_S32LSB, AUDIO_S32MSB, AUDIO_F32LSB, AUDIO_F32MSB, AUDIO_S16LSB,
     AUDIO_S16MSB, AUDIO_U16LSB, AUDIO_U16MSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_S32MSB, AUDIO_S32LSB, AUDIO_F32MSB, AUDIO_F32LSB, AUDIO_S16MSB,
     AUDIO_S16LSB, AUDIO_U16MSB, AUDIO_U16LSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_F32LSB, AUDIO_F32MSB, AUDIO_S32LSB, AUDIO_S32MSB, AUDIO_S16LSB,
     AUDIO_S16MSB, AUDIO_U16LSB, AUDIO_U16MSB, AUDIO_U8, AUDIO_S8},
    {AUDIO_F32MSB, AUDIO_F32LSB, AUDIO_S32MSB, AUDIO_S32LSB, AUDIO_S16MSB,
     AUDIO_S16LSB, AUDIO_U16MSB, AUDIO_U16LSB, AUDIO_U8, AUDIO_S8},
};

SDL_AudioFormat
SDL_FirstAudioFormat(SDL_AudioFormat format)
{
    for (format_idx = 0; format_idx < NUM_FORMATS; ++format_idx) {
        if (format_list[format_idx][0] == format) {
            break;
        }
    }
    format_idx_sub = 0;
    return (SDL_NextAudioFormat());
}

SDL_AudioFormat
SDL_NextAudioFormat(void)
{
    if ((format_idx == NUM_FORMATS) || (format_idx_sub == NUM_FORMATS)) {
        return (0);
    }
    return (format_list[format_idx][format_idx_sub++]);
}

void
SDL_CalculateAudioSpec(SDL_AudioSpec * spec)
{
    switch (spec->format) {
    case AUDIO_U8:
        spec->silence = 0x80;
        break;
    default:
        spec->silence = 0x00;
        break;
    }
    spec->size = SDL_AUDIO_BITSIZE(spec->format) / 8;
    spec->size *= spec->channels;
    spec->size *= spec->samples;
}

void
SDL_MixAudio(Uint8 * dst, const Uint8 * src, Uint32 len, int volume)
{

    SDL_AudioDevice *device = get_audio_device(1);
    if (device != NULL) {
        SDL_AudioFormat format;
        if (device->convert.needed) {
            format = device->convert.src_format;
        } else {
            format = device->spec.format;
        }
        SDL_MixAudioFormat(dst, src, format, len, volume);
    }
}
#if SDL_AUDIO_DRIVER_BSD || SDL_AUDIO_DRIVER_OSS || SDL_AUDIO_DRIVER_SUNAUDIO

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef USE_BLOCKING_WRITES
#define OPEN_FLAGS_OUTPUT O_WRONLY
#define OPEN_FLAGS_INPUT O_RDONLY
#else
#define OPEN_FLAGS_OUTPUT (O_WRONLY|O_NONBLOCK)
#define OPEN_FLAGS_INPUT (O_RDONLY|O_NONBLOCK)
#endif

void SDL_EnumUnixAudioDevices(int iscapture, int classic,
                              int (*test) (int fd), SDL_AddAudioDevice addfn);

#ifndef _PATH_DEV_DSP
#if defined(__NETBSD__) || defined(__OPENBSD__)
#define _PATH_DEV_DSP  "/dev/audio"
#else
#define _PATH_DEV_DSP  "/dev/dsp"
#endif
#endif
#ifndef _PATH_DEV_DSP24
#define _PATH_DEV_DSP24 "/dev/sound/dsp"
#endif
#ifndef _PATH_DEV_AUDIO
#define _PATH_DEV_AUDIO "/dev/audio"
#endif

static SDL_INLINE void
test_device(const char *fname, int flags, int (*test) (int fd),
            SDL_AddAudioDevice addfn)
{
    struct stat sb;
    if ((stat(fname, &sb) == 0) && (S_ISCHR(sb.st_mode))) {
        const int audio_fd = open(fname, flags, 0);
        if (audio_fd >= 0) {
            if (test(audio_fd)) {
                addfn(fname);
            }
            close(audio_fd);
        }
    }
}

static int
test_stub(int fd)
{
    return 1;
}

void
SDL_EnumUnixAudioDevices(int iscapture, int classic, int (*test)(int fd),
                         SDL_AddAudioDevice addfn)
{
    const int flags = ((iscapture) ? OPEN_FLAGS_INPUT : OPEN_FLAGS_OUTPUT);
    const char *audiodev;
    char audiopath[1024];

    if (test == NULL)
        test = test_stub;

    if (((audiodev = SDL_getenv("SDL_PATH_DSP")) == NULL) &&
        ((audiodev = SDL_getenv("AUDIODEV")) == NULL)) {
        if (classic) {
            audiodev = _PATH_DEV_AUDIO;
        } else {
            struct stat sb;

            if (((stat("/dev/sound", &sb) == 0) && S_ISDIR(sb.st_mode))
                && ((stat(_PATH_DEV_DSP24, &sb) == 0)
                    && S_ISCHR(sb.st_mode))) {
                audiodev = _PATH_DEV_DSP24;
            } else {
                audiodev = _PATH_DEV_DSP;
            }
        }
    }
    test_device(audiodev, flags, test, addfn);

    if (SDL_strlen(audiodev) < (sizeof(audiopath) - 3)) {
        int instance = 0;
        while (instance++ <= 64) {
            SDL_snprintf(audiopath, SDL_arraysize(audiopath),
                         "%s%d", audiodev, instance);
            test_device(audiopath, flags, test, addfn);
        }
    }
}

#endif
#if SDL_AUDIO_DRIVER_WINMM

#include <mmsystem.h>

#ifndef _SDL_winmm_h
#define _SDL_winmm_h

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#define NUM_BUFFERS 2

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataWINMM*)this->hidden)
struct SDL_PrivateAudioDataWINMM
{
    HWAVEOUT hout;
    HWAVEIN hin;
    HANDLE audio_sem;
    Uint8 *mixbuf;
    WAVEHDR wavebuf[NUM_BUFFERS];
    int next_buffer;
};

#endif

#ifndef WAVE_FORMAT_IEEE_FLOAT
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#endif

#define DETECT_DEV_IMPL(typ, capstyp) \
static void DetectWave##typ##Devs(SDL_AddAudioDevice addfn) { \
    const UINT devcount = wave##typ##GetNumDevs(); \
    capstyp caps; \
    UINT i; \
    for (i = 0; i < devcount; i++) { \
        if (wave##typ##GetDevCaps(i,&caps,sizeof(caps))==MMSYSERR_NOERROR) { \
            char *name = WIN_StringToUTF8(caps.szPname); \
            if (name != NULL) { \
                addfn(name); \
                SDL_free(name); \
            } \
        } \
    } \
}

DETECT_DEV_IMPL(Out, WAVEOUTCAPS)
DETECT_DEV_IMPL(In, WAVEINCAPS)

static void
WINMM_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    if (iscapture) {
        DetectWaveInDevs(addfn);
    } else {
        DetectWaveOutDevs(addfn);
    }
}

static void CALLBACK
CaptureSound(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance,
          DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    SDL_AudioDevice *this = (SDL_AudioDevice *) dwInstance;

    if (uMsg != WIM_DATA)
        return;

    ReleaseSemaphore(SDLAUDIOHIDDEN->audio_sem, 1, NULL);
}

static void CALLBACK
FillSound(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,
          DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    SDL_AudioDevice *this = (SDL_AudioDevice *) dwInstance;

    if (uMsg != WOM_DONE)
        return;

    ReleaseSemaphore(SDLAUDIOHIDDEN->audio_sem, 1, NULL);
}

static int
SetMMerror(char *function, MMRESULT code)
{
    int len;
    char errbuf[MAXERRORLENGTH];
    wchar_t werrbuf[MAXERRORLENGTH];

    SDL_snprintf(errbuf, SDL_arraysize(errbuf), "%s: ", function);
    len = SDL_static_cast(int, SDL_strlen(errbuf));

    waveOutGetErrorText(code, werrbuf, MAXERRORLENGTH - len);
    WideCharToMultiByte(CP_ACP, 0, werrbuf, -1, errbuf + len,
                        MAXERRORLENGTH - len, NULL, NULL);

    return SDL_SetError("%s", errbuf);
}

static void
WINMM_WaitDevice(_THIS)
{

    WaitForSingleObject(SDLAUDIOHIDDEN->audio_sem, INFINITE);
}

static Uint8 *
WINMM_GetDeviceBuf(_THIS)
{
    return (Uint8 *) (SDLAUDIOHIDDEN->
                      wavebuf[SDLAUDIOHIDDEN->next_buffer].lpData);
}

static void
WINMM_PlayDevice(_THIS)
{

    waveOutWrite(SDLAUDIOHIDDEN->hout,
                 &SDLAUDIOHIDDEN->wavebuf[SDLAUDIOHIDDEN->next_buffer],
                 sizeof(SDLAUDIOHIDDEN->wavebuf[0]));
    SDLAUDIOHIDDEN->next_buffer = (SDLAUDIOHIDDEN->next_buffer + 1) % NUM_BUFFERS;
}

static void
WINMM_WaitDone(_THIS)
{
    int i, left;

    do {
        left = NUM_BUFFERS;
        for (i = 0; i < NUM_BUFFERS; ++i) {
            if (SDLAUDIOHIDDEN->wavebuf[i].dwFlags & WHDR_DONE) {
                --left;
            }
        }
        if (left > 0) {
            SDL_Delay(100);
        }
    } while (left > 0);
}

static void
WINMM_CloseDevice(_THIS)
{

    if (SDLAUDIOHIDDEN != NULL) {
        int i;

        if (SDLAUDIOHIDDEN->audio_sem) {
            CloseHandle(SDLAUDIOHIDDEN->audio_sem);
            SDLAUDIOHIDDEN->audio_sem = 0;
        }

        for (i = 0; i < NUM_BUFFERS; ++i) {
            if (SDLAUDIOHIDDEN->wavebuf[i].dwUser != 0xFFFF) {
                waveOutUnprepareHeader(SDLAUDIOHIDDEN->hout,
                                       &SDLAUDIOHIDDEN->wavebuf[i],
                                       sizeof(SDLAUDIOHIDDEN->wavebuf[i]));
                SDLAUDIOHIDDEN->wavebuf[i].dwUser = 0xFFFF;
            }
        }

        SDL_free(SDLAUDIOHIDDEN->mixbuf);
        SDLAUDIOHIDDEN->mixbuf = NULL;

        if (SDLAUDIOHIDDEN->hin) {
            waveInClose(SDLAUDIOHIDDEN->hin);
            SDLAUDIOHIDDEN->hin = 0;
        }

        if (SDLAUDIOHIDDEN->hout) {
            waveOutClose(SDLAUDIOHIDDEN->hout);
            SDLAUDIOHIDDEN->hout = 0;
        }

        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static SDL_bool
PrepWaveFormat(_THIS, UINT devId, WAVEFORMATEX *pfmt, const int iscapture)
{
    SDL_zerop(pfmt);

    if (SDL_AUDIO_ISFLOAT(this->spec.format)) {
        pfmt->wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    } else {
        pfmt->wFormatTag = WAVE_FORMAT_PCM;
    }
    pfmt->wBitsPerSample = SDL_AUDIO_BITSIZE(this->spec.format);

    pfmt->nChannels = this->spec.channels;
    pfmt->nSamplesPerSec = this->spec.freq;
    pfmt->nBlockAlign = pfmt->nChannels * (pfmt->wBitsPerSample / 8);
    pfmt->nAvgBytesPerSec = pfmt->nSamplesPerSec * pfmt->nBlockAlign;

    if (iscapture) {
        return (waveInOpen(0, devId, pfmt, 0, 0, WAVE_FORMAT_QUERY) == 0);
    } else {
        return (waveOutOpen(0, devId, pfmt, 0, 0, WAVE_FORMAT_QUERY) == 0);
    }
}

static int
WINMM_OpenDevice(_THIS, const char *devname, int iscapture)
{
    SDL_AudioFormat test_format = SDL_FirstAudioFormat(this->spec.format);
    int valid_datatype = 0;
    MMRESULT result;
    WAVEFORMATEX waveformat;
    UINT devId = WAVE_MAPPER;
    char *utf8 = NULL;
    UINT i;

    if (devname != NULL) {
        if (iscapture) {
            const UINT devcount = waveInGetNumDevs();
            WAVEINCAPS caps;
            for (i = 0; (i < devcount) && (devId == WAVE_MAPPER); i++) {
                result = waveInGetDevCaps(i, &caps, sizeof (caps));
                if (result != MMSYSERR_NOERROR)
                    continue;
                else if ((utf8 = WIN_StringToUTF8(caps.szPname)) == NULL)
                    continue;
                else if (SDL_strcmp(devname, utf8) == 0)
                    devId = i;
                SDL_free(utf8);
            }
        } else {
            const UINT devcount = waveOutGetNumDevs();
            WAVEOUTCAPS caps;
            for (i = 0; (i < devcount) && (devId == WAVE_MAPPER); i++) {
                result = waveOutGetDevCaps(i, &caps, sizeof (caps));
                if (result != MMSYSERR_NOERROR)
                    continue;
                else if ((utf8 = WIN_StringToUTF8(caps.szPname)) == NULL)
                    continue;
                else if (SDL_strcmp(devname, utf8) == 0)
                    devId = i;
                SDL_free(utf8);
            }
        }

        if (devId == WAVE_MAPPER) {
            return SDL_SetError("Requested device not found");
        }
    }

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    for (i = 0; i < NUM_BUFFERS; ++i)
        SDLAUDIOHIDDEN->wavebuf[i].dwUser = 0xFFFF;

    if (this->spec.channels > 2)
        this->spec.channels = 2;

    if (this->spec.samples < (this->spec.freq / 4))
        this->spec.samples = ((this->spec.freq / 4) + 3) & ~3;

    while ((!valid_datatype) && (test_format)) {
        switch (test_format) {
        case AUDIO_U8:
        case AUDIO_S16:
        case AUDIO_S32:
        case AUDIO_F32:
            this->spec.format = test_format;
            if (PrepWaveFormat(this, devId, &waveformat, iscapture)) {
                valid_datatype = 1;
            } else {
                test_format = SDL_NextAudioFormat();
            }
            break;

        default:
            test_format = SDL_NextAudioFormat();
            break;
        }
    }

    if (!valid_datatype) {
        WINMM_CloseDevice(this);
        return SDL_SetError("Unsupported audio format");
    }

    SDL_CalculateAudioSpec(&this->spec);

    if (iscapture) {
        result = waveInOpen(&SDLAUDIOHIDDEN->hin, devId, &waveformat,
                             (DWORD_PTR) CaptureSound, (DWORD_PTR) this,
                             CALLBACK_FUNCTION);
    } else {
        result = waveOutOpen(&SDLAUDIOHIDDEN->hout, devId, &waveformat,
                             (DWORD_PTR) FillSound, (DWORD_PTR) this,
                             CALLBACK_FUNCTION);
    }

    if (result != MMSYSERR_NOERROR) {
        WINMM_CloseDevice(this);
        return SetMMerror("waveOutOpen()", result);
    }
#ifdef SOUND_DEBUG

    {
        WAVEOUTCAPS caps;

        result = waveOutGetDevCaps((UINT) SDLAUDIOHIDDEN->hout,
                                   &caps, sizeof(caps));
        if (result != MMSYSERR_NOERROR) {
            WINMM_CloseDevice(this);
            return SetMMerror("waveOutGetDevCaps()", result);
        }
        printf("Audio device: %s\n", caps.szPname);
    }
#endif

    SDLAUDIOHIDDEN->audio_sem =
        CreateSemaphore(NULL, NUM_BUFFERS - 1, NUM_BUFFERS, NULL);
    if (SDLAUDIOHIDDEN->audio_sem == NULL) {
        WINMM_CloseDevice(this);
        return SDL_SetError("Couldn't create semaphore");
    }

    SDLAUDIOHIDDEN->mixbuf =
        (Uint8 *) SDL_malloc(NUM_BUFFERS * this->spec.size);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        WINMM_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    for (i = 0; i < NUM_BUFFERS; ++i) {
        SDL_memset(&SDLAUDIOHIDDEN->wavebuf[i], 0,
                   sizeof(SDLAUDIOHIDDEN->wavebuf[i]));
        SDLAUDIOHIDDEN->wavebuf[i].dwBufferLength = this->spec.size;
        SDLAUDIOHIDDEN->wavebuf[i].dwFlags = WHDR_DONE;
        SDLAUDIOHIDDEN->wavebuf[i].lpData =
            (LPSTR) & SDLAUDIOHIDDEN->mixbuf[i * this->spec.size];
        result = waveOutPrepareHeader(SDLAUDIOHIDDEN->hout,
                                      &SDLAUDIOHIDDEN->wavebuf[i],
                                      sizeof(SDLAUDIOHIDDEN->wavebuf[i]));
        if (result != MMSYSERR_NOERROR) {
            WINMM_CloseDevice(this);
            return SetMMerror("waveOutPrepareHeader()", result);
        }
    }

    return 0;
}

static int
WINMM_Init(SDL_AudioDriverImpl * impl)
{

    impl->DetectDevices = WINMM_DetectDevices;
    impl->OpenDevice = WINMM_OpenDevice;
    impl->PlayDevice = WINMM_PlayDevice;
    impl->WaitDevice = WINMM_WaitDevice;
    impl->WaitDone = WINMM_WaitDone;
    impl->GetDeviceBuf = WINMM_GetDeviceBuf;
    impl->CloseDevice = WINMM_CloseDevice;

    return 1;
}

AudioBootStrap WINMM_bootstrap = {
    "winmm", "Windows Waveform Audio", WINMM_Init, 0
};

#endif
#if SDL_AUDIO_DRIVER_XAUDIO2

#ifdef __GNUC__

#  define SDL_XAUDIO2_HAS_SDK 1
#elif defined(__WINRT__)

#  define SDL_XAUDIO2_HAS_SDK
#else

#include <dxsdkver.h>
#if (!defined(_DXSDK_BUILD_MAJOR) || (_DXSDK_BUILD_MAJOR < 1284))
#  pragma message("Your DirectX SDK is too old. Disabling XAudio2 support.")
#else
#  define SDL_XAUDIO2_HAS_SDK 1
#endif
#endif

#ifdef SDL_XAUDIO2_HAS_SDK

#ifdef WINVER
#if WINVER >= 0x0602
#define SDL_XAUDIO2_WIN8 1
#endif
#endif

#ifdef __WINRT__
#define uuid(x)
#define DX_BUILD
#endif

#define INITGUID 1
#include <xaudio2.h>

#undef _THIS
#define _THIS   SDL_AudioDevice *this

#ifdef __WINRT__
#include "SDL_xaudio2_winrthelpers.h"
#endif

#ifdef __GNUC__
#ifdef THIS
#undef THIS
#endif
#define THIS    INTERFACE *p
#ifdef THIS_
#undef THIS_
#endif
#define THIS_   INTERFACE *p,
#endif

#undef SDLAUDIOHIDDEN
#define SDLAUDIOHIDDEN ((struct SDL_PrivateAudioDataXAUDIO2*)this->hidden)
struct SDL_PrivateAudioDataXAUDIO2
{
    IXAudio2 *ixa2;
    IXAudio2SourceVoice *source;
    IXAudio2MasteringVoice *mastering;
    SDL_sem * semaphore;
    Uint8 *mixbuf;
    int mixlen;
    Uint8 *nextbuf;
};

static void
XAUDIO2_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
    IXAudio2 *ixa2 = NULL;
    UINT32 devcount = 0;
    UINT32 i = 0;

    if (iscapture) {
        SDL_SetError("XAudio2: capture devices unsupported.");
        return;
    } else if (XAudio2Create(&ixa2, 0, XAUDIO2_DEFAULT_PROCESSOR) != S_OK) {
        SDL_SetError("XAudio2: XAudio2Create() failed at detection.");
        return;
    } else if (IXAudio2_GetDeviceCount(ixa2, &devcount) != S_OK) {
        SDL_SetError("XAudio2: IXAudio2::GetDeviceCount() failed.");
        IXAudio2_Release(ixa2);
        return;
    }

    for (i = 0; i < devcount; i++) {
        XAUDIO2_DEVICE_DETAILS details;
        if (IXAudio2_GetDeviceDetails(ixa2, i, &details) == S_OK) {
            char *str = WIN_StringToUTF8(details.DisplayName);
            if (str != NULL) {
                addfn(str);
                SDL_free(str);
            }
        }
    }

    IXAudio2_Release(ixa2);
}

static void STDMETHODCALLTYPE
VoiceCBOnBufferEnd(THIS_ void *data)
{

    SDL_AudioDevice *this = (SDL_AudioDevice *) data;
    SDL_SemPost(SDLAUDIOHIDDEN->semaphore);
}

static void STDMETHODCALLTYPE
VoiceCBOnVoiceError(THIS_ void *data, HRESULT Error)
{

    SDL_assert(0 && "write me!");
}

static void STDMETHODCALLTYPE VoiceCBOnStreamEnd(THIS) {}
static void STDMETHODCALLTYPE VoiceCBOnVoiceProcessPassStart(THIS_ UINT32 b) {}
static void STDMETHODCALLTYPE VoiceCBOnVoiceProcessPassEnd(THIS) {}
static void STDMETHODCALLTYPE VoiceCBOnBufferStart(THIS_ void *data) {}
static void STDMETHODCALLTYPE VoiceCBOnLoopEnd(THIS_ void *data) {}

static Uint8 *
XAUDIO2_GetDeviceBuf(_THIS)
{
    return SDLAUDIOHIDDEN->nextbuf;
}

static void
XAUDIO2_PlayDevice(_THIS)
{
    XAUDIO2_BUFFER buffer;
    Uint8 *mixbuf = SDLAUDIOHIDDEN->mixbuf;
    Uint8 *nextbuf = SDLAUDIOHIDDEN->nextbuf;
    const int mixlen = SDLAUDIOHIDDEN->mixlen;
    IXAudio2SourceVoice *source = SDLAUDIOHIDDEN->source;
    HRESULT result = S_OK;

    if (!this->enabled) {
        return;
    }

    SDL_zero(buffer);
    buffer.AudioBytes = mixlen;
    buffer.pAudioData = nextbuf;
    buffer.pContext = this;

    if (nextbuf == mixbuf) {
        nextbuf += mixlen;
    } else {
        nextbuf = mixbuf;
    }
    SDLAUDIOHIDDEN->nextbuf = nextbuf;

    result = IXAudio2SourceVoice_SubmitSourceBuffer(source, &buffer, NULL);
    if (result == XAUDIO2_E_DEVICE_INVALIDATED) {

    }

    if (result != S_OK) {
        IXAudio2SourceVoice_FlushSourceBuffers(source);
        this->enabled = 0;
    }
}

static void
XAUDIO2_WaitDevice(_THIS)
{
    if (this->enabled) {
        SDL_SemWait(SDLAUDIOHIDDEN->semaphore);
    }
}

static void
XAUDIO2_WaitDone(_THIS)
{
    IXAudio2SourceVoice *source = SDLAUDIOHIDDEN->source;
    XAUDIO2_VOICE_STATE state;
    SDL_assert(!this->enabled);
    IXAudio2SourceVoice_Discontinuity(source);
#if SDL_XAUDIO2_WIN8
    IXAudio2SourceVoice_GetState(source, &state, 0);
#else
    IXAudio2SourceVoice_GetState(source, &state);
#endif
    while (state.BuffersQueued > 0) {
        SDL_SemWait(SDLAUDIOHIDDEN->semaphore);
#if SDL_XAUDIO2_WIN8
        IXAudio2SourceVoice_GetState(source, &state, 0);
#else
        IXAudio2SourceVoice_GetState(source, &state);
#endif
    }
}

static void
XAUDIO2_CloseDevice(_THIS)
{
    if (SDLAUDIOHIDDEN != NULL) {
        IXAudio2 *ixa2 = SDLAUDIOHIDDEN->ixa2;
        IXAudio2SourceVoice *source = SDLAUDIOHIDDEN->source;
        IXAudio2MasteringVoice *mastering = SDLAUDIOHIDDEN->mastering;

        if (source != NULL) {
            IXAudio2SourceVoice_Stop(source, 0, XAUDIO2_COMMIT_NOW);
            IXAudio2SourceVoice_FlushSourceBuffers(source);
            IXAudio2SourceVoice_DestroyVoice(source);
        }
        if (ixa2 != NULL) {
            IXAudio2_StopEngine(ixa2);
        }
        if (mastering != NULL) {
            IXAudio2MasteringVoice_DestroyVoice(mastering);
        }
        if (ixa2 != NULL) {
            IXAudio2_Release(ixa2);
        }
        SDL_free(SDLAUDIOHIDDEN->mixbuf);
        if (SDLAUDIOHIDDEN->semaphore != NULL) {
            SDL_DestroySemaphore(SDLAUDIOHIDDEN->semaphore);
        }

        SDL_free(SDLAUDIOHIDDEN);
        this->hidden = NULL;
    }
}

static int
XAUDIO2_OpenDevice(_THIS, const char *devname, int iscapture)
{
    HRESULT result = S_OK;
    WAVEFORMATEX waveformat;
    int valid_format = 0;
    SDL_AudioFormat test_format = SDL_FirstAudioFormat(this->spec.format);
    IXAudio2 *ixa2 = NULL;
    IXAudio2SourceVoice *source = NULL;
#if defined(SDL_XAUDIO2_WIN8)
    LPCWSTR devId = NULL;
#else
    UINT32 devId = 0;
#endif

    static IXAudio2VoiceCallbackVtbl callbacks_vtable = {
        VoiceCBOnVoiceProcessPassStart,
        VoiceCBOnVoiceProcessPassEnd,
        VoiceCBOnStreamEnd,
        VoiceCBOnBufferStart,
        VoiceCBOnBufferEnd,
        VoiceCBOnLoopEnd,
        VoiceCBOnVoiceError
    };

    static IXAudio2VoiceCallback callbacks = { &callbacks_vtable };

    if (iscapture) {
        return SDL_SetError("XAudio2: capture devices unsupported.");
    } else if (XAudio2Create(&ixa2, 0, XAUDIO2_DEFAULT_PROCESSOR) != S_OK) {
        return SDL_SetError("XAudio2: XAudio2Create() failed at open.");
    }

#if ! defined(__WINRT__)
    if (devname != NULL) {
        UINT32 devcount = 0;
        UINT32 i = 0;

        if (IXAudio2_GetDeviceCount(ixa2, &devcount) != S_OK) {
            IXAudio2_Release(ixa2);
            return SDL_SetError("XAudio2: IXAudio2_GetDeviceCount() failed.");
        }
        for (i = 0; i < devcount; i++) {
            XAUDIO2_DEVICE_DETAILS details;
            if (IXAudio2_GetDeviceDetails(ixa2, i, &details) == S_OK) {
                char *str = WIN_StringToUTF8(details.DisplayName);
                if (str != NULL) {
                    const int match = (SDL_strcmp(str, devname) == 0);
                    SDL_free(str);
                    if (match) {
                        devId = i;
                        break;
                    }
                }
            }
        }

        if (i == devcount) {
            IXAudio2_Release(ixa2);
            return SDL_SetError("XAudio2: Requested device not found.");
        }
    }
#endif

    this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *SDLAUDIOHIDDEN));
    if (SDLAUDIOHIDDEN == NULL) {
        IXAudio2_Release(ixa2);
        return SDL_OutOfMemory();
    }
    SDL_memset(SDLAUDIOHIDDEN, 0, (sizeof *SDLAUDIOHIDDEN));

    SDLAUDIOHIDDEN->ixa2 = ixa2;
    SDLAUDIOHIDDEN->semaphore = SDL_CreateSemaphore(1);
    if (SDLAUDIOHIDDEN->semaphore == NULL) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: CreateSemaphore() failed!");
    }

    while ((!valid_format) && (test_format)) {
        switch (test_format) {
        case AUDIO_U8:
        case AUDIO_S16:
        case AUDIO_S32:
        case AUDIO_F32:
            this->spec.format = test_format;
            valid_format = 1;
            break;
        }
        test_format = SDL_NextAudioFormat();
    }

    if (!valid_format) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: Unsupported audio format");
    }

    SDL_CalculateAudioSpec(&this->spec);

    SDLAUDIOHIDDEN->mixlen = this->spec.size;
    SDLAUDIOHIDDEN->mixbuf = (Uint8 *) SDL_malloc(2 * SDLAUDIOHIDDEN->mixlen);
    if (SDLAUDIOHIDDEN->mixbuf == NULL) {
        XAUDIO2_CloseDevice(this);
        return SDL_OutOfMemory();
    }
    SDLAUDIOHIDDEN->nextbuf = SDLAUDIOHIDDEN->mixbuf;
    SDL_memset(SDLAUDIOHIDDEN->mixbuf, 0, 2 * SDLAUDIOHIDDEN->mixlen);

#if SDL_XAUDIO2_WIN8
    result = IXAudio2_CreateMasteringVoice(ixa2, &SDLAUDIOHIDDEN->mastering,
                                           XAUDIO2_DEFAULT_CHANNELS,
                                           this->spec.freq, 0, devId, NULL, AudioCategory_GameEffects);
#else
    result = IXAudio2_CreateMasteringVoice(ixa2, &SDLAUDIOHIDDEN->mastering,
                                           XAUDIO2_DEFAULT_CHANNELS,
                                           this->spec.freq, 0, devId, NULL);
#endif
    if (result != S_OK) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: Couldn't create mastering voice");
    }

    SDL_zero(waveformat);
    if (SDL_AUDIO_ISFLOAT(this->spec.format)) {
        waveformat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    } else {
        waveformat.wFormatTag = WAVE_FORMAT_PCM;
    }
    waveformat.wBitsPerSample = SDL_AUDIO_BITSIZE(this->spec.format);
    waveformat.nChannels = this->spec.channels;
    waveformat.nSamplesPerSec = this->spec.freq;
    waveformat.nBlockAlign =
        waveformat.nChannels * (waveformat.wBitsPerSample / 8);
    waveformat.nAvgBytesPerSec =
        waveformat.nSamplesPerSec * waveformat.nBlockAlign;
    waveformat.cbSize = sizeof(waveformat);

#ifdef __WINRT__

    result = IXAudio2_CreateSourceVoice(ixa2, &source, &waveformat,
                                        0,
                                        1.0f, &callbacks, NULL, NULL);
#else
    result = IXAudio2_CreateSourceVoice(ixa2, &source, &waveformat,
                                        XAUDIO2_VOICE_NOSRC |
                                        XAUDIO2_VOICE_NOPITCH,
                                        1.0f, &callbacks, NULL, NULL);

#endif
    if (result != S_OK) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: Couldn't create source voice");
    }
    SDLAUDIOHIDDEN->source = source;

    result = IXAudio2_StartEngine(ixa2);
    if (result != S_OK) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: Couldn't start engine");
    }

    result = IXAudio2SourceVoice_Start(source, 0, XAUDIO2_COMMIT_NOW);
    if (result != S_OK) {
        XAUDIO2_CloseDevice(this);
        return SDL_SetError("XAudio2: Couldn't start source voice");
    }

    return 0;
}

static void
XAUDIO2_Deinitialize(void)
{
#if defined(__WIN32__)
    WIN_CoUninitialize();
#endif
}

#endif

static int
XAUDIO2_Init(SDL_AudioDriverImpl * impl)
{
#ifndef SDL_XAUDIO2_HAS_SDK
    SDL_SetError("XAudio2: SDL was built without XAudio2 support (old DirectX SDK).");
    return 0;
#else

    IXAudio2 *ixa2 = NULL;
#if defined(__WIN32__)

    if (FAILED(WIN_CoInitialize())) {
        SDL_SetError("XAudio2: CoInitialize() failed");
        return 0;
    }
#endif

    if (XAudio2Create(&ixa2, 0, XAUDIO2_DEFAULT_PROCESSOR) != S_OK) {
#if defined(__WIN32__)
        WIN_CoUninitialize();
#endif
        SDL_SetError("XAudio2: XAudio2Create() failed at initialization");
        return 0;
    }
    IXAudio2_Release(ixa2);

    impl->DetectDevices = XAUDIO2_DetectDevices;
    impl->OpenDevice = XAUDIO2_OpenDevice;
    impl->PlayDevice = XAUDIO2_PlayDevice;
    impl->WaitDevice = XAUDIO2_WaitDevice;
    impl->WaitDone = XAUDIO2_WaitDone;
    impl->GetDeviceBuf = XAUDIO2_GetDeviceBuf;
    impl->CloseDevice = XAUDIO2_CloseDevice;
    impl->Deinitialize = XAUDIO2_Deinitialize;

    return 1;
#endif
}

AudioBootStrap XAUDIO2_bootstrap = {
    "xaudio2", "XAudio2", XAUDIO2_Init, 0
};

#endif
#if !SDL_THREAD_PTHREAD

struct SDL_cond
{
    SDL_mutex *lock;
    int waiting;
    int signals;
    SDL_sem *wait_sem;
    SDL_sem *wait_done;
};

SDL_cond *
SDL_CreateCond(void)
{
    SDL_cond *cond;

    cond = (SDL_cond *) SDL_malloc(sizeof(SDL_cond));
    if (cond) {
        cond->lock = SDL_CreateMutex();
        cond->wait_sem = SDL_CreateSemaphore(0);
        cond->wait_done = SDL_CreateSemaphore(0);
        cond->waiting = cond->signals = 0;
        if (!cond->lock || !cond->wait_sem || !cond->wait_done) {
            SDL_DestroyCond(cond);
            cond = NULL;
        }
    } else {
        SDL_OutOfMemory();
    }
    return (cond);
}

void
SDL_DestroyCond(SDL_cond * cond)
{
    if (cond) {
        if (cond->wait_sem) {
            SDL_DestroySemaphore(cond->wait_sem);
        }
        if (cond->wait_done) {
            SDL_DestroySemaphore(cond->wait_done);
        }
        if (cond->lock) {
            SDL_DestroyMutex(cond->lock);
        }
        SDL_free(cond);
    }
}

int
SDL_CondSignal(SDL_cond * cond)
{
    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    SDL_LockMutex(cond->lock);
    if (cond->waiting > cond->signals) {
        ++cond->signals;
        SDL_SemPost(cond->wait_sem);
        SDL_UnlockMutex(cond->lock);
        SDL_SemWait(cond->wait_done);
    } else {
        SDL_UnlockMutex(cond->lock);
    }

    return 0;
}

int
SDL_CondBroadcast(SDL_cond * cond)
{
    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    SDL_LockMutex(cond->lock);
    if (cond->waiting > cond->signals) {
        int i, num_waiting;

        num_waiting = (cond->waiting - cond->signals);
        cond->signals = cond->waiting;
        for (i = 0; i < num_waiting; ++i) {
            SDL_SemPost(cond->wait_sem);
        }

        SDL_UnlockMutex(cond->lock);
        for (i = 0; i < num_waiting; ++i) {
            SDL_SemWait(cond->wait_done);
        }
    } else {
        SDL_UnlockMutex(cond->lock);
    }

    return 0;
}

int
SDL_CondWaitTimeout(SDL_cond * cond, SDL_mutex * mutex, Uint32 ms)
{
    int retval;

    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    SDL_LockMutex(cond->lock);
    ++cond->waiting;
    SDL_UnlockMutex(cond->lock);

    SDL_UnlockMutex(mutex);

    if (ms == SDL_MUTEX_MAXWAIT) {
        retval = SDL_SemWait(cond->wait_sem);
    } else {
        retval = SDL_SemWaitTimeout(cond->wait_sem, ms);
    }

    SDL_LockMutex(cond->lock);
    if (cond->signals > 0) {

        if (retval > 0) {
            SDL_SemWait(cond->wait_sem);
        }

        SDL_SemPost(cond->wait_done);

        --cond->signals;
    }
    --cond->waiting;
    SDL_UnlockMutex(cond->lock);

    SDL_LockMutex(mutex);

    return retval;
}

int
SDL_CondWait(SDL_cond * cond, SDL_mutex * mutex)
{
    return SDL_CondWaitTimeout(cond, mutex, SDL_MUTEX_MAXWAIT);
}

#endif
#if !SDL_THREAD_PTHREAD || defined(__MACOSX__) || defined(__IPHONEOS__)
#if !SDL_THREAD_WINDOWS

#if SDL_THREADS_DISABLED

SDL_sem *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_SetError("SDL not built with thread support");
    return (SDL_sem *) 0;
}

void
SDL_DestroySemaphore(SDL_sem * sem)
{
}

int
SDL_SemTryWait(SDL_sem * sem)
{
    return SDL_SetError("SDL not built with thread support");
}

int
SDL_SemWaitTimeout(SDL_sem * sem, Uint32 timeout)
{
    return SDL_SetError("SDL not built with thread support");
}

int
SDL_SemWait(SDL_sem * sem)
{
    return SDL_SetError("SDL not built with thread support");
}

Uint32
SDL_SemValue(SDL_sem * sem)
{
    return 0;
}

int
SDL_SemPost(SDL_sem * sem)
{
    return SDL_SetError("SDL not built with thread support");
}

#else

struct SDL_semaphore
{
    Uint32 count;
    Uint32 waiters_count;
    SDL_mutex *count_lock;
    SDL_cond *count_nonzero;
};

SDL_sem *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_sem *sem;

    sem = (SDL_sem *) SDL_malloc(sizeof(*sem));
    if (!sem) {
        SDL_OutOfMemory();
        return NULL;
    }
    sem->count = initial_value;
    sem->waiters_count = 0;

    sem->count_lock = SDL_CreateMutex();
    sem->count_nonzero = SDL_CreateCond();
    if (!sem->count_lock || !sem->count_nonzero) {
        SDL_DestroySemaphore(sem);
        return NULL;
    }

    return sem;
}

void
SDL_DestroySemaphore(SDL_sem * sem)
{
    if (sem) {
        sem->count = 0xFFFFFFFF;
        while (sem->waiters_count > 0) {
            SDL_CondSignal(sem->count_nonzero);
            SDL_Delay(10);
        }
        SDL_DestroyCond(sem->count_nonzero);
        if (sem->count_lock) {
            SDL_LockMutex(sem->count_lock);
            SDL_UnlockMutex(sem->count_lock);
            SDL_DestroyMutex(sem->count_lock);
        }
        SDL_free(sem);
    }
}

int
SDL_SemTryWait(SDL_sem * sem)
{
    int retval;

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    retval = SDL_MUTEX_TIMEDOUT;
    SDL_LockMutex(sem->count_lock);
    if (sem->count > 0) {
        --sem->count;
        retval = 0;
    }
    SDL_UnlockMutex(sem->count_lock);

    return retval;
}

int
SDL_SemWaitTimeout(SDL_sem * sem, Uint32 timeout)
{
    int retval;

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    if (timeout == 0) {
        return SDL_SemTryWait(sem);
    }

    SDL_LockMutex(sem->count_lock);
    ++sem->waiters_count;
    retval = 0;
    while ((sem->count == 0) && (retval != SDL_MUTEX_TIMEDOUT)) {
        retval = SDL_CondWaitTimeout(sem->count_nonzero,
                                     sem->count_lock, timeout);
    }
    --sem->waiters_count;
    if (retval == 0) {
        --sem->count;
    }
    SDL_UnlockMutex(sem->count_lock);

    return retval;
}

int
SDL_SemWait(SDL_sem * sem)
{
    return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

Uint32
SDL_SemValue(SDL_sem * sem)
{
    Uint32 value;

    value = 0;
    if (sem) {
        SDL_LockMutex(sem->count_lock);
        value = sem->count;
        SDL_UnlockMutex(sem->count_lock);
    }
    return value;
}

int
SDL_SemPost(SDL_sem * sem)
{
    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    SDL_LockMutex(sem->count_lock);
    if (sem->waiters_count > 0) {
        SDL_CondSignal(sem->count_nonzero);
    }
    ++sem->count;
    SDL_UnlockMutex(sem->count_lock);

    return 0;
}

#endif

#endif
#endif
#if SDL_THREAD_PTHREAD

#define _GNU_SOURCE
#include <pthread.h>
#include <errno.h>

#if !SDL_THREAD_PTHREAD_RECURSIVE_MUTEX && \
    !SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
#define FAKE_RECURSIVE_MUTEX 1
#endif

struct SDL_mutex
{
    pthread_mutex_t id;
#if FAKE_RECURSIVE_MUTEX
    int recursive;
    pthread_t owner;
#endif
};

SDL_mutex *
SDL_CreateMutex(void)
{
    SDL_mutex *mutex;
    pthread_mutexattr_t attr;

    mutex = (SDL_mutex *) SDL_calloc(1, sizeof(*mutex));
    if (mutex) {
        pthread_mutexattr_init(&attr);
#if SDL_THREAD_PTHREAD_RECURSIVE_MUTEX
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#elif SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
        pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else

#endif
        if (pthread_mutex_init(&mutex->id, &attr) != 0) {
            SDL_SetError("pthread_mutex_init() failed");
            SDL_free(mutex);
            mutex = NULL;
        }
    } else {
        SDL_OutOfMemory();
    }
    return (mutex);
}

void
SDL_DestroyMutex(SDL_mutex * mutex)
{
    if (mutex) {
        pthread_mutex_destroy(&mutex->id);
        SDL_free(mutex);
    }
}

int
SDL_LockMutex(SDL_mutex * mutex)
{
#if FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif

    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

#if FAKE_RECURSIVE_MUTEX
    this_thread = pthread_self();
    if (mutex->owner == this_thread) {
        ++mutex->recursive;
    } else {

        if (pthread_mutex_lock(&mutex->id) == 0) {
            mutex->owner = this_thread;
            mutex->recursive = 0;
        } else {
            return SDL_SetError("pthread_mutex_lock() failed");
        }
    }
#else
    if (pthread_mutex_lock(&mutex->id) < 0) {
        return SDL_SetError("pthread_mutex_lock() failed");
    }
#endif
    return 0;
}

int
SDL_TryLockMutex(SDL_mutex * mutex)
{
    int retval;
#if FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif

    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    retval = 0;
#if FAKE_RECURSIVE_MUTEX
    this_thread = pthread_self();
    if (mutex->owner == this_thread) {
        ++mutex->recursive;
    } else {

        if (pthread_mutex_lock(&mutex->id) == 0) {
            mutex->owner = this_thread;
            mutex->recursive = 0;
        } else if (errno == EBUSY) {
            retval = SDL_MUTEX_TIMEDOUT;
        } else {
            retval = SDL_SetError("pthread_mutex_trylock() failed");
        }
    }
#else
    if (pthread_mutex_trylock(&mutex->id) != 0) {
        if (errno == EBUSY) {
            retval = SDL_MUTEX_TIMEDOUT;
        } else {
            retval = SDL_SetError("pthread_mutex_trylock() failed");
        }
    }
#endif
    return retval;
}

int
SDL_UnlockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

#if FAKE_RECURSIVE_MUTEX

    if (pthread_self() == mutex->owner) {
        if (mutex->recursive) {
            --mutex->recursive;
        } else {

            mutex->owner = 0;
            pthread_mutex_unlock(&mutex->id);
        }
    } else {
        return SDL_SetError("mutex not owned by this thread");
    }

#else
    if (pthread_mutex_unlock(&mutex->id) < 0) {
        return SDL_SetError("pthread_mutex_unlock() failed");
    }
#endif

    return 0;
}

#endif
#if SDL_THREAD_PTHREAD

#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#ifndef _SDL_mutex_c_h
#define _SDL_mutex_c_h

//struct SDL_mutex
//{
//    pthread_mutex_t id;
//};

#endif

struct SDL_cond
{
    pthread_cond_t cond;
};

SDL_cond *
SDL_CreateCond(void)
{
    SDL_cond *cond;

    cond = (SDL_cond *) SDL_malloc(sizeof(SDL_cond));
    if (cond) {
        if (pthread_cond_init(&cond->cond, NULL) < 0) {
            SDL_SetError("pthread_cond_init() failed");
            SDL_free(cond);
            cond = NULL;
        }
    }
    return (cond);
}

void
SDL_DestroyCond(SDL_cond * cond)
{
    if (cond) {
        pthread_cond_destroy(&cond->cond);
        SDL_free(cond);
    }
}

int
SDL_CondSignal(SDL_cond * cond)
{
    int retval;

    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    retval = 0;
    if (pthread_cond_signal(&cond->cond) != 0) {
        return SDL_SetError("pthread_cond_signal() failed");
    }
    return retval;
}

int
SDL_CondBroadcast(SDL_cond * cond)
{
    int retval;

    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    retval = 0;
    if (pthread_cond_broadcast(&cond->cond) != 0) {
        return SDL_SetError("pthread_cond_broadcast() failed");
    }
    return retval;
}

int
SDL_CondWaitTimeout(SDL_cond * cond, SDL_mutex * mutex, Uint32 ms)
{
    int retval;
    struct timeval delta;
    struct timespec abstime;

    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    }

    gettimeofday(&delta, NULL);

    abstime.tv_sec = delta.tv_sec + (ms / 1000);
    abstime.tv_nsec = (delta.tv_usec + (ms % 1000) * 1000) * 1000;
    if (abstime.tv_nsec > 1000000000) {
        abstime.tv_sec += 1;
        abstime.tv_nsec -= 1000000000;
    }

  tryagain:
    retval = pthread_cond_timedwait(&cond->cond, &mutex->id, &abstime);
    switch (retval) {
    case EINTR:
        goto tryagain;
        break;
    case ETIMEDOUT:
        retval = SDL_MUTEX_TIMEDOUT;
        break;
    case 0:
        break;
    default:
        retval = SDL_SetError("pthread_cond_timedwait() failed");
    }
    return retval;
}

int
SDL_CondWait(SDL_cond * cond, SDL_mutex * mutex)
{
    if (!cond) {
        return SDL_SetError("Passed a NULL condition variable");
    } else if (pthread_cond_wait(&cond->cond, &mutex->id) != 0) {
        return SDL_SetError("pthread_cond_wait() failed");
    }
    return 0;
}

#endif
#if SDL_THREAD_PTHREAD

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#if defined(__MACOSX__) || defined(__IPHONEOS__)
#else

struct SDL_semaphore
{
    sem_t sem;
};

SDL_sem *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_sem *sem = (SDL_sem *) SDL_malloc(sizeof(SDL_sem));
    if (sem) {
        if (sem_init(&sem->sem, 0, initial_value) < 0) {
            SDL_SetError("sem_init() failed");
            SDL_free(sem);
            sem = NULL;
        }
    } else {
        SDL_OutOfMemory();
    }
    return sem;
}

void
SDL_DestroySemaphore(SDL_sem * sem)
{
    if (sem) {
        sem_destroy(&sem->sem);
        SDL_free(sem);
    }
}

int
SDL_SemTryWait(SDL_sem * sem)
{
    int retval;

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }
    retval = SDL_MUTEX_TIMEDOUT;
    if (sem_trywait(&sem->sem) == 0) {
        retval = 0;
    }
    return retval;
}

int
SDL_SemWait(SDL_sem * sem)
{
    int retval;

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    retval = sem_wait(&sem->sem);
    if (retval < 0) {
        retval = SDL_SetError("sem_wait() failed");
    }
    return retval;
}

int
SDL_SemWaitTimeout(SDL_sem * sem, Uint32 timeout)
{
    int retval;
#ifdef HAVE_SEM_TIMEDWAIT
    struct timeval now;
    struct timespec ts_timeout;
#else
    Uint32 end;
#endif

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    if (timeout == 0) {
        return SDL_SemTryWait(sem);
    }
    if (timeout == SDL_MUTEX_MAXWAIT) {
        return SDL_SemWait(sem);
    }

#ifdef HAVE_SEM_TIMEDWAIT

    gettimeofday(&now, NULL);

    now.tv_usec += (timeout % 1000) * 1000;
    now.tv_sec += timeout / 1000;

    if ( now.tv_usec >= 1000000 ) {
        now.tv_usec -= 1000000;
        now.tv_sec ++;
    }

    ts_timeout.tv_sec = now.tv_sec;
    ts_timeout.tv_nsec = now.tv_usec * 1000;

    do {
        retval = sem_timedwait(&sem->sem, &ts_timeout);
    } while (retval < 0 && errno == EINTR);

    if (retval < 0) {
        if (errno == ETIMEDOUT) {
            retval = SDL_MUTEX_TIMEDOUT;
        } else {
            SDL_SetError(strerror(errno));
        }
    }
#else
    end = SDL_GetTicks() + timeout;
    while ((retval = SDL_SemTryWait(sem)) == SDL_MUTEX_TIMEDOUT) {
        if ((SDL_GetTicks() - end) >= 0) {
            break;
        }
        SDL_Delay(0);
    }
#endif

    return retval;
}

Uint32
SDL_SemValue(SDL_sem * sem)
{
    int ret = 0;
    if (sem) {
        sem_getvalue(&sem->sem, &ret);
        if (ret < 0) {
            ret = 0;
        }
    }
    return (Uint32) ret;
}

int
SDL_SemPost(SDL_sem * sem)
{
    int retval;

    if (!sem) {
        return SDL_SetError("Passed a NULL semaphore");
    }

    retval = sem_post(&sem->sem);
    if (retval < 0) {
        SDL_SetError("sem_post() failed");
    }
    return retval;
}

#endif

#endif
#if SDL_THREAD_PTHREAD

#include <pthread.h>

#if HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif

#include <signal.h>

#ifdef __LINUX__
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#if defined(__LINUX__) || defined(__MACOSX__) || defined(__IPHONEOS__)
#include <dlfcn.h>
#ifndef RTLD_DEFAULT
#define RTLD_DEFAULT NULL
#endif
#endif

#ifdef __ANDROID__
#include "../../core/android/SDL_android.h"
#endif

static const int sig_list[] = {
    SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGWINCH,
    SIGVTALRM, SIGPROF, 0
};

static void *
RunThread(void *data)
{
#ifdef __ANDROID__
    Android_JNI_SetupThread();
#endif
    SDL_RunThread(data);
    return NULL;
}

#if defined(__MACOSX__) || defined(__IPHONEOS__)
static SDL_bool checked_setname = SDL_FALSE;
static int (*ppthread_setname_np)(const char*) = NULL;
#elif defined(__LINUX__)
static SDL_bool checked_setname = SDL_FALSE;
static int (*ppthread_setname_np)(pthread_t, const char*) = NULL;
#endif
int
SDL_SYS_CreateThread(SDL_Thread * thread, void *args)
{
    pthread_attr_t type;

    #if defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__LINUX__)
    if (!checked_setname) {
        void *fn = dlsym(RTLD_DEFAULT, "pthread_setname_np");
        #if defined(__MACOSX__) || defined(__IPHONEOS__)
        ppthread_setname_np = (int(*)(const char*)) fn;
        #elif defined(__LINUX__)
        ppthread_setname_np = (int(*)(pthread_t, const char*)) fn;
        #endif
        checked_setname = SDL_TRUE;
    }
    #endif

    if (pthread_attr_init(&type) != 0) {
        return SDL_SetError("Couldn't initialize pthread attributes");
    }
    pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);

    if (pthread_create(&thread->handle, &type, RunThread, args) != 0) {
        return SDL_SetError("Not enough resources to create thread");
    }

    return 0;
}

void
SDL_SYS_SetupThread(const char *name)
{
    int i;
    sigset_t mask;

    if (name != NULL) {
        #if defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__LINUX__)
        SDL_assert(checked_setname);
        if (ppthread_setname_np != NULL) {
            #if defined(__MACOSX__) || defined(__IPHONEOS__)
            ppthread_setname_np(name);
            #elif defined(__LINUX__)
            ppthread_setname_np(pthread_self(), name);
            #endif
        }
        #elif HAVE_PTHREAD_SETNAME_NP
            pthread_setname_np(pthread_self(), name);
        #elif HAVE_PTHREAD_SET_NAME_NP
            pthread_set_name_np(pthread_self(), name);
        #endif
    }

    sigemptyset(&mask);
    for (i = 0; sig_list[i]; ++i) {
        sigaddset(&mask, sig_list[i]);
    }
    pthread_sigmask(SIG_BLOCK, &mask, 0);

#ifdef PTHREAD_CANCEL_ASYNCHRONOUS

    {
        int oldstate;
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate);
    }
#endif
}

SDL_threadID
SDL_ThreadID(void)
{
    return ((SDL_threadID) pthread_self());
}

int
SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
#ifdef __LINUX__
    int value;

    if (priority == SDL_THREAD_PRIORITY_LOW) {
        value = 19;
    } else if (priority == SDL_THREAD_PRIORITY_HIGH) {
        value = -20;
    } else {
        value = 0;
    }
    if (setpriority(PRIO_PROCESS, syscall(SYS_gettid), value) < 0) {

        return SDL_SetError("setpriority() failed");
    }
    return 0;
#else
    struct sched_param sched;
    int policy;
    pthread_t thread = pthread_self();

    if (pthread_getschedparam(thread, &policy, &sched) < 0) {
        return SDL_SetError("pthread_getschedparam() failed");
    }
    if (priority == SDL_THREAD_PRIORITY_LOW) {
        sched.sched_priority = sched_get_priority_min(policy);
    } else if (priority == SDL_THREAD_PRIORITY_HIGH) {
        sched.sched_priority = sched_get_priority_max(policy);
    } else {
        int min_priority = sched_get_priority_min(policy);
        int max_priority = sched_get_priority_max(policy);
        sched.sched_priority = (min_priority + (max_priority - min_priority) / 2);
    }
    if (pthread_setschedparam(thread, policy, &sched) < 0) {
        return SDL_SetError("pthread_setschedparam() failed");
    }
    return 0;
#endif
}

void
SDL_SYS_WaitThread(SDL_Thread * thread)
{
    pthread_join(thread->handle, 0);
}

#endif
#define ARRAY_CHUNKSIZE	32

static int SDL_maxthreads = 0;
static int SDL_numthreads = 0;
static SDL_Thread **SDL_Threads = NULL;
static SDL_mutex *thread_lock = NULL;

static int
SDL_ThreadsInit(void)
{
    int retval;

    retval = 0;
    thread_lock = SDL_CreateMutex();
    if (thread_lock == NULL) {
        retval = -1;
    }
    return (retval);
}

#if 0
static void
SDL_ThreadsQuit(void)
{
    SDL_mutex *mutex;

    mutex = thread_lock;
    thread_lock = NULL;
    if (mutex != NULL) {
        SDL_DestroyMutex(mutex);
    }
}
#endif

static void
SDL_AddThread(SDL_Thread * thread)
{

    if (!thread_lock) {
        if (SDL_ThreadsInit() < 0) {
            return;
        }
    }
    SDL_LockMutex(thread_lock);

#ifdef DEBUG_THREADS
    printf("Adding thread (%d already - %d max)\n",
           SDL_numthreads, SDL_maxthreads);
#endif
    if (SDL_numthreads == SDL_maxthreads) {
        SDL_Thread **threads;
        threads = (SDL_Thread **) SDL_realloc(SDL_Threads,
                                              (SDL_maxthreads +
                                               ARRAY_CHUNKSIZE) *
                                              (sizeof *threads));
        if (threads == NULL) {
            SDL_OutOfMemory();
            goto done;
        }
        SDL_maxthreads += ARRAY_CHUNKSIZE;
        SDL_Threads = threads;
    }
    SDL_Threads[SDL_numthreads++] = thread;
  done:
    SDL_mutexV(thread_lock);
}

static void
SDL_DelThread(SDL_Thread * thread)
{
    int i;

    if (!thread_lock) {
        return;
    }
    SDL_LockMutex(thread_lock);
    for (i = 0; i < SDL_numthreads; ++i) {
        if (thread == SDL_Threads[i]) {
            break;
        }
    }
    if (i < SDL_numthreads) {
        if (--SDL_numthreads > 0) {
            while (i < SDL_numthreads) {
                SDL_Threads[i] = SDL_Threads[i + 1];
                ++i;
            }
        } else {
            SDL_maxthreads = 0;
            SDL_free(SDL_Threads);
            SDL_Threads = NULL;
        }
#ifdef DEBUG_THREADS
        printf("Deleting thread (%d left - %d max)\n",
               SDL_numthreads, SDL_maxthreads);
#endif
    }
    SDL_mutexV(thread_lock);

#if 0
    if (SDL_Threads == NULL) {
        SDL_ThreadsQuit();
    }
#endif
}

static SDL_error SDL_global_error;

SDL_error *
SDL_GetErrBuf(void)
{
    SDL_error *errbuf;

    errbuf = &SDL_global_error;
    if (SDL_Threads) {
        int i;
        SDL_threadID this_thread;

        this_thread = SDL_ThreadID();
        SDL_LockMutex(thread_lock);
        for (i = 0; i < SDL_numthreads; ++i) {
            if (this_thread == SDL_Threads[i]->threadid) {
                errbuf = &SDL_Threads[i]->errbuf;
                break;
            }
        }
        SDL_mutexV(thread_lock);
    }
    return (errbuf);
}

typedef struct
{
    int (SDLCALL * func) (void *);
    void *data;
    SDL_Thread *info;
    SDL_sem *wait;
} thread_args;

void
SDL_RunThread(void *data)
{
    thread_args *args = (thread_args *) data;
    int (SDLCALL * userfunc) (void *) = args->func;
    void *userdata = args->data;
    int *statusloc = &args->info->status;

    SDL_SYS_SetupThread(args->info->name);

    args->info->threadid = SDL_ThreadID();

    SDL_SemPost(args->wait);

    *statusloc = userfunc(userdata);
}

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
#undef SDL_CreateThread
DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(int (SDLCALL * fn) (void *),
                 const char *name, void *data,
                 pfnSDL_CurrentBeginThread pfnBeginThread,
                 pfnSDL_CurrentEndThread pfnEndThread)
#else
DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(int (SDLCALL * fn) (void *),
                 const char *name, void *data)
#endif
{
    SDL_Thread *thread;
    thread_args *args;
    int ret;

    thread = (SDL_Thread *) SDL_malloc(sizeof(*thread));
    if (thread == NULL) {
        SDL_OutOfMemory();
        return (NULL);
    }
    SDL_memset(thread, 0, (sizeof *thread));
    thread->status = -1;

    if (name != NULL) {
        thread->name = SDL_strdup(name);
        if (thread->name == NULL) {
            SDL_OutOfMemory();
            SDL_free(thread);
            return (NULL);
        }
    }

    args = (thread_args *) SDL_malloc(sizeof(*args));
    if (args == NULL) {
        SDL_OutOfMemory();
        SDL_free(thread->name);
        SDL_free(thread);
        return (NULL);
    }
    args->func = fn;
    args->data = data;
    args->info = thread;
    args->wait = SDL_CreateSemaphore(0);
    if (args->wait == NULL) {
        SDL_free(thread->name);
        SDL_free(thread);
        SDL_free(args);
        return (NULL);
    }

    SDL_AddThread(thread);

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
    ret = SDL_SYS_CreateThread(thread, args, pfnBeginThread, pfnEndThread);
#else
    ret = SDL_SYS_CreateThread(thread, args);
#endif
    if (ret >= 0) {

        SDL_SemWait(args->wait);
    } else {

        SDL_DelThread(thread);
        SDL_free(thread->name);
        SDL_free(thread);
        thread = NULL;
    }
    SDL_DestroySemaphore(args->wait);
    SDL_free(args);

    return (thread);
}

SDL_threadID
SDL_GetThreadID(SDL_Thread * thread)
{
    SDL_threadID id;

    if (thread) {
        id = thread->threadid;
    } else {
        id = SDL_ThreadID();
    }
    return id;
}

const char *
SDL_GetThreadName(SDL_Thread * thread)
{
    return thread->name;
}

int
SDL_SetThreadPriority(SDL_ThreadPriority priority)
{
    return SDL_SYS_SetThreadPriority(priority);
}

void
SDL_WaitThread(SDL_Thread * thread, int *status)
{
    if (thread) {
        SDL_SYS_WaitThread(thread);
        if (status) {
            *status = thread->status;
        }
        SDL_DelThread(thread);
        SDL_free(thread->name);
        SDL_free(thread);
    }
}
#if SDL_THREAD_WINDOWS

struct SDL_mutex
{
    CRITICAL_SECTION cs;
};

SDL_mutex *
SDL_CreateMutex(void)
{
    SDL_mutex *mutex;

    mutex = (SDL_mutex *) SDL_malloc(sizeof(*mutex));
    if (mutex) {

        InitializeCriticalSectionAndSpinCount(&mutex->cs, 2000);
    } else {
        SDL_OutOfMemory();
    }
    return (mutex);
}

void
SDL_DestroyMutex(SDL_mutex * mutex)
{
    if (mutex) {
        DeleteCriticalSection(&mutex->cs);
        SDL_free(mutex);
    }
}

int
SDL_LockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    EnterCriticalSection(&mutex->cs);
    return (0);
}

int
SDL_TryLockMutex(SDL_mutex * mutex)
{
    int retval = 0;
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    if (TryEnterCriticalSection(&mutex->cs) == 0) {
        retval = SDL_MUTEX_TIMEDOUT;
    }
    return retval;
}

int
SDL_UnlockMutex(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        return SDL_SetError("Passed a NULL mutex");
    }

    LeaveCriticalSection(&mutex->cs);
    return (0);
}

#endif
#if SDL_THREAD_WINDOWS

struct SDL_semaphore
{
    HANDLE id;
    LONG count;
};

SDL_sem *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_sem *sem;

    sem = (SDL_sem *) SDL_malloc(sizeof(*sem));
    if (sem) {

        sem->id = CreateSemaphore(NULL, initial_value, 32 * 1024, NULL);
        sem->count = initial_value;
        if (!sem->id) {
            SDL_SetError("Couldn't create semaphore");
            SDL_free(sem);
            sem = NULL;
        }
    } else {
        SDL_OutOfMemory();
    }
    return (sem);
}

void
SDL_DestroySemaphore(SDL_sem * sem)
{
    if (sem) {
        if (sem->id) {
            CloseHandle(sem->id);
            sem->id = 0;
        }
        SDL_free(sem);
    }
}

int
SDL_SemWaitTimeout(SDL_sem * sem, Uint32 timeout)
{
    int retval;
    DWORD dwMilliseconds;

    if (!sem) {
        return SDL_SetError("Passed a NULL sem");
    }

    if (timeout == SDL_MUTEX_MAXWAIT) {
        dwMilliseconds = INFINITE;
    } else {
        dwMilliseconds = (DWORD) timeout;
    }
    switch (WaitForSingleObject(sem->id, dwMilliseconds)) {
    case WAIT_OBJECT_0:
        InterlockedDecrement(&sem->count);
        retval = 0;
        break;
    case WAIT_TIMEOUT:
        retval = SDL_MUTEX_TIMEDOUT;
        break;
    default:
        retval = SDL_SetError("WaitForSingleObject() failed");
        break;
    }
    return retval;
}

int
SDL_SemTryWait(SDL_sem * sem)
{
    return SDL_SemWaitTimeout(sem, 0);
}

int
SDL_SemWait(SDL_sem * sem)
{
    return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

Uint32
SDL_SemValue(SDL_sem * sem)
{
    if (!sem) {
        SDL_SetError("Passed a NULL sem");
        return 0;
    }
    return (Uint32)sem->count;
}

int
SDL_SemPost(SDL_sem * sem)
{
    if (!sem) {
        return SDL_SetError("Passed a NULL sem");
    }

    InterlockedIncrement(&sem->count);
    if (ReleaseSemaphore(sem->id, 1, NULL) == FALSE) {
        InterlockedDecrement(&sem->count);
        return SDL_SetError("ReleaseSemaphore() failed");
    }
    return 0;
}

#endif
#if SDL_THREAD_WINDOWS

#ifndef SDL_PASSED_BEGINTHREAD_ENDTHREAD

#include <process.h>

#if (defined(__MINGW32__) && (__GNUC__ < 4))
typedef unsigned long (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg,
        unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);

#elif defined(__WATCOMC__)

#if __WATCOMC__ < 1240
#define __watcall
#endif
typedef unsigned long (__watcall * pfnSDL_CurrentBeginThread) (void *,
                                                               unsigned,
                                                               unsigned
                                                               (__stdcall *
                                                                func) (void
                                                                       *),
                                                               void *arg,
                                                               unsigned,
                                                               unsigned
                                                               *threadID);
typedef void (__watcall * pfnSDL_CurrentEndThread) (unsigned code);

#else
typedef uintptr_t(__cdecl * pfnSDL_CurrentBeginThread) (void *, unsigned,
                                                        unsigned (__stdcall *
                                                                  func) (void
                                                                         *),
                                                        void *arg, unsigned,
                                                        unsigned *threadID);
typedef void (__cdecl * pfnSDL_CurrentEndThread) (unsigned code);
#endif
#endif

typedef struct ThreadStartParms
{
    void *args;
    pfnSDL_CurrentEndThread pfnCurrentEndThread;
} tThreadStartParms, *pThreadStartParms;

static DWORD
RunThread(void *data)
{
    pThreadStartParms pThreadParms = (pThreadStartParms) data;
    pfnSDL_CurrentEndThread pfnEndThread = pThreadParms->pfnCurrentEndThread;
    void *args = pThreadParms->args;
    SDL_free(pThreadParms);
    SDL_RunThread(args);
    if (pfnEndThread != NULL)
        pfnEndThread(0);
    return (0);
}

static DWORD WINAPI
RunThreadViaCreateThread(LPVOID data)
{
  return RunThread(data);
}

static unsigned __stdcall
RunThreadViaBeginThreadEx(void *data)
{
  return (unsigned) RunThread(data);
}

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
int
SDL_SYS_CreateThread(SDL_Thread * thread, void *args,
                     pfnSDL_CurrentBeginThread pfnBeginThread,
                     pfnSDL_CurrentEndThread pfnEndThread)
{
#else
int
SDL_SYS_CreateThread(SDL_Thread * thread, void *args)
{
    pfnSDL_CurrentBeginThread pfnBeginThread = _beginthreadex;
    pfnSDL_CurrentEndThread pfnEndThread = _endthreadex;
#endif
    pThreadStartParms pThreadParms =
        (pThreadStartParms) SDL_malloc(sizeof(tThreadStartParms));
    if (!pThreadParms) {
        return SDL_OutOfMemory();
    }

    pThreadParms->pfnCurrentEndThread = pfnEndThread;

    pThreadParms->args = args;

    if (pfnBeginThread) {
        unsigned threadid = 0;
        thread->handle = (SYS_ThreadHandle)
            ((size_t) pfnBeginThread(NULL, 0, RunThreadViaBeginThreadEx,
                                     pThreadParms, 0, &threadid));
    } else {
        DWORD threadid = 0;
        thread->handle = CreateThread(NULL, 0, RunThreadViaCreateThread,
                                      pThreadParms, 0, &threadid);
    }
    if (thread->handle == NULL) {
        return SDL_SetError("Not enough resources to create thread");
    }
    return 0;
}

#ifdef _MSC_VER
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType;
    LPCSTR szName;
    DWORD dwThreadID;
    DWORD dwFlags;
} THREADNAME_INFO;
#pragma pack(pop)
#endif

void
SDL_SYS_SetupThread(const char *name)
{
    if (name != NULL) {
        #if 0
        #ifdef _MSC_VER

        THREADNAME_INFO inf;
        inf.dwType = 0x1000;
        inf.szName = name;
        inf.dwThreadID = (DWORD) -1;
        inf.dwFlags = 0;

        __try
        {
            RaiseException(0x406D1388, 0, sizeof(inf)/sizeof(DWORD), (DWORD*)&inf);
        }
        __except(EXCEPTION_CONTINUE_EXECUTION)
        {

        }
        #endif
        #endif
    }
}

SDL_threadID
SDL_ThreadID(void)
{
    return ((SDL_threadID) GetCurrentThreadId());
}

int
SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
    int value;

    if (priority == SDL_THREAD_PRIORITY_LOW) {
        value = THREAD_PRIORITY_LOWEST;
    } else if (priority == SDL_THREAD_PRIORITY_HIGH) {
        value = THREAD_PRIORITY_HIGHEST;
    } else {
        value = THREAD_PRIORITY_NORMAL;
    }
    if (!SetThreadPriority(GetCurrentThread(), value)) {
        return WIN_SetError("SetThreadPriority()");
    }
    return 0;
}

void
SDL_SYS_WaitThread(SDL_Thread * thread)
{
    WaitForSingleObject(thread->handle, INFINITE);
    CloseHandle(thread->handle);
}

#endif
#ifdef SDL_TIMER_UNIX

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#if HAVE_NANOSLEEP || HAVE_CLOCK_GETTIME
#include <time.h>
#endif
#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

#if HAVE_CLOCK_GETTIME
static struct timespec start_ts;
#elif defined(__APPLE__)
static uint64_t start_mach;
mach_timebase_info_data_t mach_base_info;
#endif
static SDL_bool has_monotonic_time = SDL_FALSE;
static struct timeval start_tv;
static SDL_bool ticks_started = SDL_FALSE;

void
SDL_TicksInit(void)
{
    if (ticks_started) {
        return;
    }
    ticks_started = SDL_TRUE;

#if HAVE_CLOCK_GETTIME
    if (clock_gettime(CLOCK_MONOTONIC, &start_ts) == 0) {
        has_monotonic_time = SDL_TRUE;
    } else
#elif defined(__APPLE__)
    kern_return_t ret = mach_timebase_info(&mach_base_info);
    if (ret == 0) {
        has_monotonic_time = SDL_TRUE;
        start_mach = mach_absolute_time();
    } else
#endif
    {
        gettimeofday(&start_tv, NULL);
    }
}

void
SDL_TicksQuit(void)
{
    ticks_started = SDL_FALSE;
}

Uint32
SDL_GetTicks(void)
{
    Uint32 ticks;
    if (!ticks_started) {
        SDL_TicksInit();
    }

    if (has_monotonic_time) {
#if HAVE_CLOCK_GETTIME
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        ticks = (now.tv_sec - start_ts.tv_sec) * 1000 + (now.tv_nsec -
                                                 start_ts.tv_nsec) / 1000000;
#elif defined(__APPLE__)
        uint64_t now = mach_absolute_time();
        ticks = (((now - start_mach) * mach_base_info.numer) / mach_base_info.denom) / 1000000;
#endif
    } else {
        struct timeval now;

        gettimeofday(&now, NULL);
        ticks =
            (now.tv_sec - start_tv.tv_sec) * 1000 + (now.tv_usec -
                                                  start_tv.tv_usec) / 1000;
    }
    return (ticks);
}

Uint64
SDL_GetPerformanceCounter(void)
{
    Uint64 ticks;
    if (!ticks_started) {
        SDL_TicksInit();
    }

    if (has_monotonic_time) {
#if HAVE_CLOCK_GETTIME
        struct timespec now;

        clock_gettime(CLOCK_MONOTONIC, &now);
        ticks = now.tv_sec;
        ticks *= 1000000000;
        ticks += now.tv_nsec;
#elif defined(__APPLE__)
        ticks = mach_absolute_time();
#endif
    } else {
        struct timeval now;

        gettimeofday(&now, NULL);
        ticks = now.tv_sec;
        ticks *= 1000000;
        ticks += now.tv_usec;
    }
    return (ticks);
}

Uint64
SDL_GetPerformanceFrequency(void)
{
    if (!ticks_started) {
        SDL_TicksInit();
    }

    if (has_monotonic_time) {
#if HAVE_CLOCK_GETTIME
        return 1000000000;
#elif defined(__APPLE__)
        Uint64 freq = mach_base_info.denom;
        freq *= 1000000000;
        freq /= mach_base_info.numer;
        return freq;
#endif
    }

    return 1000000;
}

void
SDL_Delay(Uint32 ms)
{
    int was_error;

#if HAVE_NANOSLEEP
    struct timespec elapsed, tv;
#else
    struct timeval tv;
    Uint32 then, now, elapsed;
#endif

#if HAVE_NANOSLEEP
    elapsed.tv_sec = ms / 1000;
    elapsed.tv_nsec = (ms % 1000) * 1000000;
#else
    then = SDL_GetTicks();
#endif
    do {
        errno = 0;

#if HAVE_NANOSLEEP
        tv.tv_sec = elapsed.tv_sec;
        tv.tv_nsec = elapsed.tv_nsec;
        was_error = nanosleep(&tv, &elapsed);
#else

        now = SDL_GetTicks();
        elapsed = (now - then);
        then = now;
        if (elapsed >= ms) {
            break;
        }
        ms -= elapsed;
        tv.tv_sec = ms / 1000;
        tv.tv_usec = (ms % 1000) * 1000;

        was_error = select(0, NULL, NULL, NULL, &tv);
#endif
    } while (was_error && (errno == EINTR));
}

#endif
#ifdef SDL_TIMER_WINDOWS

#include <mmsystem.h>

static DWORD start;
static BOOL ticks_started = FALSE;

#ifndef USE_GETTICKCOUNT

static BOOL hires_timer_available;

static LARGE_INTEGER hires_start_ticks;

static LARGE_INTEGER hires_ticks_per_second;

#ifndef __WINRT__
static void
timeSetPeriod(UINT uPeriod)
{
    static UINT timer_period = 0;

    if (uPeriod != timer_period) {
        if (timer_period) {
            timeEndPeriod(timer_period);
        }

        timer_period = uPeriod;

        if (timer_period) {
            timeBeginPeriod(timer_period);
        }
    }
}

static void
SDL_TimerResolutionChanged(void *userdata, const char *name, const char *oldValue, const char *hint)
{
    UINT uPeriod;

    if (hint && *hint) {
        uPeriod = SDL_atoi(hint);
    } else {
        uPeriod = 1;
    }
    if (uPeriod || oldValue != hint) {
        timeSetPeriod(uPeriod);
    }
}
#endif

#endif

void
SDL_TicksInit(void)
{
    if (ticks_started) {
        return;
    }
    ticks_started = SDL_TRUE;

#ifdef USE_GETTICKCOUNT
    start = GetTickCount();
#else

    if (QueryPerformanceFrequency(&hires_ticks_per_second) == TRUE) {
        hires_timer_available = TRUE;
        QueryPerformanceCounter(&hires_start_ticks);
    } else {
        hires_timer_available = FALSE;
#ifdef __WINRT__
        start = 0;
#else
        timeSetPeriod(1);
        start = timeGetTime();

        SDL_AddHintCallback(SDL_HINT_TIMER_RESOLUTION,
                            SDL_TimerResolutionChanged, NULL);
#endif
    }
#endif
}

void
SDL_TicksQuit(void)
{
#ifndef USE_GETTICKCOUNT
    if (!hires_timer_available) {
#ifndef __WINRT__
        SDL_DelHintCallback(SDL_HINT_TIMER_RESOLUTION,
                            SDL_TimerResolutionChanged, NULL);

        timeSetPeriod(0);
#endif
    }
#endif

    ticks_started = SDL_FALSE;
}

Uint32
SDL_GetTicks(void)
{
    DWORD now;
#ifndef USE_GETTICKCOUNT
    LARGE_INTEGER hires_now;
#endif

    if (!ticks_started) {
        SDL_TicksInit();
    }

#ifdef USE_GETTICKCOUNT
    now = GetTickCount();
#else
    if (hires_timer_available) {
        QueryPerformanceCounter(&hires_now);

        hires_now.QuadPart -= hires_start_ticks.QuadPart;
        hires_now.QuadPart *= 1000;
        hires_now.QuadPart /= hires_ticks_per_second.QuadPart;

        return (DWORD) hires_now.QuadPart;
    } else {
#ifdef __WINRT__
        now = 0;
#else
        now = timeGetTime();
#endif
    }
#endif

    return (now - start);
}

Uint64
SDL_GetPerformanceCounter(void)
{
    LARGE_INTEGER counter;

    if (!QueryPerformanceCounter(&counter)) {
        return SDL_GetTicks();
    }
    return counter.QuadPart;
}

Uint64
SDL_GetPerformanceFrequency(void)
{
    LARGE_INTEGER frequency;

    if (!QueryPerformanceFrequency(&frequency)) {
        return 1000;
    }
    return frequency.QuadPart;
}

#ifdef __WINRT__
static void
Sleep(DWORD timeout)
{
    static HANDLE mutex = 0;
    if ( ! mutex )
    {
        mutex = CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS);
    }
    WaitForSingleObjectEx(mutex, timeout, FALSE);
}
#endif

void
SDL_Delay(Uint32 ms)
{
    Sleep(ms);
}

#endif

int SDL_ConvertAudio(SDL_AudioCVT* c) { return 0; }
int SDL_BuildAudioCVT(SDL_AudioCVT* c,SDL_AudioFormat sf, Uint8 sc, int sr, SDL_AudioFormat df, Uint8 dc, int dr) { return 0; }
void SDL_MixAudioFormat(Uint8 * dst, const Uint8 * src, SDL_AudioFormat format, Uint32 len, int volume) { }
static char SDL_Audio_Init_Done = 0;
Uint32 SDL_WasInit(Uint32 flags) { return (flags &16 ? SDL_Audio_Init_Done : 0); }
int SDL_InitSubSystem(Uint32 flags) { return (flags &16 ? (SDL_Audio_Init_Done = 1) : 0); }
#ifdef __WIN32__
char* WIN_StringToUTF8(const unsigned short* s) { int l = WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)s,-1,0,0,0,0); char* b = (char*)malloc(l); WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)s, -1, b, l, NULL, NULL); return b; }
unsigned short* WIN_UTF8ToString(const char* s) { int l = MultiByteToWideChar(CP_UTF8,0,s,-1,NULL,0); unsigned short* b = (unsigned short*)malloc(l*sizeof(short)); MultiByteToWideChar(CP_UTF8,0,s,-1,(LPWSTR)b,l); return b; }
#if (defined(_MSC_VER) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
#pragma comment (lib, "winmm.lib")
#endif
#endif
