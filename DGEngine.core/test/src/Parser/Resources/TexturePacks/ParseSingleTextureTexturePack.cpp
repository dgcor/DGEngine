#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseSingleTextureTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/singleTextureTexturePacks.json");

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

	auto CHECK_InvalidGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, uint32_t direction = 0)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == false);
		CHECK(texturePack->getDirection(index) == direction);
	};

	auto CHECK_TextureGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, const sf::Vector2i& position, const sf::Vector2i& size,
		const std::shared_ptr<Palette>& palette, const sf::Vector2f& offset)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == true);
		CHECK(texturePack->getTextureSize(index) == size);

		CHECK(ti.texture != nullptr);
		CHECK(ti.textureRect.getPosition() == position);
		CHECK(ti.textureRect.getSize() == size);
		CHECK(ti.palette == palette);
		CHECK(ti.offset == offset);
		CHECK(ti.absoluteOffset == false);
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

	// texture
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 1, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 8 }, {}, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 0 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 0 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 0 }, {});

	// texture + startIndex + invalid palette
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 1, 1, {});

	CHECK_InvalidGet(texturePack, 0);

	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 16, 8 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 1, 1 }, sf::milliseconds(50));

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::PlayOnce, { 1, 1 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 1, 1 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 1 }, {});

	// range + horizontal direction
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, { 2, -2 });

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// range + vertical direction + palette
	texturePack = game.Resources().getTexturePack("key04");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, pal);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 11, { 0, 4 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 13, { 8, 4 }, { 8, 4 }, pal, { 2, -2 });

	CHECK_InvalidGet(texturePack, 14);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 13 }, {});

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(9) == 0);
	CHECK(texturePack->getDirection(14) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 13 }, {});

	// odd frame range + horizontal direction + odd directions < size
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 6, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 3, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 4, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 6, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 7, 7 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 6, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 7, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// odd frame range + horizontal direction + odd directions < size
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 7, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 3, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 4, 4 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 5, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 6, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 6, AnimationType::Looped, { 7, 7 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 7, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// odd frame range + horizontal direction + odd directions > size
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 9, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 3, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 4, 4 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 5, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 6, AnimationType::Looped, { 6, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 7, AnimationType::Looped, { 7, 7 }, {});

	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 8, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 9, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 10, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// odd frame range + horizontal direction + odd directions > size
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 16, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 3, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 4, AnimationType::Looped, { 4, 4 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 5, AnimationType::Looped, { 5, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 6, AnimationType::Looped, { 6, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 7, AnimationType::Looped, { 7, 7 }, {});

	CHECK(texturePack->getDirection(8) == 0);
	CHECK(texturePack->getDirection(15) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 8, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 15, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 16, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// odd frame range + vertical direction + invalid palette + 2 directions
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 4, 7 }, {});

	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// frame number - equal to [1, n]
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 2, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 4 }, { 16, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::BackAndForth, { 0, 1 }, sf::milliseconds(50));

	CHECK(texturePack->getDirection(2) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 1 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::BackAndForth, { 0, 1 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 1 }, {});

	// texture + invalid frame range (non integer split)
	texturePack = game.Resources().getTexturePack("key11");

	CHECK(texturePack == nullptr);

	// directions array
	texturePack = game.Resources().getTexturePack("key12");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 3, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 3, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 7, 7 }, {});

	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// directions array + invalid array element + missing direction for index 1
	texturePack = game.Resources().getTexturePack("key13");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 3, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 2, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 7, 7 }, {});

	CHECK(texturePack->getDirection(1) == 0);
	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// directions array + overlapping direction ranges + bad range (first > last)
	texturePack = game.Resources().getTexturePack("key14");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 1, 6 }, sf::milliseconds(50));

	CHECK(texturePack->getDirection(1) == 0);
	CHECK(texturePack->getDirection(3) == 0);
	CHECK(texturePack->getDirection(4) == 1);
	CHECK(texturePack->getDirection(6) == 1);
	CHECK(texturePack->getDirection(7) == 0);
	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 0, 7 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// directions array + overflow range
	texturePack = game.Resources().getTexturePack("key15");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4, 1);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 1 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 2, 12 }, {});

	CHECK(texturePack->getDirection(2) == 1);
	CHECK(texturePack->getDirection(4) == 1);
	CHECK(texturePack->getDirection(12) == 1);
	CHECK(texturePack->getDirection(13) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});
}
