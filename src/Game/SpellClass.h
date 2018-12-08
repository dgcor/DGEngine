#pragma once

#include "Classifier.h"
#include "LevelObjectClassDefaults.h"
#include "Queryable.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "Utils/Utils.h"

class SpellClass : public LevelObjectClassDefaults<LevelObjValue>
{
private:
	std::shared_ptr<TexturePack> texturePack1;
	std::shared_ptr<TexturePack> texturePack2;
	size_t textureIdx1;
	size_t textureIdx2;

	std::string id;
	std::string name;
	std::string type;
	uint16_t idHash16{ 0 };
	uint16_t typeHash16{ 0 };

	std::array<std::pair<Classifier*, uint16_t>, 5> descriptions;

public:
	SpellClass(const std::shared_ptr<TexturePack>& texturePack1_,
		const std::shared_ptr<TexturePack>& texturePack2_,
		size_t textureIdx1_, size_t textureIdx2_) : texturePack1(texturePack1_),
		texturePack2(texturePack2_), textureIdx1(textureIdx1_), textureIdx2(textureIdx2_) {}

	void setTexturePack1(const std::shared_ptr<TexturePack>& texturePack_) noexcept
	{
		texturePack1 = texturePack_;
	}
	void setTexturePack2(const std::shared_ptr<TexturePack>& texturePack_) noexcept
	{
		texturePack2 = texturePack_;
	}

	void setTextureIndex1(size_t idx_) noexcept { textureIdx1 = idx_; }
	void setTextureIndex2(size_t idx_) noexcept { textureIdx2 = idx_; }

	bool getTexture1(TextureInfo& ti) const
	{
		return texturePack1->get(textureIdx1, ti);
	}
	bool getTexture2(TextureInfo& ti) const
	{
		return texturePack2->get(textureIdx2, ti);
	}

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const noexcept { return name; }
	const std::string& Type() const noexcept { return type; }
	uint16_t IdHash16() const noexcept { return idHash16; }
	uint16_t TypeHash16() const noexcept { return typeHash16; }

	void Id(const std::string id_)
	{
		id = id_;
		idHash16 = str2int16(id_);
	}
	void Name(const std::string name_) { name = name_; }
	void Type(const std::string type_)
	{
		type = type_;
		typeHash16 = str2int16(Utils::toLower(type_));
	}

	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst);

	bool getDescription(size_t idx, const Queryable& item, std::string& description) const;
};
