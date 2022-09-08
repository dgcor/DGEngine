#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/BitmapFontTexturePack.h"
#include "Utils/NumberVector.h"

namespace Parser
{
	template<class BitmapFontTP = BitmapFontTexturePack>
	std::unique_ptr<TexturePack> parseBitmapFontTexturePack(Game& game, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}
		auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
		if (texture == nullptr)
		{
			return nullptr;
		}

		std::shared_ptr<Palette> palette;
		if (isValidString(elem, "palette") == true)
		{
			palette = game.Resources().getPalette(elem["palette"sv].GetStringView());
		}

		auto rows = getIntKey(elem, "rows", 16);
		auto cols = getIntKey(elem, "cols", 16);
		auto newLine = (int16_t)getIntKey(elem, "newLine", 0);
		auto space = (int16_t)getIntKey(elem, "space", 0);
		auto tab = (int16_t)getIntKey(elem, "tab", 0);
		bool isVertical = getStringViewKey(elem, "direction") == "vertical";

		std::unique_ptr<BitmapFontTP> texturePack;

		if (elem.HasMember("charSizeFile"sv) == true)
		{
			NumberVector<uint8_t> charSizes(getStringViewVal(elem["charSizeFile"sv]), 0, 0x7FFF);
			auto startPos = getUIntKey(elem, "charSizeFileStart", 0);
			auto stepNBytes = getUIntKey(elem, "charSizeFileStep", 1);
			texturePack = std::make_unique<BitmapFontTP>(
				texture, palette, rows, cols, newLine, space, tab,
				isVertical, charSizes.getContainer(), startPos, stepNBytes
			);
		}
		else
		{
			texturePack = std::make_unique<BitmapFontTP>(
				texture, palette, rows, cols, newLine, space, tab, isVertical);
		}
		return texturePack;
	}
}
