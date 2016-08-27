#include "ParseMenuQuests.h"
#include "ParseMenuButton.h"
#include "ParseAction.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void replaceValueString(Value& value, Value::AllocatorType& allocator,
		const std::string& oldStr, const std::string& newStr)
	{
		if (value.IsObject())
		{
			for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
			{
				if (it->value.IsString())
				{
					std::string str1(it->value.GetString());
					auto str2(str1);
					Utils::replaceStringInPlace(str2, oldStr, newStr);
					if (str1 != str2)
					{
						it->value.SetString(str2.c_str(), str2.size(), allocator);
					}
				}
				else
				{
					replaceValueString(it->value, allocator, oldStr, newStr);
				}
			}
		}
		else if (value.IsArray())
		{
			for (auto it = value.Begin(); it != value.End(); ++it)
			{
				replaceValueString(*it, allocator, oldStr, newStr);
			}
		}
	}

	void parseMenuQuests(
		Game& game,
		const Value& elem,
		Menu& menu,
		const sf::Vector2f& pos,
		Anchor anchor,
		const sf::Color& color,
		HorizontalAlign horizAlign,
		int horizSpaceOffset,
		int vertSpaceOffset,
		bool isTextFont,
		const std::shared_ptr<Font2>& font,
		unsigned fontSize,
		const std::shared_ptr<BitmapFont>& bitmapFont,
		const std::shared_ptr<sf::SoundBuffer>& sound,
		const std::shared_ptr<sf::SoundBuffer>& focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick)
	{
		auto level = game.Resources().getCurrentLevel();
		if (level != nullptr)
		{
			MemoryPoolAllocator<CrtAllocator> allocator;
			for (const auto& quest : level->Quests())
			{
				auto button = parseMenuButton(anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, isTextFont, font,
					fontSize, bitmapFont, sound, focusSound, clickUp);
				button->setText(quest.Name());
				button->Position(pos);

				if (elem.HasMember("onClick"))
				{
					Value elemCopy(elem["onClick"], allocator);
					replaceValueString(elemCopy, allocator, "%ID%", quest.Id());
					button->setClickAction(parseAction(game, elemCopy));
				}

				if (elem.HasMember("onDoubleClick"))
				{
					Value elemCopy(elem["onDoubleClick"], allocator);
					replaceValueString(elemCopy, allocator, "%ID%", quest.Id());
					button->setDoubleClickAction(parseAction(game, elemCopy));
				}

				if (elem.HasMember("onFocus"))
				{
					Value elemCopy(elem["onFocus"], allocator);
					replaceValueString(elemCopy, allocator, "%ID%", quest.Id());
					button->setFocusAction(parseAction(game, elemCopy));
				}

				if (elem.HasMember("onHoverEnter"))
				{
					Value elemCopy(elem["onHoverEnter"], allocator);
					replaceValueString(elemCopy, allocator, "%ID%", quest.Id());
					button->setHoverEnterAction(parseAction(game, elemCopy));
				}

				if (elem.HasMember("onHoverLeave"))
				{
					Value elemCopy(elem["onHoverLeave"], allocator);
					replaceValueString(elemCopy, allocator, "%ID%", quest.Id());
					button->setHoverLeaveAction(parseAction(game, elemCopy));
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
}
