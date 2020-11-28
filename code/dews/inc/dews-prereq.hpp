#pragma once

#include <vector>
#include <string>
#include <functional>
#include <cstdint>

#ifdef WIN32
#	ifdef API_DEV_MOD
#		define	DEWS_API __declspec(dllexport)
#	else
#		define	DEWS_API __declspec(dllimport)
#	endif
#else
#	define DEWS_API
#endif


#define   DEWS_IN
#define   DEWS_OUT
#define   DEWS_REF

#define   DEWS_HIDDEN_TYPE  void*




