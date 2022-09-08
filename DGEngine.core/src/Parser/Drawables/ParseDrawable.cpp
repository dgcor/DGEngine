#include "ParseDrawable.h"
#include "Game/Drawables/Panel.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseDrawableProperties(Game& game, const Value& elem, UIObject& obj)
	{
		auto size = obj.Size();
		parseDrawableProperties(game, elem, obj, size);
	}

	void parseDrawableProperties(Game& game, const Value& elem, UIObject& obj, sf::Vector2f& size)
	{
		auto anchor = getAnchorKey(elem, "anchor");
		obj.setAnchor(anchor);
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		obj.Position(pos);
		obj.Size(size);
		obj.Visible(getBoolKey(elem, "visible", true));
	}

	bool parseDrawablePanel(const ResourceManager& resources, const Value& elem, const std::shared_ptr<UIObject>& drawable)
	{
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = resources.getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				return panel->addDrawable(drawable);
			}
		}
		return false;
	}

	void parseDrawablePanelAndAdd(ResourceManager& resources, const Value& elem,
		const std::string_view id, const std::shared_ptr<UIObject>& obj)
	{
		bool manageObjDrawing = parseDrawablePanel(resources, elem, obj) == false;

		resources.addDrawable(
			id, obj, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseValidIdAndAddDrawable(Game& game, const Value& elem, const ParseDrawableFuncPtr parseDrawableFunc)
	{
		auto id = parseValidId(elem);
		if (id.empty() == true)
		{
			return;
		}

		auto obj = parseDrawableFunc(game, elem);
		if (obj == nullptr)
		{
			return;
		}

		parseDrawablePanelAndAdd(game.Resources(), elem, id, obj);
	}

	void parseValidIdFileAndAddDrawable(Game& game, const Value& elem, const ParseDrawableFuncPtr parseDrawableFunc)
	{
		auto id = parseValidIdOrFile(elem);
		if (id.empty() == true)
		{
			return;
		}

		auto obj = parseDrawableFunc(game, elem);
		if (obj == nullptr)
		{
			return;
		}

		parseDrawablePanelAndAdd(game.Resources(), elem, id, obj);
	}
}
