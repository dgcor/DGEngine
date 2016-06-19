#include "ParseMenuSaveGames.h"
#include "FileUtils.h"
#include "ParseMenuButton.h"
#include "ParseAction.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMenuSaveGames(
		Game& game,
		const Value& elem,
		Menu& menu,
		const sf::Vector2f& pos,
		Anchor anchor,
		const sf::Color& color,
		const HorizontalAlign horizAlign,
		const bool isTextFont,
		const std::shared_ptr<Font2>& font,
		const unsigned fontSize,
		const std::shared_ptr<BitmapFont>& bitmapFont,
		const std::shared_ptr<sf::SoundBuffer>& sound,
		const std::shared_ptr<sf::SoundBuffer>& focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick)
	{
		std::shared_ptr<Action> clickAction;
		if (elem.HasMember("onClick"))
		{
			clickAction = parseAction(game, elem["onClick"]);
		}

		std::shared_ptr<Action> doubleClickAction;
		if (elem.HasMember("onDoubleClick"))
		{
			doubleClickAction = parseAction(game, elem["onDoubleClick"]);
		}

		std::shared_ptr<Action> focusAction;
		if (elem.HasMember("onFocus"))
		{
			focusAction = parseAction(game, elem["onFocus"]);
		}

		std::shared_ptr<Action> hoverEnterAction;
		if (elem.HasMember("onHoverEnter"))
		{
			hoverEnterAction = parseAction(game, elem["onHoverEnter"]);
		}

		std::shared_ptr<Action> hoverLeaveAction;
		if (elem.HasMember("onHoverLeave"))
		{
			hoverLeaveAction = parseAction(game, elem["onHoverLeave"]);
		}

		for (const auto& dir : FileUtils::getSaveDirList())
		{
			auto button = parseMenuButton(anchor, color, horizAlign, isTextFont, font,
				fontSize, bitmapFont, sound, focusSound, clickUp);
			button->setText(dir);
			button->Position(pos);

			if (clickAction != nullptr)
			{
				button->setClickAction(clickAction);
			}

			if (doubleClickAction != nullptr)
			{
				button->setDoubleClickAction(doubleClickAction);
			}

			if (focusAction != nullptr)
			{
				button->setFocusAction(focusAction);
			}

			if (hoverEnterAction != nullptr)
			{
				button->setHoverEnterAction(hoverEnterAction);
			}

			if (hoverLeaveAction != nullptr)
			{
				button->setHoverLeaveAction(hoverLeaveAction);
			}

			menu.addItem(button);

			if (hasFocus == true)
			{
				button->focusEnabled(focusOnClick);
				game.Resources().addFocused(button);
			}
		}
	}
}
