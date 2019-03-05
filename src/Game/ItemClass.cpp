#include "ItemClass.h"
#include "GameUtils.h"

ItemClass::ItemClass(const std::shared_ptr<TexturePack>& textureDrop_,
	const std::shared_ptr<TexturePack>& textureInventory_,
	size_t inventoryIdx_) : textureDrop(textureDrop_),
	textureInventory(textureInventory_), inventoryIdx(inventoryIdx_)
{
	if (textureDrop_ != nullptr)
	{
		dropTextureIndexRange.first = 0;
		dropTextureIndexRange.second = textureDrop_->size();
	}
}

void ItemClass::setDropTexturePack(const std::shared_ptr<TexturePack>& textureDrop_) noexcept
{
	textureDrop = textureDrop_;
	if (textureDrop_ != nullptr)
	{
		dropTextureIndexRange.first = 0;
		dropTextureIndexRange.second = textureDrop_->size();
	}
}

bool ItemClass::getInventoryTexture(const Queryable& item, TextureInfo& ti) const
{
	Variable newInvIdx;
	if (classifiers.getVar(InventoryTextureClassifier, item, newInvIdx) == true)
	{
		if (std::holds_alternative<int64_t>(newInvIdx) == true)
		{
			if (textureInventory->get((size_t)std::get<int64_t>(newInvIdx), ti) == true)
			{
				return true;
			}
		}
	}
	return textureInventory->get(inventoryIdx, ti);
}

bool ItemClass::getFullName(const Queryable& item, std::string& fullName) const
{
	std::string strPrefix;
	std::string strSuffix;
	classifiers.getText(PrefixClassifier, item, strPrefix);
	classifiers.getText(SuffixClassifier, item, strSuffix);

	bool hasPrefix = strPrefix.empty() == false;
	bool hasSuffix = strSuffix.empty() == false;

	if (hasPrefix == false && hasSuffix == false)
	{
		return false;
	}
	if (hasPrefix == true)
	{
		fullName = GameUtils::replaceStringWithQueryable(strPrefix, item) + ' ';
	}
	else
	{
		fullName.clear();
	}
	if (shortName.empty() == false)
	{
		fullName += shortName;
	}
	else
	{
		fullName += name;
	}
	if (hasSuffix == true)
	{
		fullName += ' ' + GameUtils::replaceStringWithQueryable(strSuffix, item);
	}
	return true;
}

LevelObjValue ItemClass::getClassifierNumOrFormula(size_t idx, const Queryable& item) const
{
	Variable var;
	if (classifiers.getVar(idx, item, var) == true)
	{
		if (std::holds_alternative<std::string>(var) == true)
		{
			Formula f(std::get<std::string>(var));
			return (LevelObjValue)f.eval(item);
		}
		else if (std::holds_alternative<int64_t>(var) == true)
		{
			return (LevelObjValue)std::get<int64_t>(var);
		}
		else if (std::holds_alternative<double>(var) == true)
		{
			return (LevelObjValue)std::get<double>(var);
		}
	}
	return 0;
}

LevelObjValue ItemClass::getPricePrefix1(const Queryable& item) const
{
	return getClassifierNumOrFormula(PricePrefix1Classifier, item);
}

LevelObjValue ItemClass::getPricePrefix2(const Queryable& item) const
{
	return getClassifierNumOrFormula(PricePrefix2Classifier, item);
}

LevelObjValue ItemClass::getPriceSuffix1(const Queryable& item) const
{
	return getClassifierNumOrFormula(PriceSuffix1Classifier, item);
}

LevelObjValue ItemClass::getPriceSuffix2(const Queryable& item) const
{
	return getClassifierNumOrFormula(PriceSuffix2Classifier, item);
}

void ItemClass::setPricePrefix1(Classifier* classifier)
{
	classifiers.set(PricePrefix1Classifier, classifier, 0);
}

void ItemClass::setPricePrefix2(Classifier* classifier)
{
	classifiers.set(PricePrefix2Classifier, classifier, 0);
}

void ItemClass::setPriceSuffix1(Classifier* classifier)
{
	classifiers.set(PriceSuffix1Classifier, classifier, 0);
}

void ItemClass::setPriceSuffix2(Classifier* classifier)
{
	classifiers.set(PriceSuffix2Classifier, classifier, 0);
}

void ItemClass::setInventoryTexture(Classifier* classifier)
{
	classifiers.set(InventoryTextureClassifier, classifier, 0);
}

void ItemClass::setPrefix(Classifier* classifier)
{
	classifiers.set(PrefixClassifier, classifier, 0);
}

void ItemClass::setSuffix(Classifier* classifier)
{
	classifiers.set(SuffixClassifier, classifier, 0);
}

void ItemClass::setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
{
	classifiers.set(DescriptionClassifier + idx, classifier, skipFirst);
}

bool ItemClass::getDescription(size_t idx, const Queryable& item, std::string& description) const
{
	return classifiers.getText(DescriptionClassifier + idx, item, description);
}

const SpellInstance* ItemClass::getSpell() const noexcept
{
	if (spell.spell == nullptr)
	{
		return nullptr;
	}
	return &spell;
}

void ItemClass::setSpell(Spell* obj)
{
	spell.spell = obj;
	spell.spellLevel = 1;
}

void ItemClass::setFormula(uint16_t nameHash, const std::string_view formula)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	formulas.setValue(nameHash, formula);
}

void ItemClass::deleteFormula(uint16_t nameHash)
{
	if (nameHash == str2int16(""))
	{
		return;
	}
	formulas.deleteValue(nameHash);
}

bool ItemClass::evalFormula(uint16_t nameHash, const Queryable& item,
	const Queryable& itemOwner, LevelObjValue& val, const std::string_view minMaxNumber) const
{
	const auto formula = formulas.getValue(nameHash);
	if (formula != nullptr)
	{
		val = (LevelObjValue)formula->eval(item, itemOwner, minMaxNumber);
		return true;
	}
	return false;
}

LevelObjValue ItemClass::evalFormula(uint16_t nameHash, const Queryable& item,
	const Queryable& itemOwner, const std::string_view minMaxNumber) const
{
	LevelObjValue val = 0;
	evalFormula(nameHash, item, itemOwner, val, minMaxNumber);
	return val;
}
