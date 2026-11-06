// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "util/StringCompare.hxx"

#include <gtest/gtest.h>

#include <string_view>

using std::string_view_literals::operator""sv;

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
