#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseCompositeTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/compositeTexturePacks.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
	};

	auto CHECK_InvalidGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, uint32_t direction = 0)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == false);
		CHECK(texturePack->getDirection(index) == direction);
	};

	auto CHECK_TextureGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t tiIndex, uint32_t index, const sf::Vector2i& position, const sf::Vector2i& size,
		const std::shared_ptr<Palette>& palette, const sf::Vector2f& offset,
		int32_t tiVecSize = -1)
	{
		TextureInfoVar tiVar;
		CHECK(texturePack->get(index, tiVar) == true);
		CHECK(std::holds_alternative<std::vector<TextureInfo>>(tiVar) == true);
		if (tiIndex == 0)
		{
			CHECK(texturePack->getTextureSize(index) == size);
		}

		const auto& tiVec = std::get<std::vector<TextureInfo>>(tiVar);
		CHECK(tiIndex < tiVec.size());
		if (tiVecSize >= 0)
		{
			CHECK(tiVec.size() == (size_t)tiVecSize);
		}
		if (tiIndex < tiVec.size())
		{
			const auto& ti = tiVec[tiIndex];
			CHECK(ti.texture != nullptr);
			CHECK(ti.textureRect.getPosition() == position);
			CHECK(ti.textureRect.getSize() == size);
			CHECK(ti.palette == palette);
			CHECK(ti.offset == offset);
			CHECK(ti.absoluteOffset == false);
			CHECK(ti.blendMode == BlendMode::Alpha);
			CHECK(ti.nextIndex == -1);
		}
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

	CHECK_TextureGet(texturePack, 0, 0, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 1, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 2, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 3, { 0, 0 }, { 8, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 0, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 1, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 2, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 3, { 0, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 3 }, {});

	// 2 texturePacks + multiple directions + overrides
	texturePack = game.Resources().getTexturePack("key02");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);
	CHECK_InvalidGet(texturePack, 20, 1);
	CHECK_InvalidGet(texturePack, 21);
	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 17 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 17 }, {});

	// 2 texturePacks (reverse order) + multiple directions + overrides
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 3, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 4);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 18, { 0, 0 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 19, { 8, 0 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 20, { 0, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 21, { 8, 4 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 20, 20 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});

	// 2 texturePacks + directionLayerOrders (same order)
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);
	CHECK_InvalidGet(texturePack, 20, 1);
	CHECK_InvalidGet(texturePack, 21);
	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 17 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 17 }, {});

	// 2 texturePacks + directionLayerOrders (different order)
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);
	CHECK_InvalidGet(texturePack, 20, 1);
	CHECK_InvalidGet(texturePack, 21);
	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 17 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 17 }, {});

	// 2 texturePacks + directionLayerOrders (invalid + overflow orders)
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);
	CHECK_InvalidGet(texturePack, 20, 1);
	CHECK_InvalidGet(texturePack, 21);
	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 17 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 2, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 17 }, {});

	// 2 texturePacks (reverse order) + directionLayerOrders (invalid + missing layer orders)
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 3, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 4);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 12, { 8, 0 }, { 4, 4 }, {}, {}, 1);
	CHECK_InvalidGet(texturePack, 13, 1);

	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 18, { 0, 0 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 19, { 8, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_InvalidGet(texturePack, 20, 1);

	CHECK_TextureGet(texturePack, 0, 21, { 8, 4 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 22);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 20, 20 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});

	// 2 texturePacks + directionLayerOrders (for all directions)
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);

	CHECK_TextureGet(texturePack, 0, 0, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 1, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 2, { 8, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 3, { 12, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 4, { 0, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 5, { 4, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 6, { 8, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 7, { 12, 4 }, { 4, 4 }, {}, {}, 1);

	CHECK_TextureGet(texturePack, 1, 0, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 1, { 8, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 2, { 0, 4 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 3, { 8, 4 }, { 8, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// 2 groups
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 20, 4, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 4);
	CHECK(texturePack->getDirectionCount(3) == 4);
	CHECK(texturePack->getDirectionCount(4) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 0, 10, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 11, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 12, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 13, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 14, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 15, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 16, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 17, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 18, { 0, 0 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 19, { 8, 0 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 20, { 0, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 21, { 8, 4 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 1, 10, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 11, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 12, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 15, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 16, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 1, 17, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_InvalidGet(texturePack, 22);
	CHECK_InvalidGet(texturePack, 23);
	CHECK_InvalidGet(texturePack, 24);
	CHECK_InvalidGet(texturePack, 25);
	CHECK_InvalidGet(texturePack, 26);
	CHECK_InvalidGet(texturePack, 27);
	CHECK_InvalidGet(texturePack, 28);
	CHECK_InvalidGet(texturePack, 29);
	CHECK_InvalidGet(texturePack, 30);
	CHECK_InvalidGet(texturePack, 31);

	CHECK_TextureGet(texturePack, 0, 32, { 0, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 33, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 34, { 0, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 0, 35, { 8, 4 }, { 8, 4 }, pal, {}, 1);
	CHECK_TextureGet(texturePack, 0, 36, { 0, 0 }, { 8, 4 }, pal, {}, 1);

	CHECK_TextureGet(texturePack, 0, 37, { 8, 0 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 38, { 0, 4 }, { 8, 4 }, pal, {}, 2);
	CHECK_TextureGet(texturePack, 0, 39, { 8, 4 }, { 8, 4 }, pal, {}, 2);

	CHECK_TextureGet(texturePack, 1, 32, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 33, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 34, { 8, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 37, { 12, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 38, { 0, 4 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 39, { 4, 4 }, { 4, 4 }, {}, {}, 2);

	CHECK_InvalidGet(texturePack, 40);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 20, 20 }, {});
	CHECK_AnimationAndDirection(texturePack, 3, 0, AnimationType::Looped, { 32, 39 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 39 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 3, 1, AnimationType::Looped, { 32, 39 }, {});
	CHECK_Animation(texturePack, 4, 0, AnimationType::Looped, { 10, 39 }, {});

	// 2 groups + directionLayerOrders (for all directions)
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 2, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, 0, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 1, { 8, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 2, { 8, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 3, { 12, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 1, 0, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 1, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 2, { 0, 4 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 3, { 8, 4 }, { 8, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 4, { 0, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 5, { 8, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 6, { 8, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 0, 7, { 8, 4 }, { 8, 4 }, {}, {}, 2);

	CHECK_TextureGet(texturePack, 0, 8, { 0, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 9, { 4, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 10, { 8, 4 }, { 4, 4 }, {}, {}, 1);
	CHECK_TextureGet(texturePack, 0, 11, { 12, 4 }, { 4, 4 }, {}, {}, 1);

	CHECK_TextureGet(texturePack, 1, 4, { 0, 0 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 5, { 4, 0 }, { 4, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 6, { 0, 4 }, { 8, 4 }, {}, {}, 2);
	CHECK_TextureGet(texturePack, 1, 7, { 12, 0 }, { 4, 4 }, {}, {}, 2);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 4, 11 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 11 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 3 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 4, 11 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 4, 11 }, {});
	CHECK_Animation(texturePack, 2, -1, AnimationType::Looped, { 0, 11 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 11 }, {});
}
