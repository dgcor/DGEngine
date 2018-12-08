#pragma once

#include "AnimationType.h"
#include "Classifiers.h"
#include "LevelObjectClassDefaults.h"
#include <string>
#include <SFML/System/Time.hpp>
#include "TexturePacks/TexturePack.h"

class SimpleLevelObjectClass : public LevelObjectClassDefaults<LevelObjValue>
{
private:
	const sf::Texture* texture{ nullptr };
	sf::IntRect textureRect;
	std::shared_ptr<TexturePack> texturePack;
	std::pair<size_t, size_t> textureIndexRange;
	sf::Time frameTime;
	AnimationType animType{ AnimationType::PlayOnce };

	std::string name;
	std::string text1;
	std::string text2;

	Classifiers<3> classifiers;

public:
	SimpleLevelObjectClass(const sf::Texture& texture_) : texture(&texture_) {}
	SimpleLevelObjectClass(const std::shared_ptr<TexturePack>& texturePack_,
		const std::pair<size_t, size_t>& textureIndexRange_,
		const sf::Time& frameTime_, AnimationType animType_)
		: texturePack(texturePack_), textureIndexRange(textureIndexRange_),
		frameTime(frameTime_), animType(animType_) {}

	const sf::Texture* getTexture() const noexcept { return texture; }

	const sf::IntRect& getTextureRect() const noexcept { return textureRect; }
	void setTextureRect(const sf::IntRect& textureRect_) { textureRect = textureRect_; }

	const TexturePack* getTexturePack() const noexcept { return texturePack.get(); }

	const std::pair<size_t, size_t>& getTextureIndexRange() const noexcept
	{
		return textureIndexRange;
	}

	const sf::Time& getFrameTime() const noexcept { return frameTime; }

	AnimationType getAnimationType() const noexcept { return animType; }

	const std::string& Name() const noexcept { return name; }
	const std::string& Text1() const noexcept { return text1; }
	const std::string& Text2() const noexcept { return text2; }

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
};
