#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(DecodeUtf16, Character)
{
	const char* i = "\xE0\xA4\x9C";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
}

TEST(DecodeUtf16, String)
{
	const char* i = "\xE0\xA4\x9C\xE0\xA4\xA1\xE0\xA4\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x091C, o[0]);
	EXPECT_EQ(0x0921, o[1]);
	EXPECT_EQ(0x0924, o[2]);
}

TEST(DecodeUtf16, StringEndsInMiddle)
{
	const char* i = "\xCE\xBA\xE1\xBD\xB9\x00\xCF\x83\xCE\xBC\xCE\xB5";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x03BA, o[0]);
	EXPECT_EQ(0x1F79, o[1]);
}

TEST(DecodeUtf16, StringBufferTooSmall)
{
	const char* i = "Ba\xF4\x8F\xBF\xBFy";
	const size_t s = 4;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ('B', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ(0xDBFF, o[2]);
	EXPECT_EQ(0xDFFF, o[3]);
}

TEST(DecodeUtf16, StringDataSizeUnder)
{
	const char* i = "p\xC3\xA5 xylofon";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toutf16(i, 4, o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('p', o[0]);
	EXPECT_EQ(0x00E5, o[1]);
	EXPECT_EQ(' ', o[2]);
}

TEST(DecodeUtf16, StringDataSizeOver)
{
	const char* i = "\xCE\x93\xCE\xB1\xCE\xB6";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toutf16(i, 18, o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0393, o[0]);
	EXPECT_EQ(0x03B1, o[1]);
	EXPECT_EQ(0x03B6, o[2]);
}

TEST(DecodeUtf16, Ascii)
{
	const char* i = "k";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('k', o[0]);
}

TEST(DecodeUtf16, AsciiFirst)
{
	const char* i = "\0";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, AsciiLast)
{
	const char* i = "\x7F";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x007F, o[0]);
}

TEST(DecodeUtf16, AsciiString)
{
	const char* i = "Ham";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ('H', o[0]);
	EXPECT_EQ('a', o[1]);
	EXPECT_EQ('m', o[2]);
}

TEST(DecodeUtf16, AsciiInvalid)
{
	const char* i = "\x97";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_CHARACTER, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, TwoBytes)
{
	const char* i = "\xC5\xA9";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0169, o[0]);
}

TEST(DecodeUtf16, TwoBytesFirst)
{
	const char* i = "\xC2\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0080, o[0]);
}

TEST(DecodeUtf16, TwoBytesLast)
{
	const char* i = "\xDF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x07FF, o[0]);
}

TEST(DecodeUtf16, TwoBytesString)
{
	const char* i = "\xDD\xAE\xDE\x8A\xDF\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x076E, o[0]);
	EXPECT_EQ(0x078A, o[1]);
	EXPECT_EQ(0x07C0, o[2]);
}

TEST(DecodeUtf16, TwoBytesNotEnoughData)
{
	const char* i = "\xDA";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, ThreeBytes)
{
	const char* i = "\xE1\x8C\x8A";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x130A, o[0]);
}

TEST(DecodeUtf16, ThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x0800, o[0]);
}

TEST(DecodeUtf16, ThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xFFFF, o[0]);
}

TEST(DecodeUtf16, ThreeBytesString)
{
	const char* i = "\xE3\x81\x8A\xE3\x81\x8D\xE3\x81\x99\xE3\x81\x88";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0x304A, o[0]);
	EXPECT_EQ(0x304D, o[1]);
	EXPECT_EQ(0x3059, o[2]);
	EXPECT_EQ(0x3048, o[3]);
}

TEST(DecodeUtf16, ThreeBytesNotEnoughData)
{
	const char* i = "\xEF\xA9";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, SurrogatePair)
{
	const char* i = "\xF0\x9F\x98\xA4";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD83D, o[0]);
	EXPECT_EQ(0xDE24, o[1]);
}

TEST(DecodeUtf16, SurrogatePairFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD800, o[0]);
	EXPECT_EQ(0xDC00, o[1]);
}

TEST(DecodeUtf16, SurrogatePairLast)
{
	const char* i = "\xF4\x8F\xBF\xBF";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xDBFF, o[0]);
	EXPECT_EQ(0xDFFF, o[1]);
}

TEST(DecodeUtf16, SurrogatePairString)
{
	const char* i = "\xF0\x90\x92\xA0\xF0\x90\x92\xA8";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(8, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(0, errors);
	EXPECT_EQ(0xD801, o[0]);
	EXPECT_EQ(0xDCA0, o[1]);
	EXPECT_EQ(0xD801, o[2]);
	EXPECT_EQ(0xDCA8, o[3]);
}

TEST(DecodeUtf16, SurrogatePairNotEnoughData)
{
	const char* i = "\xF0\x9F\x98";
	const size_t s = 256;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, SurrogatePairNotEnoughSpace)
{
	const char* i = "\xF0\x90\x92\xA0";
	const size_t s = 1;
	utf16_t o[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), o, s * sizeof(utf16_t), &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_EQ(0x0000, o[0]);
}

TEST(DecodeUtf16, LengthAscii)
{
	const char* i = "I";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthAsciiFirst)
{
	const char* i = "\0";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, 1, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthAsciiLast)
{
	const char* i = "\x7F";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, 1, nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthTwoBytes)
{
	const char* i = "\xCB\x9B";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthTwoBytesFirst)
{
	const char* i = "\xC2\x80";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthTwoBytesLast)
{
	const char* i = "\xDF\xBF";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthTwoBytesNotEnoughData)
{
	const char* i = "\xC8";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf16, LengthThreeBytes)
{
	const char* i = "\xE0\xB4\x8B";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthThreeBytesFirst)
{
	const char* i = "\xE0\xA0\x80";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthThreeBytesLast)
{
	const char* i = "\xEF\xBF\xBF";
	int32_t errors = 0;

	EXPECT_EQ(2, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthThreeBytesNotEnoughData)
{
	const char* i = "\xE0\xB4";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf16, LengthFourBytes)
{
	const char* i = "\xF0\x90\xB0\xAC";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthFourBytesFirst)
{
	const char* i = "\xF0\x90\x80\x80";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthFourBytesLast)
{
	const char* i = "\xF4\x80\x83\xBF";
	int32_t errors = 0;

	EXPECT_EQ(4, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(0, errors);
}

TEST(DecodeUtf16, LengthFourBytesNotEnoughData)
{
	const char* i = "\xF0\x90\xB1";
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(i, strlen(i), nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(DecodeUtf16, LengthNotEnoughData)
{
	int32_t errors = 0;

	EXPECT_EQ(0, utf8toutf16(nullptr, 1, nullptr, 0, &errors));
	EXPECT_EQ(UTF8_ERR_INVALID_DATA, errors);
}