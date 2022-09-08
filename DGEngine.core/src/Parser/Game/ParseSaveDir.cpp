#include "ParseSaveDir.h"
#include "Game/Utils/FileUtils.h"
#include "Parser/Utils/ParseUtils.h"
#include "SFML/PhysFSStream.h"

namespace Parser
{
	using namespace rapidjson;

	void parseSaveDir(const Value& elem)
	{
		auto saveDir = getStringViewVal(elem);
		if (saveDir.size() > 0 && FileUtils::setSaveDir(saveDir.data()) == true)
		{
			PHYSFS_mount(PHYSFS_getWriteDir(), nullptr, 0);
		}
	}
}
