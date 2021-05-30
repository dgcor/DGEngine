#include "ParseScrollable.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "Scrollable.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Scrollable> getScrollableObj(Game& game, const Value& elem,
		std::shared_ptr<UIObject>& uiObj)
	{
		// scrollable will manage drawing of the UIObject to scroll
		game.Resources().deleteDrawable(uiObj.get());

		auto speed = getTimeKey(elem, "refresh", sf::milliseconds(50));
		auto scrollable = std::make_shared<Scrollable>(uiObj, speed);

		auto anchor = getAnchorKey(elem, "anchor");
		scrollable->setAnchor(anchor);
		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		scrollable->Position(pos);
		scrollable->Size(size);
		scrollable->Visible(getBoolKey(elem, "visible", true));

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

	void parseScrollable(Game& game, const Value& elem,
		const getScrollableObjFuncPtr getScrollableObjFunc)
	{
		assert(getScrollableObjFunc != nullptr);

		if (isValidString(elem, "id") == false ||
			isValidString(elem, "drawable") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto uiObj = game.Resources().getDrawableSharedPtr<UIObject>(getStringViewVal(elem["drawable"sv]));
		if (uiObj == nullptr)
		{
			return;
		}

		auto scrollable = getScrollableObjFunc(game, elem, uiObj);
		if (scrollable == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr &&
				panel != uiObj.get())
			{
				panel->addDrawable(scrollable);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, scrollable, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseScrollable(Game& game, const Value& elem)
	{
		parseScrollable(game, elem, getScrollableObj);
	}
}
