// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#include "StringCompare.hxx"

#include <cstring>

/**
 * Optimized suffix check.
 * Accepting std::string_view for needle eliminates runtime strlen(needle) calls
 * when passing literals or string views.
 */
bool StringEndsWith(const char *haystack, std::string_view needle) noexcept {
	const size_t haystack_length = StringLength(haystack);

	return haystack_length >= needle.size() &&
	       std::memcmp(haystack + haystack_length - needle.size(), needle.data(),
			   needle.size()) == 0;
}

/**
 * Optimized case-insensitive suffix check.
 * Skips runtime strlen(needle) by accepting std::string_view.
 */
bool StringEndsWithIgnoreCase(const char *haystack, std::string_view needle) noexcept {
	const size_t haystack_length = StringLength(haystack);

	return haystack_length >= needle.size() &&
	       StringIsEqualIgnoreCase(haystack + haystack_length - needle.size(),
				       needle.data(), needle.size());
}

/**
 * Optimized suffix finder.
 * Skips runtime strlen(suffix) by accepting std::string_view.
 */
const char *FindStringSuffix(const char *p, std::string_view suffix) noexcept {
	const size_t p_length = StringLength(p);

	if (p_length < suffix.size())
		return nullptr;

	const char *q = p + p_length - suffix.size();
	return std::memcmp(q, suffix.data(), suffix.size()) == 0 ? q : nullptr;
}
