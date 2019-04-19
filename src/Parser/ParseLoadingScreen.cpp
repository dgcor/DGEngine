#include "ParseLoadingScreen.h"
#include "Game.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseLoadingScreen(Game& game, const Value& elem)
	{
		std::unique_ptr<LoadingScreen> loadingScreen;

		auto tex = game.Resources().getTexture(getStringKey(elem, "texture"));
		if (tex != nullptr)
		{
			loadingScreen = std::make_unique<LoadingScreen>(*tex);
		}
		else
		{
			loadingScreen = std::make_unique<LoadingScreen>();
		}

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect = loadingScreen->getTextureRect();
			loadingScreen->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		auto anchor = getAnchorKey(elem, "anchor");
		loadingScreen->setAnchor(anchor);
		auto size = loadingScreen->getSize();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		loadingScreen->setPosition(pos);
		loadingScreen->setProgressBarColor(getColorKey(elem, "color"));
		loadingScreen->setProgressBarPositionOffset(getVector2fKey<sf::Vector2f>(elem, "progressBarOffset"));
		loadingScreen->setProgressBarSize(getVector2fKey<sf::Vector2f>(elem, "size"));

		if (elem.HasMember("onComplete"))
		{
			loadingScreen->setAction(parseAction(game, elem["onComplete"]));
		}

		game.setLoadingScreen(std::move(loadingScreen));
	}
}
