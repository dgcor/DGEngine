#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseImageContainerTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/imageContainerTexturePacks.json");

	auto CHECK_TexturePack = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t size, uint32_t directions, const std::shared_ptr<Palette>& palette)
	{
		CHECK(texturePack->size() == size);
		CHECK(texturePack->getGroupCount() == 1);
		CHECK(texturePack->getDirectionCount(0) == directions);
		CHECK(texturePack->getDirectionCount(1) == 1);
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

	// imageContainer
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 3 });

	CHECK_Animation(texturePack, -1, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, 1, { 0, 3 });
	CHECK_Animation(texturePack, 1, -1, { 0, 3 });
	CHECK_Animation(texturePack, 1, 0, { 0, 3 });

	// imageContainer single array + palette
	texturePack = game.Resources().getTexturePack("key02");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, pal);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, pal, { 2, -2 });
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, pal, { 2, -2 });

	CHECK_InvalidGet(texturePack, 4);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 3 });

	CHECK_Animation(texturePack, -1, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, 1, { 0, 3 });
	CHECK_Animation(texturePack, 1, 0, { 0, 3 });

	// imageContainer + invalid palette
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, nullptr);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, { 2, -2 });

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 7 });

	CHECK_Animation(texturePack, -1, -1, { 0, 7 });
	CHECK_Animation(texturePack, 0, 1, { 0, 7 });
	CHECK_Animation(texturePack, 1, 0, { 0, 7 });

	// imageContainer + invalid palette
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 4, nullptr);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 1 });
	CHECK_AnimationAndDirection(texturePack, 0, 1, { 2, 3 });
	CHECK_AnimationAndDirection(texturePack, 0, 2, { 4, 5 });
	CHECK_AnimationAndDirection(texturePack, 0, 3, { 6, 7 });

	CHECK_Animation(texturePack, -1, -1, { 0, 7 });
	CHECK_Animation(texturePack, 0, 4, { 0, 7 });
	CHECK_Animation(texturePack, 1, 0, { 0, 7 });

	// imageContainer + invalid palette
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 3, nullptr);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 2 });
	CHECK_AnimationAndDirection(texturePack, 0, 1, { 3, 5 });
	CHECK_AnimationAndDirection(texturePack, 0, 2, { 6, 7 });

	CHECK_Animation(texturePack, -1, -1, { 0, 7 });
	CHECK_Animation(texturePack, 0, 4, { 0, 7 });
	CHECK_Animation(texturePack, 1, 0, { 0, 7 });

	// imageContainer + invalid palette
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 9, nullptr);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 7, { 0, 0 }, { 4, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 8);

	CHECK_AnimationAndDirection(texturePack, 0, 0, { 0, 0 });
	CHECK_AnimationAndDirection(texturePack, 0, 1, { 1, 1 });
	CHECK_AnimationAndDirection(texturePack, 0, 2, { 2, 2 });
	CHECK_AnimationAndDirection(texturePack, 0, 3, { 3, 3 });
	CHECK_AnimationAndDirection(texturePack, 0, 4, { 4, 4 });
	CHECK_AnimationAndDirection(texturePack, 0, 5, { 5, 5 });
	CHECK_AnimationAndDirection(texturePack, 0, 6, { 6, 6 });
	CHECK_AnimationAndDirection(texturePack, 0, 7, { 7, 7 });

	CHECK_Animation(texturePack, -1, -1, { 0, 7 });
	CHECK_Animation(texturePack, 0, 8, { 0, 7 });
	CHECK_Animation(texturePack, 1, 0, { 0, 7 });

	// invalid imageContainer
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack == nullptr);
}
