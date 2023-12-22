#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseBitmapFontTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks/bitmapFontTexturePacks.json");

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

	auto CHECK_InvalidGet = [](const std::shared_ptr<TexturePack>& texturePack, uint32_t index)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == false);
		CHECK(texturePack->getDirection(index) == 0);
	};

	auto CHECK_TextureGet = [](const std::shared_ptr<TexturePack>& texturePack,
		uint32_t index, const sf::Vector2i& position, const sf::Vector2i& size,
		const std::shared_ptr<Palette>& palette, const sf::Vector2f& offset, bool skipPosition = false)
	{
		TextureInfo ti;
		CHECK(texturePack->get(index, ti) == true);
		CHECK(texturePack->getTextureSize(index) == size);

		CHECK(ti.texture != nullptr);
		if (skipPosition == false)
		{
			CHECK(ti.textureRect.getPosition() == position);
		}
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

	// grid font texture + default rows, cols and direction
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 97, 32 }, { 9, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 33, 96 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 256);

	CHECK_Animation(texturePack, -1, -1, { 0, 255 });
	CHECK_Animation(texturePack, 0, 0, { 0, 255 });
	CHECK_Animation(texturePack, 0, 'a', { 0, 255 });
	CHECK_Animation(texturePack, 0, 255, { 0, 255 });
	CHECK_Animation(texturePack, 0, 256, { 0, 255 });
	CHECK_Animation(texturePack, 1, 0, { 0, 255 });

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(255) == 0);
	CHECK(texturePack->getDirection(256) == 0);

	// grid font texture + rows, cols = 16 + horizontal direction
	texturePack = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 32, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 16, 20 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 97, 32 }, { 9, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 33, 96 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture + imageContainer + vertical direction
	texturePack = game.Resources().getTexturePack("key03");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 0, 144 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 0, 160 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 32, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 33, 96 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 97, 32 }, { 9, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// horizontal font texture + -1 sizes for newLine, space, tab
	texturePack = game.Resources().getTexturePack("key04");
	auto pal = game.Resources().getPalette("pal");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, pal);

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, pal, {});
	CHECK_TextureGet(texturePack, '\t', { 36, 0 }, { 8, 4 }, pal, {});
	CHECK_TextureGet(texturePack, '\n', { 40, 0 }, { 4, 2 }, pal, {});
	CHECK_TextureGet(texturePack, ' ', { 128, 0 }, { 2, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 'a', { 388, 0 }, { 1, 4 }, pal, {});
	CHECK_TextureGet(texturePack, 255, { 1020, 0 }, { 1, 4 }, pal, {});

	// vertical font texture
	texturePack = game.Resources().getTexturePack("key05");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 4, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 'a', { 0, 388 }, { 1, 4 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 0, 1020 }, { 1, 4 }, {}, {});

	// invalid rows + cols
	texturePack = game.Resources().getTexturePack("key06");

	CHECK(texturePack == nullptr);

	// invalid texture
	texturePack = game.Resources().getTexturePack("key07");

	CHECK(texturePack == nullptr);

	// invalid imageContiner
	texturePack = game.Resources().getTexturePack("key08");

	CHECK(texturePack == nullptr);

	// imageContainer
	// uses ImageUtils::packImages internally to pack textures into 1
	texturePack = game.Resources().getTexturePack("key09");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, '\t', {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, '\n', {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, ' ', {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, '&', {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, 'b', {}, { 16, 16 }, {}, {}, true);
	CHECK_TextureGet(texturePack, 255, {}, { 16, 16 }, {}, {}, true);

	CHECK_InvalidGet(texturePack, 256);

	CHECK_Animation(texturePack, -1, -1, { 0, 255 });
	CHECK_Animation(texturePack, 0, 0, { 0, 255 });
	CHECK_Animation(texturePack, 0, 'a', { 0, 255 });
	CHECK_Animation(texturePack, 0, 255, { 0, 255 });
	CHECK_Animation(texturePack, 0, 256, { 0, 255 });
	CHECK_Animation(texturePack, 1, 0, { 0, 255 });

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(255) == 0);
	CHECK(texturePack->getDirection(256) == 0);

	// imageContainer with small number of textures + palette
	texturePack = game.Resources().getTexturePack("key10");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 4, 1, pal);

	CHECK_TextureGet(texturePack, 0, {}, { 8, 4 }, pal, {}, true);
	CHECK_TextureGet(texturePack, 1, {}, { 8, 4 }, pal, {}, true);
	CHECK_TextureGet(texturePack, 2, {}, { 8, 4 }, pal, {}, true);
	CHECK_TextureGet(texturePack, 3, {}, { 8, 4 }, pal, {}, true);

	CHECK_InvalidGet(texturePack, 4);

	CHECK_Animation(texturePack, -1, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, -1, { 0, 3 });
	CHECK_Animation(texturePack, 0, 0, { 0, 3 });
	CHECK_Animation(texturePack, 0, 3, { 0, 3 });
	CHECK_Animation(texturePack, 0, 4, { 0, 3 });
	CHECK_Animation(texturePack, 1, -1, { 0, 3 });
	CHECK_Animation(texturePack, 1, 0, { 0, 3 });

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(3) == 0);
	CHECK(texturePack->getDirection(4) == 0);

	// grid font texture 256 + charSizeFile 256
	texturePack = game.Resources().getTexturePack("key11");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 4, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture 128 + charSizeFile 128
	texturePack = game.Resources().getTexturePack("key12");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 128, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 4, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 127, { 240, 112 }, { 3, 16 }, {}, {});

	CHECK_InvalidGet(texturePack, 128);

	CHECK_Animation(texturePack, -1, -1, { 0, 127 });
	CHECK_Animation(texturePack, 0, 0, { 0, 127 });
	CHECK_Animation(texturePack, 0, 'a', { 0, 127 });
	CHECK_Animation(texturePack, 0, 127, { 0, 127 });
	CHECK_Animation(texturePack, 0, 128, { 0, 127 });
	CHECK_Animation(texturePack, 1, 0, { 0, 127 });

	CHECK(texturePack->getDirection(0) == 0);
	CHECK(texturePack->getDirection(127) == 0);
	CHECK(texturePack->getDirection(128) == 0);

	// grid font texture 256 + charSizeFile 258
	texturePack = game.Resources().getTexturePack("key13");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 20 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture 128 + charSizeFile 130
	texturePack = game.Resources().getTexturePack("key14");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 128, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 20 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 127, { 240, 112 }, { 3, 16 }, {}, {});

	// grid font texture 256 + charSizeFile 256 + charSizeFileStep
	texturePack = game.Resources().getTexturePack("key15");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 4, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture 256 + charSizeFile 256 + charSizeFileStep + charSizeFileStart
	texturePack = game.Resources().getTexturePack("key16");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 8, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 4, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 96, 32 }, { 10, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 32, 96 }, { 7, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture 256 + charSizeFile 255 (invalid - defaults to calculating sizes)
	texturePack = game.Resources().getTexturePack("key17");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 256, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 0 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 32 }, { 16, 16 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 97, 32 }, { 9, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 33, 96 }, { 6, 16 }, {}, {});
	CHECK_TextureGet(texturePack, 255, { 240, 240 }, { 6, 16 }, {}, {});

	// grid font texture 128 + charSizeFile 127 (invalid - defaults to calculating sizes)
	// the 128 char image has 2 pixels that can be remove from the top and bottom (smaller images than the 256 char image)
	texturePack = game.Resources().getTexturePack("key18");

	CHECK(texturePack != nullptr);
	CHECK_TexturePack(texturePack, 128, 1, {});

	CHECK_TextureGet(texturePack, 0, { 0, 2 }, { 16, 14 }, {}, {});
	CHECK_TextureGet(texturePack, '\t', { 144, 2 }, { 16, 14 }, {}, {});
	CHECK_TextureGet(texturePack, '\n', { 160, 2 }, { 16, 14 }, {}, {});
	CHECK_TextureGet(texturePack, ' ', { 0, 34 }, { 16, 14 }, {}, {});
	CHECK_TextureGet(texturePack, '&', { 97, 34 }, { 9, 14 }, {}, {});
	CHECK_TextureGet(texturePack, 'b', { 33, 98 }, { 6, 14 }, {}, {});
	CHECK_TextureGet(texturePack, 127, { 240, 114 }, { 2, 14 }, {}, {});
}
