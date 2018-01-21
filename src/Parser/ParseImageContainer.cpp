#include "ParseImageContainer.h"
#include "ImageContainers/CelImageContainer.h"
#include "ImageContainers/SimpleImageContainer.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<ImageContainer> parseImageContainerObj(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return nullptr;
		}

		std::string fileName = elem["file"].GetString();
		auto fileNameLower = Utils::toLower(fileName);
		bool isCel = Utils::endsWith(fileNameLower, "cel");
		bool isCl2 = Utils::endsWith(fileNameLower, "cl2");

		if (isCel == false && isCl2 == false)
		{
			auto frames = getFramesKey(elem, "frames");

			auto imgContainer = std::make_shared<SimpleImageContainer>(
				fileName.c_str(), frames.first, frames.second);

			if (imgContainer->size() == 0)
			{
				return nullptr;
			}
			return imgContainer;
		}
		else
		{
			auto isTileCel = getBoolKey(elem, "isTileCel", elem.HasMember("min"));
			auto imgContainer = std::make_shared<CelImageContainer>(
				fileName.c_str(), isCl2, isTileCel);

			if (imgContainer->size() == 0)
			{
				return nullptr;
			}
			if (elem.HasMember("celSize") == true)
			{
				auto size = getVector2uVal<sf::Vector2u>(elem["celSize"]);
				imgContainer->setDefaultSize(size.x, size.y);
			}
			return imgContainer;
		}
	}

	bool parseImageContainerFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getImageContainer(fromId);
					if (obj != nullptr)
					{
						game.Resources().addImageContainer(id, obj);
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
			id = elem["id"].GetString();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			std::string file(elem["file"].GetString());
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
		game.Resources().addImageContainer(id, imgRes);
	}

	bool getOrParseImageContainer(Game& game, const Value& elem,
		const char* idKey, std::shared_ptr<ImageContainer>& imgContainer)
	{
		if (isValidString(elem, idKey) == true)
		{
			std::string id = elem[idKey].GetString();
			imgContainer = game.Resources().getImageContainer(id);
			if (imgContainer != nullptr)
			{
				return true;
			}
			imgContainer = parseImageContainerObj(game, elem);
			if (isValidId(id) == true &&
				imgContainer != nullptr)
			{
				game.Resources().addImageContainer(id, imgContainer);
				return true;
			}
		}
		else
		{
			imgContainer = parseImageContainerObj(game, elem);
		}
		return false;
	}
}
