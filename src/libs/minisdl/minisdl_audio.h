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

#ifndef _SDL_CONFIG_H
#define _SDL_CONFIG_H

#ifndef _SDL_platform_h
#define _SDL_platform_h

#if defined(_AIX)
#undef __AIX__
#define __AIX__     1
#endif
#if defined(__HAIKU__)
#undef __HAIKU__
#define __HAIKU__   1
#endif
#if defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
#undef __BSDI__
#define __BSDI__    1
#endif
#if defined(_arch_dreamcast)
#undef __DREAMCAST__
#define __DREAMCAST__   1
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#undef __FREEBSD__
#define __FREEBSD__ 1
#endif
#if defined(hpux) || defined(__hpux) || defined(__hpux__)
#undef __HPUX__
#define __HPUX__    1
#endif
#if defined(sgi) || defined(__sgi) || defined(__sgi__) || defined(_SGI_SOURCE)
#undef __IRIX__
#define __IRIX__    1
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
#undef __LINUX__
#define __LINUX__   1
#endif
#if defined(ANDROID) || defined(__ANDROID__)
#undef __ANDROID__
#undef __LINUX__
#define __ANDROID__ 1
#endif

#if defined(__APPLE__)

#include "AvailabilityMacros.h"
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE

#undef __IPHONEOS__
#define __IPHONEOS__ 1
#undef __MACOSX__
#else

#undef __MACOSX__
#define __MACOSX__  1
#if MAC_OS_X_VERSION_MIN_REQUIRED < 1050
# error SDL for Mac OS X only supports deploying on 10.5 and above.
#endif
#endif
#endif

#if defined(__NetBSD__)
#undef __NETBSD__
#define __NETBSD__  1
#endif
#if defined(__OpenBSD__)
#undef __OPENBSD__
#define __OPENBSD__ 1
#endif
#if defined(__OS2__)
#undef __OS2__
#define __OS2__     1
#endif
#if defined(osf) || defined(__osf) || defined(__osf__) || defined(_OSF_SOURCE)
#undef __OSF__
#define __OSF__     1
#endif
#if defined(__QNXNTO__)
#undef __QNXNTO__
#define __QNXNTO__  1
#endif
#if defined(riscos) || defined(__riscos) || defined(__riscos__)
#undef __RISCOS__
#define __RISCOS__  1
#endif
#if defined(__SVR4)
#undef __SOLARIS__
#define __SOLARIS__ 1
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)

#if defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1700) && !_USING_V110_SDK71_)
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#undef __WINDOWS__
#define __WINDOWS__   1

#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#undef __WINRT__
#define __WINRT__ 1
#endif
#else
#undef __WINDOWS__
#define __WINDOWS__   1
#endif
#endif

#if defined(__WINDOWS__)
#undef __WIN32__
#define __WIN32__ 1
#endif
#if defined(__PSP__)
#undef __PSP__
#define __PSP__ 1
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

extern DECLSPEC const char * SDLCALL SDL_GetPlatform (void);

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

#if defined(__WIN32__)
#define HAVE_LIBC

#ifndef _SDL_config_windows_h
#define _SDL_config_windows_h

#if !defined(_STDINT_H_) && (!defined(HAVE_STDINT_H) || !_HAVE_STDINT_H)
#if defined(__GNUC__) || defined(__DMC__) || defined(__WATCOMC__)
#define HAVE_STDINT_H   1
#elif defined(_MSC_VER)
typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#ifndef _UINTPTR_T_DEFINED
#ifdef  _WIN64
typedef unsigned __int64 uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif
#define _UINTPTR_T_DEFINED
#endif

#if ((_MSC_VER <= 1200) && (!defined(DWORD_PTR)))
#define DWORD_PTR DWORD
#endif
#if ((_MSC_VER <= 1200) && (!defined(LONG_PTR)))
#define LONG_PTR LONG
#endif
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#ifndef _SIZE_T_DEFINED_
#define _SIZE_T_DEFINED_
typedef unsigned int size_t;
#endif
typedef unsigned int uintptr_t;
#endif
#endif

#ifdef _WIN64
# define SIZEOF_VOIDP 8
#else
# define SIZEOF_VOIDP 4
#endif

#ifdef HAVE_LIBC

#define HAVE_STDIO_H 1
#define STDC_HEADERS 1
#define HAVE_STRING_H 1
#define HAVE_CTYPE_H 1
#define HAVE_MATH_H 1
#define HAVE_SIGNAL_H 1

#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FREE 1
#define HAVE_ALLOCA 1
#define HAVE_QSORT 1
#define HAVE_ABS 1
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMCMP 1
#define HAVE_STRLEN 1
#define HAVE__STRREV 1
#define HAVE__STRUPR 1
#define HAVE__STRLWR 1
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE__LTOA 1
#define HAVE__ULTOA 1
#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1
#define HAVE_STRTOD 1
#define HAVE_ATOI 1
#define HAVE_ATOF 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
#define HAVE__STRICMP 1
#define HAVE__STRNICMP 1
#define HAVE_ATAN 1
#define HAVE_ATAN2 1
#define HAVE_ACOS  1
#define HAVE_ASIN  1
#define HAVE_CEIL 1
#define HAVE_COS 1
#define HAVE_COSF 1
#define HAVE_FABS 1
#define HAVE_FLOOR 1
#define HAVE_LOG 1
#define HAVE_POW 1
#define HAVE_SIN 1
#define HAVE_SINF 1
#define HAVE_SQRT 1
#if _MSC_VER >= 1800
#define HAVE_STRTOLL 1
#define HAVE_VSSCANF 1
#define HAVE_COPYSIGN 1
#define HAVE_SCALBN 1
#endif
#if !defined(_MSC_VER) || defined(_USE_MATH_DEFINES)
#define HAVE_M_PI 1
#endif
#else
#define HAVE_STDARG_H   1
#define HAVE_STDDEF_H   1
#endif

#define SDL_AUDIO_DRIVER_DSOUND 1
#define SDL_AUDIO_DRIVER_XAUDIO2    1
#define SDL_AUDIO_DRIVER_WINMM  1
#define SDL_AUDIO_DRIVER_DISK   1
#define SDL_AUDIO_DRIVER_DUMMY  1

#define SDL_JOYSTICK_DINPUT 1
#define SDL_HAPTIC_DINPUT   1

#define SDL_LOADSO_WINDOWS  1

#define SDL_THREAD_WINDOWS  1

#define SDL_TIMER_WINDOWS   1

#define SDL_VIDEO_DRIVER_DUMMY  1
#define SDL_VIDEO_DRIVER_WINDOWS    1

#ifndef SDL_VIDEO_RENDER_D3D
#define SDL_VIDEO_RENDER_D3D    1
#endif
#ifndef SDL_VIDEO_RENDER_D3D11
#define SDL_VIDEO_RENDER_D3D11	0
#endif

#ifndef SDL_VIDEO_OPENGL
#define SDL_VIDEO_OPENGL    1
#endif
#ifndef SDL_VIDEO_OPENGL_WGL
#define SDL_VIDEO_OPENGL_WGL    1
#endif
#ifndef SDL_VIDEO_RENDER_OGL
#define SDL_VIDEO_RENDER_OGL    1
#endif
#ifndef SDL_VIDEO_RENDER_OGL_ES2
#define SDL_VIDEO_RENDER_OGL_ES2    1
#endif
#ifndef SDL_VIDEO_OPENGL_ES2
#define SDL_VIDEO_OPENGL_ES2    1
#endif
#ifndef SDL_VIDEO_OPENGL_EGL
#define SDL_VIDEO_OPENGL_EGL    1
#endif

#define SDL_POWER_WINDOWS 1

#define SDL_FILESYSTEM_WINDOWS  1

#ifndef _WIN64
#define SDL_ASSEMBLY_ROUTINES   1
#endif

#endif

#elif defined(__MACOSX__)

#ifndef _SDL_config_macosx_h
#define _SDL_config_macosx_h

#include <AvailabilityMacros.h>

#ifdef __LP64__
    #define SIZEOF_VOIDP 8
#else
    #define SIZEOF_VOIDP 4
#endif

#define HAVE_ALLOCA_H       1
#define HAVE_SYS_TYPES_H    1
#define HAVE_STDIO_H    1
#define STDC_HEADERS    1
#define HAVE_STRING_H   1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H   1
#define HAVE_CTYPE_H    1
#define HAVE_MATH_H 1
#define HAVE_SIGNAL_H   1

#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC    1
#define HAVE_FREE   1
#define HAVE_ALLOCA 1
#define HAVE_GETENV 1
#define HAVE_SETENV 1
#define HAVE_PUTENV 1
#define HAVE_UNSETENV   1
#define HAVE_QSORT  1
#define HAVE_ABS    1
#define HAVE_BCOPY  1
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE    1
#define HAVE_MEMCMP 1
#define HAVE_STRLEN 1
#define HAVE_STRLCPY    1
#define HAVE_STRLCAT    1
#define HAVE_STRDUP 1
#define HAVE_STRCHR 1
#define HAVE_STRRCHR    1
#define HAVE_STRSTR 1
#define HAVE_STRTOL 1
#define HAVE_STRTOUL    1
#define HAVE_STRTOLL    1
#define HAVE_STRTOULL   1
#define HAVE_STRTOD 1
#define HAVE_ATOI   1
#define HAVE_ATOF   1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP    1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_VSSCANF 1
#define HAVE_VSNPRINTF  1
#define HAVE_CEIL   1
#define HAVE_COPYSIGN   1
#define HAVE_COS    1
#define HAVE_COSF   1
#define HAVE_FABS   1
#define HAVE_FLOOR  1
#define HAVE_LOG    1
#define HAVE_POW    1
#define HAVE_SCALBN 1
#define HAVE_SIN    1
#define HAVE_SINF   1
#define HAVE_SQRT   1
#define HAVE_SIGACTION  1
#define HAVE_SETJMP 1
#define HAVE_NANOSLEEP  1
#define HAVE_SYSCONF    1
#define HAVE_SYSCTLBYNAME 1
#define HAVE_ATAN 1
#define HAVE_ATAN2 1
#define HAVE_ACOS 1
#define HAVE_ASIN 1

#define SDL_AUDIO_DRIVER_COREAUDIO  1
#define SDL_AUDIO_DRIVER_DISK   1
#define SDL_AUDIO_DRIVER_DUMMY  1

#define SDL_JOYSTICK_IOKIT  1
#define SDL_HAPTIC_IOKIT    1

#define SDL_LOADSO_DLOPEN   1

#define SDL_THREAD_PTHREAD  1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX  1

#define SDL_TIMER_UNIX  1

#define SDL_VIDEO_DRIVER_COCOA  1
#define SDL_VIDEO_DRIVER_DUMMY  1
#undef SDL_VIDEO_DRIVER_X11
#define SDL_VIDEO_DRIVER_X11_DYNAMIC "/usr/X11R6/lib/libX11.6.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT "/usr/X11R6/lib/libXext.6.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XINERAMA "/usr/X11R6/lib/libXinerama.1.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XINPUT2 "/usr/X11R6/lib/libXi.6.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XRANDR "/usr/X11R6/lib/libXrandr.2.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XSS "/usr/X11R6/lib/libXss.1.dylib"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XVIDMODE "/usr/X11R6/lib/libXxf86vm.1.dylib"
#define SDL_VIDEO_DRIVER_X11_XINERAMA 1
#define SDL_VIDEO_DRIVER_X11_XRANDR 1
#define SDL_VIDEO_DRIVER_X11_XSCRNSAVER 1
#define SDL_VIDEO_DRIVER_X11_XSHAPE 1
#define SDL_VIDEO_DRIVER_X11_XVIDMODE 1
#define SDL_VIDEO_DRIVER_X11_HAS_XKBKEYCODETOKEYSYM 1

#ifdef MAC_OS_X_VERSION_10_8

#define SDL_VIDEO_DRIVER_X11_XINPUT2 1
#define SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1
#define SDL_VIDEO_DRIVER_X11_CONST_PARAM_XEXTADDDISPLAY 1
#endif

#ifndef SDL_VIDEO_RENDER_OGL
#define SDL_VIDEO_RENDER_OGL    1
#endif

#ifndef SDL_VIDEO_OPENGL
#define SDL_VIDEO_OPENGL    1
#endif
#ifndef SDL_VIDEO_OPENGL_CGL
#define SDL_VIDEO_OPENGL_CGL    1
#endif
#ifndef SDL_VIDEO_OPENGL_GLX
#define SDL_VIDEO_OPENGL_GLX    1
#endif

#define SDL_POWER_MACOSX 1

#define SDL_FILESYSTEM_COCOA   1

#define SDL_ASSEMBLY_ROUTINES   1
#ifdef __ppc__
#define SDL_ALTIVEC_BLITTERS    1
#endif

#endif

#elif defined(__LINUX__)

#ifndef _SDL_config_h
#define _SDL_config_h

#ifdef _MSC_VER
#error You should run hg revert SDL_config.h
#endif

#ifdef __LP64__
#define SIZEOF_VOIDP 8
#else
#define SIZEOF_VOIDP 4
#endif
#define HAVE_GCC_ATOMICS 1

#define HAVE_PTHREAD_SPINLOCK 1

#define HAVE_LIBC 1
#if HAVE_LIBC

#define HAVE_ALLOCA_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDIO_H 1
#define STDC_HEADERS 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_CTYPE_H 1
#define HAVE_MATH_H 1
#define HAVE_ICONV_H 1
#define HAVE_SIGNAL_H 1

#define HAVE_MALLOC 1
#define HAVE_CALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FREE 1
#define HAVE_ALLOCA 1
#ifndef __WIN32__
#define HAVE_GETENV 1
#define HAVE_SETENV 1
#define HAVE_PUTENV 1
#define HAVE_UNSETENV 1
#endif
#define HAVE_QSORT 1
#define HAVE_ABS 1
#define HAVE_BCOPY 1
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMCMP 1
#define HAVE_STRLEN 1

#define HAVE_STRDUP 1

#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1

#define HAVE_STRTOL 1
#define HAVE_STRTOUL 1

#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
#define HAVE_STRTOD 1
#define HAVE_ATOI 1
#define HAVE_ATOF 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1

#define HAVE_STRCASECMP 1

#define HAVE_STRNCASECMP 1

#define HAVE_VSSCANF 1

#define HAVE_VSNPRINTF 1
#define HAVE_M_PI
#define HAVE_ATAN 1
#define HAVE_ATAN2 1
#define HAVE_ACOS 1
#define HAVE_ASIN 1
#define HAVE_CEIL 1
#define HAVE_COPYSIGN 1
#define HAVE_COS 1
#define HAVE_COSF 1
#define HAVE_FABS 1
#define HAVE_FLOOR 1
#define HAVE_LOG 1
#define HAVE_POW 1
#define HAVE_SCALBN 1
#define HAVE_SIN 1
#define HAVE_SINF 1
#define HAVE_SQRT 1
#define HAVE_FSEEKO 1
#define HAVE_FSEEKO64 1
#define HAVE_SIGACTION 1
#define HAVE_SA_SIGACTION 1
#define HAVE_SETJMP 1
#define HAVE_NANOSLEEP 1
#define HAVE_SYSCONF 1

#define HAVE_CLOCK_GETTIME 1

#define HAVE_MPROTECT 1
#define HAVE_ICONV 1
#define HAVE_PTHREAD_SETNAME_NP 1

#define HAVE_SEM_TIMEDWAIT 1

#else
#define HAVE_STDARG_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#endif

#define SDL_AUDIO_DRIVER_ALSA 1
#if defined(__LP64__) || defined(_LP64) || defined(__LLP64__) || defined(__x86_64__)
#define SDL_AUDIO_DRIVER_ALSA_DYNAMIC "/usr/lib/x86_64-linux-gnu/libasound.so.2"
#else
#define SDL_AUDIO_DRIVER_ALSA_DYNAMIC "/usr/lib/i386-linux-gnu/libasound.so.2"
#endif

#define SDL_AUDIO_DRIVER_DISK 1
#define SDL_AUDIO_DRIVER_DUMMY 1

#define SDL_AUDIO_DRIVER_OSS 1

#define SDL_INPUT_LINUXEV 1
#define SDL_INPUT_LINUXKD 1

#define SDL_JOYSTICK_LINUX 1

#define SDL_HAPTIC_LINUX 1

#define SDL_LOADSO_DLOPEN 1

#define SDL_THREAD_PTHREAD 1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX 1

#define SDL_TIMER_UNIX 1

#define SDL_VIDEO_DRIVER_DUMMY 1

#define SDL_VIDEO_DRIVER_X11 1

#define SDL_VIDEO_DRIVER_X11_DYNAMIC "libX11.so.6"
#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT "libXext.so.6"

#define SDL_VIDEO_DRIVER_X11_DYNAMIC_XVIDMODE "libXxf86vm.so.1"

#define SDL_VIDEO_DRIVER_X11_XSHAPE 1
#define SDL_VIDEO_DRIVER_X11_XVIDMODE 1
#define SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1
#define SDL_VIDEO_DRIVER_X11_CONST_PARAM_XDATA32 1
#define SDL_VIDEO_DRIVER_X11_CONST_PARAM_XEXTADDDISPLAY 1
#define SDL_VIDEO_DRIVER_X11_HAS_XKBKEYCODETOKEYSYM 1

#define SDL_VIDEO_RENDER_OGL 1

#define SDL_VIDEO_OPENGL 1

#define SDL_VIDEO_OPENGL_GLX 1

#define SDL_POWER_LINUX 1

#define SDL_FILESYSTEM_UNIX 1

#define SDL_ASSEMBLY_ROUTINES 1

#endif

#else

#ifndef _SDL_config_minimal_h
#define _SDL_config_minimal_h

#define HAVE_STDARG_H   1
#define HAVE_STDDEF_H   1

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned int size_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long uintptr_t;
#else
#define HAVE_STDINT_H 1
#endif

#ifdef __GNUC__
#define HAVE_GCC_SYNC_LOCK_TEST_AND_SET 1
#endif

#define SDL_AUDIO_DRIVER_DUMMY  1

#define SDL_JOYSTICK_DISABLED   1

#define SDL_HAPTIC_DISABLED 1

#define SDL_LOADSO_DISABLED 1

#define SDL_THREADS_DISABLED    1

#define SDL_TIMERS_DISABLED 1

#define SDL_VIDEO_DRIVER_DUMMY  1

#define SDL_FILESYSTEM_DUMMY  1

#endif

#endif

#ifdef DECLSPEC
#undef DECLSPEC
#endif
#define DECLSPEC

#if (defined(_MSC_VER) && !defined(WINAPI_FAMILY))
//#pragma warning(disable:4002)
//#pragma warning(disable:4003)
//#pragma warning(disable:4018)
//#pragma warning(disable:4244)
#pragma once
#endif

#define SDL_INIT_AUDIO          0x00000010
#define SDL_INIT_VIDEO          0x00000020
#define SDL_INIT_EVENTS         0x00004000
#define SDL_QuitSubSystem(s) NULL
#define HAVE_ITOA 1
#undef SDL_AUDIO_DRIVER_XAUDIO2
#undef SDL_AUDIO_DRIVER_WINMM
#undef SDL_AUDIO_DRIVER_DUMMY
#undef SDL_AUDIO_DRIVER_DISK
#undef SDL_VIDEO_DRIVER_DUMMY
#undef SDL_VIDEO_RENDER_OGL_ES2
#undef SDL_VIDEO_OPENGL_ES2
#undef SDL_VIDEO_OPENGL_EGL
#undef SDL_FILESYSTEM_WINDOWS
#undef SDL_FILESYSTEM_COCOA
#undef SDL_FILESYSTEM_DUMMY
#undef SDL_FILESYSTEM_UNIX
#undef SDL_HAPTIC_DINPUT
#undef SDL_JOYSTICK_WINMM
#undef SDL_HAPTIC_IOKIT
#define SDL_DISABLE_WINDOWS_IME 1
#define SDL_DISABLE_SCANCODENAMES 1

#define _SDL_H
#define _SDL_error_h
#define _SDL_rwops_h
#define _SDL_hints_h
#define _SDL_assert_h
#define _SDL_main_h
#define _SDL_render_h
#define _SDL_gesture_c_h
#define _SDL_stdinc_h
static SDL_INLINE int SDL_SetError(const char *errstr, ...) { (void)errstr; return 1; }
#define SDL_assert(a)
#define SDL_Error(e)
#define SDL_ASSERT_LEVEL 0
#define SDL_ClearError()
static SDL_INLINE int SDL_OutOfMemory() { return 1; }
#define SDL_Unsupported() 1
#define SDL_InvalidParamError(p) 1
#define SDL_arraysize(array) (sizeof(array)/sizeof(array[0]))
#define SDL_RWops void
#define SDL_RWFromMem(b,l) NULL
#define SDL_RWread(ctx, ptr, size, n) 0
#define SDL_RWwrite(ctx, ptr, size, n) 0
#define SDL_RWclose(ctx) 0
#define SDL_WriteLE16(a, b) 0
#define SDL_WriteLE32(a, b) 0

#define SDL_GestureAddTouch(t) NULL
#define SDL_GestureProcessEvent(e) NULL

#define SDL_GetHint(n) NULL
#define SDL_AddHintCallback(n, f, x) NULL
#define SDL_DelHintCallback(n, f, x) NULL
#undef HAVE_M_PI
#include <stdlib.h>
#define HAVE_GETENV 1
#undef getenv
#define getenv(env) NULL
#define HAVE_STRDUP 1
#define HAVE_SNPRINTF 1
#include <stdio.h>
#ifndef snprintf
#define snprintf _snprintf
#endif

#define SDL_Renderer void
#define SDL_Texture void
typedef struct SDL_RendererInfo { const char* name; int num_texture_formats; int *texture_formats; } SDL_RendererInfo;
#define SDL_GetNumRenderDrivers() 0
#define SDL_GetRenderDriverInfo(i, info) { (info)->name = NULL; (info)->num_texture_formats = 0; }
#define SDL_CreateRenderer(a,b,c) NULL
#define SDL_DestroyRenderer(a)
#define SDL_DestroyTexture(a)
#define SDL_GetRendererInfo(a, b) NULL
#define SDL_TEXTUREACCESS_STREAMING 0
#define SDL_RenderSetViewport(a,b)
#define SDL_UpdateTexture(a,b,c,d) 0
#define SDL_RenderCopy(a,b,c,d) 0
#define SDL_RenderPresent(a)
#define SDL_CreateTexture(a, b, c, d, e) NULL

#define SDL_malloc(s) malloc(s)
#define SDL_calloc(n,s) calloc(n,s)
#define SDL_realloc(m,s) realloc(m,s)
#define SDL_free(m) free(m)
#define SDL_getenv(e) NULL
#define SDL_qsort(a,b,c,d) NULL
#define SDL_abs(v) abs(v)

#define SDL_iconv_utf8_locale(S) strdup(S)
#define SDL_iconv_string(t,f,S,l) strdup(S)

#ifdef __WIN32__
#define _INCLUDED_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#define STRICT 1
#define UNICODE 1
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#include <windows.h>
#undef CreateWindow
#ifdef __WINRT__
#define WIN_CoInitialize() S_OK
#define WIN_CoUninitialize() NULL
#else
#define WIN_CoInitialize() S_OK;{HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);if(hr==RPC_E_CHANGED_MODE)hr=CoInitializeEx(NULL,COINIT_MULTITHREADED);}
#define WIN_CoUninitialize() {CoUninitialize();}
#endif

#define WIN_SetError(e) -1
#define CreateWindow(a, b, c, d, e, f, g, h, i, j, k) CreateWindowW(a, b, c, d, e, f, g, h, i, j, k)
#endif

#ifndef _SDL_stdinc_h_ZL
#define _SDL_stdinc_h_ZL

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#if defined(STDC_HEADERS)
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
#else
# if defined(HAVE_STDLIB_H)
#  include <stdlib.h>
# elif defined(HAVE_MALLOC_H)
#  include <malloc.h>
# endif
# if defined(HAVE_STDDEF_H)
#  include <stddef.h>
# endif
# if defined(HAVE_STDARG_H)
#  include <stdarg.h>
# endif
#endif
#ifdef HAVE_STRING_H
# if !defined(STDC_HEADERS) && defined(HAVE_MEMORY_H)
#  include <memory.h>
# endif
# include <string.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif
#if defined(HAVE_INTTYPES_H)
# include <inttypes.h>
#elif defined(HAVE_STDINT_H)
# include <stdint.h>
#endif
#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif
#ifdef HAVE_MATH_H
# if defined(__WINRT__)

#  define _USE_MATH_DEFINES
# endif
# include <math.h>
#endif
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#if defined(HAVE_ICONV) && defined(HAVE_ICONV_H)
# include <iconv.h>
#endif

#define SDL_arraysize(array)    (sizeof(array)/sizeof(array[0]))
#define SDL_TABLESIZE(table)    SDL_arraysize(table)

#ifdef __cplusplus
#define SDL_reinterpret_cast(type, expression) reinterpret_cast<type>(expression)
#define SDL_static_cast(type, expression) static_cast<type>(expression)
#define SDL_const_cast(type, expression) const_cast<type>(expression)
#else
#define SDL_reinterpret_cast(type, expression) ((type)(expression))
#define SDL_static_cast(type, expression) ((type)(expression))
#define SDL_const_cast(type, expression) ((type)(expression))
#endif

#define SDL_FOURCC(A, B, C, D) \
    ((SDL_static_cast(Uint32, SDL_static_cast(Uint8, (A))) << 0) | \
     (SDL_static_cast(Uint32, SDL_static_cast(Uint8, (B))) << 8) | \
     (SDL_static_cast(Uint32, SDL_static_cast(Uint8, (C))) << 16) | \
     (SDL_static_cast(Uint32, SDL_static_cast(Uint8, (D))) << 24))

typedef enum
{
    SDL_FALSE = 0,
    SDL_TRUE = 1
} SDL_bool;

typedef int8_t Sint8;

typedef uint8_t Uint8;

typedef int16_t Sint16;

typedef uint16_t Uint16;

typedef int32_t Sint32;

typedef uint32_t Uint32;

typedef int64_t Sint64;

typedef uint64_t Uint64;

#define SDL_COMPILE_TIME_ASSERT(name, x)               \
       typedef int SDL_dummy_ ## name[(x) * 2 - 1]

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
SDL_COMPILE_TIME_ASSERT(uint8, sizeof(Uint8) == 1);
SDL_COMPILE_TIME_ASSERT(sint8, sizeof(Sint8) == 1);
SDL_COMPILE_TIME_ASSERT(uint16, sizeof(Uint16) == 2);
SDL_COMPILE_TIME_ASSERT(sint16, sizeof(Sint16) == 2);
SDL_COMPILE_TIME_ASSERT(uint32, sizeof(Uint32) == 4);
SDL_COMPILE_TIME_ASSERT(sint32, sizeof(Sint32) == 4);
SDL_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
SDL_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);
#endif

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#if !defined(__ANDROID__)

typedef enum
{
    DUMMY_ENUM_VALUE
} SDL_DUMMY_ENUM;

SDL_COMPILE_TIME_ASSERT(enum, sizeof(SDL_DUMMY_ENUM) == sizeof(int));
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

#if defined(HAVE_ALLOCA) && !defined(alloca)
# if defined(HAVE_ALLOCA_H)
#  include <alloca.h>
# elif defined(__GNUC__)
#  define alloca __builtin_alloca
# elif defined(_MSC_VER)
#  include <malloc.h>
#  define alloca _alloca
# elif defined(__WATCOMC__)
#  include <malloc.h>
# elif defined(__BORLANDC__)
#  include <malloc.h>
# elif defined(__DMC__)
#  include <stdlib.h>
# elif defined(__AIX__)
#pragma alloca
# elif defined(__MRC__)
void *alloca(unsigned);
# else
char *alloca();
# endif
#endif
#ifdef HAVE_ALLOCA
#define SDL_stack_alloc(type, count)    (type*)alloca(sizeof(type)*(count))
#define SDL_stack_free(data)
#else
#define SDL_stack_alloc(type, count)    (type*)malloc(sizeof(type)*(count))
#define SDL_stack_free(data)            free(data)
#endif

#define SDL_min(x, y) (((x) < (y)) ? (x) : (y))
#define SDL_max(x, y) (((x) > (y)) ? (x) : (y))

#define SDL_isdigit isdigit
#define SDL_isspace isspace
#define SDL_toupper toupper
#define SDL_tolower tolower
#define SDL_memset memset
#define SDL_zero(x) memset(&(x), 0, sizeof((x)))
#define SDL_zerop(x) memset((x), 0, sizeof(*(x)))
#define SDL_memset4(dst, val, dwords) memset((dst), (val), (dwords)*4)
#define SDL_memcpy memcpy
#define SDL_memcpy4(dst, src, dwords) memcpy((dst), (src), (dwords)*4)
#define SDL_memmove memmove
#define SDL_memcmp memcmp
#define SDL_strlen strlen
#define SDL_strlcpy(dst,src,maxlen) strncpy(dst,src,maxlen-1)
#define SDL_utf8strlcpy(dst,src,dst_bytes) strncpy(dst,src,dst_bytes-1)
#define SDL_strlcat(dst,src,maxlen) strcat(dst,src)
#define SDL_strdup strdup
#define SDL_strchr strchr
#define SDL_strrchr strrchr
#define SDL_strstr strstr

#define SDL_itoa(i, s, rdx) sprintf(s, "%i", (int)i); { typedef int _chk[(rdx==10)?1:-1]; }

#define SDL_atoi atoi
#define SDL_atof atof
#define SDL_strtol strtol
#define SDL_strtoul strtoul
#define SDL_strtod strtod

#define SDL_strcmp strcmp
#define SDL_strncmp strncmp
#define SDL_strcasecmp strcmp
#define SDL_strncasecmp strncmp
#if __STDC_WANT_SECURE_LIB__
static SDL_INLINE int SDL_snprintf(char *text, size_t maxlen, const char *fmt, ...) { va_list ap; va_start(ap, fmt); return vsprintf_s(text, maxlen, fmt, ap); }
#else
static SDL_INLINE int SDL_snprintf(char *text, size_t maxlen, const char *fmt, ...) { va_list ap; (void)maxlen; va_start(ap, fmt); return vsprintf(text, fmt, ap); }
#endif
#define SDL_sscanf(t,f,v) NULL

#ifndef HAVE_M_PI
#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288
#endif
#endif

#define SDL_ceil ceil

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

#ifdef __cplusplus
extern "C" {
#endif
extern DECLSPEC void SDLCALL SDL_Delay(Uint32 ms);
extern DECLSPEC Uint32 SDLCALL SDL_WasInit(Uint32 flags);
extern DECLSPEC int SDLCALL SDL_InitSubSystem(Uint32 flags);
#ifdef __WIN32__
extern DECLSPEC int SDLCALL SDL_RegisterApp(char *name, Uint32 style, void *hInst);
extern DECLSPEC void SDLCALL SDL_UnregisterApp(void);
char* WIN_StringToUTF8(const unsigned short* s);
unsigned short* WIN_UTF8ToString(const char* s);
#endif
#define SDL_TLSCreate() 0
#if defined(__LP64__) || defined(_LP64) || defined(__LLP64__) || defined(__x86_64__) || defined(__ia64__) || defined(_WIN64) || defined(_M_X64)
unsigned int SDL_TLSSet64(void*);
void* SDL_TLSGet64(unsigned int);
#define SDL_TLSSet(tls, v, cb) tls = SDL_TLSSet64(v)
#define SDL_TLSGet(tls) SDL_TLSGet64(tls)
#else
#define SDL_TLSSet(tls, v, cb) tls = (SDL_TLSID)(v)
#define SDL_TLSGet(tls) (tls)
#endif
#ifdef __cplusplus
}
#endif

#endif

#ifndef _SDL_audio_h
#define _SDL_audio_h

#ifndef _SDL_error_h
#define _SDL_error_h

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

extern DECLSPEC int SDLCALL SDL_SetError(const char *fmt, ...);
extern DECLSPEC const char *SDLCALL SDL_GetError(void);
extern DECLSPEC void SDLCALL SDL_ClearError(void);

#define SDL_OutOfMemory()   SDL_Error(SDL_ENOMEM)
#define SDL_Unsupported()   SDL_Error(SDL_UNSUPPORTED)
#define SDL_InvalidParamError(param)    SDL_SetError("Parameter '%s' is invalid", (param))
typedef enum
{
    SDL_ENOMEM,
    SDL_EFREAD,
    SDL_EFWRITE,
    SDL_EFSEEK,
    SDL_UNSUPPORTED,
    SDL_LASTERROR
} SDL_errorcode;

extern DECLSPEC int SDLCALL SDL_Error(SDL_errorcode code);

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

#ifndef _SDL_endian_h
#define _SDL_endian_h

#define SDL_LIL_ENDIAN  1234
#define SDL_BIG_ENDIAN  4321

#ifndef SDL_BYTEORDER
#ifdef __linux__
#include <endian.h>
#define SDL_BYTEORDER  __BYTE_ORDER
#else
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define SDL_BYTEORDER   SDL_BIG_ENDIAN
#else
#define SDL_BYTEORDER   SDL_LIL_ENDIAN
#endif
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

#if defined(__GNUC__) && defined(__i386__) && \
   !(__GNUC__ == 2 && __GNUC_MINOR__ == 95)
SDL_FORCE_INLINE Uint16
SDL_Swap16(Uint16 x)
{
  __asm__("xchgb %b0,%h0": "=q"(x):"0"(x));
    return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
SDL_FORCE_INLINE Uint16
SDL_Swap16(Uint16 x)
{
  __asm__("xchgb %b0,%h0": "=Q"(x):"0"(x));
    return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
SDL_FORCE_INLINE Uint16
SDL_Swap16(Uint16 x)
{
    int result;

  __asm__("rlwimi %0,%2,8,16,23": "=&r"(result):"0"(x >> 8), "r"(x));
    return (Uint16)result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
SDL_FORCE_INLINE Uint16
SDL_Swap16(Uint16 x)
{
  __asm__("rorw #8,%0": "=d"(x): "0"(x):"cc");
    return x;
}
#else
SDL_FORCE_INLINE Uint16
SDL_Swap16(Uint16 x)
{
    return SDL_static_cast(Uint16, ((x << 8) | (x >> 8)));
}
#endif

#if defined(__GNUC__) && defined(__i386__)
SDL_FORCE_INLINE Uint32
SDL_Swap32(Uint32 x)
{
  __asm__("bswap %0": "=r"(x):"0"(x));
    return x;
}
#elif defined(__GNUC__) && defined(__x86_64__)
SDL_FORCE_INLINE Uint32
SDL_Swap32(Uint32 x)
{
  __asm__("bswapl %0": "=r"(x):"0"(x));
    return x;
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
SDL_FORCE_INLINE Uint32
SDL_Swap32(Uint32 x)
{
    Uint32 result;

  __asm__("rlwimi %0,%2,24,16,23": "=&r"(result):"0"(x >> 24), "r"(x));
  __asm__("rlwimi %0,%2,8,8,15": "=&r"(result):"0"(result), "r"(x));
  __asm__("rlwimi %0,%2,24,0,7": "=&r"(result):"0"(result), "r"(x));
    return result;
}
#elif defined(__GNUC__) && (defined(__M68000__) || defined(__M68020__)) && !defined(__mcoldfire__)
SDL_FORCE_INLINE Uint32
SDL_Swap32(Uint32 x)
{
  __asm__("rorw #8,%0\n\tswap %0\n\trorw #8,%0": "=d"(x): "0"(x):"cc");
    return x;
}
#else
SDL_FORCE_INLINE Uint32
SDL_Swap32(Uint32 x)
{
    return SDL_static_cast(Uint32, ((x << 24) | ((x << 8) & 0x00FF0000) |
                                    ((x >> 8) & 0x0000FF00) | (x >> 24)));
}
#endif

#if defined(__GNUC__) && defined(__i386__)
SDL_FORCE_INLINE Uint64
SDL_Swap64(Uint64 x)
{
    union
    {
        struct
        {
            Uint32 a, b;
        } s;
        Uint64 u;
    } v;
    v.u = x;
  __asm__("bswapl %0 ; bswapl %1 ; xchgl %0,%1": "=r"(v.s.a), "=r"(v.s.b):"0"(v.s.a),
            "1"(v.s.
                b));
    return v.u;
}
#elif defined(__GNUC__) && defined(__x86_64__)
SDL_FORCE_INLINE Uint64
SDL_Swap64(Uint64 x)
{
  __asm__("bswapq %0": "=r"(x):"0"(x));
    return x;
}
#else
SDL_FORCE_INLINE Uint64
SDL_Swap64(Uint64 x)
{
    Uint32 hi, lo;

    lo = SDL_static_cast(Uint32, x & 0xFFFFFFFF);
    x >>= 32;
    hi = SDL_static_cast(Uint32, x & 0xFFFFFFFF);
    x = SDL_Swap32(lo);
    x <<= 32;
    x |= SDL_Swap32(hi);
    return (x);
}
#endif

SDL_FORCE_INLINE float
SDL_SwapFloat(float x)
{
    union
    {
        float f;
        Uint32 ui32;
    } swapper;
    swapper.f = x;
    swapper.ui32 = SDL_Swap32(swapper.ui32);
    return swapper.f;
}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SDL_SwapLE16(X) (X)
#define SDL_SwapLE32(X) (X)
#define SDL_SwapLE64(X) (X)
#define SDL_SwapFloatLE(X)  (X)
#define SDL_SwapBE16(X) SDL_Swap16(X)
#define SDL_SwapBE32(X) SDL_Swap32(X)
#define SDL_SwapBE64(X) SDL_Swap64(X)
#define SDL_SwapFloatBE(X)  SDL_SwapFloat(X)
#else
#define SDL_SwapLE16(X) SDL_Swap16(X)
#define SDL_SwapLE32(X) SDL_Swap32(X)
#define SDL_SwapLE64(X) SDL_Swap64(X)
#define SDL_SwapFloatLE(X)  SDL_SwapFloat(X)
#define SDL_SwapBE16(X) (X)
#define SDL_SwapBE32(X) (X)
#define SDL_SwapBE64(X) (X)
#define SDL_SwapFloatBE(X)  (X)
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

#ifndef _SDL_mutex_h
#define _SDL_mutex_h

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

#define SDL_MUTEX_TIMEDOUT  1

#define SDL_MUTEX_MAXWAIT   (~(Uint32)0)

struct SDL_mutex;
typedef struct SDL_mutex SDL_mutex;

extern DECLSPEC SDL_mutex *SDLCALL SDL_CreateMutex(void);

#define SDL_mutexP(m)   SDL_LockMutex(m)
extern DECLSPEC int SDLCALL SDL_LockMutex(SDL_mutex * mutex);

extern DECLSPEC int SDLCALL SDL_TryLockMutex(SDL_mutex * mutex);

#define SDL_mutexV(m)   SDL_UnlockMutex(m)
extern DECLSPEC int SDLCALL SDL_UnlockMutex(SDL_mutex * mutex);

extern DECLSPEC void SDLCALL SDL_DestroyMutex(SDL_mutex * mutex);

struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;

extern DECLSPEC SDL_sem *SDLCALL SDL_CreateSemaphore(Uint32 initial_value);

extern DECLSPEC void SDLCALL SDL_DestroySemaphore(SDL_sem * sem);

extern DECLSPEC int SDLCALL SDL_SemWait(SDL_sem * sem);

extern DECLSPEC int SDLCALL SDL_SemTryWait(SDL_sem * sem);

extern DECLSPEC int SDLCALL SDL_SemWaitTimeout(SDL_sem * sem, Uint32 ms);

extern DECLSPEC int SDLCALL SDL_SemPost(SDL_sem * sem);

extern DECLSPEC Uint32 SDLCALL SDL_SemValue(SDL_sem * sem);

struct SDL_cond;
typedef struct SDL_cond SDL_cond;

extern DECLSPEC SDL_cond *SDLCALL SDL_CreateCond(void);

extern DECLSPEC void SDLCALL SDL_DestroyCond(SDL_cond * cond);

extern DECLSPEC int SDLCALL SDL_CondSignal(SDL_cond * cond);

extern DECLSPEC int SDLCALL SDL_CondBroadcast(SDL_cond * cond);

extern DECLSPEC int SDLCALL SDL_CondWait(SDL_cond * cond, SDL_mutex * mutex);

extern DECLSPEC int SDLCALL SDL_CondWaitTimeout(SDL_cond * cond,
                                                SDL_mutex * mutex, Uint32 ms);

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

#ifndef _SDL_thread_h
#define _SDL_thread_h

#ifndef _SDL_atomic_h_
#define _SDL_atomic_h_

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

typedef int SDL_SpinLock;

extern DECLSPEC SDL_bool SDLCALL SDL_AtomicTryLock(SDL_SpinLock *lock);

extern DECLSPEC void SDLCALL SDL_AtomicLock(SDL_SpinLock *lock);

extern DECLSPEC void SDLCALL SDL_AtomicUnlock(SDL_SpinLock *lock);

#if defined(_MSC_VER) && (_MSC_VER > 1200)
void _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)
#define SDL_CompilerBarrier()   _ReadWriteBarrier()
#elif defined(__GNUC__)
#define SDL_CompilerBarrier()   __asm__ __volatile__ ("" : : : "memory")
#else
#define SDL_CompilerBarrier()   \
{ SDL_SpinLock _tmp = 0; SDL_AtomicLock(&_tmp); SDL_AtomicUnlock(&_tmp); }
#endif

#if defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
#define SDL_MemoryBarrierRelease()   __asm__ __volatile__ ("lwsync" : : : "memory")
#define SDL_MemoryBarrierAcquire()   __asm__ __volatile__ ("lwsync" : : : "memory")
#elif defined(__GNUC__) && defined(__arm__)
#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define SDL_MemoryBarrierRelease()   __asm__ __volatile__ ("dmb ish" : : : "memory")
#define SDL_MemoryBarrierAcquire()   __asm__ __volatile__ ("dmb ish" : : : "memory")
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#ifdef __thumb__

extern DECLSPEC void SDLCALL SDL_MemoryBarrierRelease();
extern DECLSPEC void SDLCALL SDL_MemoryBarrierAcquire();
#else
#define SDL_MemoryBarrierRelease()   __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r"(0) : "memory")
#define SDL_MemoryBarrierAcquire()   __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r"(0) : "memory")
#endif
#else
#define SDL_MemoryBarrierRelease()   __asm__ __volatile__ ("" : : : "memory")
#define SDL_MemoryBarrierAcquire()   __asm__ __volatile__ ("" : : : "memory")
#endif
#else

#define SDL_MemoryBarrierRelease()  SDL_CompilerBarrier()
#define SDL_MemoryBarrierAcquire()  SDL_CompilerBarrier()
#endif

typedef struct { int value; } SDL_atomic_t;

extern DECLSPEC SDL_bool SDLCALL SDL_AtomicCAS(SDL_atomic_t *a, int oldval, int newval);

extern DECLSPEC int SDLCALL SDL_AtomicSet(SDL_atomic_t *a, int v);

extern DECLSPEC int SDLCALL SDL_AtomicGet(SDL_atomic_t *a);

extern DECLSPEC int SDLCALL SDL_AtomicAdd(SDL_atomic_t *a, int v);

#ifndef SDL_AtomicIncRef
#define SDL_AtomicIncRef(a)    SDL_AtomicAdd(a, 1)
#endif

#ifndef SDL_AtomicDecRef
#define SDL_AtomicDecRef(a)    (SDL_AtomicAdd(a, -1) == 1)
#endif

extern DECLSPEC SDL_bool SDLCALL SDL_AtomicCASPtr(void **a, void *oldval, void *newval);

extern DECLSPEC void* SDLCALL SDL_AtomicSetPtr(void **a, void* v);

extern DECLSPEC void* SDLCALL SDL_AtomicGetPtr(void **a);

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

struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

typedef unsigned long SDL_threadID;

typedef unsigned int SDL_TLSID;

typedef enum {
    SDL_THREAD_PRIORITY_LOW,
    SDL_THREAD_PRIORITY_NORMAL,
    SDL_THREAD_PRIORITY_HIGH
} SDL_ThreadPriority;

typedef int (SDLCALL * SDL_ThreadFunction) (void *data);

#if defined(__WIN32__) && !defined(HAVE_LIBC)

#define SDL_PASSED_BEGINTHREAD_ENDTHREAD
#include <process.h>

typedef uintptr_t(__cdecl * pfnSDL_CurrentBeginThread) (void *, unsigned,
                                                        unsigned (__stdcall *
                                                                  func) (void
                                                                         *),
                                                        void *arg, unsigned,
                                                        unsigned *threadID);
typedef void (__cdecl * pfnSDL_CurrentEndThread) (unsigned code);

extern DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data,
                 pfnSDL_CurrentBeginThread pfnBeginThread,
                 pfnSDL_CurrentEndThread pfnEndThread);

#if defined(SDL_CreateThread) && SDL_DYNAMIC_API
#undef SDL_CreateThread
#define SDL_CreateThread(fn, name, data) SDL_CreateThread_REAL(fn, name, data, (pfnSDL_CurrentBeginThread)_beginthreadex, (pfnSDL_CurrentEndThread)_endthreadex)
#else
#define SDL_CreateThread(fn, name, data) SDL_CreateThread(fn, name, data, (pfnSDL_CurrentBeginThread)_beginthreadex, (pfnSDL_CurrentEndThread)_endthreadex)
#endif

#else

extern DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data);

#endif

extern DECLSPEC const char *SDLCALL SDL_GetThreadName(SDL_Thread *thread);

extern DECLSPEC SDL_threadID SDLCALL SDL_ThreadID(void);

extern DECLSPEC SDL_threadID SDLCALL SDL_GetThreadID(SDL_Thread * thread);

extern DECLSPEC int SDLCALL SDL_SetThreadPriority(SDL_ThreadPriority priority);

extern DECLSPEC void SDLCALL SDL_WaitThread(SDL_Thread * thread, int *status);

extern DECLSPEC void SDLCALL SDL_DetachThread(SDL_Thread * thread);

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

#ifndef _SDL_rwops_h
#define _SDL_rwops_h

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

#define SDL_RWOPS_UNKNOWN   0
#define SDL_RWOPS_WINFILE   1
#define SDL_RWOPS_STDFILE   2
#define SDL_RWOPS_JNIFILE   3
#define SDL_RWOPS_MEMORY    4
#define SDL_RWOPS_MEMORY_RO 5

typedef struct SDL_RWops
{

    Sint64 (SDLCALL * size) (struct SDL_RWops * context);

    Sint64 (SDLCALL * seek) (struct SDL_RWops * context, Sint64 offset,
                             int whence);

    size_t (SDLCALL * read) (struct SDL_RWops * context, void *ptr,
                             size_t size, size_t maxnum);

    size_t (SDLCALL * write) (struct SDL_RWops * context, const void *ptr,
                              size_t size, size_t num);

    int (SDLCALL * close) (struct SDL_RWops * context);

    Uint32 type;
    union
    {
#if defined(ANDROID)
        struct
        {
            void *fileNameRef;
            void *inputStreamRef;
            void *readableByteChannelRef;
            void *readMethod;
            void *assetFileDescriptorRef;
            long position;
            long size;
            long offset;
            int fd;
        } androidio;
#elif defined(__WIN32__)
        struct
        {
            SDL_bool append;
            void *h;
            struct
            {
                void *data;
                size_t size;
                size_t left;
            } buffer;
        } windowsio;
#endif

#ifdef HAVE_STDIO_H
        struct
        {
            SDL_bool autoclose;
            FILE *fp;
        } stdio;
#endif
        struct
        {
            Uint8 *base;
            Uint8 *here;
            Uint8 *stop;
        } mem;
        struct
        {
            void *data1;
            void *data2;
        } unknown;
    } hidden;

} SDL_RWops;

extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFile(const char *file,
                                                  const char *mode);

#ifdef HAVE_STDIO_H
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFP(FILE * fp,
                                                SDL_bool autoclose);
#else
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFP(void * fp,
                                                SDL_bool autoclose);
#endif

extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromMem(void *mem, int size);
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromConstMem(const void *mem,
                                                      int size);

extern DECLSPEC SDL_RWops *SDLCALL SDL_AllocRW(void);
extern DECLSPEC void SDLCALL SDL_FreeRW(SDL_RWops * area);

#define RW_SEEK_SET 0
#define RW_SEEK_CUR 1
#define RW_SEEK_END 2

#define SDL_RWsize(ctx)         (ctx)->size(ctx)
#define SDL_RWseek(ctx, offset, whence) (ctx)->seek(ctx, offset, whence)
#define SDL_RWtell(ctx)         (ctx)->seek(ctx, 0, RW_SEEK_CUR)
#define SDL_RWread(ctx, ptr, size, n)   (ctx)->read(ctx, ptr, size, n)
#define SDL_RWwrite(ctx, ptr, size, n)  (ctx)->write(ctx, ptr, size, n)
#define SDL_RWclose(ctx)        (ctx)->close(ctx)

extern DECLSPEC Uint8 SDLCALL SDL_ReadU8(SDL_RWops * src);
extern DECLSPEC Uint16 SDLCALL SDL_ReadLE16(SDL_RWops * src);
extern DECLSPEC Uint16 SDLCALL SDL_ReadBE16(SDL_RWops * src);
extern DECLSPEC Uint32 SDLCALL SDL_ReadLE32(SDL_RWops * src);
extern DECLSPEC Uint32 SDLCALL SDL_ReadBE32(SDL_RWops * src);
extern DECLSPEC Uint64 SDLCALL SDL_ReadLE64(SDL_RWops * src);
extern DECLSPEC Uint64 SDLCALL SDL_ReadBE64(SDL_RWops * src);

extern DECLSPEC size_t SDLCALL SDL_WriteU8(SDL_RWops * dst, Uint8 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE16(SDL_RWops * dst, Uint16 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE16(SDL_RWops * dst, Uint16 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE32(SDL_RWops * dst, Uint32 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE32(SDL_RWops * dst, Uint32 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE64(SDL_RWops * dst, Uint64 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE64(SDL_RWops * dst, Uint64 value);

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

typedef Uint16 SDL_AudioFormat;

#define SDL_AUDIO_MASK_BITSIZE       (0xFF)
#define SDL_AUDIO_MASK_DATATYPE      (1<<8)
#define SDL_AUDIO_MASK_ENDIAN        (1<<12)
#define SDL_AUDIO_MASK_SIGNED        (1<<15)
#define SDL_AUDIO_BITSIZE(x)         (x & SDL_AUDIO_MASK_BITSIZE)
#define SDL_AUDIO_ISFLOAT(x)         (x & SDL_AUDIO_MASK_DATATYPE)
#define SDL_AUDIO_ISBIGENDIAN(x)     (x & SDL_AUDIO_MASK_ENDIAN)
#define SDL_AUDIO_ISSIGNED(x)        (x & SDL_AUDIO_MASK_SIGNED)
#define SDL_AUDIO_ISINT(x)           (!SDL_AUDIO_ISFLOAT(x))
#define SDL_AUDIO_ISLITTLEENDIAN(x)  (!SDL_AUDIO_ISBIGENDIAN(x))
#define SDL_AUDIO_ISUNSIGNED(x)      (!SDL_AUDIO_ISSIGNED(x))

#define AUDIO_U8        0x0008
#define AUDIO_S8        0x8008
#define AUDIO_U16LSB    0x0010
#define AUDIO_S16LSB    0x8010
#define AUDIO_U16MSB    0x1010
#define AUDIO_S16MSB    0x9010
#define AUDIO_U16       AUDIO_U16LSB
#define AUDIO_S16       AUDIO_S16LSB

#define AUDIO_S32LSB    0x8020
#define AUDIO_S32MSB    0x9020
#define AUDIO_S32       AUDIO_S32LSB

#define AUDIO_F32LSB    0x8120
#define AUDIO_F32MSB    0x9120
#define AUDIO_F32       AUDIO_F32LSB

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_U16SYS    AUDIO_U16LSB
#define AUDIO_S16SYS    AUDIO_S16LSB
#define AUDIO_S32SYS    AUDIO_S32LSB
#define AUDIO_F32SYS    AUDIO_F32LSB
#else
#define AUDIO_U16SYS    AUDIO_U16MSB
#define AUDIO_S16SYS    AUDIO_S16MSB
#define AUDIO_S32SYS    AUDIO_S32MSB
#define AUDIO_F32SYS    AUDIO_F32MSB
#endif

#define SDL_AUDIO_ALLOW_FREQUENCY_CHANGE    0x00000001
#define SDL_AUDIO_ALLOW_FORMAT_CHANGE       0x00000002
#define SDL_AUDIO_ALLOW_CHANNELS_CHANGE     0x00000004
#define SDL_AUDIO_ALLOW_ANY_CHANGE          (SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE)

typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream,
                                            int len);

typedef struct SDL_AudioSpec
{
    int freq;
    SDL_AudioFormat format;
    Uint8 channels;
    Uint8 silence;
    Uint16 samples;
    Uint16 padding;
    Uint32 size;
    SDL_AudioCallback callback;
    void *userdata;
} SDL_AudioSpec;

struct SDL_AudioCVT;
typedef void (SDLCALL * SDL_AudioFilter) (struct SDL_AudioCVT * cvt,
                                          SDL_AudioFormat format);

#ifdef __GNUC__

#define SDL_AUDIOCVT_PACKED __attribute__((packed))
#else
#define SDL_AUDIOCVT_PACKED
#endif

typedef struct SDL_AudioCVT
{
    int needed;
    SDL_AudioFormat src_format;
    SDL_AudioFormat dst_format;
    double rate_incr;
    Uint8 *buf;
    int len;
    int len_cvt;
    int len_mult;
    double len_ratio;
    SDL_AudioFilter filters[10];
    int filter_index;
} SDL_AUDIOCVT_PACKED SDL_AudioCVT;

extern DECLSPEC int SDLCALL SDL_GetNumAudioDrivers(void);
extern DECLSPEC const char *SDLCALL SDL_GetAudioDriver(int index);

extern DECLSPEC int SDLCALL SDL_AudioInit(const char *driver_name);
extern DECLSPEC void SDLCALL SDL_AudioQuit(void);

extern DECLSPEC const char *SDLCALL SDL_GetCurrentAudioDriver(void);

extern DECLSPEC int SDLCALL SDL_OpenAudio(SDL_AudioSpec * desired,
                                          SDL_AudioSpec * obtained);

typedef Uint32 SDL_AudioDeviceID;

extern DECLSPEC int SDLCALL SDL_GetNumAudioDevices(int iscapture);

extern DECLSPEC const char *SDLCALL SDL_GetAudioDeviceName(int index,
                                                           int iscapture);

extern DECLSPEC SDL_AudioDeviceID SDLCALL SDL_OpenAudioDevice(const char
                                                              *device,
                                                              int iscapture,
                                                              const
                                                              SDL_AudioSpec *
                                                              desired,
                                                              SDL_AudioSpec *
                                                              obtained,
                                                              int
                                                              allowed_changes);

typedef enum
{
    SDL_AUDIO_STOPPED = 0,
    SDL_AUDIO_PLAYING,
    SDL_AUDIO_PAUSED
} SDL_AudioStatus;
extern DECLSPEC SDL_AudioStatus SDLCALL SDL_GetAudioStatus(void);

extern DECLSPEC SDL_AudioStatus SDLCALL
SDL_GetAudioDeviceStatus(SDL_AudioDeviceID dev);

extern DECLSPEC void SDLCALL SDL_PauseAudio(int pause_on);
extern DECLSPEC void SDLCALL SDL_PauseAudioDevice(SDL_AudioDeviceID dev,
                                                  int pause_on);

extern DECLSPEC SDL_AudioSpec *SDLCALL SDL_LoadWAV_RW(SDL_RWops * src,
                                                      int freesrc,
                                                      SDL_AudioSpec * spec,
                                                      Uint8 ** audio_buf,
                                                      Uint32 * audio_len);

#define SDL_LoadWAV(file, spec, audio_buf, audio_len) \
    SDL_LoadWAV_RW(SDL_RWFromFile(file, "rb"),1, spec,audio_buf,audio_len)

extern DECLSPEC void SDLCALL SDL_FreeWAV(Uint8 * audio_buf);

extern DECLSPEC int SDLCALL SDL_BuildAudioCVT(SDL_AudioCVT * cvt,
                                              SDL_AudioFormat src_format,
                                              Uint8 src_channels,
                                              int src_rate,
                                              SDL_AudioFormat dst_format,
                                              Uint8 dst_channels,
                                              int dst_rate);

extern DECLSPEC int SDLCALL SDL_ConvertAudio(SDL_AudioCVT * cvt);

#define SDL_MIX_MAXVOLUME 128

extern DECLSPEC void SDLCALL SDL_MixAudio(Uint8 * dst, const Uint8 * src,
                                          Uint32 len, int volume);

extern DECLSPEC void SDLCALL SDL_MixAudioFormat(Uint8 * dst,
                                                const Uint8 * src,
                                                SDL_AudioFormat format,
                                                Uint32 len, int volume);

extern DECLSPEC void SDLCALL SDL_LockAudio(void);
extern DECLSPEC void SDLCALL SDL_LockAudioDevice(SDL_AudioDeviceID dev);
extern DECLSPEC void SDLCALL SDL_UnlockAudio(void);
extern DECLSPEC void SDLCALL SDL_UnlockAudioDevice(SDL_AudioDeviceID dev);

extern DECLSPEC void SDLCALL SDL_CloseAudio(void);
extern DECLSPEC void SDLCALL SDL_CloseAudioDevice(SDL_AudioDeviceID dev);

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
