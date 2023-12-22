#include "catch2/catch_session.hpp"

#include "Game/Utils/FileUtils.h"
#include "Utils/Log.h"

int main(int argc, char* argv[])
{
	DGENGINE_INIT_LOGGING();

	FileUtils::initPhysFS(argv[0]);

	FileUtils::mount("data", "", true);

	int result = Catch::Session().run(argc, argv);

	FileUtils::deinitPhysFS();

	return result;
}
