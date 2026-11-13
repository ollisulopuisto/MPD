// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "util/StringCompare.hxx"

#include <gtest/gtest.h>

#include <string_view>

using std::string_view_literals::operator""sv;

TEST(StringCompare, StringStartsWith) {
	EXPECT_TRUE(StringStartsWith("hello.world", "hello"));
	EXPECT_TRUE(StringStartsWith("hello.world", "hello"sv));
	EXPECT_TRUE(StringStartsWith("hello.world"sv, "hello"sv));

	EXPECT_TRUE(StringStartsWith("hello.world", ""));
	EXPECT_TRUE(StringStartsWith("hello.world", ""sv));
	EXPECT_TRUE(StringStartsWith("hello.world"sv, ""sv));

	EXPECT_TRUE(StringStartsWith("", ""));
	EXPECT_TRUE(StringStartsWith("", ""sv));
	EXPECT_TRUE(StringStartsWith(""sv, ""sv));

	EXPECT_FALSE(StringStartsWith("", "a"));
	EXPECT_FALSE(StringStartsWith("", "a"sv));
	EXPECT_FALSE(StringStartsWith(""sv, "a"sv));

	EXPECT_FALSE(StringStartsWith("short", "longer_needle"));
	EXPECT_FALSE(StringStartsWith("short", "longer_needle"sv));
	EXPECT_FALSE(StringStartsWith("short"sv, "longer_needle"sv));

	// Case sensitive checks
	EXPECT_FALSE(StringStartsWith("hello.world", "HELLO"));
	EXPECT_FALSE(StringStartsWith("hello.world", "HELLO"sv));
	EXPECT_FALSE(StringStartsWith("hello.world"sv, "HELLO"sv));
	EXPECT_FALSE(StringStartsWith("HELLO.world", "hello"));
	EXPECT_FALSE(StringStartsWith("HELLO.world", "hello"sv));
	EXPECT_FALSE(StringStartsWith("HELLO.world"sv, "hello"sv));

	// Different initial character
	EXPECT_FALSE(StringStartsWith("hello.world", "world"));
	EXPECT_FALSE(StringStartsWith("hello.world", "world"sv));
	EXPECT_FALSE(StringStartsWith("hello.world"sv, "world"sv));

	// Partial match but not prefix
	EXPECT_FALSE(StringStartsWith("hello.world", "hellp"));
	EXPECT_FALSE(StringStartsWith("hello.world", "hellp"sv));
	EXPECT_FALSE(StringStartsWith("hello.world"sv, "hellp"sv));
}

TEST(StringCompare, StringStartsWithStopsAtTerminator) {
	/* a needle that starts with a NUL byte must not make the check read
	   past the end of the (empty) haystack */
	EXPECT_TRUE(StringStartsWith("", "\0abc"sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase("", "\0abc"sv));
}

TEST(StringCompare, StringStartsWithIgnoreCase) {
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", "hello"));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", "hello"sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world"sv, "hello"sv));

	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", "HELLO"));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", "HELLO"sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world"sv, "HELLO"sv));

	EXPECT_TRUE(StringStartsWithIgnoreCase("HELLO.world", "hello"));
	EXPECT_TRUE(StringStartsWithIgnoreCase("HELLO.world", "hello"sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase("HELLO.world"sv, "hello"sv));

	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", ""));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world", ""sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase("hello.world"sv, ""sv));

	EXPECT_TRUE(StringStartsWithIgnoreCase("", ""));
	EXPECT_TRUE(StringStartsWithIgnoreCase("", ""sv));
	EXPECT_TRUE(StringStartsWithIgnoreCase(""sv, ""sv));

	EXPECT_FALSE(StringStartsWithIgnoreCase("", "a"));
	EXPECT_FALSE(StringStartsWithIgnoreCase("", "a"sv));
	EXPECT_FALSE(StringStartsWithIgnoreCase(""sv, "a"sv));

	EXPECT_FALSE(StringStartsWithIgnoreCase("short", "longer_needle"));
	EXPECT_FALSE(StringStartsWithIgnoreCase("short", "longer_needle"sv));
	EXPECT_FALSE(StringStartsWithIgnoreCase("short"sv, "longer_needle"sv));

	// Different initial character
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world", "world"));
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world", "world"sv));
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world"sv, "world"sv));

	// Matching initial character, differing rest
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world", "HELLP"));
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world", "HELLP"sv));
	EXPECT_FALSE(StringStartsWithIgnoreCase("hello.world"sv, "HELLP"sv));
}

TEST(StringCompare, StringEndsWith) {
	EXPECT_TRUE(StringEndsWith("hello.world", ".world"));
	EXPECT_TRUE(StringEndsWith("hello.world", ".world"sv));
	EXPECT_TRUE(StringEndsWith("hello.world"sv, ".world"sv));

	EXPECT_TRUE(StringEndsWith("hello.world", ""));
	EXPECT_TRUE(StringEndsWith("hello.world", ""sv));
	EXPECT_TRUE(StringEndsWith("hello.world"sv, ""sv));

	EXPECT_TRUE(StringEndsWith("", ""));
	EXPECT_TRUE(StringEndsWith("", ""sv));
	EXPECT_TRUE(StringEndsWith(""sv, ""sv));

	EXPECT_FALSE(StringEndsWith("", "a"));
	EXPECT_FALSE(StringEndsWith("", "a"sv));
	EXPECT_FALSE(StringEndsWith(""sv, "a"sv));

	EXPECT_FALSE(StringEndsWith("short", "longer_needle"));
	EXPECT_FALSE(StringEndsWith("short", "longer_needle"sv));
	EXPECT_FALSE(StringEndsWith("short"sv, "longer_needle"sv));

	// Case sensitive checks
	EXPECT_FALSE(StringEndsWith("hello.WORLD", ".world"));
	EXPECT_FALSE(StringEndsWith("hello.world", ".WORLD"));
	EXPECT_FALSE(StringEndsWith("hello.world"sv, ".WORLD"sv));
}

TEST(StringCompare, StringEndsWithIgnoreCase) {
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ".world"));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ".world"sv));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world"sv, ".world"sv));

	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.WORLD", ".world"));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.WORLD", ".world"sv));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ".WORLD"));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ".WORLD"sv));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world"sv, ".WORLD"sv));

	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ""));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world", ""sv));
	EXPECT_TRUE(StringEndsWithIgnoreCase("hello.world"sv, ""sv));

	EXPECT_TRUE(StringEndsWithIgnoreCase("", ""));
	EXPECT_TRUE(StringEndsWithIgnoreCase("", ""sv));
	EXPECT_TRUE(StringEndsWithIgnoreCase(""sv, ""sv));

	EXPECT_FALSE(StringEndsWithIgnoreCase("", "a"));
	EXPECT_FALSE(StringEndsWithIgnoreCase("", "a"sv));
	EXPECT_FALSE(StringEndsWithIgnoreCase(""sv, "a"sv));

	EXPECT_FALSE(StringEndsWithIgnoreCase("short", "longer_needle"));
	EXPECT_FALSE(StringEndsWithIgnoreCase("short", "longer_needle"sv));
	EXPECT_FALSE(StringEndsWithIgnoreCase("short"sv, "longer_needle"sv));

	EXPECT_FALSE(StringEndsWithIgnoreCase("hello.world", ".word"));
	EXPECT_FALSE(StringEndsWithIgnoreCase("hello.world"sv, ".word"sv));
}

TEST(StringCompare, FindStringSuffix) {
	const char *s = "music_track.flac";

	EXPECT_EQ(FindStringSuffix(s, ".flac"), s + 11);
	EXPECT_EQ(FindStringSuffix(s, ".flac"sv), s + 11);
	EXPECT_EQ(FindStringSuffix(s, "music_track.flac"), s);
	EXPECT_EQ(FindStringSuffix(s, ""), s + 16);
	EXPECT_EQ(FindStringSuffix(s, ""sv), s + 16);

	EXPECT_EQ(FindStringSuffix(s, ".mp3"), nullptr);
	EXPECT_EQ(FindStringSuffix(s, ".mp3"sv), nullptr);
	EXPECT_EQ(FindStringSuffix(s, "longer_than_the_haystack_itself"), nullptr);
}
