#include "ParseDrawableActions.h"
#include "Game/Actions/ActDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseDrawableAddToPosition(const Value& elem)
	{
		return std::make_shared<ActDrawableAddToPosition>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableAddToSize(const Value& elem)
	{
		return std::make_shared<ActDrawableAddToSize>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableAnchor(const Value& elem)
	{
		return std::make_shared<ActDrawableAnchor>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchor"),
			getAnchorKey(elem, "anchor"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableAnchorSize(const Value& elem, bool applyToY)
	{
		return std::make_shared<ActDrawableAnchorSizeXY>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			(float)getIntKey(elem, "offset"),
			applyToY);
	}

	std::shared_ptr<Action> parseDrawableAnchorSizeX(const Value& elem)
	{
		return parseDrawableAnchorSize(elem, false);
	}

	std::shared_ptr<Action> parseDrawableAnchorSizeY(const Value& elem)
	{
		return parseDrawableAnchorSize(elem, true);
	}

	std::shared_ptr<Action> parseDrawableAnchorToFocused(const Value& elem)
	{
		return std::make_shared<ActDrawableAnchorToFocused>(
			getStringViewKey(elem, "id"),
			getAnchorKey(elem, "anchor"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableBringToFront(const Value& elem)
	{
		return std::make_shared<ActDrawableBringToFront>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseDrawableCenter(const Value& elem)
	{
		return std::make_shared<ActDrawableCenter>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idCenterOn"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableCenterOnMouseX(const Value& elem)
	{
		return std::make_shared<ActDrawableCenterOnMouseX>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "steps"));
	}

	std::shared_ptr<Action> parseDrawableCenterOnMouseY(const Value& elem)
	{
		return std::make_shared<ActDrawableCenterOnMouseY>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "steps"));
	}

	std::shared_ptr<Action> parseDrawableDelete(const Value& elem)
	{
		return std::make_shared<ActDrawableDelete>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseDrawableExecuteAction(const Value& elem)
	{
		return std::make_shared<ActDrawableExecuteAction>(
			getStringViewKey(elem, "id"),
			str2int16(getStringViewKey(elem, "action")));
	}

	std::shared_ptr<Action> parseDrawableHorizontalAnchorToFocused(const Value& elem)
	{
		return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
			getStringViewKey(elem, "id"),
			(float)getIntKey(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableMoveX(const Value& elem)
	{
		return std::make_shared<ActDrawableMoveX>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "position"),
			getUIntKey(elem, "min"),
			getUIntKey(elem, "max"),
			getVariableKey(elem, "steps"));
	}

	std::shared_ptr<Action> parseDrawableMoveY(const Value& elem)
	{
		return std::make_shared<ActDrawableMoveY>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "position"),
			getUIntKey(elem, "min"),
			getUIntKey(elem, "max"),
			getVariableKey(elem, "steps"));
	}

	std::shared_ptr<Action> parseDrawableMoveStepX(const Value& elem)
	{
		return std::make_shared<ActDrawableMoveStepX>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "steps"),
			getIntKey(elem, "stepOffset"));
	}

	std::shared_ptr<Action> parseDrawableMoveStepY(const Value& elem)
	{
		return std::make_shared<ActDrawableMoveStepY>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idAnchorTo"),
			getUIntKey(elem, "range"),
			getVariableKey(elem, "steps"),
			getIntKey(elem, "stepOffset"));
	}

	std::shared_ptr<Action> parseDrawableResize(const Value& elem, bool applyToY)
	{
		return std::make_shared<ActDrawableResizeXY>(
			getStringViewKey(elem, "id"),
			getVariableKey(elem, "size"),
			getVariableKey(elem, "inputRangeMin"),
			getVariableKey(elem, "inputRangeMax"),
			getVector2iKey<sf::Vector2i>(elem, "range"),
			applyToY);
	}

	std::shared_ptr<Action> parseDrawableResizeX(const Value& elem)
	{
		return parseDrawableResize(elem, false);
	}

	std::shared_ptr<Action> parseDrawableResizeY(const Value& elem)
	{
		return parseDrawableResize(elem, true);
	}

	std::shared_ptr<Action> parseDrawableResizeOnMouseX(const Value& elem)
	{
		return std::make_shared<ActDrawableResizeOnMouseX>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "range"));
	}

	std::shared_ptr<Action> parseDrawableResizeOnMouseY(const Value& elem)
	{
		return std::make_shared<ActDrawableResizeOnMouseY>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "range"));
	}

	std::shared_ptr<Action> parseDrawableResizeToPositionX(const Value& elem)
	{
		return std::make_shared<ActDrawableResizeToPositionX>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idToPosition"),
			(float)getIntKey(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableResizeToPositionY(const Value& elem)
	{
		return std::make_shared<ActDrawableResizeToPositionY>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idToPosition"),
			(float)getIntKey(elem, "offset"));
	}

	std::shared_ptr<Action> parseDrawableSendToBack(const Value& elem)
	{
		return std::make_shared<ActDrawableSendToBack>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseDrawableSetAction(Game& game, const Value& elem)
	{
		return std::make_shared<ActDrawableSetAction>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "event"),
			getActionKey(game, elem, "action"));
	}

	std::shared_ptr<Action> parseDrawableSetPosition(const Value& elem)
	{
		return std::make_shared<ActDrawableSetPosition>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "position"),
			getVector2fKey<sf::Vector2f>(elem, "offset"),
			getBoolKey(elem, "relativeCoords", true));
	}

	std::shared_ptr<Action> parseDrawableSetPosition(const Value& elem, bool applyToY)
	{
		return std::make_shared<ActDrawableSetPositionXY>(
			getStringViewKey(elem, "id"),
			(float)getIntKey(elem, "position"),
			applyToY);
	}

	std::shared_ptr<Action> parseDrawableSetPositionX(const Value& elem)
	{
		return parseDrawableSetPosition(elem, false);
	}

	std::shared_ptr<Action> parseDrawableSetPositionY(const Value& elem)
	{
		return parseDrawableSetPosition(elem, true);
	}

	std::shared_ptr<Action> parseDrawableSetSize(const Value& elem)
	{
		return std::make_shared<ActDrawableSetSize>(
			getStringViewKey(elem, "id"),
			getVector2fKey<sf::Vector2f>(elem, "size"));
	}

	std::shared_ptr<Action> parseDrawableSetSize(const Value& elem, bool applyToY)
	{
		return std::make_shared<ActDrawableSetSizeXY>(
			getStringViewKey(elem, "id"),
			(float)getIntKey(elem, "size"),
			applyToY);
	}

	std::shared_ptr<Action> parseDrawableSetSizeX(const Value& elem)
	{
		return parseDrawableSetSize(elem, false);
	}

	std::shared_ptr<Action> parseDrawableSetSizeY(const Value& elem)
	{
		return parseDrawableSetSize(elem, true);
	}

	std::shared_ptr<Action> parseDrawableVerticalAnchorToFocused(const Value& elem)
	{
		return std::make_shared<ActDrawableVerticalAnchorToFocused>(
			getStringViewKey(elem, "id"),
			(float)getIntKey(elem, "offset"));
	}
}
