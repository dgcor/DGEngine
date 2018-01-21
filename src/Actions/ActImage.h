#pragma once

#include "Action.h"
#include "Game.h"
#include "Image.h"
#include "Utils/Utils.h"

class ActImageCenterTexture : public Action
{
private:
	std::string id;

public:
	ActImageCenterTexture(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			image->centerTexture();
		}
		return true;
	}
};

class ActImageEnableOutline : public Action
{
private:
	std::string id;
	bool enable;

public:
	ActImageEnableOutline(const std::string& id_, bool enable_)
		: id(id_), enable(enable_) {}

	virtual bool execute(Game& game) noexcept
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			image->setOutlineEnabled(enable);
		}
		return true;
	}
};

class ActImageInverseResizeXY : public Action
{
private:
	std::string id;
	Variable size;
	Variable inputRangeMin;
	Variable inputRangeMax;
	sf::Vector2i sizeRange;
	bool applyToY;

public:
	ActImageInverseResizeXY(const std::string& id_, const Variable& size_,
		const Variable& inputRangeMin_, const Variable& inputRangeMax_,
		const sf::Vector2i& sizeRange_, bool applyToY_) : id(id_), size(size_),
		inputRangeMin(inputRangeMin_), inputRangeMax(inputRangeMax_),
		sizeRange(sizeRange_), applyToY(applyToY_) {}

	virtual bool execute(Game& game)
	{
		if (sizeRange.y > sizeRange.x)
		{
			auto image = game.Resources().getResource<Image>(id);
			if (image != nullptr)
			{
				sf::Vector2i inputRange(
					(int)game.getVarOrPropLong(inputRangeMin),
					(int)game.getVarOrPropLong(inputRangeMax));

				if (inputRange.y > inputRange.x)
				{
					auto newSize = (int)game.getVarOrPropLong(size);
					newSize = (int)Utils::normalizeNumber<sf::Vector2i>(
						(long)newSize, inputRange, sizeRange);

					auto rect = image->getTextureRect();
					int diff;
					if (applyToY == false)
					{
						diff = rect.left - newSize;
						rect.left = newSize;
						rect.width += diff;
					}
					else
					{
						diff = rect.top - newSize;
						rect.top = newSize;
						rect.height += diff;
					}
					image->setTextureRect(rect);

					auto pos = image->Position();
					if (applyToY == false)
					{
						pos.x -= (float)diff;
					}
					else
					{
						pos.y -= (float)diff;
					}
					image->Position(pos);
				}
			}
		}
		return true;
	}
};

class ActImageSetOutline : public Action
{
private:
	std::string id;
	sf::Color outline;
	sf::Color ignore;

public:
	ActImageSetOutline(const std::string& id_,
		const sf::Color& outline_, const sf::Color& ignore_)
		: id(id_), outline(outline_), ignore(ignore_) {}

	virtual bool execute(Game& game) noexcept
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			image->setOutline(outline, ignore);
		}
		return true;
	}
};

class ActImageSetPalette : public Action
{
private:
	std::string id;
	std::string idPalette;
	sf::Color color;

public:
	ActImageSetPalette(const std::string& id_,
		const std::string& idPalette_, const sf::Color& color_)
		: id(id_), idPalette(idPalette_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			if (image->hasPalette() == true)
			{
				auto palette = game.Resources().getPalette(idPalette);
				if (palette != nullptr)
				{
					image->setPalette(palette);
				}
			}
			else
			{
				image->setColor(color);
			}
		}
		return true;
	}
};

class ActImageSetTexture : public Action
{
private:
	std::string id;
	std::string idTexture;
	bool resetRect;

public:
	ActImageSetTexture(const std::string& id_,
		const std::string& idTexture_, bool resetRect_)
		: id(id_), idTexture(idTexture_), resetRect(resetRect_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			auto tex = game.Resources().getTexture(idTexture);
			if (tex != nullptr)
			{
				image->setTexture(*tex, resetRect);
				image->setPalette(nullptr);
			}
		}
		return true;
	}
};

class ActImageSetTextureFromItem : public Action
{
private:
	std::string id;
	std::string idLevel;
	ItemLocation itemLocation;
	bool resetRect;

public:
	ActImageSetTextureFromItem(const std::string& id_, const std::string& idLevel_,
		const ItemLocation& itemLocation_, bool resetRect_) : id(id_),
		idLevel(idLevel_), itemLocation(itemLocation_), resetRect(resetRect_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			auto level = game.Resources().getLevel(idLevel);
			if (level != nullptr)
			{
				auto item = level->getItem(itemLocation);
				if (item != nullptr)
				{
					const sf::Texture* texture;
					sf::IntRect rect;
					if (item->Class()->getInventoryTexture(&texture, rect) == true)
					{
						image->setTexture(*texture);
						if (resetRect == true)
						{
							image->setTextureRect(rect);
						}
					}
					if (item->Class()->getInventoryTexturePack()->isIndexed() == true)
					{
						image->setPalette(item->Class()->getInventoryTexturePack()->getPalette());
					}
					else
					{
						image->setPalette(nullptr);
					}
				}
			}
		}
		return true;
	}
};

class ActImageSetTextureFromPack : public Action
{
private:
	std::string id;
	std::string idTexturePack;
	std::pair<size_t, size_t> textureIdx;
	bool resetRect;

public:
	ActImageSetTextureFromPack(const std::string& id_,
		const std::string& idTexturePack_, const std::pair<size_t, size_t>& textureIdx_,
		bool resetRect_) : id(id_), idTexturePack(idTexturePack_),
		textureIdx(textureIdx_), resetRect(resetRect_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			auto tex = game.Resources().getTexturePack(idTexturePack);
			if (tex != nullptr)
			{
				const sf::Texture* texture;
				sf::IntRect rect;
				if (tex->get(textureIdx.first, textureIdx.second, &texture, rect) == true)
				{
					image->setTexture(*texture);
					if (resetRect == true)
					{
						image->setTextureRect(rect);
					}
				}
				if (tex->isIndexed() == true)
				{
					image->setPalette(tex->getPalette());
				}
				else
				{
					image->setPalette(nullptr);
				}
			}
		}
		return true;
	}
};

class ActImageSetTextureRect : public Action
{
private:
	std::string id;
	sf::IntRect rect;

public:
	ActImageSetTextureRect(const std::string& id_,
		const sf::IntRect& rect_) : id(id_), rect(rect_) {}

	virtual bool execute(Game& game)
	{
		auto image = game.Resources().getResource<Image>(id);
		if (image != nullptr)
		{
			image->setTextureRect(rect);
		}
		return true;
	}
};
