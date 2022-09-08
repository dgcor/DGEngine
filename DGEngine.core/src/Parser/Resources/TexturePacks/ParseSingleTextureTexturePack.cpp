#include "ParseSingleTextureTexturePack.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseTexturePackDirectionVector(const Value& elem, uint32_t& directions,
		std::vector<std::pair<uint32_t, uint32_t>>& directionsVec)
	{
		if (isValidArray(elem, "directions") == true)
		{
			for (const auto& dirVal : elem["directions"sv])
			{
				auto directionRange = getVector2uVal<std::pair<uint32_t, uint32_t>>(dirVal);
				if (directionRange.second < directionRange.first)
				{
					continue;
				}
				directionsVec.push_back(directionRange);
			}
		}
		else
		{
			directions = getUIntKey(elem, "directions");
		}
	}
}
