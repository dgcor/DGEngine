#include "ParsePanelActions.h"
#include "Game/Actions/ActPanel.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parsePanelAddDrawable(const Value& elem)
	{
		return std::make_shared<ActPanelAddDrawable>(getStringViewKey(elem, "id"), getStringViewKey(elem, "drawable"));
	}
}
