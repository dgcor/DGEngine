#pragma once

#include "Action.h"
#include "IgnoreResource.h"
#include "InputEvent.h"
#include <memory>
#include "Resources/AudioSource.h"
#include "Resources/FileBytes.h"
#include "Resources/Font.h"
#include "Resources/ImageContainer.h"
#include "Resources/Palette.h"
#include "Resources/TexturePack.h"
#include <SFML/Audio/Sound.hpp>
#include "SFML/Music2.h"
#include "SFML/MusicLoops.h"
#include <string>
#include "UIObject.h"
#include "Utils/UnorderedStringMap.h"
#include <variant>
#include <vector>

class Button;

struct ResourceBundle
{
	using Resource = std::variant<
		std::shared_ptr<FileBytes>,
		Font,
		std::shared_ptr<sf::Texture>,
		AudioSource,
		std::shared_ptr<Palette>,
		std::shared_ptr<ImageContainer>,
		std::shared_ptr<TexturePack>>;

	IgnoreResource ignore{ IgnoreResource::None };
	std::string id;
	std::shared_ptr<Action> anyKeyAction;
	UnorderedStringMap<std::shared_ptr<Action>> actions;
	std::unordered_map<sf::Event, std::shared_ptr<Action>, CompareEvent, CompareEvent> inputActions;
	std::vector<std::pair<InputEvent, uint16_t>> inputEvents;
	UnorderedStringMultiMap<Resource> resources;
	UnorderedStringMap<std::shared_ptr<sf::Music2>> songs;
	UnorderedStringMap<std::shared_ptr<UIObject>> drawableIds;
	std::vector<UIObject*> drawables;
	mutable std::vector<std::weak_ptr<Button>> focusButtons;
	mutable size_t focusIdx{ 0 };

	ResourceBundle() noexcept {}
	ResourceBundle(const std::string& id_) :id(id_) {}

	template <class T>
	bool hasResource(const std::string_view key) const
	{
		auto range = resources.equal_range(key);
		for (; range.first != range.second; ++range.first)
		{
			if (std::holds_alternative<T>(range.first->second) == true)
			{
				return true;
			}
		}
		return false;
	}

	template <class T>
	T getResource(const std::string_view key) const
	{
		auto range = resources.equal_range(key);
		for (; range.first != range.second; ++range.first)
		{
			if (std::holds_alternative<T>(range.first->second) == true)
			{
				return std::get<T>(range.first->second);
			}
		}
		return nullptr;
	}
};
