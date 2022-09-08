#include "ParseImageContainer.h"
#include <cassert>
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Hooks.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseResource.h"
#include "Resources/FileBytes.h"
#include "Resources/ImageContainers/SimpleImageContainer.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<ImageContainer> getImageContainerObj(Game& game, const Value& elem)
	{
		std::shared_ptr<FileBytes> fileBytes;
		std::string_view fileName;
		if (isValidString(elem, "fileBytes") == true)
		{
			fileBytes = game.Resources().getFileBytes(getStringViewVal(elem["fileBytes"sv]));
		}
		else if (isValidString(elem, "file") == true)
		{
			fileName = getStringViewVal(elem["file"sv]);
			fileBytes = std::make_shared<FileBytes>(FileUtils::readChar(fileName));
		}
		if (fileBytes == nullptr || fileBytes->empty() == true)
		{
			return nullptr;
		}

		std::shared_ptr<ImageContainer> imgContainer;

		if (Hooks::GetImageContainerObj == nullptr ||
			Hooks::GetImageContainerObj(fileBytes, fileName, elem, imgContainer) == false)
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

	void parseImageContainerF(Game& game, const Value& elem, const getImageContainerObjFuncPtr getImageContainerObjFunc)
	{
		assert(getImageContainerObjFunc != nullptr);

		if (parseResourceFromId<std::shared_ptr<ImageContainer>>(game.Resources(), elem) == true)
		{
			return;
		}

		auto id = parseValidIdOrFile(elem);
		if (id.empty() == true)
		{
			return;
		}

		if (game.Resources().hasImageContainer(id) == true)
		{
			return;
		}
		auto imageContainer = getImageContainerObjFunc(game, elem);
		if (imageContainer == nullptr)
		{
			return;
		}
		game.Resources().addImageContainer(id, imageContainer, getStringViewKey(elem, "resource"));
	}

	void parseImageContainer(Game& game, const Value& elem)
	{
		parseImageContainerF(game, elem, getImageContainerObj);
	}
}
