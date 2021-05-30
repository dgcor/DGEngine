#include "ParsePanel.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Panel> getPanelObj(Game& game, const Value& elem)
	{
		auto panel = std::make_shared<Panel>(getBoolKey(elem, "relativePositions"));

		auto anchor = getAnchorKey(elem, "anchor");
		panel->setAnchor(anchor);
		sf::Vector2f size;
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		panel->Position(pos);
		panel->Visible(getBoolKey(elem, "visible", true));

		return panel;
	}

	void parsePanel(Game& game, const Value& elem,
		const getPanelObjFuncPtr getPanelObjFunc)
	{
		assert(getPanelObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto panel = getPanelObjFunc(game, elem);
		if (panel == nullptr)
		{
			return;
		}

		game.Resources().addDrawable(
			id, panel, true, getStringViewKey(elem, "resource")
		);
	}

	void parsePanel(Game& game, const Value& elem)
	{
		parsePanel(game, elem, getPanelObj);
	}
}
