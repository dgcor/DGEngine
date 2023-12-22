#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexturePacks2", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks.json");

	auto CHECK_ImageContainer = [](const std::shared_ptr<ImageContainer>& imageContainer,
		uint32_t size, uint32_t directions, BlendMode blendMode)
	{
		CHECK(imageContainer != nullptr);
		CHECK(imageContainer->size() == size);
		CHECK(imageContainer->getDirections() == directions);
		CHECK(imageContainer->getBlendMode() == blendMode);
	};

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
	};

	auto CHECK_Animation = [](const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIndex, int32_t directionIndex, AnimationType animType,
		const std::pair<uint32_t, uint32_t>& indexRange, sf::Time refresh)
	{
		auto animInfo = texturePack->getAnimation(groupIndex, directionIndex);
		CHECK(animInfo.animType == animType);
		CHECK(animInfo.indexRange == indexRange);
		CHECK(animInfo.refresh == refresh);
	};

	auto CHECK_AnimationAndDirection = [&CHECK_Animation](const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIndex, uint32_t directionIndex, AnimationType animType,
		const std::pair<uint32_t, uint32_t>& indexRange, sf::Time refresh)
	{
		CHECK_Animation(texturePack, groupIndex, directionIndex, animType, indexRange, refresh);
		CHECK(texturePack->getDirection(indexRange.first) == directionIndex);
		CHECK(texturePack->getDirection(indexRange.second) == directionIndex);
	};

	// imageContainers
	auto imageContainer = game.Resources().getImageContainer("S7TRLITA1HTH");

	CHECK_ImageContainer(imageContainer, 128, 8, BlendMode::Add);

	imageContainer = game.Resources().getImageContainer("S7TRLITDDHTH");

	CHECK_ImageContainer(imageContainer, 8, 8, BlendMode::Add);

	imageContainer = game.Resources().getImageContainer("S7TRLITDTHTH");

	CHECK_ImageContainer(imageContainer, 112, 8, BlendMode::Add);

	imageContainer = game.Resources().getImageContainer("S7TRLITGHHTH");

	CHECK_ImageContainer(imageContainer, 48, 8, BlendMode::Add);

	imageContainer = game.Resources().getImageContainer("S7TRLITNUHTH");

	CHECK_ImageContainer(imageContainer, 64, 8, BlendMode::Add);

	imageContainer = game.Resources().getImageContainer("S7TRLITWLHTH");

	CHECK_ImageContainer(imageContainer, 144, 8, BlendMode::Add);

	// multiImageContainer texturePack
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 504, 6, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 16, 31 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 32, 47 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 48, 63 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 64, 79 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 80, 95 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 6, AnimationType::Looped, { 96, 111 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 7, AnimationType::Looped, { 112, 127 }, {});

	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 128, 128 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 129, 129 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 130, 130 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 3, AnimationType::Looped, { 131, 131 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 4, AnimationType::Looped, { 132, 132 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 5, AnimationType::Looped, { 133, 133 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 6, AnimationType::Looped, { 134, 134 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 7, AnimationType::Looped, { 135, 135 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 503 }, {});
	CHECK_Animation(texturePack, 0, 8, AnimationType::Looped, { 0, 127 }, {});
	CHECK_Animation(texturePack, 1, 8, AnimationType::Looped, { 128, 135 }, {});
	CHECK_Animation(texturePack, 2, 8, AnimationType::Looped, { 136, 247 }, {});
	CHECK_Animation(texturePack, 3, 8, AnimationType::Looped, { 248, 311 }, {});
	CHECK_Animation(texturePack, 4, 8, AnimationType::Looped, { 312, 359 }, {});
	CHECK_Animation(texturePack, 5, 8, AnimationType::Looped, { 360, 503 }, {});
	CHECK_Animation(texturePack, 6, 0, AnimationType::Looped, { 0, 503 }, {});

	// multiImageContainer texturePack + normalizeDirections
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 504, 6, {});

	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 0, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 16, 31 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 32, 47 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 7, AnimationType::Looped, { 48, 63 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 64, 79 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 80, 95 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 96, 111 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 6, AnimationType::Looped, { 112, 127 }, {});

	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 128, 128 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 3, AnimationType::Looped, { 129, 129 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 5, AnimationType::Looped, { 130, 130 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 7, AnimationType::Looped, { 131, 131 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 132, 132 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 133, 133 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 4, AnimationType::Looped, { 134, 134 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 6, AnimationType::Looped, { 135, 135 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 503 }, {});
	CHECK_Animation(texturePack, 0, 8, AnimationType::Looped, { 0, 127 }, {});
	CHECK_Animation(texturePack, 1, 8, AnimationType::Looped, { 128, 135 }, {});
	CHECK_Animation(texturePack, 2, 8, AnimationType::Looped, { 136, 247 }, {});
	CHECK_Animation(texturePack, 3, 8, AnimationType::Looped, { 248, 311 }, {});
	CHECK_Animation(texturePack, 4, 8, AnimationType::Looped, { 312, 359 }, {});
	CHECK_Animation(texturePack, 5, 8, AnimationType::Looped, { 360, 503 }, {});
	CHECK_Animation(texturePack, 6, 0, AnimationType::Looped, { 0, 503 }, {});
}
