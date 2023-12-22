#include "catch2/catch_test_macros.hpp"

#include "TestUtils/LazyImageContainerFixture.h"

TEST_CASE("LazyImageContainer::decode", "LazyImageContainer")
{
	SECTION("default")
	{
		std::shared_ptr<FileBytes> fileBytes;
		LazyImageContainerFixture obj(fileBytes);
		ImageContainer* imgContainer = &obj;

		CHECK(imgContainer->size() == 0);
		CHECK(imgContainer->get(0, nullptr).getSize() == sf::Vector2u());
	}
	SECTION("with data")
	{
		auto fileBytes = std::make_shared<FileBytes>(FileBytes{(std::byte)1, (std::byte)2});
		LazyImageContainerFixture imgContainer(fileBytes);

		CHECK(imgContainer.size() == 2);

		ImageContainer::ImageInfo imgInfo;
		auto img = imgContainer.get(0, nullptr, imgInfo);

		CHECK(img.getSize() == sf::Vector2u(4, 4));
		CHECK(img.getPixel(0, 0) == sf::Color(1, 1, 1));

		CHECK(imgInfo.offset == sf::Vector2f());
		CHECK(imgInfo.absoluteOffset == false);
		CHECK(imgInfo.blendMode == BlendMode::Alpha);
		CHECK(imgInfo.nextIndex == -1);

		img = imgContainer.get(1, nullptr, imgInfo);

		CHECK(img.getSize() == sf::Vector2u(4, 4));
		CHECK(img.getPixel(0, 0) == sf::Color(2, 2, 2));

		CHECK(imgInfo.offset == sf::Vector2f());
		CHECK(imgInfo.absoluteOffset == false);
		CHECK(imgInfo.blendMode == BlendMode::Alpha);
		CHECK(imgInfo.nextIndex == -1);

		img = imgContainer.get(2, nullptr, imgInfo);

		CHECK(img.getSize() == sf::Vector2u(0, 0));
	}
}
