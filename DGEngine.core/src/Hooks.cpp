#include "Hooks.h"

namespace Hooks
{
	InitializeShaderManagerFuncPtr InitializeShaderManager{ nullptr };

	ParseDocumentElemFuncPtr ParseDocumentElem{ nullptr };

	ParseActionElemFuncPtr ParseActionElem{ nullptr };

	GetPredicateObjFuncPtr GetPredicateObj{ nullptr };

	GetImageContainerObjFuncPtr GetImageContainerObj{ nullptr };

	DecodeImageFuncPtr DecodeImage{ nullptr };

	ParseTextureImgFuncPtr ParseTextureImg{ nullptr };

	RegisterArchiversFuncPtr RegisterArchivers{ nullptr };

	std::vector<std::string_view> ArchiveExtensions{ ".zip" , ".7z" };
}
