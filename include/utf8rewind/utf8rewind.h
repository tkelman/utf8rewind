/*
	Copyright (C) 2014 Quinten Lansu

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

/*! 
	@file utf8rewind.h
	@brief Functions for working with UTF-8 encoded text.
*/

#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

/// @cond IGNORE
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
/// @endcond

#define UTF8_ERR_INVALID_CHARACTER (-1)
#define UTF8_ERR_INVALID_DATA (-2)
#define UTF8_ERR_NOT_ENOUGH_SPACE (-3)
#define UTF8_ERR_OUT_OF_RANGE (-4)
#define UTF8_ERR_UNHANDLED_SURROGATE_PAIR (-5)
#define UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR (-6)
#define UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR (-7)

//! @defgroup configuration Global configuration
//! @{

#ifndef UTF8_WCHAR_SIZE
	#if (__SIZEOF_WCHAR_T__ == 4) || (WCHAR_MAX > UINT16_MAX) || (__WCHAR_MAX__ > UINT16_MAX)
		#define UTF8_WCHAR_SIZE (4)
	#else
		#define UTF8_WCHAR_SIZE (2)
	#endif
#endif

#if (UTF8_WCHAR_SIZE == 4)
	#define UTF8_WCHAR_UTF32 (1)
#elif (UTF8_WCHAR_SIZE == 2)
	#define UTF8_WCHAR_UTF16 (1)
#else
	#error Invalid size for wchar_t type.
#endif

//! @}

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t unicode_t; /*!< Unicode codepoint. */
typedef uint16_t ucs2_t; /*!< UCS-2 encoded codepoint. */
typedef uint16_t utf16_t; /*!< UTF-16 encoded codepoint. */

//! Check if a character is valid according to UTF-8 encoding.
/*!
	@param encodedCharacter Character to check.

	@return 1 on success or 0 on failure.
*/
int8_t utf8charvalid(char encodedCharacter);

//! Returns the length in bytes of the encoded character.
/*!
	A UTF-8 encoded codepoint must start with a special byte.
	This byte indicates how many bytes are used to encode the
	codepoint, up to a maximum of 6.

	This function can be used to determine the amount of bytes
	used to encode a codepoint.

	@param encodedCharacter Character to check.

	@return Amount of bytes written or SIZE_MAX on error.
*/
size_t utf8charlen(char encodedCharacter);

//! Get the length in codepoints of a UTF-8 encoded string.
/*!
	Example:

	@code{.c}
		int CheckPassword(const char* password)
		{
			size_t length = utf8len(password);
			return (length == utf8len("hunter2"));
		}
	@endcode

	@param text UTF-8 encoded string.

	@return Length in codepoints or an error code.
		- SIZE_MAX An invalid character was encountered.
*/
size_t utf8len(const char* text);

//! Encode a Unicode codepoint to UTF-8.
/*!
	Unicode codepoints must be in the range 0 - U+10FFFF,
	however the range U+D800 to U+DFFF is reserved for
	surrogate pairs and cannot be encoded.

	Example:

	@code{.c}
		char result[128];
		char* dst;
		int32_t errors = 0;

		memset(result, 0, 128);
		strcat(result, "STARG");
		dst = result + strlen(result);
		utf8encode(0x1402, dst, 128 - strlen(result), &errors);
		strcat(result, "TE");
	@endcode

	@param codepoint Unicode codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.

	Errors:
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	@sa wctoutf8
	@sa utf8convertucs2
*/
size_t utf8encode(unicode_t codepoint, char* target, size_t targetSize, int32_t* errors);

//! Convert a UTF-16 encoded string to a UTF-8 encoded string.
/*!
	This function should only be called directly if you are positive
	that you're working with UTF-16 encoded text. If you're working
	with wide strings, take a look at wctoutf8() instead.

	Example:

	@code{.c}
		int8_t Player_SetName(const utf16_t* name, size_t nameSize)
		{
			int32_t errors = 0;
			char converted_name[256] = { 0 };
			utf16toutf8(name, nameSize, converted_name, 256, &errors);
			if (errors != 0)
			{
				return 0;
			}

			return Player_SetName(converted_name);
		}
	@endcode

	@param input UTF-16 encoded string.
	@param inputSize Size of the input in bytes.
	@param target Output buffer for the result.
	@param targetSize Size of the output buffer in bytes.
	@param errors Output for errors.

	@return Amount of bytes written.

	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for encoding.
	- #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR High surrogate pair was not matched.
	- #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR Low surrogate pair was not matched.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
	- #UTF8_ERR_INVALID_CHARACTER Codepoint could not be encoded.

	@sa utf32toutf8
	@sa wctoutf8
*/
size_t utf16toutf8(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

//! Convert a UTF-32 encoded string to a UTF-8 encoded string.
/*!
	This function should only be called directly if you are positive
	that you're working with UTF-32 encoded text. If you're working
	with wide strings, take a look at wctoutf8() instead.

	Example:

	@code{.c}
		int8_t Database_ExecuteQuery(const unicode_t* query, size_t querySize)
		{
			int32_t errors = 0;
			char* converted = 0;
			int8_t result = 0;
			size_t converted_size = utf32toutf8(query, querySize, 0, 0, &errors);
			if (errors != 0)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			memset(converted, 0, converted_size + 1);

			utf32toutf8(query, querySize, converted, converted_size, &errors);
			if (errors != 0)
			{
				goto cleanup;
			}

			result = Database_ExecuteQuery(converted);

		cleanup:
			if (converted != 0)
			{
				free(converted);
				converted = 0;
			}
			return result;
		}
	@endcode

	@param input UTF-32 encoded string.
	@param inputSize Size of the input in bytes.
	@param target Output buffer for the result.
	@param targetSize Size of the output buffer in bytes.
	@param errors Output for errors.

	@return Amount of bytes written.

	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for encoding.
	- #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR High surrogate pair was not matched.
	- #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR Low surrogate pair was not matched.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
	- #UTF8_ERR_INVALID_CHARACTER Codepoint could not be encoded.

	@sa utf16toutf8
	@sa wctoutf8
*/
size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

//! Convert a wide string to a UTF-8 encoded string.
/*!
	Depending on the platform, wide strings are either UTF-16
	or UTF-32 encoded. This function takes a wide string as
	input and automatically calls the correct conversion
	function.
	
	This allows for a cross-platform treatment of wide text and
	is preferable to using the UTF-16 or UTF-32 versions
	directly.

	Example:

	@code{.c}
		const wchar_t* input = L"textures/\xD803\xDC11.png";
		size_t input_size = wcslen(input) * sizeof(wchar_t);
		size_t output_size = 0;
		char* output = 0;
		size_t result = 0;
		int32_t errors = 0;

		result = wctoutf8(input, input_size, 0, 0, &errors);
		if (errors == 0)
		{
			output_size = result + 1;

			output = (char*)malloc(output_size);
			memset(output, 0, output_size);

			wctoutf8(input, wcslen(input) * sizeof(wchar_t), output, output_size, &errors);
			if (errors == 0)
			{
				Texture_Load(output);
			}

			free(output);
		}
	@endcode

	@param input Wide encoded string.
	@param inputSize Size of the input in bytes.
	@param target Output buffer for the result.
	@param targetSize Size of the output buffer in bytes.
	@param errors Output for errors.

	@return Amount of bytes written.
	
	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for encoding.
	- #UTF8_ERR_UNMATCHED_HIGH_SURROGATE_PAIR High surrogate pair was not matched.
	- #UTF8_ERR_UNMATCHED_LOW_SURROGATE_PAIR Low surrogate pair was not matched.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
	- #UTF8_ERR_INVALID_CHARACTER Codepoint could not be encoded.

	@sa utf8towc
	@sa utf16toutf8
	@sa utf32toutf8
*/
size_t wctoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

size_t utf8toutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors);
size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors);

//! Convert a UTF-8 encoded string to a wide string.
/*!
	Depending on the platform, wide strings are either UTF-16
	or UTF-32 encoded. This function takes a UTF-8 encoded
	string as input and automatically calls the correct
	conversion function.

	This allows for a cross-platform treatment of wide text and
	is preferable to using the UTF-16 or UTF-32 versions
	directly.

	Codepoints outside the Basic Multilingual Plane (BMP) are
	converted to surrogate pairs when using UTF-16. This means
	that strings containing characters outside the BMP
	converted on a platform with UTF-32 wide strings are *not*
	compatible with platforms with UTF-16 wide strings.

	Hence, it is preferable to keep all data as UTF-8 and only
	convert to wide strings when required by a third-party
	interface.

	Example:

	@code{.c}
		const char* input = "Bj\xC3\xB6rn Zonderland";
		size_t input_size = strlen(input);
		wchar_t* output = 0;
		size_t output_size = 0;
		size_t result = 0;
		int32_t errors = 0;

		output_size = utf8towc(input, input_size, 0, 0, &errors);
		if (errors == 0)
		{
			output = (wchar_t*)malloc(output_size);
			memset(output, 0, output_size);

			utf8towc(input, input_size, output, output_size, &errors);
			if (errors == 0)
			{
				Player_SetName(output);
			}
		}
	@endcode

	@param input UTF-8 encoded string.
	@param inputSize Size of the input in bytes.
	@param target Output buffer for the result.
	@param targetSize Size of the output buffer in bytes.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.

	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.

	@sa wctoutf8
	@sa utf8toutf16
	@sa utf8toutf32
*/
size_t utf8towc(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors);

//! Convert a UCS-2 codepoint to UTF-8.
/*!
	UCS-2 encoding is similar to UTF-16 encoding, except that it
	does not use surrogate pairs to encode values beyond U+FFFF.

	This encoding was standard on Microsoft Windows XP. Newer
	versions of Windows use UTF-16 instead.

	If 0 is specified as the target buffer, this function
	returns the number of bytes needed to store the codepoint.

	@note Surrogate pairs cannot be converted using this function.
	Use wctoutf8() instead.

	Example:

	@code{.c}
		ucs2_t input[] = { 0x3041, 0x304B, 0x3060, 0x3074 };
		const size_t input_size = sizeof(input) / sizeof(ucs2_t);
		const size_t text_size = 128;
		char text[text_size] = { 0 };
		char* dst = text;
		size_t i;
		size_t offset;
		int32_t errors = 0;

		for (i = 0; i < input_size; ++i)
		{
			offset = utf8convertucs2(input[i], dst, text_size, &errors);
			if (offset == SIZE_MAX)
			{
				return errors;
			}

			dst += offset;
		}
	@endcode

	@param codepoint UCS-2 encoded codepoint.
	@param target String to write the result to.
	@param targetSize Amount of bytes remaining in the string.
	@param errors Output for errors.

	@return Amount of bytes written or SIZE_MAX on error.

	Errors:
		- #UTF8_ERR_NOT_ENOUGH_SPACE Target buffer could not contain result.
		- #UTF8_ERR_UNHANDLED_SURROGATE_PAIR Codepoint is part of a surrogate pair.

	@sa wctoutf8
*/
size_t utf8convertucs2(ucs2_t codepoint, char* target, size_t targetSize, int32_t* errors);

//! Decode a UTF-8 encoded codepoint to a Unicode codepoint.
/*!
	The result of this function can be used to offset the input
	string in order to decode all characters in a string.

	Example:

	@code{.c}
		const char* input = "Name: Bj\xC3\xB6rn Zonderland";
		const char* src = input;
		unicode_t codepoint = 0;
		int32_t errors = 0;
		size_t offset;
		size_t i;

		FontBatch_Start();

		for (i = 0; i < utf8len(input); ++i)
		{
			offset = utf8decode(src, &codepoint, &errors);
			if (offset == SIZE_MAX)
			{
				break;
			}

			Font_AddCharacter(codepoint);

			src += offset;
		}

		FontBatch_End();
		FontBatch_Draw(100, 100);
	@endcode

	@param text Input string.
	@param result String to write the result to.
	@param errors Output for errors.

	@return Input offset in bytes or SIZE_MAX on error.

	Errors:
	- #UTF8_ERR_INVALID_DATA Input does not contain enough bytes for decoding.
	- #UTF8_ERR_INVALID_CHARACTER Input does not point to a valid UTF-8 encoded character.
	- #UTF8_ERR_NOT_ENOUGH_SPACE Could not write result.

	@sa utf8encode
*/
size_t utf8decode(const char* text, unicode_t* result, int32_t* errors);

//! Seek into a UTF-8 encoded string.
/*!
	Working with UTF-8 encoded strings can be tricky due to
	the nature of the variable-length encoding. Because one
	character no longer equals one byte, it can be difficult
	to skip around in a UTF-8 encoded string without
	decoding the codepoints.

	This function provides an interface similar to `fseek`
	in order to enable skipping to another part of the
	string.

	Example:

	@code{.c}
		const char* text = "Input: <LEFT ARROW>";
		const char* input = utf8seek(text, text, utf8len("Input: "), SEEK_SET);
	@endcode

	Directions:
	- `SEEK_SET` Offset is from the start of the string.
	- `SEEK_CUR` Offset is from the current position of the string.
	- `SEEK_END` Offset is from the end of the string.

	@note `textStart` must come before `text` in memory when
	seeking from the current or end position.

	@param text Input string.
	@param textStart Start of input string.
	@param offset Requested offset in codepoints.
	@param direction Direction to seek in.

	@return Changed string or no change on error.
*/
const char* utf8seek(const char* text, const char* textStart, off_t offset, int direction);

#if defined(__cplusplus)
}
#endif

#endif
