#include "ParseImageContainer.h"
#include "FileUtils.h"
#include "Game.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "ImageContainers/CELImageContainer.h"
#include "ImageContainers/CL2ImageContainer.h"
#include "ImageContainers/DC6ImageContainer.h"
#include "ImageContainers/DCCImageContainer.h"
#include "ImageContainers/DT1ImageContainer.h"
#endif
#include "ImageContainers/SimpleImageContainer.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	enum class ImageContainerType
	{
		Simple,
		CEL,
		CL2,
		DC6,
		DCC,
		DT1
	};

	ImageContainerType getImageContainerType(const std::string_view fileName, const Value& elem)
	{
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		std::string fileType;

		if (isValidString(elem, "type") == true)
		{
			fileType = Utils::toLower(getStringViewVal(elem["type"sv]));
		}
		else
		{
			fileType = Utils::toLower(FileUtils::getFileExtension(fileName));
		}

		if (fileType.empty() == false && fileType.front() == '.')
		{
			fileType = fileType.substr(1);
		}

		if (fileType == "cel")
		{
			return ImageContainerType::CEL;
		}
		else if (fileType == "cl2")
		{
			return ImageContainerType::CL2;
		}
		else if (fileType == "dc6")
		{
			return ImageContainerType::DC6;
		}
		else if (fileType == "dcc")
		{
			return ImageContainerType::DCC;
		}
		else if (fileType == "dt1")
		{
			return ImageContainerType::DT1;
		}
#endif
		return ImageContainerType::Simple;
	}

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

		switch (getImageContainerType(fileName, elem))
		{
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		case ImageContainerType::CEL:
		{
			imgContainer = std::make_shared<CELImageContainer>(fileBytes);
			break;
		}
		case ImageContainerType::CL2:
		{
			imgContainer = std::make_shared<CL2ImageContainer>(fileBytes);
			break;
		}
		case ImageContainerType::DC6:
		{
			auto stitch = getBoolKey(elem, "stitch", true);
			auto useOffsets = getBoolKey(elem, "useOffsets");
			imgContainer = std::make_shared<DC6ImageContainer>(fileBytes, stitch, useOffsets);
			break;
		}
		case ImageContainerType::DCC:
		{
			imgContainer = std::make_shared<DCCImageContainer>(fileBytes);
			break;
		}
		case ImageContainerType::DT1:
		{
			imgContainer = std::make_shared<DT1ImageContainer>(fileBytes);
			break;
		}
#endif
		case ImageContainerType::Simple:
		default:
		{
			auto frames = getFramesKey(elem, "frames");
			auto directions = getUIntKey(elem, "directions");
			imgContainer = std::make_shared<SimpleImageContainer>(
				fileName, frames.first, frames.second, directions, false);
			break;
		}
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
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
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
			id = elem["id"sv].GetStringView();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = getStringViewVal(elem["file"sv]);
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
		auto imgRes = getImageContainerObj(game, elem);
		if (imgRes == nullptr)
		{
			return;
		}
		game.Resources().addImageContainer(id, imgRes, getStringViewKey(elem, "resource"));
	}
}
