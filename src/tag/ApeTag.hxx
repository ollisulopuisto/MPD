// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#ifndef MPD_APE_TAG_HXX
#define MPD_APE_TAG_HXX

#include <cstdint>
#include <string_view>

enum TagType : uint8_t;

class InputStream;
class TagHandler;

/**
 * Parse an APE tag name into a #TagType.
 */
[[gnu::pure]]
TagType tag_ape_name_parse(std::string_view name) noexcept;

/**
 * Scan the APE tags of a stream.
 *
 * Throws on I/O error.
 *
 * @param path_fs the path of the file in filesystem encoding
 */
bool
tag_ape_scan2(InputStream &is, TagHandler &handler);

#endif
