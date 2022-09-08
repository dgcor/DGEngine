#include "ParsePanel.h"
#include "Game/Drawables/Panel.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Panel> getPanelObj(Game& game, const Value& elem)
	{
		auto panel = std::make_shared<Panel>(getBoolKey(elem, "relativePositions"));

		sf::Vector2f size;
		parseDrawableProperties(game, elem, *panel, size);

		return panel;
	}

	void parsePanel(Game& game, const Value& elem)
	{
		auto id = parseValidId(elem);
		if (id.empty() == true)
		{
			return;
		}

		auto panel = getPanelObj(game, elem);
		if (panel == nullptr)
		{
			return;
		}

		game.Resources().addDrawable(
			id, panel, true, getStringViewKey(elem, "resource")
		);
	}
}
