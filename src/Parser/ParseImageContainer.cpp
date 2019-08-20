#include "ParseImageContainer.h"
#include "Game.h"
#include "ImageContainers/CELImageContainer.h"
#include "ImageContainers/CL2ImageContainer.h"
#include "ImageContainers/DC6ImageContainer.h"
#include "ImageContainers/DCCImageContainer.h"
#include "ImageContainers/DT1ImageContainer.h"
#include "ImageContainers/SimpleImageContainer.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<ImageContainer> parseImageContainerObj(Game& game,
		const Value& elem, const char* fileElem)
	{
		if (isValidString(elem, fileElem) == false)
		{
			return nullptr;
		}

		std::shared_ptr<ImageContainer> imgContainer;

		auto fileName = getStringViewVal(elem[fileElem]);
		auto fileNameLower = Utils::toLower(fileName);

#ifndef NO_DIABLO_FORMAT_SUPPORT
		if (Utils::endsWith(fileNameLower, ".cel") == true)
		{
			imgContainer = std::make_shared<CELImageContainer>(fileName);
		}
		else if (Utils::endsWith(fileNameLower, ".cl2") == true)
		{
			imgContainer = std::make_shared<CL2ImageContainer>(fileName);
		}
		else if (Utils::endsWith(fileNameLower, ".dc6") == true)
		{
			auto stitch = getBoolKey(elem, "stitch", true);
			auto useOffsets = getBoolKey(elem, "useOffsets");
			imgContainer = std::make_shared<DC6ImageContainer>(fileName, stitch, useOffsets);
		}
		else if (Utils::endsWith(fileNameLower, ".dcc") == true)
		{
			imgContainer = std::make_shared<DCCImageContainer>(fileName);
		}
		else if (Utils::endsWith(fileNameLower, ".dt1") == true)
		{
			imgContainer = std::make_shared<DT1ImageContainer>(fileName);
		}
#else
		if (false) {}
#endif
		else
		{
			auto frames = getFramesKey(elem, "frames");
			auto directions = getUIntKey(elem, "directions");
			imgContainer = std::make_shared<SimpleImageContainer>(
				fileName, frames.first, frames.second, directions, false);
		}

		if (imgContainer->size() == 0)
		{
			return nullptr;
		}
		imgContainer->setBlendMode(getBlendModeKey(elem, "blendMode"));
		return imgContainer;
	}

	bool parseImageContainerFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"].GetStringStr();
				auto id = elem["id"].GetStringStr();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getImageContainer(fromId);
					if (obj != nullptr)
					{
						game.Resources().addImageContainer(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseImageContainer(Game& game, const Value& elem)
	{
		if (parseImageContainerFromId(game, elem) == true)
		{
			return;
		}
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetStringStr();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = getStringViewVal(elem["file"]);
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}
		if (game.Resources().hasImageContainer(id) == true)
		{
			return;
		}
		auto imgRes = parseImageContainerObj(game, elem);
		if (imgRes == nullptr)
		{
			return;
		}
		game.Resources().addImageContainer(id, imgRes, getStringViewKey(elem, "resource"));
	}

	bool getOrParseImageContainer(Game& game, const Value& elem,
		const char* idKey, std::shared_ptr<ImageContainer>& imgContainer,
		const char* fileElem)
	{
		if (isValidString(elem, idKey) == true)
		{
			auto id = elem[idKey].GetStringStr();
			imgContainer = game.Resources().getImageContainer(id);
			if (imgContainer != nullptr)
			{
				return true;
			}
			imgContainer = parseImageContainerObj(game, elem, fileElem);
			if (isValidId(id) == true &&
				imgContainer != nullptr)
			{
				game.Resources().addImageContainer(id, imgContainer, getStringViewKey(elem, "resource"));
				return true;
			}
		}
		else
		{
			imgContainer = parseImageContainerObj(game, elem, fileElem);
		}
		return false;
	}
}
