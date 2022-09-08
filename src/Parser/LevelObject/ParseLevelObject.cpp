#include "ParseLevelObject.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/SimpleLevelObject/SimpleLevelObject.h"
#include "ParseLevelObjectCommon.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseLevelObject(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		if (isValidString(elem, "class") == false)
		{
			return;
		}
		auto class_ = level->LevelObjects().getClass<SimpleLevelObjectClass>(elem["class"sv].GetStringView());
		if (class_ == nullptr)
		{
			return;
		}

		auto mapPos = getVector2UnsignedNumberKey<PairFloat, float>(elem, "mapPosition");
		if (level->Map().isMapCoordValid(mapPos) == false)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos];

		if (mapCell.getObject<SimpleLevelObject>() != nullptr)
		{
			return;
		}

		auto id = getStringViewKey(elem, "id");
		if (isValidId(id) == false)
		{
			id = {};
		}
		if (id.empty() == false && level->LevelObjects().get(id) != nullptr)
		{
			return;
		}

		auto levelObj = std::make_unique<SimpleLevelObject>(class_);

		levelObj->Id(id);
		levelObj->Hoverable(getBoolKey(elem, "enableHover", true));
		levelObj->Name(getStringViewKey(elem, "name"));
		levelObj->Text1(getStringViewKey(elem, "text1", class_->Text1()));
		levelObj->Text2(getStringViewKey(elem, "text2", class_->Text2()));
		levelObj->setCellSize(getVector2iKey<PairInt8>(elem, "size", class_->getCellSize()));

		parseLevelObjectProperties(*levelObj, *level, elem);

		levelObj->MapPosition(level->Map(), mapPos);

		level->addLevelObject(std::move(levelObj));
	}
}
