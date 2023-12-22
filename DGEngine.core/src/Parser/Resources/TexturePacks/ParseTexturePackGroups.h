#pragma once

#include "Game/AnimationInfo.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/GroupTexturePack.h"
#include <type_traits>

namespace Parser
{
	void parseTexturePackGroupDirectionVector(const rapidjson::Value& elem, uint32_t& directions,
		std::vector<std::pair<uint32_t, uint32_t>>& directionsVec);

	template<class GroupTP = GroupTexturePack>
	void parseTexturePackGroups(std::unique_ptr<TexturePack>& texturePack, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<GroupTexturePack, GroupTP>);

		using namespace std::literals;

		if (isValidArray(elem, "groups") == false &&
			isValidObject(elem, "groups") == false)
		{
			return;
		}

		auto defaultAnimInfo = texturePack->getAnimation(-1, -1);

		auto texturePack2 = dynamic_cast<GroupTP*>(texturePack.get());
		if (texturePack2 == nullptr)
		{
			texturePack = std::make_unique<GroupTP>(std::move(texturePack));
			texturePack2 = dynamic_cast<GroupTP*>(texturePack.get());
		}

		auto parseGroup = [](const rapidjson::Value& elem, const AnimationInfo& defaultAnimInfo)
		{
			AnimationGroup group;

			parseTexturePackGroupDirectionVector(elem, group.directions, group.directionsVec);

			group.indexRange = getRange0Key(elem, "range", defaultAnimInfo.indexRange);
			group.animType = getAnimationTypeKey(elem, "animationType", defaultAnimInfo.animType);
			group.refresh = getTimeUKey(elem, "refresh", defaultAnimInfo.refresh);

			return group;
		};

		const auto& elemGroups = elem["groups"sv];
		if (elemGroups.IsArray() == true)
		{
			for (const auto& val : elemGroups)
			{
				texturePack2->addGroup(std::move(parseGroup(val, defaultAnimInfo)));
			}
		}
		else
		{
			texturePack2->addGroup(std::move(parseGroup(elemGroups, defaultAnimInfo)));
		}
	}
}
