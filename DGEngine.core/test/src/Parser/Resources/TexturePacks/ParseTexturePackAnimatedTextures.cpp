#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexturePackAnimatedTextures", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/texturePackAnimatedTextures.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t directions, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == 1);
		CHECK(texturePack->getDirectionCount(0) == directions);
		CHECK(texturePack->getDirectionCount(1) == 1);
		CHECK(texturePack->getPalette() == palette);
		CHECK(texturePack->getTexture() != nullptr);
	};

	auto CHECK_MultiTexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
		CHECK(texturePack->getTexture() == nullptr);
	};

	auto CHECK_InvalidGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, uint32_t direction = 0)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == false);
		CHECK(texturePack->getDirection(index) == direction);
	};

	auto CHECK_TextureGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, const sf::Vector2i& position, const sf::Vector2i& size,
		const std::shared_ptr<Palette>& palette, const sf::Vector2f& offset,
		bool absoluteOffsets = false)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == true);
		CHECK(texturePack->getTextureSize(index) == size);

		CHECK(ti.texture != nullptr);
		CHECK(ti.textureRect.getPosition() == position);
		CHECK(ti.textureRect.getSize() == size);
		CHECK(ti.palette == palette);
		CHECK(ti.offset == offset);
		CHECK(ti.absoluteOffset == absoluteOffsets);
		CHECK(ti.blendMode == BlendMode::Alpha);
		CHECK(ti.nextIndex == -1);
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
		int32_t groupIndex, int32_t directionIndex, AnimationType animType,
		const std::pair<uint32_t, uint32_t>& indexRange, sf::Time refresh)
	{
		CHECK_Animation(texturePack, groupIndex, directionIndex, animType, indexRange, refresh);
		CHECK(texturePack->getDirection(indexRange.first) == directionIndex);
		CHECK(texturePack->getDirection(indexRange.second) == directionIndex);
	};

	// animatedTextures
	auto texturePack = game.Resources().getTexturePack("key01");
	int epoch = 0;

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});

	texturePack->update(epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});

	texturePack->update(epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(30));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 8, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(29));

	CHECK_TextureGet(texturePack, 2, { 8, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(0));

	CHECK_TextureGet(texturePack, 2, { 8, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(1));

	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// animatedTextures + different refresh rates
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(60));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 4, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(15));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 4, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(15));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(20));

	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 4, 4 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// animatedTextures + default refresh rate + custom index
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 1, { 12, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 1, { 12, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// animatedTextures + out of range custom index
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 50, { 12, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 50, { 12, 4 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 50, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// animatedTextures + textureIndexes
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, {});

	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 1);
	CHECK_InvalidGet(texturePack, 2);
	CHECK_InvalidGet(texturePack, 3);
	CHECK_InvalidGet(texturePack, 10);
	CHECK_InvalidGet(texturePack, 12);

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_InvalidGet(texturePack, 10);
	CHECK_InvalidGet(texturePack, 12);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 13 }, {});

	// textureIndexes + animatedTextures
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 14, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 10, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 12, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);
	CHECK_InvalidGet(texturePack, 9);
	CHECK_InvalidGet(texturePack, 14);

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 13 }, {});

	// animatedTextures + textureIndexes (different json order)
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 14, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);
	CHECK_InvalidGet(texturePack, 9);
	CHECK_InvalidGet(texturePack, 14);

	texturePack->update(++epoch, sf::milliseconds(50));

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 0, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 13 }, {});
}
