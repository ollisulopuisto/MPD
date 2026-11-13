// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include "CharUtil.hxx"
#include "StringAPI.hxx"

#ifdef _UNICODE
#include "WStringCompare.hxx"
#endif

#include <string_view>

[[gnu::pure]] [[gnu::nonnull]]
static constexpr bool
StringIsEmpty(const char *string) noexcept
{
	return *string == 0;
}

[[gnu::pure]]
static constexpr bool
StringIsEqual(std::string_view a, std::string_view b) noexcept
{
	return a == b;
}

[[gnu::pure]]
static inline bool
StringIsEqualIgnoreCase(std::string_view a, std::string_view b) noexcept
{
	return a.size() == b.size() &&
		StringIsEqualIgnoreCase(a.data(), b.data(), b.size());
}

/**
 * Optimized prefix check.
 * Checks the initial character upfront before calling StringIsEqual,
 * skipping strncmp for non-matching candidates.
 */
[[gnu::pure]] [[gnu::nonnull]]
static inline bool
StringStartsWith(const char *haystack, std::string_view needle) noexcept
{
	if (needle.empty())
		return true;

	/* cheap rejection of the common case; the full comparison below
	   keeps the exact strncmp() semantics (it never reads past the
	   terminator of "haystack") */
	if (*haystack != needle.front())
		return false;

	return StringIsEqual(haystack, needle.data(), needle.size());
}

[[gnu::pure]]
static constexpr bool StringStartsWith(std::string_view haystack,
				       std::string_view needle) noexcept {
	return haystack.starts_with(needle);
}

[[gnu::pure]] [[gnu::nonnull]]
bool StringEndsWith(const char *haystack, std::string_view needle) noexcept;

[[gnu::pure]]
static constexpr bool StringEndsWith(std::string_view haystack,
				     std::string_view needle) noexcept {
	return haystack.ends_with(needle);
}

[[gnu::pure]] [[gnu::nonnull]]
bool StringEndsWithIgnoreCase(const char *haystack, std::string_view needle) noexcept;

/**
 * Returns the portion of the string after a prefix.  If the string
 * does not begin with the specified prefix, this function returns
 * nullptr.
 */
[[gnu::pure]] [[gnu::nonnull]]
static inline const char *
StringAfterPrefix(const char *haystack, std::string_view needle) noexcept
{
	return StringStartsWith(haystack, needle)
		? haystack + needle.size()
		: nullptr;
}

[[gnu::pure]]
static inline std::string_view
StringAfterPrefix(std::string_view haystack, std::string_view needle) noexcept
{
	return haystack.starts_with(needle)
		? haystack.substr(needle.size())
		: std::string_view{};
}

/**
 * Optimized case-insensitive prefix check.
 * Performs fast uppercase initial-character filtering to avoid
 * expensive case-insensitive string comparisons for non-matching prefixes.
 */
[[gnu::pure]] [[gnu::nonnull]]
static inline bool
StringStartsWithIgnoreCase(const char *haystack, std::string_view needle) noexcept
{
	if (needle.empty())
		return true;

	if (ToUpperASCII(*haystack) != ToUpperASCII(needle.front()))
		return false;

	return StringIsEqualIgnoreCase(haystack, needle.data(), needle.size());
}

[[gnu::pure]]
static inline bool
StringStartsWithIgnoreCase(std::string_view haystack, std::string_view needle) noexcept
{
	return haystack.size() >= needle.size() &&
	       (needle.empty() ||
		(ToUpperASCII(haystack.front()) == ToUpperASCII(needle.front()) &&
		 StringIsEqualIgnoreCase(haystack.data() + 1, needle.data() + 1,
					 needle.size() - 1)));
}

[[gnu::pure]]
static inline bool
StringEndsWithIgnoreCase(std::string_view haystack, std::string_view needle) noexcept
{
	/* Avoid substr() slice creation: directly compare needle-sized suffix */
	return haystack.size() >= needle.size() &&
	       StringIsEqualIgnoreCase(haystack.data() + haystack.size() - needle.size(),
				       needle.data(), needle.size());
}

/**
 * Returns the portion of the string after a prefix.  If the string
 * does not begin with the specified prefix, this function returns
 * nullptr.
 * This function is case-independent.
 */
[[gnu::pure]] [[gnu::nonnull]]
static inline const char *
StringAfterPrefixIgnoreCase(const char *haystack, std::string_view needle) noexcept
{
	return StringStartsWithIgnoreCase(haystack, needle)
		? haystack + needle.size()
		: nullptr;
}

[[gnu::pure]]
static inline std::string_view
StringAfterPrefixIgnoreCase(std::string_view haystack,
			    std::string_view needle) noexcept
{
	return StringStartsWithIgnoreCase(haystack, needle)
		? haystack.substr(needle.size())
		: std::string_view{};
}

/**
 * Check if the given string ends with the specified suffix.  If yes,
 * returns the position of the suffix, and nullptr otherwise.
 */
[[gnu::pure]] [[gnu::nonnull]]
const char *FindStringSuffix(const char *p, std::string_view suffix) noexcept;

template<typename T>
constexpr bool
SkipPrefix(std::basic_string_view<T> &haystack,
	   std::basic_string_view<T> needle) noexcept
{
	bool match = haystack.starts_with(needle);
	if (match)
		haystack.remove_prefix(needle.size());
	return match;
}

template<typename T>
constexpr bool
RemoveSuffix(std::basic_string_view<T> &haystack,
	     std::basic_string_view<T> needle) noexcept
{
	bool match = haystack.ends_with(needle);
	if (match)
		haystack.remove_suffix(needle.size());
	return match;
}

template<typename T>
constexpr bool
SkipPrefixIgnoreCase(std::basic_string_view<T> &haystack,
		     std::basic_string_view<T> needle) noexcept
{
	bool match = StringStartsWithIgnoreCase(haystack, needle);
	if (match)
		haystack.remove_prefix(needle.size());
	return match;
}

template<typename T>
constexpr bool
RemoveSuffixIgnoreCase(std::basic_string_view<T> &haystack,
		       std::basic_string_view<T> needle) noexcept
{
	bool match = StringEndsWithIgnoreCase(haystack, needle);
	if (match)
		haystack.remove_suffix(needle.size());
	return match;
}
