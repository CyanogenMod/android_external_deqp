/*-------------------------------------------------------------------------
 * drawElements Quality Program EGL Utilities
 * ------------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief EGL call wrapper for logging.
 *//*--------------------------------------------------------------------*/

#include "egluCallLogWrapper.hpp"
#include "egluStrUtil.hpp"
#include "deStringUtil.hpp"
#include "deInt32.h"

namespace eglu
{

using tcu::toHex;
using tcu::TestLog;

CallLogWrapper::CallLogWrapper (TestLog& log)
	: m_log			(log)
	, m_enableLog	(false)
{
}

CallLogWrapper::~CallLogWrapper (void)
{
}

// Pointer formatter.

template <typename T>
class PointerFmt
{
public:
	const T*	arr;
	deUint32	size;

	PointerFmt (const T* arr_, deUint32 size_) : arr(arr_), size(size_) {}
};

template <typename T>
std::ostream& operator<< (std::ostream& str, PointerFmt<T> fmt)
{
	if (fmt.arr != DE_NULL)
	{
		str << "{ ";
		for (deUint32 ndx = 0; ndx < fmt.size; ndx++)
		{
			if (ndx != 0)
				str << ", ";
			str << fmt.arr[ndx];
		}
		str << " }";
		return str;
	}
	else
		return str << "(null)";
}

template <typename T>
inline PointerFmt<T> getPointerStr (const T* arr, deUint32 size)
{
	return PointerFmt<T>(arr, size);
}

typedef const char* (*GetEnumNameFunc) (int value);

// Enum pointer formatter.

class EnumPointerFmt
{
public:
	const int*		value;
	GetEnumNameFunc	getName;

	EnumPointerFmt (const int* value_, GetEnumNameFunc getName_) : value(value_), getName(getName_) {}
};

inline std::ostream& operator<< (std::ostream& str, EnumPointerFmt fmt)
{
	if (fmt.value)
		return str << tcu::Format::Enum(fmt.getName, *fmt.value);
	else
		return str << "(null)";
}

inline EnumPointerFmt getEnumPointerStr (const int* value, GetEnumNameFunc getName)
{
	return EnumPointerFmt(value, getName);
}

// String formatter.

class StringFmt
{
public:
	const char* str;
	StringFmt (const char* str_) : str(str_) {}
};

inline std::ostream& operator<< (std::ostream& str, StringFmt fmt)
{
	return str << (fmt.str ? fmt.str : "NULL");
}

inline StringFmt getStringStr (const char* value) { return StringFmt(value); }

// Config attrib pointer formatter

class ConfigAttribValuePointerFmt
{
public:
	deUint32		attrib;
	const int*		value;
	ConfigAttribValuePointerFmt (deUint32 attrib_, const int* value_) : attrib(attrib_), value(value_) {}
};

inline ConfigAttribValuePointerFmt getConfigAttribValuePointerStr (deUint32 attrib, const int* value) { return ConfigAttribValuePointerFmt(attrib, value); }

inline std::ostream& operator<< (std::ostream& str, const ConfigAttribValuePointerFmt& fmt)
{
	if (fmt.value)
		return str << getConfigAttribValueStr(fmt.attrib, *fmt.value);
	else
		return str << "NULL";
}

// Context attrib pointer formatter

class ContextAttribValuePointerFmt
{
public:
	deUint32		attrib;
	const int*		value;
	ContextAttribValuePointerFmt (deUint32 attrib_, const int* value_) : attrib(attrib_), value(value_) {}
};

inline ContextAttribValuePointerFmt getContextAttribValuePointerStr (deUint32 attrib, const int* value) { return ContextAttribValuePointerFmt(attrib, value); }

inline std::ostream& operator<< (std::ostream& str, const ContextAttribValuePointerFmt& fmt)
{
	if (fmt.value)
		return str << getContextAttribValueStr(fmt.attrib, *fmt.value);
	else
		return str << "NULL";
}

// Surface attrib pointer formatter

class SurfaceAttribValuePointerFmt
{
public:
	deUint32		attrib;
	const int*		value;
	SurfaceAttribValuePointerFmt (deUint32 attrib_, const int* value_) : attrib(attrib_), value(value_) {}
};

inline SurfaceAttribValuePointerFmt getSurfaceAttribValuePointerStr (deUint32 attrib, const int* value) { return SurfaceAttribValuePointerFmt(attrib, value); }

inline std::ostream& operator<< (std::ostream& str, const SurfaceAttribValuePointerFmt& fmt)
{
	if (fmt.value)
		return str << getSurfaceAttribValueStr(fmt.attrib, *fmt.value);
	else
		return str << "NULL";
}

// EGLDisplay formatter

class EGLDisplayFmt
{
public:
	EGLDisplay display;
	EGLDisplayFmt (EGLDisplay display_) : display(display_) {}
};

inline EGLDisplayFmt getEGLDisplayStr (EGLDisplay display) { return EGLDisplayFmt(display); }

inline std::ostream& operator<< (std::ostream& str, const EGLDisplayFmt& fmt)
{
	if (fmt.display == EGL_NO_DISPLAY)
		return str << "EGL_NO_DISPLAY";
	else
		return str << toHex(fmt.display);
}

// EGLSurface formatter

class EGLSurfaceFmt
{
public:
	EGLSurface display;
	EGLSurfaceFmt (EGLSurface display_) : display(display_) {}
};

inline EGLSurfaceFmt getEGLSurfaceStr (EGLSurface display) { return EGLSurfaceFmt(display); }

inline std::ostream& operator<< (std::ostream& str, const EGLSurfaceFmt& fmt)
{
	if (fmt.display == EGL_NO_SURFACE)
		return str << "EGL_NO_SURFACE";
	else
		return str << toHex(fmt.display);
}

// EGLContext formatter

class EGLContextFmt
{
public:
	EGLContext display;
	EGLContextFmt (EGLContext display_) : display(display_) {}
};

inline EGLContextFmt getEGLContextStr (EGLContext display) { return EGLContextFmt(display); }

inline std::ostream& operator<< (std::ostream& str, const EGLContextFmt& fmt)
{
	if (fmt.display == EGL_NO_CONTEXT)
		return str << "EGL_NO_CONTEXT";
	else
		return str << toHex(fmt.display);
}

// API entry-point implementations are auto-generated
#include "egluCallLogWrapper.inl"

} // eglu
