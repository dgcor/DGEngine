#include "ParseMountFile.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMountFile(Game& game, const Value& elem)
	{
		auto file = getStringKey(elem, "file");
		if (file.empty() == true)
		{
			return;
		}
		if (getBoolKey(elem, "useSaveDir") == false)
		{
			file = game.Path() + file;
		}
		else
		{
			file = FileUtils::getSaveDir() + file;
		}
		if (getBoolKey(elem, "unmount") == false)
		{
			auto mount = getStringViewKey(elem, "mount");
			bool append = getBoolKey(elem, "append", true);
			FileUtils::mount(file, mount, append);
		}
		else
		{
			FileUtils::unmount(file);
		}
	}
}
