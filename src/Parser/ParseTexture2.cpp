#include "ParseTexture2.h"
#include "Game.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "ImageContainers/DC6ImageContainer.h"
#endif
#include "ImageUtils.h"
#include "Parser/ParseImageContainer.h"
#include "Parser/Utils/ParseUtils.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Utils/Utils.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	bool parseTextureImg(Game& game, const Value& elem, sf::Image& img)
	{
		auto path = getStringViewVal(elem["file"sv]);
		auto pathLower = Utils::toLower(path);

		if (Utils::endsWith(pathLower, ".cel") == true ||
			Utils::endsWith(pathLower, ".cl2") == true ||
			Utils::endsWith(pathLower, ".dc6") == true)
		{
			auto pal = game.Resources().getPalette(getStringKey(elem, "palette"));
			PaletteArray* palArray = nullptr;
			if (getBoolKey(elem, "indexed") == true &&
				game.Shaders().hasSpriteShader() == true)
			{
				pal = nullptr;
			}
			if (pal != nullptr)
			{
				palArray = &pal->palette;
			}
			auto imgContainer = getImageContainerObj(game, elem);
			if (imgContainer == nullptr)
			{
				return true;
			}
			if (elem.HasMember("charMapFile"sv) == true)
			{
				img = ImageUtils::loadBitmapFontImage(
					*imgContainer, getStringViewVal(elem["charMapFile"sv]), pal);
			}
			else if (elem.HasMember("frame"sv) == true)
			{
				auto frameIdx = getUIntVal(elem["frame"sv]);
				img = ImageUtils::loadImageFrame(*imgContainer, palArray, frameIdx);
			}
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
			else if (elem.HasMember("stitch"sv) == true &&
				elem.HasMember("size"sv) == true &&
				dynamic_cast<DC6ImageContainer*>(imgContainer.get()) != nullptr)
			{
				auto dc6ImgCont = dynamic_cast<DC6ImageContainer*>(imgContainer.get());
				auto stitch = getVector2uVal<sf::Vector2u>(elem["stitch"sv]);
				auto size = getVector2uVal<sf::Vector2u>(elem["size"sv]);
				img = dc6ImgCont->get(stitch, size, palArray);
			}
#endif
			else
			{
				img = ImageUtils::loadImage(*imgContainer, pal);
			}
			return true;
		}
		return false;
	}
}
