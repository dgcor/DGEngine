#include "ParseImage.h"
#include "Game.h"
#include "Game/Level.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePanel(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"].GetStringStr();
		if (isValidId(id) == false)
		{
			return;
		}

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

		bool manageObjDrawing = true;
		if (isValidString(elem, "level") == true)
		{
			std::string levelId = getStringVal(elem["level"]);
			auto level = game.Resources().getLevel(levelId);
			if (level != nullptr)
			{
				LevelDrawable drawable;
				drawable.id = id;
				drawable.drawable = panel;
				drawable.anchorTo = level->getLevelObjectPtr(getStringKey(elem, "anchorTo"));
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
