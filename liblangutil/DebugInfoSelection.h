/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * Handles selections of debug info components.
 */

#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>

namespace solidity::langutil
{

/**
 * Represents a set of flags corresponding to components of debug info selected for some purpose.
 *
 * Provides extra functionality for enumerating the components and serializing/deserializing the
 * selection to/from a comma-separated string.
 */
struct DebugInfoSelection
{
	static DebugInfoSelection const All(bool _value = true);
	static DebugInfoSelection const None() { return All(false); }
	static DebugInfoSelection const Default() { return All(); }

	static std::optional<DebugInfoSelection> fromString(std::string_view _input);
	bool enable(std::string_view _component);

	bool all() const;
	bool any() const;
	bool none() const { return !any(); }

	bool operator!=(DebugInfoSelection const& _other) const noexcept { return !(*this == _other); }
	bool operator==(DebugInfoSelection const& _other) const noexcept
	{
		return location == _other.location && snippet == _other.snippet;
	}

	friend std::ostream& operator<<(std::ostream& _stream, DebugInfoSelection const& _selection);

	static auto const& componentMap()
	{
		static std::map<std::string, bool DebugInfoSelection::*> const components = {
			{"location", &DebugInfoSelection::location},
			{"snippet", &DebugInfoSelection::snippet},
		};
		return components;
	}

	bool location = false; ///< Include source location (e.g. `@src 3:50:100`)
	bool snippet = false;  ///< Include source code snippet next to location. E.g. `@src 3:50:100 "contract C {..."`
};

std::ostream& operator<<(std::ostream& _stream, DebugInfoSelection const& _selection);

}
