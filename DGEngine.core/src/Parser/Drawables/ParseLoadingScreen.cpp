#include "ParseLoadingScreen.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseLoadingScreen(Game& game, const Value& elem)
	{
		std::unique_ptr<LoadingScreen> loadingScreen;

		auto tex = game.Resources().getTexture(getStringViewKey(elem, "texture"));
		if (tex != nullptr)
		{
			loadingScreen = std::make_unique<LoadingScreen>(*tex);
		}
		else
		{
			auto texPack = game.Resources().getTexturePack(getStringViewKey(elem, "texturePack"));
			if (texPack != nullptr)
			{
				auto animInfo = texPack->getAnimation(-1, -1);
				animInfo.indexRange = getFramesKey(elem, "frames", animInfo.indexRange);
				loadingScreen = std::make_unique<LoadingScreen>(texPack, animInfo);
			}
		}

		if (loadingScreen == nullptr)
		{
			loadingScreen = std::make_unique<LoadingScreen>();
		}

		if (elem.HasMember("textureRect"sv))
		{
			sf::IntRect rect = loadingScreen->getTextureRect();
			loadingScreen->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		parseDrawableProperties(game, elem, *loadingScreen);

		loadingScreen->setProgressBarColor(getColorKey(elem, "color"));
		loadingScreen->setProgressBarPositionOffset(getVector2fKey<sf::Vector2f>(elem, "progressBarOffset"));
		loadingScreen->setProgressBarSize(getVector2fKey<sf::Vector2f>(elem, "size"));

		if (elem.HasMember("onComplete"sv))
		{
			loadingScreen->setAction(
				str2int16("complete"), getActionVal(game, elem["onComplete"sv])
			);
		}

		game.setLoadingScreen(std::move(loadingScreen));
	}
}
