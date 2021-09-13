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

#include <liblangutil/DebugInfoSelection.h>

#include <liblangutil/Exceptions.h>

#include <libsolutil/StringUtils.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/split.hpp>

#include <vector>

using namespace std;
using namespace solidity;
using namespace solidity::langutil;
using namespace solidity::util;

DebugInfoSelection const DebugInfoSelection::All(bool _value) {
	DebugInfoSelection result;
	for (bool DebugInfoSelection::* member: componentMap() | ranges::views::values)
		result.*member = _value;
	return result;
}

optional<DebugInfoSelection> DebugInfoSelection::fromString(string_view _input)
{
	// TODO: Make more stuff constexpr and make it a static_assert().
	solAssert(componentMap().count("all") == 0, "");
	solAssert(componentMap().count("none") == 0, "");

	DebugInfoSelection selection;
	if (_input == "all")
		return All();
	if (_input == "none")
		return None();

	for (auto const& component: _input | ranges::views::split(',') | ranges::to<vector<string>>)
		if (!selection.enable(component))
			return nullopt;

	return selection;
}

bool DebugInfoSelection::enable(string_view _component)
{
	// TODO: Use transparent comparator to avoid materializing the string?
	auto memberIt = componentMap().find(string(_component));
	if (memberIt == componentMap().end())
		return false;

	this->*(memberIt->second) = true;
	return true;
}

bool DebugInfoSelection::any() const
{
	for (bool DebugInfoSelection::* member: componentMap() | ranges::views::values)
		if (this->*member)
			return true;

	return false;
}

bool DebugInfoSelection::all() const
{
	for (bool DebugInfoSelection::* member: componentMap() | ranges::views::values)
		if (!(this->*member))
			return false;

	return true;
}

ostream& langutil::operator<<(ostream& _stream, DebugInfoSelection const& _selection)
{
	vector<string> selectedComponentNames;
	for (auto const& [name, member]: _selection.componentMap())
		if (_selection.*member)
			selectedComponentNames.push_back(name);

	return _stream << joinHumanReadable(selectedComponentNames, ",");
}
