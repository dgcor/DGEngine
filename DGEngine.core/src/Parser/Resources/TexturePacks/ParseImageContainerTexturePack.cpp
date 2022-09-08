#include "ParseImageContainerTexturePack.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::vector<std::shared_ptr<ImageContainer>> getImageContainers(Game& game, const Value& elem)
	{
		std::vector<std::shared_ptr<ImageContainer>> imgContainers;

		const auto& imgElem = elem["imageContainer"sv];
		if (imgElem.IsString() == true)
		{
			auto imgCont = game.Resources().getImageContainer(imgElem.GetStringView());
			if (imgCont != nullptr)
			{
				imgContainers.push_back(imgCont);
			}
		}
		else if (imgElem.IsArray() == true)
		{
			for (const auto& val : imgElem)
			{
				auto imgCont = game.Resources().getImageContainer(getStringViewVal(val));
				if (imgCont != nullptr)
				{
					imgContainers.push_back(imgCont);
				}
			}
		}
		return imgContainers;
	}
}
