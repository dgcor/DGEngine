#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseMultiTextureTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/multiTextureTexturePacks.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
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

	// invalid texture array (missing id)
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack == nullptr);

	// texture array
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 1, 1, {});
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 8 }, {}, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 0 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 0 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 0 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 0 }, {});

	// palette + directions array
	texturePack = game.Resources().getTexturePack("key03");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, pal);
	CHECK(texturePack->getDirectionCount(0) == 3);
	CHECK(texturePack->getDirectionCount(1) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, pal, { 1, 1 });

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::BackAndForth, { 0, 2 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::BackAndForth, { 3, 6 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::BackAndForth, { 7, 7 }, sf::milliseconds(50));

	CHECK(texturePack->getDirection(8) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::BackAndForth, { 0, 7 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 0, 3, AnimationType::BackAndForth, { 0, 7 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// directions array + directions number
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 16, 2, {});
	CHECK(texturePack->getDirectionCount(0) == 3);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, {}, { 1, 1 });

	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 9, { 0, 2 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 10, { 0, 4 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 11, { 0, 6 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 13, { 8, 2 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 14, { 8, 4 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 15, { 8, 6 }, { 8, 2 }, {}, { 2, 1 });

	CHECK_InvalidGet(texturePack, 16);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::BackAndForth, { 0, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::BackAndForth, { 3, 6 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::BackAndForth, { 7, 7 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::PlayOnce, { 8, 9 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::PlayOnce, { 10, 11 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::PlayOnce, { 12, 13 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 3, AnimationType::PlayOnce, { 14, 15 }, sf::milliseconds(50));

	CHECK(texturePack->getDirection(16) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 15 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::BackAndForth, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 4, AnimationType::PlayOnce, { 8, 15 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 15 }, {});

	// directions array + directions number (reverse order)
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 16, 2, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 3);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 1, { 0, 2 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 3, { 0, 6 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 5, { 8, 2 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 7, { 8, 6 }, { 8, 2 }, pal, { 2, 1 });

	CHECK_TextureGet(texturePack, 8, { 0, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 9, { 0, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 10, { 0, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 11, { 0, 6 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 13, { 8, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 14, { 8, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 15, { 8, 6 }, { 8, 2 }, pal, { 1, 1 });

	CHECK_InvalidGet(texturePack, 16);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 0, 1 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 2, 3 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 4, 5 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 6, 7 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::BackAndForth, { 8, 10 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::BackAndForth, { 11, 14 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::BackAndForth, { 15, 15 }, {});

	CHECK(texturePack->getDirection(16) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 15 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 0, 7 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 3, AnimationType::BackAndForth, { 8, 15 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 15 }, {});

	// start index + directions array + directions number
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 16, 2, {});
	CHECK(texturePack->getDirectionCount(0) == 3);
	CHECK(texturePack->getDirectionCount(1) == 4);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 11, { 0, 2 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 12, { 0, 4 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 13, { 0, 6 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 14, { 8, 0 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 15, { 8, 2 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 16, { 8, 4 }, { 8, 2 }, {}, { 1, 1 });
	CHECK_TextureGet(texturePack, 17, { 8, 6 }, { 8, 2 }, {}, { 1, 1 });

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);

	CHECK_TextureGet(texturePack, 20, { 0, 0 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 21, { 0, 2 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 22, { 0, 4 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 23, { 0, 6 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 24, { 8, 0 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 25, { 8, 2 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 26, { 8, 4 }, { 8, 2 }, {}, { 2, 1 });
	CHECK_TextureGet(texturePack, 27, { 8, 6 }, { 8, 2 }, {}, { 2, 1 });

	CHECK_InvalidGet(texturePack, 28);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::BackAndForth, { 10, 12 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::BackAndForth, { 13, 16 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::BackAndForth, { 17, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::PlayOnce, { 20, 21 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::PlayOnce, { 22, 23 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::PlayOnce, { 24, 25 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 3, AnimationType::PlayOnce, { 26, 27 }, sf::milliseconds(50));

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(9) == 0);
	CHECK(texturePack->getDirection(18) == 0);
	CHECK(texturePack->getDirection(19) == 0);
	CHECK(texturePack->getDirection(28) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 27 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::BackAndForth, { 10, 17 }, {});
	CHECK_Animation(texturePack, 1, 4, AnimationType::PlayOnce, { 20, 27 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 27 }, {});

	// start index + directions array + directions number (reverse order)
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 16, 2, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 3);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 11, { 0, 2 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 12, { 0, 4 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 13, { 0, 6 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 14, { 8, 0 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 15, { 8, 2 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 16, { 8, 4 }, { 8, 2 }, pal, { 2, 1 });
	CHECK_TextureGet(texturePack, 17, { 8, 6 }, { 8, 2 }, pal, { 2, 1 });

	CHECK_InvalidGet(texturePack, 18);
	CHECK_InvalidGet(texturePack, 19);

	CHECK_TextureGet(texturePack, 20, { 0, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 21, { 0, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 22, { 0, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 23, { 0, 6 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 24, { 8, 0 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 25, { 8, 2 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 26, { 8, 4 }, { 8, 2 }, pal, { 1, 1 });
	CHECK_TextureGet(texturePack, 27, { 8, 6 }, { 8, 2 }, pal, { 1, 1 });

	CHECK_InvalidGet(texturePack, 28);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, sf::milliseconds(50));
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::BackAndForth, { 20, 22 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::BackAndForth, { 23, 26 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::BackAndForth, { 27, 27 }, {});

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(9) == 0);
	CHECK(texturePack->getDirection(18) == 0);
	CHECK(texturePack->getDirection(19) == 0);
	CHECK(texturePack->getDirection(28) == 0);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 27 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 10, 17 }, sf::milliseconds(50));
	CHECK_Animation(texturePack, 1, 3, AnimationType::BackAndForth, { 20, 27 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 10, 27 }, {});

	// overlapping directions + gaps
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 6, 3, {});
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 2);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 0 }, { 8, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 8, 8 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 8, 0 }, { 8, 8 }, {}, {});

	CHECK_InvalidGet(texturePack, 6, 3);
	CHECK_InvalidGet(texturePack, 7, 3);
	CHECK_InvalidGet(texturePack, 8, 0);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 1 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 2, 3 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 4, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::Looped, { 6, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 2, 3 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 5, 5 }, {});
	CHECK_Animation(texturePack, 2, 1, AnimationType::Looped, { 6, 6 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 5 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 2, 3 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 4, 5 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 0, 5 }, {});

	// odd directions number
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 16, 2, {});
	CHECK(texturePack->getDirectionCount(0) == 3);
	CHECK(texturePack->getDirectionCount(1) == 9);
	CHECK(texturePack->getDirectionCount(2) == 1);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);
	CHECK_InvalidGet(texturePack, 9);

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 11, { 4, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 12, { 8, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 13, { 12, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 14, { 0, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 15, { 4, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 16, { 8, 4 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 12, 4 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 18);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 2 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::Looped, { 3, 5 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::Looped, { 6, 7 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 10, 10 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 1, AnimationType::Looped, { 11, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 2, AnimationType::Looped, { 12, 12 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 3, AnimationType::Looped, { 13, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 4, AnimationType::Looped, { 14, 14 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 5, AnimationType::Looped, { 15, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 6, AnimationType::Looped, { 16, 16 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 7, AnimationType::Looped, { 17, 17 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 17 }, {});
	CHECK_Animation(texturePack, 0, 3, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 8, AnimationType::Looped, { 10, 17 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 0, 17 }, {});

	// overlapping directions + startIndex in first texturePack
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 3, pal);
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
	CHECK_Animation(texturePack, 0, -1, AnimationType::PlayOnce, { 10, 13 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::PlayOnce, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_Animation(texturePack, 2, -1, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 2, 2, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 3, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});

	// default groups
	texturePack = game.Resources().getTexturePack("key11");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 12, 3, pal);
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
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

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 10, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 1, 0, AnimationType::Looped, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 18, 21 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 13 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 14, 17 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 14, 17 }, {});
	CHECK_Animation(texturePack, 2, -1, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 2, 1, AnimationType::Looped, { 18, 21 }, {});
	CHECK_Animation(texturePack, 3, -1, AnimationType::Looped, { 10, 21 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 10, 21 }, {});
}
