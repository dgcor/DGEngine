#include "ParseSoundActions.h"
#include "Game/Actions/ActSound.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseSoundLoadPlay(const Value& elem)
	{
		return std::make_shared<ActSoundLoadPlay>(
			getStringViewKey(elem, "file"),
			getVariableKey(elem, "volume"),
			getTimeKey(elem, "seek"),
			getBoolKey(elem, "unique"));
	}

	std::shared_ptr<Action> parseSoundPlay(const Value& elem)
	{
		return std::make_shared<ActSoundPlay>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "volume"),
			getTimeKey(elem, "seek"),
			getBoolKey(elem, "unique"));
	}
}
