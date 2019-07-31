#include "ParseLevelObject.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/SimpleLevelObject.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseLevelObject(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		if (isValidString(elem, "class") == false)
		{
			return;
		}
		auto class_ = level->getClass<SimpleLevelObjectClass>(elem["class"].GetStringStr());
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

		auto id = getStringKey(elem, "id");
		if (isValidId(id) == false)
		{
			id = {};
		}
		if (id.empty() == false && level->getLevelObject(id) != nullptr)
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

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto name = it->name.GetStringView();
						auto nameHash = str2int16(name);
						level->setPropertyName(nameHash, name);
						levelObj->setIntByHash(nameHash,
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		levelObj->MapPosition(*level, mapPos);

		level->addLevelObject(std::move(levelObj));
	}
}
