// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "lib/xiph/ScanVorbisComment.hxx"
#include "tag/Handler.hxx"
#include "tag/Type.hxx"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {

class TestTagHandler final : public NullTagHandler {
public:
	std::vector<std::pair<TagType, std::string>> tags;
	std::vector<std::pair<std::string, std::string>> pairs;

	explicit TestTagHandler(unsigned mask) noexcept : NullTagHandler(mask) {}

	void OnTag(TagType type, std::string_view value) noexcept override {
		tags.emplace_back(type, std::string(value));
	}

	void OnPair(std::string_view key, std::string_view value) noexcept override {
		pairs.emplace_back(std::string(key), std::string(value));
	}
};

} // namespace

TEST(ScanVorbisComment, BasicAndCaseInsensitive) {
	TestTagHandler handler(TagHandler::WANT_TAG | TagHandler::WANT_PAIR);

	ScanVorbisComment("TITLE=Song Title", handler);
	ScanVorbisComment("artist=Cool Artist", handler);
	ScanVorbisComment("TRACKNUMBER=5", handler);
	ScanVorbisComment("discnumber=1", handler);
	ScanVorbisComment("DESCRIPTION=Comment text", handler);
	ScanVorbisComment("CUSTOM_KEY=custom_val", handler);
	ScanVorbisComment("MALFORMED", handler);
	ScanVorbisComment("=empty_key", handler);

	ASSERT_EQ(handler.tags.size(), 5u);
	EXPECT_EQ(handler.tags[0].first, TAG_TITLE);
	EXPECT_EQ(handler.tags[0].second, "Song Title");
	EXPECT_EQ(handler.tags[1].first, TAG_ARTIST);
	EXPECT_EQ(handler.tags[1].second, "Cool Artist");
	EXPECT_EQ(handler.tags[2].first, TAG_TRACK);
	EXPECT_EQ(handler.tags[2].second, "5");
	EXPECT_EQ(handler.tags[3].first, TAG_DISC);
	EXPECT_EQ(handler.tags[3].second, "1");
	EXPECT_EQ(handler.tags[4].first, TAG_COMMENT);
	EXPECT_EQ(handler.tags[4].second, "Comment text");

	ASSERT_EQ(handler.pairs.size(), 6u);
	EXPECT_EQ(handler.pairs[0].first, "TITLE");
	EXPECT_EQ(handler.pairs[0].second, "Song Title");
	EXPECT_EQ(handler.pairs[1].first, "artist");
	EXPECT_EQ(handler.pairs[1].second, "Cool Artist");
	EXPECT_EQ(handler.pairs[5].first, "CUSTOM_KEY");
	EXPECT_EQ(handler.pairs[5].second, "custom_val");
}

TEST(ScanVorbisComment, WantPairOnly) {
	TestTagHandler handler(TagHandler::WANT_PAIR);

	ScanVorbisComment("TITLE=Song Title", handler);
	EXPECT_TRUE(handler.tags.empty());
	ASSERT_EQ(handler.pairs.size(), 1u);
	EXPECT_EQ(handler.pairs[0].first, "TITLE");
	EXPECT_EQ(handler.pairs[0].second, "Song Title");
}

TEST(ScanVorbisComment, WantTagOnly) {
	TestTagHandler handler(TagHandler::WANT_TAG);

	ScanVorbisComment("TITLE=Song Title", handler);
	EXPECT_TRUE(handler.pairs.empty());
	ASSERT_EQ(handler.tags.size(), 1u);
	EXPECT_EQ(handler.tags[0].first, TAG_TITLE);
	EXPECT_EQ(handler.tags[0].second, "Song Title");
}
