#include "SpellClass.h"
#include "GameUtils.h"

void SpellClass::setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
{
	if (idx < descriptions.size())
	{
		descriptions[idx] = std::make_pair(classifier, skipFirst);
	}
}

bool SpellClass::getDescription(size_t idx, const Queryable& item, std::string& description) const
{
	if (idx >= descriptions.size())
	{
		return false;
	}
	auto classifier = descriptions[idx].first;
	if (classifier == nullptr)
	{
		return false;
	}
	description = VarUtils::toString(classifier->get(item, descriptions[idx].second));
	if (description.empty() == false)
	{
		description = GameUtils::replaceStringWithQueryable(description, item);
	}
	return true;
}
