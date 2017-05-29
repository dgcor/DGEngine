#pragma once

#include "Action.h"
#include "Game.h"
#include "Image.h"
#include "Utils.h"

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
