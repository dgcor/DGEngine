#pragma once

#include <initializer_list>
#include <list>
#include "ResourceBundle.h"
#include "ShaderManager.h"
#include "Utils/ReverseIterable.h"

class Image;

class ResourceManager
{
private:
	std::vector<ResourceBundle> resources;
	ShaderManager shaders;
	std::vector<std::shared_ptr<Image>> cursors;
	std::vector<uint16_t> activeInputEvents;
	std::list<sf::Sound> playingSounds;
	std::weak_ptr<UIObject> currentLevel;

	bool addSong(ResourceBundle& res,
		const std::string_view key, const std::shared_ptr<sf::Music2>& obj);

	void addDrawable(ResourceBundle& res, const std::string_view key,
		const std::shared_ptr<UIObject>& obj, bool manageObjDrawing);

	void addFocused(ResourceBundle& res, const std::shared_ptr<Button>& obj);

	void removeUnusedFocused(const ResourceBundle& res) const;

	template <class T>
	bool addResource(ResourceBundle& res, const std::string_view key, const T& obj)
	{
		if (res.hasResource<T>(key) == false)
		{
			res.resources.insert(std::make_pair(key, obj));
			return true;
		}
		return false;
	}

public:
	auto begin() noexcept { return resources.begin(); }
	auto end() noexcept { return resources.end(); }
	auto begin() const noexcept { return resources.begin(); }
	auto end() const noexcept { return resources.end(); }
	auto cbegin() const noexcept { return resources.cbegin(); }
	auto cend() const noexcept { return resources.cend(); }
	auto rbegin() noexcept { return resources.rbegin(); }
	auto rend() noexcept { return resources.rend(); }
	auto rbegin() const noexcept { return resources.rbegin(); }
	auto rend() const noexcept { return resources.rend(); }
	auto crbegin() const noexcept { return resources.crbegin(); }
	auto crend() const noexcept { return resources.crend(); }

	ResourceManager() noexcept : resources(1, ResourceBundle()) {}

	auto& Shaders() { return shaders; };
	auto& Shaders() const { return shaders; };

	void setCurrentLevel(std::weak_ptr<UIObject> level) noexcept { currentLevel = level; }

	void addResource(const std::string& id);
	void popResource();
	void popResource(const std::string& id);
	void popAllResources(const std::string& id);
	void popAllResources(bool popBaseResources);
	void ignoreResources(const std::string& id, IgnoreResource ignore) noexcept;
	void ignoreTopResource(IgnoreResource ignore);
	bool resourceExists(const std::string_view id) const noexcept;
	void bringResourceToFront(const std::string& id);

	Image* getCursor() const;

	template <class T>
	std::shared_ptr<T> getCurrentLevel() const noexcept { return std::dynamic_pointer_cast<T>(currentLevel.lock()); }

	template <class T>
	std::shared_ptr<T> getLevel(const std::string_view id) const noexcept
	{
		if (id.empty() == true)
		{
			return getCurrentLevel<T>();
		}
		return getDrawableSharedPtr<T>(id);
	}

	void addCursor(const std::shared_ptr<Image>& cursor_) { cursors.push_back(cursor_); }
	void popCursor(bool popAll = false);
	void popAllCursors() noexcept { cursors.clear(); }
	size_t cursorCount() const noexcept { return cursors.size(); }

	void setInputAction(const sf::Event& evt, const std::shared_ptr<Action>& obj);
	void setInputEvent(const InputEvent& evt, uint16_t actionHash16);
	void setAction(const std::string_view key, const std::shared_ptr<Action>& obj);

	bool addFileBytes(const std::string_view key, const std::shared_ptr<FileBytes>& obj,
		const std::string_view resourceId = {});
	bool addFont(const std::string_view key, const Font& obj,
		const std::string_view resourceId = {});
	bool addTexture(const std::string_view key, const std::shared_ptr<sf::Texture>& obj,
		const std::string_view resourceId = {});
	bool addAudioSource(const std::string_view key, const AudioSource& obj,
		const std::string_view resourceId = {});
	bool addSong(const std::string_view key, const std::shared_ptr<sf::Music2>& obj,
		const std::string_view resourceId = {});
	bool addPalette(const std::string_view key, const std::shared_ptr<Palette>& obj,
		const std::string_view resourceId = {});
	bool addImageContainer(const std::string_view key, const std::shared_ptr<ImageContainer>& obj,
		const std::string_view resourceId = {});
	bool addTexturePack(const std::string_view key, const std::shared_ptr<TexturePack>& obj,
		const std::string_view resourceId = {});

	void addDrawable(const std::string_view key, const std::shared_ptr<UIObject>& obj,
		bool manageObjDrawing, const std::string_view resourceId = {});

	void addPlayingSound(const sf::Sound& obj, bool unique = false,
		sf::Time seek = sf::Time::Zero);

	void clearFinishedSounds();
	void clearPlayingSounds() noexcept { playingSounds.clear(); }

	template <class T>
	bool addResource(const std::string_view key, const T& obj, const std::string_view resourceId)
	{
		if (resourceId.empty() == true)
		{
			return addResource(resources.back(), key, obj);
		}
		for (auto& res : reverse(resources))
		{
			if (res.id == resourceId)
			{
				return addResource(res, key, obj);
			}
		}
		return false;
	}

	template <class T>
	bool hasResource(const std::string_view key, bool checkTopOnly) const
	{
		for (const auto& resource : resources)
		{
			if (resource.hasResource<T>(key) == true)
			{
				return true;
			}
			if (checkTopOnly == true)
			{
				return false;
			}
		}
		return false;
	}

	template <class T>
	T getResource(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto range = res.resources.equal_range(key);
			for (; range.first != range.second; ++range.first)
			{
				if (std::holds_alternative<T>(range.first->second) == true)
				{
					return std::get<T>(range.first->second);
				}
			}
		}
		return {};
	}

	std::shared_ptr<Action> getInputAction(const sf::Event& key) const;
	std::shared_ptr<Action> getAction(const std::string_view key) const;
	std::shared_ptr<FileBytes> getFileBytes(const std::string_view key) const;
	Font getFont(const std::string_view key) const;
	std::shared_ptr<sf::Texture> getTexture(const std::string_view key) const;
	AudioSource getAudioSource(const std::string_view key) const;
	sf::SoundBuffer* getSoundBuffer(const std::string_view key) const;
	std::shared_ptr<sf::Music2> getSong(const std::string_view key) const;
	std::shared_ptr<Palette> getPalette(const std::string_view key) const;
	std::shared_ptr<ImageContainer> getImageContainer(const std::string_view key) const;
	std::shared_ptr<TexturePack> getTexturePack(const std::string_view key) const;

	bool hasFileBytes(const std::string_view key) const;
	bool hasFont(const std::string_view key) const;
	bool hasTexture(const std::string_view key) const;
	bool hasAudioSource(const std::string_view key) const;
	bool hasSong(const std::string_view key, bool checkTopOnly = false) const;
	bool hasPalette(const std::string_view key) const;
	bool hasImageContainer(const std::string_view key) const;
	bool hasTexturePack(const std::string_view key) const;

	bool hasDrawable(const std::string_view key) const;

	void bringDrawableToFront(const std::string& id);
	void sendDrawableToBack(const std::string& id);

	auto getDrawable(const std::string_view key) const
	{
		return getDrawable<UIObject>(key);
	}

	template <class T>
	T* getDrawable(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(key);
			if (it != res.drawableIds.cend())
			{
				return dynamic_cast<T*>(it->second.get());
			}
		}
		return nullptr;
	}

	template <class T>
	std::shared_ptr<T> getDrawableSharedPtr(const std::string_view key) const
	{
		for (const auto& res : reverse(resources))
		{
			auto it = res.drawableIds.find(key);
			if (it != res.drawableIds.cend())
			{
				return std::dynamic_pointer_cast<T>(it->second);
			}
		}
		return nullptr;
	}

	// deletes a drawable from drawableIds and drawables vector
	void deleteDrawable(const std::string& id);

	// deletes a drawable from the drawables vector (and not from drawableIds)
	void deleteDrawable(UIObject* obj);

	void play(const std::string_view key);
	void playPaused(bool restart);
	void pauseSongs();
	void stopSongs();
	void deleteSong(const std::string_view key);

	void addFocused(const std::shared_ptr<Button>& obj, const std::string_view resourceId = {});
	void clickFocused(Game& game, bool playSound);
	void rightClickFocused(Game& game, bool playSound);
	std::shared_ptr<Button> getFocused() const;
	void setFocused(const Button* obj);

	void moveFocusDown(Game& game);
	void moveFocusUp(Game& game);
	void updateFocus(Game& game);

	bool hasActiveInputEvents() const noexcept { return activeInputEvents.empty() == false; }
	bool hasActiveInputEvents(const std::initializer_list<uint16_t> nameHashes) const;
	void updateActiveInputEvents();
};
