#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseStackedTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/stackedTexturePacks.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t directions, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == 1);
		CHECK(texturePack->getDirectionCount(0) == directions);
		CHECK(texturePack->getDirectionCount(1) == 1);
		CHECK(texturePack->getPalette() == palette);
	};

	auto CHECK_TexturePackGroup = [](const std::shared_ptr<TexturePack>& texturePack,
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

	// 2 texturePacks + overlapping indexes
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 10, 1, {});

	CHECK_TextureGet(texturePack, 1, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 7, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 8, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 9, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 10, { 4, 4 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 11);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 1, 6 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 10 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 1, 10 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 10 }, {});

	// 2 texturePacks + index gaps
	texturePack = game.Resources().getTexturePack("key02");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 17, 1, pal);

	CHECK_TextureGet(texturePack, 1, { 8, 4 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 3, { 8, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, pal, { 1, 1 });

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 7);
	CHECK_InvalidGet(texturePack, 8);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 13);
	CHECK_InvalidGet(texturePack, 14);

	CHECK_TextureGet(texturePack, 15, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 16, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 4, 4 }, { 4, 4 }, {}, {});

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 1, 6 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 1, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 17 }, {});

	// 1 texturePack
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 6, 1, pal);

	CHECK_TextureGet(texturePack, 1, { 8, 4 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 3, { 8, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, pal, { 1, 1 });

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 7);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 1, 6 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 6 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 1, 6 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 6 }, {});

	// 2 texturePacks + multiple directions + no overrides
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePackGroup(texturePack, 12, 3, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 2);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 11, { 8, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 12, { 0, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 13, { 8, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 14, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 15, { 8, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 16, { 0, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 17, { 8, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 18, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 19, { 8, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 20, { 0, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 21, { 8, 4 }, { 8, 4 }, pal, {});

	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 20, 20 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});

	// 2 texturePacks + multiple directions + overrides
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePackGroup(texturePack, 12, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 13, { 8, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 14, { 0, 0 }, { 8, 4 }, pal, {});

	CHECK_TextureGet(texturePack, 15, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 16, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 4, 4 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 18, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 19, { 8, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 20, { 0, 4 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 21, { 8, 4 }, { 8, 4 }, pal, {});

	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 20, 20 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 2, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 3, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});
}
