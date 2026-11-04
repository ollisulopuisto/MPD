// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "tag/Table.hxx"
#include "tag/Type.hxx"

#include <gtest/gtest.h>

#include <string_view>

static constexpr struct tag_table sample_table[] = {
	{"Artist", TAG_ARTIST}, {"Album", TAG_ALBUM}, {"album artist", TAG_ALBUM_ARTIST},
	{"Title", TAG_TITLE},	{"year", TAG_DATE},   {nullptr, TAG_NUM_OF_ITEM_TYPES}};

TEST(TagTable, ExactLookup) {
	EXPECT_EQ(tag_table_lookup(sample_table, "Artist"), TAG_ARTIST);
	EXPECT_EQ(tag_table_lookup(sample_table, std::string_view("Artist")), TAG_ARTIST);
	EXPECT_EQ(tag_table_lookup(sample_table, "Album"), TAG_ALBUM);
	EXPECT_EQ(tag_table_lookup(sample_table, "album artist"), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_table_lookup(sample_table, "Title"), TAG_TITLE);
	EXPECT_EQ(tag_table_lookup(sample_table, "year"), TAG_DATE);

	// Case sensitive should not match different case
	EXPECT_EQ(tag_table_lookup(sample_table, "artist"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup(sample_table, "YEAR"), TAG_NUM_OF_ITEM_TYPES);

	// Non-existent or empty
	EXPECT_EQ(tag_table_lookup(sample_table, ""), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup(sample_table, std::string_view{}),
		  TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup(sample_table, "NonExistent"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup(sample_table, "Art"), TAG_NUM_OF_ITEM_TYPES);
}

TEST(TagTable, CaseInsensitiveLookup) {
	EXPECT_EQ(tag_table_lookup_i(sample_table, "Artist"), TAG_ARTIST);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "artist"), TAG_ARTIST);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "ARTIST"), TAG_ARTIST);
	EXPECT_EQ(tag_table_lookup_i(sample_table, std::string_view("artist")),
		  TAG_ARTIST);

	EXPECT_EQ(tag_table_lookup_i(sample_table, "album artist"), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "Album Artist"), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "ALBUM ARTIST"), TAG_ALBUM_ARTIST);

	EXPECT_EQ(tag_table_lookup_i(sample_table, "year"), TAG_DATE);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "Year"), TAG_DATE);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "YEAR"), TAG_DATE);

	EXPECT_EQ(tag_table_lookup_i(sample_table, ""), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup_i(sample_table, std::string_view{}),
		  TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "NonExistent"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "Art"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_table_lookup_i(sample_table, "ArtistExtra"), TAG_NUM_OF_ITEM_TYPES);
}

TEST(TagTable, ReverseLookup) {
	EXPECT_STREQ(tag_table_lookup(sample_table, TAG_ARTIST), "Artist");
	EXPECT_STREQ(tag_table_lookup(sample_table, TAG_ALBUM), "Album");
	EXPECT_STREQ(tag_table_lookup(sample_table, TAG_ALBUM_ARTIST), "album artist");
	EXPECT_STREQ(tag_table_lookup(sample_table, TAG_TITLE), "Title");
	EXPECT_STREQ(tag_table_lookup(sample_table, TAG_DATE), "year");
	EXPECT_EQ(tag_table_lookup(sample_table, TAG_COMMENT), nullptr);
}
