// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "MakeTag.hxx"
#include "song/TagSongFilter.hxx"
#include "song/LightSong.hxx"
#include "tag/Type.hxx"
#include "lib/icu/Init.hxx"

#include <gtest/gtest.h>

class TagSongFilterTest : public ::testing::Test {
protected:
	void SetUp() override {
		IcuInit();
	}

	void TearDown() override {
		IcuFinish();
	}
};

static bool
InvokeFilter(const TagSongFilter &f, const Tag &tag) noexcept
{
	return f.Match(LightSong("dummy", tag));
}

TEST_F(TagSongFilterTest, Basic)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"needle", false, false, StringFilter::Position::FULL, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_TITLE, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle", TAG_TITLE, "foo")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ARTIST, "foo", TAG_TITLE, "needle", TAG_ALBUM, "bar")));

	EXPECT_FALSE(InvokeFilter(f, MakeTag()));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_TITLE, "bar")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_ARTIST, "needle")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "FOOneedleBAR")));
}

/**
 * Test with empty string.  This matches tags where the given tag type
 * does not exist.
 */
TEST_F(TagSongFilterTest, Empty)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"", false, false, StringFilter::Position::FULL, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag()));

	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_TITLE, "bar")));
}

TEST_F(TagSongFilterTest, Substring)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"needle", false, false, StringFilter::Position::ANYWHERE, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needleBAR")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "FOOneedle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "FOOneedleBAR")));

	EXPECT_FALSE(InvokeFilter(f, MakeTag()));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "eedle")));
}

TEST_F(TagSongFilterTest, Startswith)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"needle", false, false, StringFilter::Position::PREFIX, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "needleBAR")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "FOOneedle")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "FOOneedleBAR")));

	EXPECT_FALSE(InvokeFilter(f, MakeTag()));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "eedle")));
}

TEST_F(TagSongFilterTest, Negated)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"needle", false, false, StringFilter::Position::FULL, true},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag()));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo")));
}

/**
 * Combine the "Empty" and "Negated" tests.
 */
TEST_F(TagSongFilterTest, EmptyNegated)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"", false, false, StringFilter::Position::FULL, true},
	};

	EXPECT_FALSE(InvokeFilter(f, MakeTag()));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo")));
}

/**
 * Negation with multiple tag values.
 */
TEST_F(TagSongFilterTest, MultiNegated)
{
	const TagSongFilter f{
		TAG_TITLE,
		{"needle", false, false, StringFilter::Position::FULL, true},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_TITLE, "bar")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "needle", TAG_TITLE, "bar")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_TITLE, "foo", TAG_TITLE, "needle")));
}

/**
 * Check whether fallback tags work, e.g. AlbumArtist falls back to
 * just Artist if there is no AlbumArtist.
 */
TEST_F(TagSongFilterTest, Fallback)
{
	const TagSongFilter f{
		TAG_ALBUM_ARTIST,
		{"needle", false, false, StringFilter::Position::FULL, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ALBUM_ARTIST, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ARTIST, "needle")));

	EXPECT_FALSE(InvokeFilter(f, MakeTag()));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ALBUM_ARTIST, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ARTIST, "foo")));

	/* no fallback, thus the Artist tag isn't used and this must
	   be a mismatch */
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ARTIST, "needle", TAG_ALBUM_ARTIST, "foo")));
}

/**
 * Combine the "Empty" and "Fallback" tests.
 */
TEST_F(TagSongFilterTest, EmptyFallback)
{
	const TagSongFilter f{
		TAG_ALBUM_ARTIST,
		{"", false, false, StringFilter::Position::FULL, false},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag()));

	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ALBUM_ARTIST, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ARTIST, "foo")));
}

/**
 * Combine the "Negated" and "Fallback" tests.
 */
TEST_F(TagSongFilterTest, NegatedFallback)
{
	const TagSongFilter f{
		TAG_ALBUM_ARTIST,
		{"needle", false, false, StringFilter::Position::FULL, true},
	};

	EXPECT_TRUE(InvokeFilter(f, MakeTag()));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ALBUM_ARTIST, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ALBUM_ARTIST, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ARTIST, "foo")));
	EXPECT_FALSE(InvokeFilter(f, MakeTag(TAG_ARTIST, "needle")));
	EXPECT_TRUE(InvokeFilter(f, MakeTag(TAG_ARTIST, "needle", TAG_ALBUM_ARTIST, "foo")));
}

#include "song/Filter.hxx"

TEST_F(TagSongFilterTest, ParseFilterTypes) {
	// Standard tag names (case insensitive)
	{
		SongFilter filter;
		const char *const args[] = {"artist", "Test Artist"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag = MakeTag(TAG_ARTIST, "Test Artist");
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	{
		SongFilter filter;
		const char *const args[] = {"Title", "Test Title"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag = MakeTag(TAG_TITLE, "Test Title");
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	{
		SongFilter filter;
		const char *const args[] = {"ALBUM", "Test Album"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag = MakeTag(TAG_ALBUM, "Test Album");
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	// Special keywords
	{
		SongFilter filter;
		const char *const args[] = {"file", "test.ogg"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag;
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	{
		SongFilter filter;
		const char *const args[] = {"filename", "test.ogg"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag;
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	{
		SongFilter filter;
		const char *const args[] = {"any", "Test"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		Tag tag = MakeTag(TAG_ARTIST, "Test");
		LightSong song("test.ogg", tag);
		EXPECT_TRUE(filter.Match(song));
	}

	{
		SongFilter filter;
		const char *const args[] = {"base", "dir"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
		EXPECT_STREQ(filter.GetBase(), "dir");
	}

	{
		SongFilter filter;
		const char *const args[] = {"modified-since", "2020-01-01"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
	}

	{
		SongFilter filter;
		const char *const args[] = {"added-since", "2020-01-01"};
		EXPECT_NO_THROW(filter.Parse(args, false, false));
	}

	// Unknown tag
	{
		SongFilter filter;
		const char *const args[] = {"unknown_tag_xyz", "value"};
		EXPECT_THROW(filter.Parse(args, false, false), std::runtime_error);
	}
}
