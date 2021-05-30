#pragma once

#include "FileBytes.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/ParserProperties.h"
#include <string_view>
#include "Variable.h"
#include <vector>

class Action;
class Game;
class ImageContainer;
class Predicate;
class Queryable;
class ShaderManager;

namespace sf
{
	class Image;
	class InputStream;
}

namespace Hooks
{
	typedef void(*InitializeShaderManagerFuncPtr)(ShaderManager& shaderManager);
	typedef bool(*ParseDocumentElemFuncPtr)(Game& game, uint16_t nameHash16, const rapidjson::Value& elem,
		ReplaceVars& replaceVars, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
	typedef std::shared_ptr<Action>(*ParseActionElemFuncPtr)(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
	typedef std::shared_ptr<Predicate>(*GetPredicateObjFuncPtr)(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
	typedef bool(*GetImageContainerObjFuncPtr)(std::shared_ptr<FileBytes>& fileBytes,
		const std::string_view fileName, const rapidjson::Value& elem,
		std::shared_ptr<ImageContainer>& imgContainer);
	typedef bool(*DecodeImageFuncPtr)(sf::InputStream& file, sf::Image& image);
	typedef bool(*ParseTextureImgFuncPtr)(Game& game, const rapidjson::Value& elem, sf::Image& img);
	typedef void(*RegisterArchiversFuncPtr)();

	extern InitializeShaderManagerFuncPtr InitializeShaderManager;
	extern ParseDocumentElemFuncPtr ParseDocumentElem;
	extern ParseActionElemFuncPtr ParseActionElem;
	extern GetPredicateObjFuncPtr GetPredicateObj;
	extern GetImageContainerObjFuncPtr GetImageContainerObj;
	extern DecodeImageFuncPtr DecodeImage;
	extern ParseTextureImgFuncPtr ParseTextureImg;
	extern RegisterArchiversFuncPtr RegisterArchivers;

	extern std::vector<std::string_view> ArchiveExtensions;
}
