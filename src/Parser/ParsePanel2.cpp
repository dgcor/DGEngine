#include "ParsePanel2.h"
#include "Game.h"
#include "Game/Level.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Parser/ParsePanel.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	void parsePanel(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto panel = getPanelObj(game, elem);
		if (panel == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "level") == true)
		{
			auto levelId = getStringViewVal(elem["level"sv]);
			auto level = game.Resources().getLevel<Level>(levelId);
			if (level != nullptr)
			{
				LevelDrawable drawable;
				drawable.id = id;
				drawable.drawable = panel;
				drawable.anchorTo = level->getLevelObjectPtr(getStringViewKey(elem, "anchorTo"));
				drawable.offset = getVector2fKey<sf::Vector2f>(elem, "offset");
				drawable.visibleRectOffset = (float)std::clamp(
					getNumberKey<int>(elem, "visibleRectOffset", -1), -1, 10
				);
				level->addDrawable(drawable);
				manageObjDrawing = false;
			}
		}

		game.Resources().addDrawable(
			id, panel, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
