// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "tag/Names.hxx"
#include "tag/ParseName.hxx"

#include <gtest/gtest.h>

#include <string>

TEST(ParseName, ExactAndCaseInsensitive) {
	for (unsigned i = 0; i < TAG_NUM_OF_ITEM_TYPES; ++i) {
		const char *name = tag_item_names[i];
		ASSERT_NE(name, nullptr);

		EXPECT_EQ(tag_name_parse(name), (TagType)i);
		EXPECT_EQ(tag_name_parse(std::string_view(name)), (TagType)i);
		EXPECT_EQ(tag_name_parse_i(name), (TagType)i);
		EXPECT_EQ(tag_name_parse_i(std::string_view(name)), (TagType)i);

		// Lower case
		std::string lower(name);
		for (char &c : lower) {
			if (c >= 'A' && c <= 'Z')
				c += ('a' - 'A');
		}
		EXPECT_EQ(tag_name_parse_i(lower.c_str()), (TagType)i);
		EXPECT_EQ(tag_name_parse_i(std::string_view(lower)), (TagType)i);

		// Upper case
		std::string upper(name);
		for (char &c : upper) {
			if (c >= 'a' && c <= 'z')
				c -= ('a' - 'A');
		}
		EXPECT_EQ(tag_name_parse_i(upper.c_str()), (TagType)i);
		EXPECT_EQ(tag_name_parse_i(std::string_view(upper)), (TagType)i);
	}
}

TEST(ParseName, NonExistent) {
	EXPECT_EQ(tag_name_parse(""), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse(std::string_view{}), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i(""), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i(std::string_view{}), TAG_NUM_OF_ITEM_TYPES);

	EXPECT_EQ(tag_name_parse("NonExistent"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i("NonExistent"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i(std::string_view("nonexistent")),
		  TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i("artistx"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i("art"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_name_parse_i("xartist"), TAG_NUM_OF_ITEM_TYPES);
}
