#include "ParseImageContainer2.h"
#include "FileUtils.h"
#include "Game.h"
#include "ImageContainers/CELImageContainer.h"
#include "ImageContainers/CL2ImageContainer.h"
#include "ImageContainers/DC6ImageContainer.h"
#include "ImageContainers/DCCImageContainer.h"
#include "ImageContainers/DT1ImageContainer.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	enum class ImageContainerType
	{
		Unknown,
		CEL,
		CL2,
		DC6,
		DCC,
		DT1
	};

	ImageContainerType getImageContainerType(const std::string_view fileName, const Value& elem)
	{
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
		return ImageContainerType::Unknown;
	}

	bool GetImageContainerObj(std::shared_ptr<FileBytes>& fileBytes,
		const std::string_view fileName, const Value& elem,
		std::shared_ptr<ImageContainer>& imgContainer)
	{
		switch (getImageContainerType(fileName, elem))
		{
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
		case ImageContainerType::Unknown:
		default:
			return false;
		}
		return true;
	}
}
