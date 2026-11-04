// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "Table.hxx"
#include "util/ASCII.hxx"
#include "util/CharUtil.hxx"
#include "util/StringCompare.hxx"

#include <cassert>
#include <string.h>

/**
 * Looks up a string in a tag translation table (case sensitive).
 * Returns TAG_NUM_OF_ITEM_TYPES if the specified name was not found
 * in the table.
 */
TagType
tag_table_lookup(const struct tag_table *table, const char *name) noexcept
{
	assert(name != nullptr);
	return tag_table_lookup(table, std::string_view{name});
}

/**
 * Optimized case-sensitive tag table lookup.
 * Performs fast first-character filtering before comparing the full string.
 */
TagType
tag_table_lookup(const struct tag_table *table, std::string_view name) noexcept
{
	if (name.empty())
		return TAG_NUM_OF_ITEM_TYPES;

	const char first = name.front();

	for (; table->name != nullptr; ++table) {
		if (table->name[0] == first && name == table->name)
			return table->type;
	}

	return TAG_NUM_OF_ITEM_TYPES;
}

/**
 * Looks up a string in a tag translation table (case insensitive).
 * Returns TAG_NUM_OF_ITEM_TYPES if the specified name was not found
 * in the table.
 */
TagType
tag_table_lookup_i(const struct tag_table *table, const char *name) noexcept
{
	assert(name != nullptr);
	return tag_table_lookup_i(table, std::string_view{name});
}

/**
 * Optimized case-insensitive tag table lookup.
 * Performs fast uppercase first-character check before invoking StringIsEqualIgnoreCase.
 * This skips expensive case-insensitive string comparisons for non-matching candidates.
 */
TagType
tag_table_lookup_i(const struct tag_table *table,
		   std::string_view name) noexcept
{
	if (name.empty())
		return TAG_NUM_OF_ITEM_TYPES;

	const char first_upper = ToUpperASCII(name.front());

	for (; table->name != nullptr; ++table) {
		if (ToUpperASCII(table->name[0]) == first_upper &&
		    StringIsEqualIgnoreCase(name, table->name))
			return table->type;
	}

	return TAG_NUM_OF_ITEM_TYPES;
}

const char *
tag_table_lookup(const tag_table *table, TagType type) noexcept
{
	for (; table->name != nullptr; ++table)
		if (table->type == type)
			return table->name;

	return nullptr;
}
