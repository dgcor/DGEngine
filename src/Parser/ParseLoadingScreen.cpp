#include "ParseAction.h"
#include "GameUtils.h"
#include "ParseLoadingScreen.h"
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

		auto anchor = getAnchorKey(elem, "anchor");
		loadingScreen->setAnchor(anchor);
		auto size = loadingScreen->getSize();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
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
