#include "ParseScrollable.h"
#include "Game/Drawables/Scrollable.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Scrollable> getScrollableObj(Game& game, const Value& elem)
	{
		auto uiObj = game.Resources().getDrawableSharedPtr<UIObject>(getStringViewVal(elem["drawable"sv]));
		if (uiObj == nullptr)
		{
			return nullptr;
		}

		// scrollable will manage drawing of the UIObject to scroll
		game.Resources().deleteDrawable(uiObj.get());

		auto speed = getTimeKey(elem, "refresh", sf::milliseconds(50));
		auto scrollable = std::make_shared<Scrollable>(uiObj, speed);

		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
		parseDrawableProperties(game, elem, *scrollable, size);

		scrollable->setOffset((float)getIntKey(elem, "offset"));
		scrollable->setLoop(getBoolKey(elem, "loop"));
		scrollable->setPause(getBoolKey(elem, "pause"));

		scrollable->reset();
		scrollable->updateView(game);

		if (elem.HasMember("onComplete"sv))
		{
			scrollable->setAction(str2int16("complete"), getActionVal(game, elem["onComplete"sv]));
		}
		return scrollable;
	}

	static std::shared_ptr<UIObject> parseScrollableFunc(Game& game, const Value& elem)
	{
		return getScrollableObj(game, elem);
	}

	void parseScrollable(Game& game, const Value& elem)
	{
		if (isValidString(elem, "drawable") == false)
		{
			return;
		}
		parseValidIdAndAddDrawable(game, elem, parseScrollableFunc);
	}
}
