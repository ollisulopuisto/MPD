// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "tag/ApeTag.hxx"
#include "tag/Type.hxx"

#include <gtest/gtest.h>

#include <string_view>

using std::string_view_literals::operator""sv;

TEST(ApeTag, ParseStandardTags) {
	EXPECT_EQ(tag_ape_name_parse("Title"), TAG_TITLE);
	EXPECT_EQ(tag_ape_name_parse("title"sv), TAG_TITLE);
	EXPECT_EQ(tag_ape_name_parse("TITLE"), TAG_TITLE);

	EXPECT_EQ(tag_ape_name_parse("Artist"), TAG_ARTIST);
	EXPECT_EQ(tag_ape_name_parse("artist"sv), TAG_ARTIST);
	EXPECT_EQ(tag_ape_name_parse("ARTIST"), TAG_ARTIST);

	EXPECT_EQ(tag_ape_name_parse("Album"), TAG_ALBUM);
	EXPECT_EQ(tag_ape_name_parse("album"sv), TAG_ALBUM);

	EXPECT_EQ(tag_ape_name_parse("Genre"), TAG_GENRE);
	EXPECT_EQ(tag_ape_name_parse("genre"sv), TAG_GENRE);

	EXPECT_EQ(tag_ape_name_parse("Track"), TAG_TRACK);
	EXPECT_EQ(tag_ape_name_parse("track"sv), TAG_TRACK);

	EXPECT_EQ(tag_ape_name_parse("Date"), TAG_DATE);
	EXPECT_EQ(tag_ape_name_parse("date"sv), TAG_DATE);

	EXPECT_EQ(tag_ape_name_parse("Disc"), TAG_DISC);
	EXPECT_EQ(tag_ape_name_parse("disc"sv), TAG_DISC);

	EXPECT_EQ(tag_ape_name_parse("Comment"), TAG_COMMENT);
	EXPECT_EQ(tag_ape_name_parse("comment"sv), TAG_COMMENT);
}

TEST(ApeTag, ParseCustomApeTags) {
	// APE-specific alias: "year" maps to TAG_DATE
	EXPECT_EQ(tag_ape_name_parse("year"), TAG_DATE);
	EXPECT_EQ(tag_ape_name_parse("Year"sv), TAG_DATE);
	EXPECT_EQ(tag_ape_name_parse("YEAR"), TAG_DATE);

	// APE-specific alias: "album artist" (with space) maps to TAG_ALBUM_ARTIST
	EXPECT_EQ(tag_ape_name_parse("album artist"), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_ape_name_parse("Album Artist"sv), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_ape_name_parse("ALBUM ARTIST"), TAG_ALBUM_ARTIST);

	// Standard name "AlbumArtist" also works via tag_name_parse_i
	EXPECT_EQ(tag_ape_name_parse("AlbumArtist"), TAG_ALBUM_ARTIST);
	EXPECT_EQ(tag_ape_name_parse("albumartist"sv), TAG_ALBUM_ARTIST);
}

TEST(ApeTag, ParseInvalid) {
	EXPECT_EQ(tag_ape_name_parse(""), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_ape_name_parse(""sv), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_ape_name_parse("NonExistentTag"), TAG_NUM_OF_ITEM_TYPES);
	EXPECT_EQ(tag_ape_name_parse("random_key"sv), TAG_NUM_OF_ITEM_TYPES);
}
