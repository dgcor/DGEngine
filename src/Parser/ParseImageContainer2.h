#pragma once

#include "FileBytes.h"
#include "Json/JsonParser.h"
#include <memory>

class ImageContainer;

namespace Parser2
{
	bool GetImageContainerObj(std::shared_ptr<FileBytes>& fileBytes,
		const std::string_view fileName, const rapidjson::Value& elem,
		std::shared_ptr<ImageContainer>& imgContainer);
}
