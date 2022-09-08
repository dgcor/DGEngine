#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/StackedTexturePack.h"

namespace Parser
{
	template<class StackedTP = StackedTexturePack>
	std::shared_ptr<StackedTP> parseStackedTexturePack(Game& game, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		auto texturePack = std::make_shared<StackedTP>();
		if (isValidArray(elem, "texturePacks") == true)
		{
			for (const auto& val : elem["texturePacks"sv])
			{
				auto texPack = game.Resources().getTexturePack(getStringVal(val));
				if (texPack != nullptr)
				{
					texturePack->addTexturePack(texPack);
				}
			}
		}
		if (texturePack->size() == 0)
		{
			return nullptr;
		}
		return texturePack;
	}
}
