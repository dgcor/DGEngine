#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexturePackRects", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/texturePackRects.json");

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

	auto CHECK_TexturePackGroup = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
		CHECK(texturePack->getTexture() != nullptr);
	};

	auto CHECK_MultiTexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
		if (groups == 1)
		{
			CHECK(texturePack->getTexture() != nullptr);
		}
		else
		{
			CHECK(texturePack->getTexture() == nullptr);
		}
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

	// invalid rects
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// horizontal direction + invalid texture index
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 5, 1, {});

	CHECK_TextureGet(texturePack, 0, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 16, 8 }, {}, {});

	CHECK_InvalidGet(texturePack, 3);
	CHECK_InvalidGet(texturePack, 5);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 4 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 4 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 4 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 4 }, {});

	// directions number + unused offset
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 4, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// odd directions number + global offset + line offset
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 3, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, { 2, 2 });

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// directions number > size + line offset + absoluteOffsets + invertOffsets
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 5, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {}, true);
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {}, true);
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {}, true);
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, { 1, -2 }, true);

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 1, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// directions array
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 10, 4, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 16, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 9, { 0, 0 }, { 16, 8 }, {}, {});

	CHECK_InvalidGet(texturePack, 10);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 7, 7 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 8, 8 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 9, 9 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 9 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 0, 9 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 9 }, {});

	// multi textures + directions numbers + directions array
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 12, 3, {});
	CHECK(texturePack->getDirectionCount(0) == 3);
	CHECK(texturePack->getDirectionCount(1) == 3);
	CHECK(texturePack->getDirectionCount(2) == 9);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 9, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 10, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 12, 1);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 6, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 7, 7 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 8, 10 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 11, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::BackAndForth, { 16, 16 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::BackAndForth, { 17, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 2, AnimationType::BackAndForth, { 18, 18 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 3, AnimationType::BackAndForth, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 4, AnimationType::BackAndForth, { 20, 20 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 5, AnimationType::BackAndForth, { 21, 21 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 6, AnimationType::BackAndForth, { 22, 22 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 7, AnimationType::BackAndForth, { 23, 23 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 11 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 3, AnimationType::Looped, { 8, 15 }, {});
	CHECK_Animation(texturePack, 2, 8, AnimationType::BackAndForth, { 16, 23 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 0, 11 }, {});

	// groups
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack != nullptr);
	CHECK_TexturePackGroup(texturePack, 4, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 11, 12 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 11, 12 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 13, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 14, 14 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 11, 12 }, {});
	CHECK_Animation(texturePack, 1, 3, AnimationType::Looped, { 11, 14 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 3 }, {});

	// groups + multi textures
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 12, 3, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 3);
	CHECK(texturePack->getDirectionCount(2) == 3);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 9, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 10, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 12);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 6, 7 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 8, 8 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 9, 9 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::BackAndForth, { 10, 10 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::BackAndForth, { 11, 11 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 11 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 0, 5 }, {});
	CHECK_Animation(texturePack, 1, 3, AnimationType::Looped, { 6, 9 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::BackAndForth, { 10, 11 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 0, 11 }, {});

	// imageContainer
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 4, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_Animation(texturePack, 1, 2, AnimationType::Looped, { 2, 3 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 3 }, {});

	// multiImageContainer
	texturePack = game.Resources().getTexturePack("key11");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 4, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_Animation(texturePack, 1, 2, AnimationType::Looped, { 2, 3 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 3 }, {});

	// bitmapFont
	texturePack = game.Resources().getTexturePack("key12");

	CHECK(texturePack != nullptr);
	CHECK_TexturePackGroup(texturePack, 4, 2, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 2, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 3, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 0, 1 }, {});
	CHECK_Animation(texturePack, 1, 2, AnimationType::Looped, { 2, 3 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 3 }, {});

	// only groups (no rects)
	texturePack = game.Resources().getTexturePack("key13");

	CHECK(texturePack != nullptr);
	CHECK_TexturePackGroup(texturePack, 256, 3, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 17, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 254, { 225, 240 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 256);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 0 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 1, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 3, 4 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 5, 8 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 255 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::PlayOnce, { 0, 0 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::PlayOnce, { 0, 0 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 1, 4 }, {});
	CHECK_Animation(texturePack, 1, 2, AnimationType::Looped, { 1, 4 }, {});
	CHECK_Animation(texturePack, 2, -1, AnimationType::Looped, { 5, 8 }, {});
	CHECK_Animation(texturePack, 2, 1, AnimationType::Looped, { 5, 8 }, {});
	CHECK_Animation(texturePack, 3, -1, AnimationType::Looped, { 0, 255 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 0, 255 }, {});
}
