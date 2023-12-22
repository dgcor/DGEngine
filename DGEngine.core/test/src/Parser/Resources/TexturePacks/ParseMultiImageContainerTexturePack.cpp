#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseMultiImageContainerTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/multiImageContainerTexturePacks.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t groups, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == groups);
		CHECK(texturePack->getPalette() == palette);
		CHECK(texturePack->getTexture() == nullptr);
	};

	auto CHECK_InvalidGet = [](const std::shared_ptr<TexturePack>& texturePack, uint32_t index)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == false);
		CHECK(texturePack->getDirection(index) == 0);
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
		int32_t groupIndex, int32_t directionIndex, const std::pair<uint32_t, uint32_t>& indexRange)
	{
		auto animInfo = texturePack->getAnimation(groupIndex, directionIndex);
		CHECK(animInfo.animType == AnimationType::Looped);
		CHECK(animInfo.indexRange == indexRange);
		CHECK(animInfo.refresh == sf::Time::Zero);
	};

	auto CHECK_AnimationAndDirection = [&CHECK_Animation](const std::shared_ptr<TexturePack>& texturePack,
		int32_t groupIndex, int32_t directionIndex, const std::pair<uint32_t, uint32_t>& indexRange)
	{
		CHECK_Animation(texturePack, groupIndex, directionIndex, indexRange);
		CHECK(texturePack->getDirection(indexRange.first) == directionIndex);
		CHECK(texturePack->getDirection(indexRange.second) == directionIndex);
	};

	// imageContainer array
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 2, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, { 2, -2 });

	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 9, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 11, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });

	CHECK_InvalidGet(texturePack, 12);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 3 });
	CHECK_AnimationAndDirection(texturePack, 1, 0, { 4, 11 });

	CHECK_Animation(texturePack, -1, -1, { 0, 11 });
	CHECK_Animation(texturePack, 0, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, 1, { 0, 3 });
	CHECK_Animation(texturePack, 1, -1, { 4, 11 });
	CHECK_Animation(texturePack, 1, 1, { 4, 11 });
	CHECK_Animation(texturePack, 2, -1, { 0, 11 });
	CHECK_Animation(texturePack, 2, 0, { 0, 11 });

	// imageContainer array + multiple directions
	texturePack = game.Resources().getTexturePack("key02");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 2, pal);
	CHECK(texturePack->getDirectionCount(0) == 2);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, pal, {});

	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 9, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 11, { 0, 0 }, { 4, 4 }, pal, {});

	CHECK_InvalidGet(texturePack, 12);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 1 });
	CHECK_AnimationAndDirection(texturePack, 0, 1, { 2, 3 });
	CHECK_AnimationAndDirection(texturePack, 1, 0, { 4, 5 });
	CHECK_AnimationAndDirection(texturePack, 1, 1, { 6, 7 });
	CHECK_AnimationAndDirection(texturePack, 1, 2, { 8, 9 });
	CHECK_AnimationAndDirection(texturePack, 1, 3, { 10, 11 });

	CHECK_Animation(texturePack, -1, -1, { 0, 11 });
	CHECK_Animation(texturePack, 0, 2, { 0, 3 });
	CHECK_Animation(texturePack, 1, 4, { 4, 11 });
	CHECK_Animation(texturePack, 2, 0, { 0, 11 });

	// imageContainer array + multiple odd directions
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 24, 3, {});
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 3);
	CHECK(texturePack->getDirectionCount(2) == 9);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 9, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 14, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 15, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_TextureGet(texturePack, 16, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 18, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 19, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 20, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 21, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 22, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 23, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 24);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 1 });
	CHECK_AnimationAndDirection(texturePack, 0, 1, { 2, 3 });
	CHECK_AnimationAndDirection(texturePack, 0, 2, { 4, 5 });
	CHECK_AnimationAndDirection(texturePack, 0, 3, { 6, 7 });
	CHECK_AnimationAndDirection(texturePack, 1, 0, { 8, 10 });
	CHECK_AnimationAndDirection(texturePack, 1, 1, { 11, 13 });
	CHECK_AnimationAndDirection(texturePack, 1, 2, { 14, 15 });
	CHECK_AnimationAndDirection(texturePack, 2, 0, { 16, 16 });
	CHECK_AnimationAndDirection(texturePack, 2, 1, { 17, 17 });
	CHECK_AnimationAndDirection(texturePack, 2, 2, { 18, 18 });
	CHECK_AnimationAndDirection(texturePack, 2, 3, { 19, 19 });
	CHECK_AnimationAndDirection(texturePack, 2, 4, { 20, 20 });
	CHECK_AnimationAndDirection(texturePack, 2, 5, { 21, 21 });
	CHECK_AnimationAndDirection(texturePack, 2, 6, { 22, 22 });
	CHECK_AnimationAndDirection(texturePack, 2, 7, { 23, 23 });

	CHECK_Animation(texturePack, -1, -1, { 0, 23 });
	CHECK_Animation(texturePack, 0, -1, { 0, 7 });
	CHECK_Animation(texturePack, 0, 4, { 0, 7 });
	CHECK_Animation(texturePack, 1, -1, { 8, 15 });
	CHECK_Animation(texturePack, 1, 3, { 8, 15 });
	CHECK_Animation(texturePack, 2, -1, { 16, 23 });
	CHECK_Animation(texturePack, 2, 8, { 16, 23 });
	CHECK_Animation(texturePack, 3, -1, { 0, 23 });
	CHECK_Animation(texturePack, 3, 0, { 0, 23 });
}
