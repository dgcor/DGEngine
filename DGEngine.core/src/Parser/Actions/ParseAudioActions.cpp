#include "ParseAudioActions.h"
#include "Game/Actions/ActAudio.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseAudioDelete(const Value& elem)
	{
		return std::make_shared<ActAudioDelete>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseAudioPause(const Value& elem)
	{
		return std::make_shared<ActAudioPause>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseAudioPauseAll()
	{
		return std::make_shared<ActAudioPauseAll>();
	}

	std::shared_ptr<Action> parseAudioPlay(const Value& elem)
	{
		auto action = std::make_shared<ActAudioPlay>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "volume"),
			getBoolKey(elem, "clear"));

		if (elem.HasMember("loop"sv) == true)
		{
			action->setLoop(getBoolVal(elem["loop"sv]));
		}
		return action;
	}

	std::shared_ptr<Action> parseAudioResumeAll(const Value& elem)
	{
		return std::make_shared<ActAudioResumeAll>(getBoolKey(elem, "restart"));
	}

	std::shared_ptr<Action> parseAudioSeek(const Value& elem)
	{
		return std::make_shared<ActAudioSeek>(
			getStringViewKey(elem, "id"),
			getTimeUKey(elem, "time"));
	}

	std::shared_ptr<Action> parseAudioSetVolume(const Value& elem)
	{
		return std::make_shared<ActAudioSetVolume>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "volume"));
	}

	std::shared_ptr<Action> parseAudioStop(const Value& elem)
	{
		return std::make_shared<ActAudioStop>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseAudioStopAll()
	{
		return std::make_shared<ActAudioStopAll>();
	}
}
