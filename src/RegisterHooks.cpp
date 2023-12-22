#include "RegisterHooks.h"
#include "Game/ShaderManager2.h"
#include "Hooks.h"
#include "Parser/ParseAction2.h"
#include "Parser/ParseFile2.h"
#include "Parser/ParsePredicate2.h"
#include "Parser/Resources/ParseImageContainer2.h"
#include "Parser/Resources/ParseTexture2.h"
#ifdef PHYSFS_MPQ_SUPPORT
#include "PhysFSArchiverMPQ.h"
#endif
#include "Resources/Pcx.h"
#include "Utils/Log.h"

namespace Hooks
{
#ifdef PHYSFS_MPQ_SUPPORT
	static void RegisterMPQArchiver()
	{
		if (PHYSFS_registerArchiver(&PHYSFS_Archiver_MPQ) == 0)
		{
			SPDLOG_ERROR("RegisterMPQArchiver");
		}
	}
#endif

	void registerHooks()
	{
		InitializeShaderManager = ShaderManager2::init;
		DecodeImage = ImageUtils::LoadImagePCX;
		ParseDocumentElem = Parser2::parseDocumentElem;
		ParseActionElem = Parser2::parseActionElem;
		GetPredicateObj = Parser2::getPredicateObj;
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		GetImageContainerObj = Parser2::GetImageContainerObj;
#endif
		ParseTextureImg = Parser2::parseTextureImg;
#ifdef PHYSFS_MPQ_SUPPORT
		RegisterArchivers = RegisterMPQArchiver;

		ArchiveExtensions.push_back(".mpq");
#endif
	}
}
