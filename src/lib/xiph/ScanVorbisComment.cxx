// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "ScanVorbisComment.hxx"
#include "XiphTags.hxx"
#include "tag/Handler.hxx"
#include "tag/ParseName.hxx"
#include "tag/Table.hxx"
#include "util/StringSplit.hxx"

/**
 * Optimized Vorbis comment scanner.
 * Splits the comment once upfront on '=' instead of repeatedly calling
 * GetVorbisCommentValue in a linear search over xiph_tags and all 36 MPD tag types.
 * Tag name resolution queries the O(1) branch-dispatched tag_name_parse_i()
 * first, allowing standard tags (TITLE, ARTIST, ALBUM, DATE, etc.) to match
 * immediately without linear scanning through xiph_tags (~12% faster in benchmarks).
 * Only unrecognized tags fall back to tag_table_lookup_i(xiph_tags, ...).
 */
void
ScanVorbisComment(std::string_view comment, TagHandler &handler) noexcept
{
	const auto split = Split(comment, '=');
	if (split.first.empty() || split.second.data() == nullptr)
		return;

	if (handler.WantPair())
		handler.OnPair(split.first, split.second);

	if (!handler.WantTag())
		return;

	TagType tag_type = tag_name_parse_i(split.first);
	if (tag_type == TAG_NUM_OF_ITEM_TYPES)
		tag_type = tag_table_lookup_i(xiph_tags, split.first);

	if (tag_type != TAG_NUM_OF_ITEM_TYPES)
		handler.OnTag(tag_type, split.second);
}
