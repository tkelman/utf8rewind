#include "tests-base.hpp"

#include "utf8rewind.h"

TEST(TransformDecompose, Found)
{
	const char* c = "Bj\xC3\xB6rn Zonderland";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(18, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Bjo\xCC\x88rn Zonderland", b);
}

TEST(TransformDecompose, FoundFirst)
{
	const char* c = "\xC3\x80";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("A\xCC\x80", b);
}

TEST(TransformDecompose, FoundLast)
{
	const char* c = "\xF0\xAF\xA8\x9D";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(4, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xF0\xAA\x98\x80", b);
}

TEST(TransformDecompose, FoundNotEnoughSpace)
{
	const char* c = "\xE1\xB8\xAE";
	const size_t s = 4;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(TransformDecompose, ExpandedNotEnoughSpace)
{
	const char* c = "Am\xC3\x87zing";
	const size_t s = 7;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("AmC\xCC\xA7z", b);
}

TEST(TransformDecompose, NoChange)
{
	const char* c = "\xE1\xA2\xA2";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xE1\xA2\xA2", b);
}

TEST(TransformDecompose, NoChangeNotEnoughSpace)
{
	const char* c = "\xE2\xA0\x81";
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}

TEST(TransformDecompose, Ascii)
{
	const char* c = "Ruler";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("Ruler", b);
}

TEST(TransformDecompose, AsciiNotEnoughSpace)
{
	const char* c = "Spacebro";
	const size_t s = 6;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("Space", b);
}

TEST(TransformDecompose, JustEnoughSpace)
{
	const char* c = "Ar\xE1\xB9\x9Eogance";
	const size_t s = 12;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(11, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("ArR\xCC\xB1ogance", b);
}

TEST(TransformDecompose, JustEnoughSpaceAtEnd)
{
	const char* c = "Pounc\xE1\xB8\x94";
	const size_t s = 11;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(10, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("PouncE\xCC\x84\xCC\x80", b);
}

TEST(TransformDecompose, JustEnoughSpaceAtStart)
{
	const char* c = "\xE1\xB8\x9C";
	const size_t s = 6;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(5, utf8transform(c, strlen(c), b, s - 1, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("E\xCC\xA7\xCC\x86", b);
}

TEST(TransformDecompose, InvalidCodepointSurrogatePair)
{
	const char* c = "\xED\xA0\x80\xED\xB0\x81";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(6, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBD\xEF\xBF\xBD", b);
}

TEST(TransformDecompose, InvalidCodepointOverlong)
{
	const char* c = "\xF8\x80\x80\x80\xAF";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBD", b);
}

TEST(TransformDecompose, InvalidCodepointNotEnoughData)
{
	const char* c = "\xED\xAB";
	const size_t s = 512;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(3, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(0, errors);
	EXPECT_STREQ("\xEF\xBF\xBD", b);
}

TEST(TransformDecompose, InvalidCodepointNotEnoughSpace)
{
	const char* c = "\xF0\x91\x88\x81";
	const size_t s = 3;
	char b[s] = { 0 };
	int32_t errors = 0;

	EXPECT_EQ(0, utf8transform(c, strlen(c), b, s, UTF8_TRANSFORM_DECOMPOSED, &errors));
	EXPECT_EQ(UTF8_ERR_NOT_ENOUGH_SPACE, errors);
	EXPECT_STREQ("", b);
}