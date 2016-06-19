#include "ParseAction.h"
#include "GameUtils.h"
#include "ParseScrollableText.h"
#include "ParseText.h"
#include "ParseUtils.h"
#include "ScrollableText.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseScrollableText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		auto text = parseDrawableTextObj(game, elem);
		if (text == nullptr)
		{
			return;
		}

		auto split = getUInt(elem, "splitText");
		if (split > 0)
		{
			auto splitText = Utils::splitInLines(text->getText(), split);
			text->setText(splitText);
		}

		auto speed = sf::milliseconds(getUInt(elem, "refresh"));
		auto scrollable = std::make_shared<ScrollableText>(std::move(text), speed);

		auto anchor = getAnchor(elem, "anchor");
		scrollable->setAnchor(anchor);
		auto pos = getVector2f<sf::Vector2f>(elem, "position");
		auto size = getVector2f<sf::Vector2f>(elem, "boxSize");
		if (getBool(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		scrollable->Position(pos);
		scrollable->Size(size);
		scrollable->Visible(getBool(elem, "visible", true));

		scrollable->setLoop(getBool(elem, "loop"));
		scrollable->setPause(getBool(elem, "pause"));

		scrollable->reset();
		scrollable->updateViewPort(game);

		if (elem.HasMember("onComplete"))
		{
			scrollable->setAction(parseAction(game, elem["onComplete"]));
		}

		game.Resources().addDrawable(elem["id"].GetString(), scrollable);
	}
}
