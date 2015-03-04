/*
	Copyright (C) 2014-2015 Quinten Lansu

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _UTF8REWIND_INTERNAL_CODEPOINT_H_
#define _UTF8REWIND_INTERNAL_CODEPOINT_H_

/*!
	\cond INTERNAL

	\file codepoint.h
	\brief Internal functions and definitions used for managing codepoints.
*/

#include "utf8rewind.h"

/*!
	\addtogroup internal Internal functions and definitions
	\{
*/

/*!
	\def MAX_BASIC_MULTILINGUAR_PLANE
	\brief The maximum legal codepoint in the Basic Multilinguar Plane (BMP).
*/
#define MAX_BASIC_MULTILINGUAR_PLANE  0xFFFF

/*!
	\def MAX_LEGAL_UNICODE
	\brief The maximum legal codepoint in Unicode
*/
#define MAX_LEGAL_UNICODE             0x10FFFF

/*!
	\def REPLACEMENT_CHARACTER
	\brief The replacement character used to replace illegal codepoints.
*/
#define REPLACEMENT_CHARACTER         0xFFFD

/*!
	\def SURROGATE_HIGH_START
	\brief The minimum codepoint for the high member of a surrogate pair.
*/
#define SURROGATE_HIGH_START          0xD800

/*!
	\def SURROGATE_HIGH_END
	\brief The maximum codepoint for the high member of a surrogate pair.
*/
#define SURROGATE_HIGH_END            0xDBFF

/*!
	\def SURROGATE_LOW_START
	\brief The minimum codepoint for the low member of a surrogate pair.
*/
#define SURROGATE_LOW_START           0xDC00

/*!
	\def SURROGATE_LOW_END
	\brief The maximum codepoint for the low member of a surrogate pair.
*/
#define SURROGATE_LOW_END             0xDFFF

/*!
	\def HANGUL_L_FIRST
	\brief The minimum codepoint for Hangul L values.
*/
#define HANGUL_L_FIRST                0x1100

/*!
	\def HANGUL_L_LAST
	\brief The maximum codepoint for Hangul L values.
*/
#define HANGUL_L_LAST                 0x1112

/*!
	\def HANGUL_L_COUNT
	\brief The number of codepoints in the Hangul L section.
*/
#define HANGUL_L_COUNT                19

/*!
	\def HANGUL_V_FIRST
	\brief The minimum codepoint for Hangul V values.
*/
#define HANGUL_V_FIRST                0x1161

/*!
	\def HANGUL_V_LAST
	\brief The maximum codepoint for Hangul V values.
*/
#define HANGUL_V_LAST                 0x1175

/*!
	\def HANGUL_V_COUNT
	\brief The number of codepoints in the Hangul V section.
*/
#define HANGUL_V_COUNT                21

/*!
	\def HANGUL_T_FIRST
	\brief The minimum codepoint for Hangul T values.
*/
#define HANGUL_T_FIRST                0x11A7

/*!
	\def HANGUL_T_LAST
	\brief The maximum codepoint for Hangul T values.
*/
#define HANGUL_T_LAST                 0x11C2

/*!
	\def HANGUL_T_COUNT
	\brief The number of codepoints in the Hangul T section.
*/
#define HANGUL_T_COUNT                28

/*!
	\def HANGUL_N_COUNT
	\brief Combined count of Hangul V and T sections.
*/
#define HANGUL_N_COUNT                588 /* VCount * TCount */

/*!
	\def HANGUL_S_FIRST
	\brief The minimum codepoint for Hangul S values.
*/
#define HANGUL_S_FIRST                0xAC00

/*!
	\def HANGUL_S_LAST
	\brief The maximum codepoint for Hangul S values.
*/
#define HANGUL_S_LAST                 0xD7A3

/*!
	\def HANGUL_S_COUNT
	\brief The number of codepoints in the Hangul S section (combines L with N).
*/
#define HANGUL_S_COUNT                11172 /* LCount * NCount */

/*!
	\brief Get the number of bytes used for encoding a codepoint.

	\param[in]  byte  Encoded byte

	\return Number of bytes needed for decoding or 0 if input is illegal.
*/
extern const uint8_t codepoint_decoded_length[256];

/*!
	\brief Get the number of bytes needed to encode a codepoint.

	\param[in]  codepoint  Unicode codepoint

	\return Number of bytes needed for encoding or 0 if input is illegal.
*/
uint8_t codepoint_encoded_length(unicode_t codepoint);

/*!
	\brief Write Unicode codepoint to UTF-8 encoded string.

	Target buffer and size is modified by encoded size.

	\param[in]      encoded     Unicode codepoint
	\param[in,out]  target      Target buffer
	\param[in,out]  targetSize  Size of output buffer in bytes

	\return Bytes needed for encoding or 0 on error.
*/
uint8_t codepoint_write(unicode_t encoded, char** target, size_t* targetSize);

/*!
	\brief Read Unicode codepoint from UTF-8 encoded string.

	\param[in]   input      Input buffer
	\param[in]   inputSize  Size of input buffer in bytes
	\param[out]  decoded    Unicode codepoint

	\return Bytes read from string or 0 on error.
*/
uint8_t codepoint_read(const char* input, size_t inputSize, unicode_t* decoded);

/*!
	\}
*/

/*! \endcond */

#endif