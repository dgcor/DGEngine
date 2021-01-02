#pragma once

#include "Action.h"
#include "Anchor.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

class ActDrawableAddToPosition : public Action
{
private:
	std::string id;
	sf::Vector2f offset;

public:
	ActDrawableAddToPosition(const std::string_view id_, const sf::Vector2f& offset_)
		: id(id_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Position(item->Position() + offset);
		}
		return true;
	}
};

class ActDrawableAddToSize : public Action
{
private:
	std::string id;
	sf::Vector2f offset;

public:
	ActDrawableAddToSize(const std::string_view id_, const sf::Vector2f& offset_)
		: id(id_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Size(item->Size() + offset);
		}
		return true;
	}
};

class ActDrawableAnchor : public Action
{
private:
	std::string id;
	std::string idAnchor;
	Anchor anchor;
	sf::Vector2f offset;

public:
	ActDrawableAnchor(const std::string_view id_, const std::string_view idAnchor_,
		Anchor anchor_, const sf::Vector2f& offset_)
		: id(id_), idAnchor(idAnchor_), anchor(anchor_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto itemAnchor = game.Resources().getDrawable(idAnchor);
		auto itemToChange = game.Resources().getDrawable(id);

		if (itemAnchor != nullptr && itemToChange != nullptr)
		{
			auto pos = GameUtils::getAnchorPosition(anchor,
				itemToChange->Size(),
				itemAnchor->DrawPosition() + offset,
				itemAnchor->Size());

			itemToChange->Position(pos);
		}
		return true;
	}
};

class ActDrawableAnchorSizeXY : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	float offset;
	bool applyToY;

public:
	ActDrawableAnchorSizeXY(const std::string_view id_,
		const std::string_view idAnchorTo_, float offset_, bool applyToY_)
		: id(id_), idAnchorTo(idAnchorTo_), offset(offset_), applyToY(applyToY_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchorTo = game.Resources().getDrawable(idAnchorTo);

		if (item != nullptr && itemAnchorTo != nullptr)
		{
			const auto& posA = itemAnchorTo->Position();
			const auto& posB = item->Position();
			float newSize;
			if (applyToY == false)
			{
				newSize = (posA.x - posB.x) + offset;
			}
			else
			{
				newSize = (posA.y - posB.y) + offset;
			}
			if (newSize < 0.f)
			{
				newSize = 0.f;
			}
			auto size = item->Size();
			if (applyToY == false)
			{
				size.x = newSize;
			}
			else
			{
				size.y = newSize;
			}
			item->Size(size);
		}
		return true;
	}
};

class ActDrawableAnchorToFocused : public Action
{
private:
	std::string id;
	Anchor anchor;
	sf::Vector2f offset;

public:
	ActDrawableAnchorToFocused(const std::string_view id_, Anchor anchor_,
		const sf::Vector2f& offset_) : id(id_), anchor(anchor_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto itemAnchor = game.Resources().getFocused();
		auto itemToChange = game.Resources().getDrawable(id);

		if (itemAnchor != nullptr && itemToChange != nullptr)
		{
			auto pos = GameUtils::getAnchorPosition(anchor,
				itemToChange->Size(),
				itemAnchor->DrawPosition() + offset,
				itemAnchor->Size());

			itemToChange->Position(pos);
		}
		return true;
	}
};

class ActDrawableBringToFront : public Action
{
private:
	std::string id;

public:
	ActDrawableBringToFront(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		game.Resources().bringDrawableToFront(id);
		return true;
	}
};

class ActDrawableCenter : public Action
{
private:
	std::string id;
	std::string idCenterOn;
	sf::Vector2f offset;

public:
	ActDrawableCenter(const std::string_view id_, const std::string_view idCenterOn_,
		const sf::Vector2f& offset_) : id(id_), idCenterOn(idCenterOn_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemCenter = game.Resources().getDrawable(idCenterOn);
		if (item != nullptr && itemCenter != nullptr)
		{
			auto centerPos = itemCenter->Position();
			auto centerSize = itemCenter->Size();
			auto itemSize = item->Size();
			sf::Vector2f newPos(
				std::round(centerPos.x + (centerSize.x / 2.f) - (itemSize.x / 2.f)),
				std::round(centerPos.y + (centerSize.y / 2.f) - (itemSize.y / 2.f))
			);
			item->Position(newPos + offset);
		}
		return true;
	}
};

class ActDrawableCenterOnMouseX : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable steps;

public:
	ActDrawableCenterOnMouseX(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& steps_)
		: id(id_), idAnchorTo(idAnchorTo_), range(range_), steps(steps_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->Position();
			auto itemSize = item->Size().x;
			auto newRange = std::min(range - (unsigned)itemSize, range);
			auto offset = std::clamp(game.MousePositionf().x - itemPos.x, 0.f, (float)range);
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			float newPos = itemPos.x;
			offset = std::clamp(offset - std::round(itemSize / 2.f), 0.f, (float)newRange);
			if (numSteps > 1 && newRange > 0)
			{
				auto idx = std::round((offset * (float)(numSteps - 1)) / (float)newRange);
				newPos += std::round((idx * (float)newRange) / (float)(numSteps - 1));
			}
			else if (numSteps < 0)
			{
				newPos += offset;
				if (newPos < itemPos.x)
				{
					newPos = itemPos.x;
				}
				auto maxPos = (itemPos.x + (float)newRange) - itemSize;
				if (newPos > maxPos)
				{
					newPos = maxPos;
				}
			}
			if (item->Position().x != newPos)
			{
				itemPos.x = newPos;
				item->Position(itemPos);
			}
		}
		return true;
	}
};

class ActDrawableCenterOnMouseY : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable steps;

public:
	ActDrawableCenterOnMouseY(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& steps_)
		: id(id_), idAnchorTo(idAnchorTo_), range(range_), steps(steps_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->Position();
			auto itemSize = item->Size().y;
			auto newRange = std::min(range - (unsigned)itemSize, range);
			auto offset = std::clamp(game.MousePositionf().y - itemPos.y, 0.f, (float)range);
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			float newPos = itemPos.y;
			offset = std::clamp(offset - std::round(itemSize / 2.f), 0.f, (float)newRange);
			if (numSteps > 1 && newRange > 0)
			{
				auto idx = std::round((offset * (float)(numSteps - 1)) / (float)newRange);
				newPos += std::round((idx * (float)newRange) / (float)(numSteps - 1));
			}
			else if (numSteps < 0)
			{
				newPos += offset;
				if (newPos < itemPos.y)
				{
					newPos = itemPos.y;
				}
				auto maxPos = (itemPos.y + (float)newRange) - itemSize;
				if (newPos > maxPos)
				{
					newPos = maxPos;
				}
			}
			if (item->Position().y != newPos)
			{
				itemPos.y = newPos;
				item->Position(itemPos);
			}
		}
		return true;
	}
};

class ActDrawableDelete : public Action
{
private:
	std::string id;

public:
	ActDrawableDelete(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		game.Resources().deleteDrawable(id);
		return true;
	}
};

class ActDrawableExecuteAction : public Action
{
private:
	std::string id;
	uint16_t actionHash16;

public:
	ActDrawableExecuteAction(const std::string_view id_, uint16_t actionHash16_)
		: id(id_), actionHash16(actionHash16_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto action = item->getAction(actionHash16).get();
			if (action != nullptr)
			{
				return action->execute(game);
			}
		}
		return true;
	}
};

class ActDrawableHorizontalAnchorToFocused : public Action
{
private:
	std::string id;
	float offset;

public:
	ActDrawableHorizontalAnchorToFocused(const std::string_view id_, float offset_)
		: id(id_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto itemAnchor = game.Resources().getFocused();
		auto itemToChange = game.Resources().getDrawable(id);

		if (itemAnchor != nullptr && itemToChange != nullptr)
		{
			auto pos = itemToChange->Position();
			pos.x = itemAnchor->DrawPosition().x + offset;
			itemToChange->Position(pos);
		}
		return true;
	}
};

class ActDrawableMoveX : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable position;
	unsigned min;
	unsigned max;
	Variable steps;

public:
	ActDrawableMoveX(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& position_, unsigned min_, unsigned max_,
		const Variable& steps_) : id(id_), idAnchorTo(idAnchorTo_), range(range_),
		position(position_), min(min_), max(max_), steps(steps_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->DrawPosition();
			auto itemAnchorPos = itemPos;
			auto newRange = (float)std::max(0, (int)range - (int)item->Size().x);
			auto offset = (float)std::clamp(game.getVarOrProp<int64_t, unsigned>(position), min, max) - min;
			auto newMax = max - min;
			offset = std::round((offset * newRange) / (float)newMax);
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			if (numSteps > 1)
			{
				auto tmp = std::round((offset * (float)(numSteps - 1)) / newRange);
				offset = std::round((tmp * newRange) / (float)(numSteps - 1));
			}
			itemPos.x += offset;
			if (itemPos.x < itemAnchorPos.x)
			{
				itemPos.x = itemAnchorPos.x;
			}
			auto maxPos = itemAnchorPos.x + newRange;
			if (itemPos.x > maxPos)
			{
				itemPos.x = maxPos;
			}
			item->Position(itemPos);
		}
		return true;
	}
};

class ActDrawableMoveY : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable position;
	unsigned min;
	unsigned max;
	Variable steps;

public:
	ActDrawableMoveY(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& position_, unsigned min_, unsigned max_,
		const Variable& steps_) : id(id_), idAnchorTo(idAnchorTo_), range(range_),
		position(position_), min(min_), max(max_), steps(steps_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->DrawPosition();
			auto itemAnchorPos = itemPos;
			auto newRange = (float)std::max(0, (int)range - (int)item->Size().y);
			auto offset = (float)std::clamp(game.getVarOrProp<int64_t, unsigned>(position), min, max) - min;
			auto newMax = max - min;
			offset = std::round((offset * newRange) / (float)newMax);
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			if (numSteps > 1)
			{
				auto tmp = std::round((offset * (float)(numSteps - 1)) / newRange);
				offset = std::round((tmp * newRange) / (float)(numSteps - 1));
			}
			itemPos.y += offset;
			if (itemPos.y < itemAnchorPos.y)
			{
				itemPos.y = itemAnchorPos.y;
			}
			auto maxPos = itemAnchorPos.y + newRange;
			if (itemPos.y > maxPos)
			{
				itemPos.y = maxPos;
			}
			item->Position(itemPos);
		}
		return true;
	}
};

class ActDrawableMoveStepX : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable steps;
	int stepOffset;

public:
	ActDrawableMoveStepX(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& steps_, int stepOffset_)
		: id(id_), idAnchorTo(idAnchorTo_), range(range_),
		steps(steps_), stepOffset(stepOffset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->DrawPosition();
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			if (numSteps > 1)
			{
				numSteps--;
				auto newRange = (float)std::max(0, (int)range - (int)item->Size().x);
				auto currSize = item->DrawPosition().x - itemPos.x;
				auto currStep = std::round((currSize * numSteps) / newRange);
				auto newStep = currStep + (float)stepOffset;
				if (newStep < 0)
				{
					newStep = 0;
				}
				else if (newStep > numSteps)
				{
					newStep = (float)numSteps;
				}
				if (newStep >= 1)
				{
					itemPos.x += std::round((newStep * newRange) / numSteps);
				}
			}
			item->Position(itemPos);
		}
		return true;
	}
};

class ActDrawableMoveStepY : public Action
{
private:
	std::string id;
	std::string idAnchorTo;
	unsigned range;
	Variable steps;
	int stepOffset;

public:
	ActDrawableMoveStepY(const std::string_view id_, const std::string_view idAnchorTo_,
		unsigned range_, const Variable& steps_, int stepOffset_)
		: id(id_), idAnchorTo(idAnchorTo_), range(range_),
		steps(steps_), stepOffset(stepOffset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemAnchor = game.Resources().getDrawable(idAnchorTo);
		if (item != nullptr && itemAnchor != nullptr)
		{
			auto itemPos = itemAnchor->DrawPosition();
			auto numSteps = game.getVarOrProp<int64_t, int>(steps, -1);
			if (numSteps > 1)
			{
				numSteps--;
				auto newRange = (float)std::max(0, (int)range - (int)item->Size().y);
				auto currSize = item->DrawPosition().y - itemPos.y;
				auto currStep = std::round((currSize * numSteps) / newRange);
				auto newStep = currStep + (float)stepOffset;
				if (newStep < 0)
				{
					newStep = 0;
				}
				else if (newStep > numSteps)
				{
					newStep = (float)numSteps;
				}
				if (newStep >= 1)
				{
					itemPos.y += std::round((newStep * newRange) / numSteps);
				}
			}
			item->Position(itemPos);
		}
		return true;
	}
};

class ActDrawableResizeXY : public Action
{
private:
	std::string id;
	Variable size;
	Variable inputRangeMin;
	Variable inputRangeMax;
	sf::Vector2i sizeRange;
	bool applyToY;

public:
	ActDrawableResizeXY(const std::string_view id_, const Variable& size_,
		const Variable& inputRangeMin_, const Variable& inputRangeMax_,
		const sf::Vector2i& sizeRange_, bool applyToY_) : id(id_), size(size_),
		inputRangeMin(inputRangeMin_), inputRangeMax(inputRangeMax_),
		sizeRange(sizeRange_), applyToY(applyToY_) {}

	bool execute(Game& game) override
	{
		if (sizeRange.y > sizeRange.x)
		{
			auto item = game.Resources().getDrawable(id);
			if (item != nullptr)
			{
				sf::Vector2i inputRange(
					(int)game.getVarOrPropInt64V(inputRangeMin),
					(int)game.getVarOrPropInt64V(inputRangeMax));

				if (inputRange.y > inputRange.x)
				{
					auto newSize = game.getVarOrPropInt64V(size);
					newSize = (int64_t)Utils::normalizeNumber<sf::Vector2i>(
						(long)newSize, inputRange, sizeRange);

					auto itemSize = item->Size();
					if (applyToY == false)
					{
						itemSize.x = (float)newSize;
					}
					else
					{
						itemSize.y = (float)newSize;
					}
					item->Size(itemSize);
				}
			}
		}
		return true;
	}
};

class ActDrawableResizeOnMouseX : public Action
{
private:
	std::string id;
	sf::Vector2f range;

public:
	ActDrawableResizeOnMouseX(const std::string_view id_, const sf::Vector2f& range_)
		: id(id_), range(range_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto newSize = game.MousePositionf().x - item->Position().x;
			if (newSize < range.x)
			{
				newSize = range.x;
			}
			if (newSize > range.y)
			{
				newSize = range.y;
			}
			auto itemSize = item->Size();
			itemSize.x = newSize;
			item->Size(itemSize);
		}
		return true;
	}
};

class ActDrawableResizeOnMouseY : public Action
{
private:
	std::string id;
	sf::Vector2f range;

public:
	ActDrawableResizeOnMouseY(const std::string_view id_, const sf::Vector2f& range_)
		: id(id_), range(range_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto newSize = game.MousePositionf().y - item->Position().y;
			if (newSize < range.x)
			{
				newSize = range.x;
			}
			if (newSize > range.y)
			{
				newSize = range.y;
			}
			auto itemSize = item->Size();
			itemSize.y = newSize;
			item->Size(itemSize);
		}
		return true;
	}
};

class ActDrawableResizeToPositionX : public Action
{
private:
	std::string id;
	std::string idToPosition;
	float offset;

public:
	ActDrawableResizeToPositionX(const std::string_view id_, const std::string_view idToPosition_,
		float offset_) : id(id_), idToPosition(idToPosition_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemToPosition = game.Resources().getDrawable(idToPosition);
		if (item != nullptr && itemToPosition != nullptr)
		{
			auto newSize = itemToPosition->DrawPosition().x - item->DrawPosition().x;
			auto itemSize = item->Size();
			itemSize.x = newSize + offset;
			item->Size(itemSize);
		}
		return true;
	}
};

class ActDrawableResizeToPositionY : public Action
{
private:
	std::string id;
	std::string idToPosition;
	float offset;

public:
	ActDrawableResizeToPositionY(const std::string_view id_, const std::string_view idToPosition_,
		float offset_) : id(id_), idToPosition(idToPosition_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		auto itemToPosition = game.Resources().getDrawable(idToPosition);
		if (item != nullptr && itemToPosition != nullptr)
		{
			auto newSize = itemToPosition->DrawPosition().x - item->DrawPosition().y;
			auto itemSize = item->Size();
			itemSize.y = newSize + offset;
			item->Size(itemSize);
		}
		return true;
	}
};

class ActDrawableSetAction : public Action
{
private:
	std::string id;
	uint16_t nameHash;
	std::shared_ptr<Action> action;

public:
	ActDrawableSetAction(const std::string_view id_,
		const std::string_view name, const std::shared_ptr<Action>& action_)
		: id(id_), nameHash(str2int16(name)), action(action_) {}

	bool execute(Game& game) noexcept override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->setAction(nameHash, action);
		}
		return true;
	}
};

class ActDrawableSendToBack : public Action
{
private:
	std::string id;

public:
	ActDrawableSendToBack(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		game.Resources().sendDrawableToBack(id);
		return true;
	}
};

class ActDrawableSetPosition : public Action
{
private:
	std::string id;
	sf::Vector2f pos;
	sf::Vector2f offset;
	bool relativeCoords;

public:
	ActDrawableSetPosition(const std::string_view id_, const sf::Vector2f& pos_,
		const sf::Vector2f& offset_, bool relativeCoords_) : id(id_),
		pos(pos_), offset(offset_), relativeCoords(relativeCoords_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto newPos = pos;
			if (relativeCoords == true &&
				game.RefSize() != game.DrawRegionSize())
			{
				auto anchor = item->getAnchor();
				auto size = item->Size();
				GameUtils::setAnchorPosSize(anchor, newPos, size, game.RefSize(), game.DrawRegionSize());
			}
			item->Position(newPos + offset);
		}
		return true;
	}
};

class ActDrawableSetPositionXY : public Action
{
private:
	std::string id;
	float pos;
	bool applyToY;

public:
	ActDrawableSetPositionXY(const std::string_view id_, float pos_,
		bool applyToY_) : id(id_), pos(pos_), applyToY(applyToY_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto pos_ = item->Position();
			if (applyToY == false)
			{
				pos_.x = pos;
			}
			else
			{
				pos_.y = pos;
			}
			item->Position(pos_);
		}
		return true;
	}
};

class ActDrawableSetSize : public Action
{
private:
	std::string id;
	sf::Vector2f size;

public:
	ActDrawableSetSize(const std::string_view id_, const sf::Vector2f& size_) : id(id_), size(size_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			item->Size(size);
		}
		return true;
	}
};

class ActDrawableSetSizeXY : public Action
{
private:
	std::string id;
	float size;
	bool applyToY;

public:
	ActDrawableSetSizeXY(const std::string_view id_, float size_,
		bool applyToY_) : id(id_), size(size_), applyToY(applyToY_) {}

	bool execute(Game& game) override
	{
		auto item = game.Resources().getDrawable(id);
		if (item != nullptr)
		{
			auto size_ = item->Size();
			if (applyToY == false)
			{
				size_.x = size;
			}
			else
			{
				size_.y = size;
			}
			item->Size(size_);
		}
		return true;
	}
};

class ActDrawableVerticalAnchorToFocused : public Action
{
private:
	std::string id;
	float offset;

public:
	ActDrawableVerticalAnchorToFocused(const std::string_view id_, float offset_)
		: id(id_), offset(offset_) {}

	bool execute(Game& game) override
	{
		auto itemAnchor = game.Resources().getFocused();
		auto itemToChange = game.Resources().getDrawable(id);

		if (itemAnchor != nullptr && itemToChange != nullptr)
		{
			auto pos = itemToChange->Position();
			pos.y = itemAnchor->DrawPosition().y + offset;
			itemToChange->Position(pos);
		}
		return true;
	}
};
