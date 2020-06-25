#ifndef _EINA_API_H
#define _EINA_API_H

#ifdef EINA_API
#error EINA_API should not be already defined
#endif

#ifdef _WIN32
# ifndef EINA_STATIC
#  ifdef EINA_BUILD
#   define EINA_API __declspec(dllexport)
#  else
#   define EINA_API __declspec(dllimport)
#  endif
# else
#  define EINA_API
# endif
# define EINA_API_WEAK
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EINA_API __attribute__ ((visibility("default")))
#   define EINA_API_WEAK __attribute__ ((weak))
#  else
#   define EINA_API
#   define EINA_API_WEAK
#  endif
# else
#  define EINA_API
#  define EINA_API_WEAK
# endif
#endif

#endif
