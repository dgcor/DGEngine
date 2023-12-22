#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexturePackIndexes", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/texturePackIndexes.json");

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

	auto CHECK_TextureGetVar = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, const sf::Vector2i& position, const sf::Vector2i& size,
		const std::shared_ptr<Palette>& palette, const sf::Vector2f& offset)
	{
		TextureInfoVar tiVar;
		CHECK(texturePack->get(index, tiVar) == true);
		CHECK(std::holds_alternative<TextureInfo>(tiVar) == true);
		CHECK(texturePack->getTextureSize(index) == size);

		const auto& ti = std::get<TextureInfo>(tiVar);
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

	// textureIndexes (array) + default onlyUseIndexed (true)
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 6, 1, {});

	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1024, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1025, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1026, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1027, { 0, 4 }, { 8, 4 }, {}, {});

	// check get var (test that texturePackIndexWrapper uses correct index mapping)

	CHECK_TextureGetVar(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGetVar(texturePack, 1027, { 0, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 3);
	CHECK_InvalidGet(texturePack, 1023);
	CHECK_InvalidGet(texturePack, 1028);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 1027 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 1, 1027 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 1, 1027 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 1027 }, {});

	// textureIndexes (array) + onlyUseIndexed (false) + reverseMapIndex (ignored)
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 1028, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1024, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1025, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1026, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1027, { 0, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);
	CHECK_InvalidGet(texturePack, 1023);
	CHECK_InvalidGet(texturePack, 1028);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 1027 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 1027 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 1027 }, {});

	// textureIndexes (number) + default onlyUseIndexed (true)
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 5, 1, {});

	CHECK_TextureGet(texturePack, 16, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 18, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 19, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 1);
	CHECK_InvalidGet(texturePack, 2);
	CHECK_InvalidGet(texturePack, 3);
	CHECK_InvalidGet(texturePack, 15);
	CHECK_InvalidGet(texturePack, 20);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 16, 20 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 16, 20 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 16, 20 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 16, 20 }, {});

	// textureIndexes (number) + onlyUseIndexed (false)
	texturePack = game.Resources().getTexturePack("key04");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 21, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 16, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 17, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 18, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 19, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 4);
	CHECK_InvalidGet(texturePack, 15);
	CHECK_InvalidGet(texturePack, 20);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::Looped, { 0, 3 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 20 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 20 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 20 }, {});

	// textureIndexes (number) + reverseMapIndex
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 8, 1, {});

	CHECK_TextureGet(texturePack, 0, { 8, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 1, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 3, { 0, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 4, { 8, 0 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 5, { 0, 4 }, { 8, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 6, { 8, 4 }, { 8, 4 }, {}, {});

	CHECK_InvalidGet(texturePack, 7);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 7 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 7 }, {});

	// textureIndexes (number) + multiTexturepack + startIndex
	texturePack = game.Resources().getTexturePack("key06");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 17, 1, pal);
	CHECK(texturePack->getDirectionCount(0) == 1);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 1);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 3, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 4, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 20, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 21, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 22, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 23, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 30, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 31, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 32, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 33, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 5);
	CHECK_InvalidGet(texturePack, 8);
	CHECK_InvalidGet(texturePack, 9);
	CHECK_InvalidGet(texturePack, 13);
	CHECK_InvalidGet(texturePack, 19);
	CHECK_InvalidGet(texturePack, 24);
	CHECK_InvalidGet(texturePack, 29);
	CHECK_InvalidGet(texturePack, 34);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 2, 0, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 1, 50 }, {});

	// textureIndexes (number) + onlyUseIndexed (false) + multiTexturepack + startIndex
	texturePack = game.Resources().getTexturePack("key07");
	pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_MultiTexturePack(texturePack, 50, 3, pal);
	CHECK(texturePack->getDirectionCount(0) == 4);
	CHECK(texturePack->getDirectionCount(1) == 1);
	CHECK(texturePack->getDirectionCount(2) == 4);
	CHECK(texturePack->getDirectionCount(3) == 1);

	CHECK_TextureGet(texturePack, 1, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 2, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 3, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 4, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 11, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 12, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 13, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 14, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 15, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 16, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 17, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 18, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 19, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 20, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 21, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 22, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 23, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_TextureGet(texturePack, 30, { 0, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 31, { 8, 0 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 32, { 0, 4 }, { 8, 4 }, pal, { 1, 0 });
	CHECK_TextureGet(texturePack, 33, { 8, 4 }, { 8, 4 }, pal, { 1, 0 });

	CHECK_InvalidGet(texturePack, 5);
	CHECK_InvalidGet(texturePack, 9);
	CHECK_InvalidGet(texturePack, 24);
	CHECK_InvalidGet(texturePack, 29);
	CHECK_InvalidGet(texturePack, 34);

	CHECK_AnimationAndDirection(texturePack, 0, 0, AnimationType::PlayOnce, { 10, 11 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 1, AnimationType::PlayOnce, { 12, 13 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 2, AnimationType::PlayOnce, { 14, 15 }, {});
	CHECK_AnimationAndDirection(texturePack, 0, 3, AnimationType::PlayOnce, { 16, 17 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::BackAndForth, { 14, 17 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 0, AnimationType::Looped, { 18, 18 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 1, AnimationType::Looped, { 19, 19 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 2, AnimationType::Looped, { 20, 20 }, {});
	CHECK_AnimationAndDirection(texturePack, 2, 3, AnimationType::Looped, { 21, 21 }, {});

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 0, 4, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 1, 1, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 2, 4, AnimationType::Looped, { 1, 50 }, {});
	CHECK_Animation(texturePack, 3, 0, AnimationType::Looped, { 1, 50 }, {});

	// utf8Indexes
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 26, 1, {});

	CHECK_TextureGet(texturePack, 'a', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 97, 122 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 97, 122 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 97, 122 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 97, 122 }, {});

	// utf8IndexFile
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 26, 1, {});

	CHECK_TextureGet(texturePack, 'A', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'Z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 65, 90 }, {});

	// textureIndexRange
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 26, 1, {});

	CHECK_TextureGet(texturePack, 'A', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'Z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 65, 90 }, {});

	// inverted textureIndexRange
	texturePack = game.Resources().getTexturePack("key11");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 26, 1, {});

	CHECK_TextureGet(texturePack, 'A', { 400, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'Z', { 0, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 65, 90 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 65, 90 }, {});

	// textureIndexRange + reverseMapIndex
	texturePack = game.Resources().getTexturePack("key12");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 26, 1, {});

	CHECK_TextureGet(texturePack, 0, { 160, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 15, { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 16);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 0, 25 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 0, 25 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 0, 25 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 0, 25 }, {});

	// empty range textureIndexRange
	texturePack = game.Resources().getTexturePack("key13");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 1, 1, {});

	CHECK_TextureGet(texturePack, 10, { 0, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 9);
	CHECK_InvalidGet(texturePack, 11);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 10, 10 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 10, 10 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 10, 10 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 10, 10 }, {});

	// utf8Indexes + textureIndexRange + textureIndexRangeStart
	texturePack = game.Resources().getTexturePack("key14");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 52, 1, {});

	CHECK_TextureGet(texturePack, 'A', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'Z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_TextureGet(texturePack, 'a', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 65, 122 }, {});

	// utf8Indexes + inverted textureIndexRange + textureIndexRangeStart
	texturePack = game.Resources().getTexturePack("key15");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 52, 1, {});

	CHECK_TextureGet(texturePack, 'A', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'Z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_TextureGet(texturePack, 'a', { 400, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'z', { 0, 0 }, { 16, 16 }, {}, {});

	// check get var (test that texturePackIndexWrapper uses correct index mapping)

	CHECK_TextureGetVar(texturePack, 'A', { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGetVar(texturePack, 'Z', { 400, 0 }, { 16, 16 }, {}, {});

	CHECK_TextureGetVar(texturePack, 'a', { 400, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGetVar(texturePack, 'z', { 0, 0 }, { 16, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 0);
	CHECK_InvalidGet(texturePack, 64);
	CHECK_InvalidGet(texturePack, 91);
	CHECK_InvalidGet(texturePack, 96);
	CHECK_InvalidGet(texturePack, 123);

	CHECK_Animation(texturePack, -1, -1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 0, -1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 0, 0, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 0, 1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 1, -1, AnimationType::Looped, { 65, 122 }, {});
	CHECK_Animation(texturePack, 1, 0, AnimationType::Looped, { 65, 122 }, {});
}
