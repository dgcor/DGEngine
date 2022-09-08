#include "ParsePanel2.h"
#include "Game/Drawables/Panel.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Parser/ParseCommon.h"
#include "Parser/Drawables/ParsePanel.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

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
				drawable.anchorTo = level->LevelObjects().getByQueryId(getStringViewKey(elem, "anchorTo"));
				drawable.offset = getVector2fKey<sf::Vector2f>(elem, "offset");
				drawable.visibleRectOffset = (float)std::clamp(
					getNumberKey<int>(elem, "visibleRectOffset", -1), -1, 10
				);
				level->Drawables().add(*level, drawable);
				manageObjDrawing = false;
			}
		}

		game.Resources().addDrawable(
			id, panel, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
