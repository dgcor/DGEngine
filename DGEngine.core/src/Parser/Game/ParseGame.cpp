#include "ParseGame.h"
#include "Game/Game.h"
#include "ParseIcon.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseFile.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseSaveDir.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseGameElem(Game& game, uint16_t nameHash16, const Value& elem)
	{
		switch (nameHash16)
		{
		case str2int16("frameRate"):
			game.FrameRate(getUIntVal(elem));
			break;
		case str2int16("fullScreen"):
			game.FullScreen(getBoolVal(elem));
			break;
		case str2int16("icon"):
			parseIcon(game, elem);
			break;
		case str2int16("init"):
			game.init();
			break;
		case str2int16("load"):
			parseLoad(game, elem);
			break;
		case str2int16("keepAR"):
			game.KeepAR(getBoolVal(elem, true));
			break;
		case str2int16("maxHeight"):
			game.MaxHeight(getUIntVal(elem));
			break;
		case str2int16("minSize"):
		{
			sf::Vector2u minSize(Game::MinSizeX, Game::MinSizeY);
			game.MinSize(getVector2uVal<sf::Vector2u>(elem, minSize));
			break;
		}
		case str2int16("onClose"):
			game.setAction(str2int16("close"), getActionVal(game, elem));
			break;
		case str2int16("onFocusGain"):
			game.setAction(str2int16("focusGain"), getActionVal(game, elem));
			break;
		case str2int16("onFocusLoss"):
			game.setAction(str2int16("focusLoss"), getActionVal(game, elem));
			break;
		case str2int16("onResize"):
			game.setAction(str2int16("resize"), getActionVal(game, elem));
			break;
		case str2int16("position"):
			game.Position(getVector2iVal<sf::Vector2i>(elem));
			break;
		case str2int16("refSize"):
		{
			sf::Vector2u refSize(Game::RefSizeX, Game::RefSizeY);
			game.RefSize(getVector2uVal<sf::Vector2u>(elem, refSize));
			break;
		}
		case str2int16("saveDir"):
			parseSaveDir(elem);
			break;
		case str2int16("size"):
		{
			sf::Vector2u defSize(Game::DefaultSizeX, Game::DefaultSizeY);
			game.Size(getVector2uVal<sf::Vector2u>(elem, defSize));
			break;
		}
		case str2int16("smoothScreen"):
			game.SmoothScreen(getBoolVal(elem));
			break;
		case str2int16("stretchToFit"):
			game.StretchToFit(getBoolVal(elem));
			break;
		case str2int16("title"):
			game.Title(getStringVal(elem, game.Title()));
			break;
		case str2int16("version"):
			game.Version(getStringVal(elem, game.Version()));
			break;
		default:
			break;
		}
	}

	void parseGame(Game& game, const Value& elem)
	{
		if (elem.IsObject() == false)
		{
			return;
		}
		for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
		{
			parseGameElem(game, str2int16(it->name.GetStringView()), it->value);
		}
	}
}
