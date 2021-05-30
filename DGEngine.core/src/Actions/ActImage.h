#pragma once

#include "Action.h"
#include "Game.h"
#include "Image.h"
#include "Utils/Utils.h"

class ActImageEnableOutline : public Action
{
private:
	std::string id;
	bool enable;

public:
	ActImageEnableOutline(const std::string_view id_, bool enable_)
		: id(id_), enable(enable_) {}

	bool execute(Game& game) noexcept override
	{
		auto image = game.Resources().getDrawable<Image>(id);
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
	ActImageInverseResizeXY(const std::string_view id_, const Variable& size_,
		const Variable& inputRangeMin_, const Variable& inputRangeMax_,
		const sf::Vector2i& sizeRange_, bool applyToY_) : id(id_), size(size_),
		inputRangeMin(inputRangeMin_), inputRangeMax(inputRangeMax_),
		sizeRange(sizeRange_), applyToY(applyToY_) {}

	bool execute(Game& game) override
	{
		if (sizeRange.y > sizeRange.x)
		{
			auto image = game.Resources().getDrawable<Image>(id);
			if (image != nullptr)
			{
				sf::Vector2i inputRange(
					(int)game.getVarOrPropInt64V(inputRangeMin),
					(int)game.getVarOrPropInt64V(inputRangeMax));

				if (inputRange.y > inputRange.x)
				{
					auto newSize = (int)game.getVarOrPropInt64V(size);
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
	ActImageSetOutline(const std::string_view id_,
		const sf::Color& outline_, const sf::Color& ignore_)
		: id(id_), outline(outline_), ignore(ignore_) {}

	bool execute(Game& game) noexcept override
	{
		auto image = game.Resources().getDrawable<Image>(id);
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
	ActImageSetPalette(const std::string_view id_,
		const std::string_view idPalette_, const sf::Color& color_)
		: id(id_), idPalette(idPalette_), color(color_) {}

	bool execute(Game& game) override
	{
		auto image = game.Resources().getDrawable<Image>(id);
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
	ActImageSetTexture(const std::string_view id_,
		const std::string_view idTexture_, bool resetRect_)
		: id(id_), idTexture(idTexture_), resetRect(resetRect_) {}

	bool execute(Game& game) override
	{
		auto image = game.Resources().getDrawable<Image>(id);
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

class ActImageSetTextureFromPack : public Action
{
private:
	std::string id;
	std::string idTexturePack;
	uint32_t textureIdx;

public:
	ActImageSetTextureFromPack(const std::string_view id_,
		const std::string_view idTexturePack_, uint32_t textureIdx_)
		: id(id_), idTexturePack(idTexturePack_), textureIdx(textureIdx_) {}

	bool execute(Game& game) override
	{
		auto image = game.Resources().getDrawable<Image>(id);
		if (image != nullptr)
		{
			auto tex = game.Resources().getTexturePack(idTexturePack);
			if (tex != nullptr)
			{
				TextureInfo ti;
				if (tex->get(textureIdx, ti) == true)
				{
					image->setTexture(ti);
				}
			}
		}
		return true;
	}
};

class ActImageSetTextureFromQueryable : public Action
{
private:
	std::string id;
	std::string query;
	uint32_t textureIdx;

public:
	ActImageSetTextureFromQueryable(const std::string_view id_,
		const std::string_view query_, uint32_t textureIdx_)
		: id(id_), query(query_), textureIdx(textureIdx_) {}

	bool execute(Game& game) override
	{
		auto image = game.Resources().getDrawable<Image>(id);
		if (image != nullptr)
		{
			auto queryable = game.getQueryable(query);
			if (queryable != nullptr)
			{
				TextureInfo ti;
				if (queryable->getTexture(textureIdx, ti) == true)
				{
					image->setTexture(ti);
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
	ActImageSetTextureRect(const std::string_view id_,
		const sf::IntRect& rect_) : id(id_), rect(rect_) {}

	bool execute(Game& game) override
	{
		auto image = game.Resources().getDrawable<Image>(id);
		if (image != nullptr)
		{
			image->setTextureRect(rect);
		}
		return true;
	}
};
