#pragma once

#include "Game/AnimationType.h"
#include "Game/Classifiers.h"
#include "Game/LevelObject/LevelObjectClassDefaults.h"
#include "Resources/TexturePack.h"
#include <string>
#include <SFML/System/Time.hpp>

class SimpleLevelObjectClass : public LevelObjectClassDefaults<LevelObjValue>
{
private:
	const sf::Texture* texture{ nullptr };
	sf::IntRect textureRect;
	std::shared_ptr<TexturePack> texturePack;
	std::pair<uint32_t, uint32_t> textureIndexRange;
	sf::Time frameTime;
	AnimationType animType{ AnimationType::PlayOnce };

	std::string name;
	std::string text1;
	std::string text2;

	Classifiers<3> classifiers;

	PairInt8 cellSize;

public:
	SimpleLevelObjectClass(const sf::Texture& texture_) : texture(&texture_) {}
	SimpleLevelObjectClass(const std::shared_ptr<TexturePack>& texturePack_,
		const std::pair<uint32_t, uint32_t>& textureIndexRange_,
		const sf::Time& frameTime_, AnimationType animType_)
		: texturePack(texturePack_), textureIndexRange(textureIndexRange_),
		frameTime(frameTime_), animType(animType_) {}

	auto getTexture() const noexcept { return texture; }

	auto& getTextureRect() const noexcept { return textureRect; }
	void setTextureRect(const sf::IntRect& textureRect_) { textureRect = textureRect_; }

	auto& getTexturePack() const noexcept { return texturePack; }

	auto& getTextureIndexRange() const noexcept { return textureIndexRange; }

	auto& getFrameTime() const noexcept { return frameTime; }

	auto getAnimationType() const noexcept { return animType; }

	auto& Name() const noexcept { return name; }
	auto& Text1() const noexcept { return text1; }
	auto& Text2() const noexcept { return text2; }

	void Name(const std::string_view name_) { name = name_; }
	void Text1(const std::string_view text1_) { text1 = text1_; }
	void Text2(const std::string_view text2_) { text2 = text2_; }

	bool getFullName(const Queryable& obj, std::string& fullName) const
	{
		return classifiers.getText(0, obj, fullName);
	}
	void setNameClassifier(Classifier* classifier) { classifiers.set(0, classifier, 0); }

	bool getDescription(size_t idx, const Queryable& obj, std::string& description) const
	{
		return classifiers.getText(idx + 1, obj, description);
	}
	void setDescription(size_t idx, Classifier* classifier, uint16_t skipFirst)
	{
		classifiers.set(idx + 1, classifier, skipFirst);
	}

	auto getCellSize() const noexcept { return cellSize; }
	void setCellSize(PairInt8 cellSize_) noexcept { cellSize = cellSize_; }
};
