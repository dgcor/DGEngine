#include "ParseAction.h"
#include "GameUtils.h"
#include "ParseLoadingScreen.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseLoadingScreen(Game& game, const Value& elem)
	{
		if (isValidString(elem, "texture") == false)
		{
			return;
		}

		auto tex = game.Resources().getTexture(elem["texture"].GetString());
		if (tex == nullptr)
		{
			return;
		}

		auto loadingScreen = std::make_unique<LoadingScreen>(*tex);

		auto anchor = getAnchor(elem, "anchor");
		loadingScreen->setAnchor(anchor);
		auto pos = getVector2f<sf::Vector2f>(elem, "position");
		if (getBool(elem, "relativeCoords", true) == true)
		{
			auto sizeTex = tex->getSize();
			auto size = sf::Vector2f((float)sizeTex.x, (float)sizeTex.y);
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		loadingScreen->setPosition(pos);
		loadingScreen->setProgressBarColor(getColor(elem, "color"));
		loadingScreen->setProgressBarPositionOffset(getVector2f<sf::Vector2f>(elem, "progressBarOffset"));
		loadingScreen->setProgressBarSize(getVector2f<sf::Vector2f>(elem, "size"));

		if (elem.HasMember("onComplete"))
		{
			loadingScreen->setAction(parseAction(game, elem["onComplete"]));
		}

		game.setLoadingScreen(std::move(loadingScreen));
	}
}
