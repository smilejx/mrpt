/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2020, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include "system-precomp.h"	 // Precompiled headers
//
#include <mrpt/config.h>
#include <mrpt/system/thread_name.h>

// Based on https://stackoverflow.com/a/23899379

#if defined(_WIN32)
#include <windows.h>
#include <winnt.h>
#include <winternl.h>

#pragma pack(push, 8)
typedef struct
{
	DWORD dwType;
	LPCSTR szName;
	DWORD dwThreadID;
	DWORD dwFlags;
} THREADNAME_INFO;
#pragma pack(pop)

static EXCEPTION_DISPOSITION NTAPI
	ignore_handler(EXCEPTION_RECORD* rec, void* frame, CONTEXT* ctx, void* disp)
{
	return ExceptionContinueExecution;
}

// dwThreadID=-1 for current
static void set_thread_name(uint32_t dwThreadID, const char* name)
{
	static const DWORD MS_VC_EXCEPTION = 0x406D1388;

	// Don't bother if a debugger isn't attached to receive the event
	if (!IsDebuggerPresent()) return;

	// Thread information for VS compatible debugger. -1 sets current thread.
	THREADNAME_INFO ti = {
		.dwType = 0x1000,
		.szName = name,
		.dwThreadID = dwThreadID,
	};

	// Push an exception handler to ignore all following exceptions
	NT_TIB* tib = ((NT_TIB*)NtCurrentTeb());
	EXCEPTION_REGISTRATION_RECORD rec = {
		.Next = tib->ExceptionList,
		.Handler = ignore_handler,
	};
	tib->ExceptionList = &rec;

	// Visual Studio and compatible debuggers receive thread names from the
	// program through a specially crafted exception
	RaiseException(
		MS_VC_EXCEPTION, 0, sizeof(ti) / sizeof(ULONG_PTR), (ULONG_PTR*)&ti);

	// Pop exception handler
	tib->ExceptionList = tib->ExceptionList->Next;
}

#else
#include <sys/prctl.h>

static void SetThreadName(std::thread& thread, const char* threadName)
{
	auto handle = thread.native_handle();
	pthread_setname_np(handle, threadName);
}

static std::string GetThreadName(std::thread& thread)
{
	auto handle = thread.native_handle();
	char buf[1000];
	buf[0] = '\0';
	pthread_getname_np(handle, buf, sizeof(buf));
	return std::string(buf);
}

static void SetThreadName(const char* threadName)
{
	prctl(PR_SET_NAME, threadName, 0L, 0L, 0L);
}
static std::string GetThreadName()
{
	char buf[100] = {0};
	prctl(PR_GET_NAME, buf, 0L, 0L, 0L);
	return std::string(buf);
}
#endif

void mrpt::system::thread_name(const std::string& name)
{
#if defined(_WIN32)
	set_thread_name(-1, name.c_str());
#else
	SetThreadName(name.c_str());
#endif
}

void mrpt::system::thread_name(const std::string& name, std::thread& theThread)
{
#if defined(_WIN32)
	set_thread_name(theThread.get_id(), name.c_str());
#else
	SetThreadName(theThread, name.c_str());
#endif
}

std::string mrpt::system::thread_name()
{
#if defined(_WIN32)
	THROW_EXCEPTION("to-do");
#else
	return GetThreadName();
#endif
}

std::string mrpt::system::thread_name(std::thread& theThread)
{
#if defined(_WIN32)
	THROW_EXCEPTION("to-do");
#else
	return GetThreadName(theThread);
#endif
}
