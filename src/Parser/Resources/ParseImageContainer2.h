#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Resources/FileBytes.h"

class ImageContainer;

namespace Parser2
{
	bool GetImageContainerObj(std::shared_ptr<FileBytes>& fileBytes,
		const std::string_view fileName, const rapidjson::Value& elem,
		std::shared_ptr<ImageContainer>& imgContainer);
}
