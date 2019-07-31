#include "ResourceManager.h"
#include "Button.h"
#include <cctype>
#include "Game.h"
#include "Game/Level.h"

void ResourceManager::addResource(const std::string& id)
{
	resources.push_back(ResourceBundle(id));
}

void ResourceManager::popResource()
{
	if (resources.size() > 0)
	{
		resources.pop_back();
		clearCurrentLevel();
	}
}

void ResourceManager::popResource(const std::string& id)
{
	for (auto it = resources.rbegin(); it != resources.rend(); ++it)
	{
		if (it->id == id)
		{
			auto idx = std::distance(std::begin(resources), it.base()) - 1;
			if (idx == (int)currentLevelResourceIdx)
			{
				currentLevel = nullptr;
				currentLevelResourceIdx = 0;
			}
			else if (idx < (int)currentLevelResourceIdx)
			{
				currentLevelResourceIdx--;
			}
			resources.erase(--it.base());
			return;
		}
	}
}

void ResourceManager::popAllResources(const std::string& id)
{
	for (auto it = resources.rbegin(); it != resources.rend(); ++it)
	{
		if (it->id == id && it.base() != resources.begin())
		{
			resources.erase(--it.base(), resources.end());
			clearCurrentLevel();
			return;
		}
	}
}

void ResourceManager::popAllResources(bool popBaseResources)
{
	resources.resize(1);
	if (popBaseResources)
	{
		resources.front() = {};
		currentLevel = nullptr;
		currentLevelResourceIdx = 0;
	}
	else
	{
		clearCurrentLevel();
	}
}

void ResourceManager::ignoreResources(const std::string& id, IgnoreResource ignore) noexcept
{
	for (auto& res : reverse(resources))
	{
		if (res.id == id)
		{
			res.ignore = ignore;
			return;
		}
	}
}

void ResourceManager::ignoreTopResource(IgnoreResource ignore)
{
	if (resources.size() > 0)
	{
		resources.back().ignore = ignore;
	}
}

bool ResourceManager::resourceExists(const std::string_view id) const noexcept
{
	for (auto& res : resources)
	{
		if (res.id == id)
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::bringResourceToFront(const std::string& id)
{
	auto it = std::find_if(resources.begin(), resources.end(),
		[&id](const auto& res) -> bool { return res.id == id; }
	);
	if (it != resources.end())
	{
		std::rotate(it, it + 1, resources.end());
	}
}

Level* ResourceManager::getLevel(const std::string& id) const noexcept
{
	if (id.empty() == true)
	{
		return currentLevel;
	}
	return getDrawable<Level>(id);
}

Image* ResourceManager::getCursor() const
{
	if (cursors.empty() == true)
	{
		return nullptr;
	}
	return cursors.back().get();
}

void ResourceManager::popCursor(bool popAll)
{
	if (cursors.empty() == true)
	{
		return;
	}
	if (popAll)
	{
		cursors.clear();
	}
	else
	{
		cursors.pop_back();
	}
}

void ResourceManager::setInputAction(const sf::Event& evt, const std::shared_ptr<Action>& obj)
{
	if (evt.type == sf::Event::KeyPressed &&
		evt.key.code == sf::Keyboard::KeyCount)
	{
		resources.back().anyKeyAction = obj;
		return;
	}
	resources.back().inputActions[evt] = obj;
}

void ResourceManager::setInputAction(const CompositeInputEvent& evt, const std::shared_ptr<Action>& obj)
{
	resources.back().compositeInputActions.push_back(std::make_pair(evt, obj));
}

void ResourceManager::setAction(const std::string& key, const std::shared_ptr<Action>& obj)
{
	resources.back().actions[key] = obj;
}

bool ResourceManager::addFont(const std::string& key,
	const Font& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addTexture(const std::string& key,
	const std::shared_ptr<sf::Texture>& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addAudioSource(const std::string& key,
	const AudioSource& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addSong(ResourceBundle& res,
	const std::string& key, const std::shared_ptr<sf::Music2>& obj)
{
	if (hasSong(key, true) == false)
	{
		res.songs[key] = obj;
		return true;
	}
	return false;
}

bool ResourceManager::addSong(const std::string& key,
	const std::shared_ptr<sf::Music2>& obj, const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		return addSong(resources.back(), key, obj);
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			return addSong(res, key, obj);
		}
	}
	return false;
}

bool ResourceManager::addPalette(const std::string& key,
	const std::shared_ptr<Palette>& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addImageContainer(const std::string& key,
	const std::shared_ptr<ImageContainer>& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addTexturePack(const std::string& key,
	const std::shared_ptr<TexturePack>& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

bool ResourceManager::addCompositeTexture(const std::string& key,
	const std::shared_ptr<CompositeTexture>& obj, const std::string_view resourceId)
{
	return addResource(key, obj, resourceId);
}

void ResourceManager::addDrawable(ResourceBundle& res, const std::string& key,
	const std::shared_ptr<UIObject>& obj, bool manageObjDrawing)
{
	auto it = res.drawableIds.find(key);
	if (it == res.drawableIds.end())
	{
		res.drawableIds[key] = obj;
		if (manageObjDrawing == true)
		{
			res.drawables.push_back(obj.get());
		}
	}
}

void ResourceManager::addDrawable(const std::string& key,
	const std::shared_ptr<UIObject>& obj, bool manageObjDrawing,
	const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		addDrawable(resources.back(), key, obj, manageObjDrawing);
		return;
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			addDrawable(res, key, obj, manageObjDrawing);
			return;
		}
	}
}

void ResourceManager::addPlayingSound(const sf::Sound& obj, bool unique)
{
	if (unique == true)
	{
		for (const auto& snd : playingSounds)
		{
			if (snd.getBuffer() == obj.getBuffer())
			{
				return;
			}
		}
	}
	playingSounds.push_back(obj);
	playingSounds.back().play();
}

void ResourceManager::clearFinishedSounds()
{
	for (auto it = playingSounds.begin(); it != playingSounds.end();)
	{
		if ((*it).getStatus() != sf::Sound::Playing)
		{
			it = playingSounds.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::shared_ptr<Action> ResourceManager::getInputAction(const sf::Event& key) const
{
	for (const auto& res : reverse(resources))
	{
		if (key.type == sf::Event::KeyPressed &&
			res.anyKeyAction != nullptr)
		{
			return res.anyKeyAction;
		}
		const auto elem = res.inputActions.find(key);
		if (elem != res.inputActions.cend())
		{
			return elem->second;
		}
	}
	return {};
}

std::shared_ptr<Action> ResourceManager::getAction(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.actions.find(key);
		if (elem != res.actions.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

Font ResourceManager::getFont(const std::string& key) const
{
	return getResource<Font>(key);
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& key) const
{
	return getResource<std::shared_ptr<sf::Texture>>(key);
}

AudioSource ResourceManager::getAudioSource(const std::string& key) const
{
	return getResource<AudioSource>(key);
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& key) const
{
	auto elem = getAudioSource(key);
	if (std::holds_alternative<std::shared_ptr<sf::SoundBuffer>>(elem) == true)
	{
		return std::get<std::shared_ptr<sf::SoundBuffer>>(elem).get();
	}
	else if (std::holds_alternative<std::shared_ptr<SoundBufferLoops>>(elem) == true)
	{
		return &std::get<std::shared_ptr<SoundBufferLoops>>(elem)->soundBuffer;
	}
	return nullptr;
}

std::shared_ptr<sf::Music2> ResourceManager::getSong(const std::string& key) const
{
	for (const auto& res : reverse(resources))
	{
		const auto elem = res.songs.find(key);
		if (elem != res.songs.cend())
		{
			return elem->second;
		}
	}
	return nullptr;
}

std::shared_ptr<Palette> ResourceManager::getPalette(const std::string& key) const
{
	return getResource<std::shared_ptr<Palette>>(key);
}

std::shared_ptr<ImageContainer> ResourceManager::getImageContainer(const std::string& key) const
{
	return getResource<std::shared_ptr<ImageContainer>>(key);
}

std::shared_ptr<TexturePack> ResourceManager::getTexturePack(const std::string& key) const
{
	return getResource<std::shared_ptr<TexturePack>>(key);
}

std::shared_ptr<CompositeTexture> ResourceManager::getCompositeTexture(const std::string& key) const
{
	return getResource<std::shared_ptr<CompositeTexture>>(key);
}

bool ResourceManager::hasFont(const std::string& key) const
{
	return hasResource<Font>(key, false);
}

bool ResourceManager::hasTexture(const std::string& key) const
{
	return hasResource<std::shared_ptr<sf::Texture>>(key, false);
}

bool ResourceManager::hasAudioSource(const std::string& key) const
{
	return hasResource<AudioSource>(key, false);
}

bool ResourceManager::hasSong(const std::string& key, bool checkTopOnly) const
{
	for (const auto& resource : resources)
	{
		if (resource.songs.find(key) != resource.songs.cend())
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

bool ResourceManager::hasPalette(const std::string& key) const
{
	return hasResource<std::shared_ptr<Palette>>(key, false);
}

bool ResourceManager::hasImageContainer(const std::string& key) const
{
	return hasResource<std::shared_ptr<ImageContainer>>(key, false);
}

bool ResourceManager::hasTexturePack(const std::string& key) const
{
	return hasResource<std::shared_ptr<TexturePack>>(key, false);
}

bool ResourceManager::hasCompositeTexture(const std::string& key) const
{
	return hasResource<std::shared_ptr<CompositeTexture>>(key, false);
}

bool ResourceManager::hasDrawable(const std::string& key) const
{
	for (const auto& resource : resources)
	{
		if (resource.drawableIds.find(key) != resource.drawableIds.cend())
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::bringDrawableToFront(const std::string& id)
{
	for (auto& resource : reverse(resources))
	{
		auto itDraw = resource.drawableIds.find(id);
		if (itDraw == resource.drawableIds.cend())
		{
			continue;
		}
		auto drawablePtr = itDraw->second.get();
		auto it = std::find_if(resource.drawables.begin(), resource.drawables.end(),
			[&drawablePtr](const auto& res) -> bool { return res == drawablePtr; }
		);
		if (it != resource.drawables.end())
		{
			std::rotate(it, it + 1, resource.drawables.end());
		}
	}
}

void ResourceManager::sendDrawableToBack(const std::string& id)
{
	for (auto& resource : reverse(resources))
	{
		auto itDraw = resource.drawableIds.find(id);
		if (itDraw == resource.drawableIds.cend())
		{
			continue;
		}
		auto drawablePtr = itDraw->second.get();
		auto it = std::find_if(resource.drawables.begin(), resource.drawables.end(),
			[&drawablePtr](const auto& res) -> bool { return res == drawablePtr; }
		);
		if (it != resource.drawables.end())
		{
			std::rotate(resource.drawables.begin(), it, it + 1);
		}
	}
}

void ResourceManager::deleteDrawable(const std::string& id)
{
	for (auto& res : reverse(resources))
	{
		auto it1 = res.drawableIds.find(id);
		if (it1 != res.drawableIds.end())
		{
			auto objPtr = it1->second.get();
			res.drawableIds.erase(it1);
			auto& drawables = res.drawables;
			for (auto it2 = drawables.begin(); it2 != drawables.end(); ++it2)
			{
				if (*it2 == objPtr)
				{
					drawables.erase(it2);
					return;
				}
			}
		}
	}
}

void ResourceManager::deleteDrawable(UIObject* obj)
{
	for (auto& res : reverse(resources))
	{
		auto& drawables = res.drawables;
		for (auto it2 = drawables.begin(); it2 != drawables.end(); ++it2)
		{
			if (*it2 == obj)
			{
				drawables.erase(it2);
				return;
			}
		}
	}
}

void ResourceManager::play(const std::string& key)
{
	auto& song = resources.back().songs[key];
	if (song != nullptr)
	{
		song->play();
	}
}

void ResourceManager::playPaused(bool restart)
{
	for (auto& res : resources)
	{
		for (auto& song : res.songs)
		{
			if (song.second->getStatus() == sf::SoundSource::Paused)
			{
				if (restart)
				{
					song.second->setPlayingOffset(sf::Time::Zero);
				}
				song.second->play();
			}
		}
	}
}

void ResourceManager::pauseSongs()
{
	for (auto& res : resources)
	{
		for (auto& song : res.songs)
		{
			if (song.second->getStatus() == sf::SoundSource::Playing)
			{
				song.second->pause();
			}
		}
	}
}

void ResourceManager::stopSongs()
{
	for (auto& res : resources)
	{
		for (auto& song : res.songs)
		{
			song.second->stop();
		}
	}
}

void ResourceManager::addFocused(ResourceBundle& res, const std::shared_ptr<Button>& obj)
{
	for (const auto& focusBtn : res.focusButtons)
	{
		auto btnPtr = focusBtn.lock();
		if (btnPtr != nullptr && btnPtr == obj)
		{
			return;
		}
	}
	res.focusButtons.push_back(obj);
}

void ResourceManager::addFocused(const std::shared_ptr<Button>& obj,
	const std::string_view resourceId)
{
	if (resourceId.empty() == true)
	{
		addFocused(resources.back(), obj);
		return;
	}
	for (auto& res : reverse(resources))
	{
		if (res.id == resourceId)
		{
			addFocused(res, obj);
			return;
		}
	}
}

void ResourceManager::removeUnusedFocused(const ResourceBundle& res) const
{
	for (auto it = res.focusButtons.begin(); it != res.focusButtons.end();)
	{
		if (it->expired() == true)
		{
			it = res.focusButtons.erase(it);
			if (res.focusIdx != 0 &&
				res.focusIdx >= res.focusButtons.size())
			{
				res.focusIdx--;
			}
		}
		else
		{
			++it;
		}
	}
}

void ResourceManager::clickFocused(Game& game, bool playSound)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false &&
			res.focusIdx < res.focusButtons.size())
		{
			res.focusButtons[res.focusIdx].lock()->click(game, playSound);
		}
		break;
	}
}

const Button* ResourceManager::getFocused() const
{
	for (const auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false &&
			res.focusIdx < res.focusButtons.size())
		{
			return res.focusButtons[res.focusIdx].lock().get();
		}
		break;
	}
	return nullptr;
}

void ResourceManager::setFocused(const Button* obj)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		if (res.focusButtons.empty() == false)
		{
			for (size_t i = 0; i < res.focusButtons.size(); i++)
			{
				if (res.focusButtons[i].lock().get() == obj)
				{
					res.focusIdx = i;
					return;
				}
			}
		}
		break;
	}
}

void ResourceManager::moveFocusDown(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx + 1 < res.focusButtons.size())
				{
					idx++;
				}
				else
				{
					idx = 0;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
				focus->focus(game);
			}
		}
		break;
	}
}

void ResourceManager::moveFocusUp(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx > 0)
				{
					idx--;
				}
				else
				{
					idx = res.focusButtons.size() - 1;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
				focus->focus(game);
			}
		}
		break;
	}
}

void ResourceManager::updateFocus(Game& game)
{
	for (auto& res : reverse(resources))
	{
		if (res.ignore != IgnoreResource::None)
		{
			continue;
		}
		removeUnusedFocused(res);
		if (res.focusButtons.empty() == false)
		{
			if (res.focusButtons[res.focusIdx].lock()->isEnabled() == true)
			{
				break;
			}
			auto idx = res.focusIdx;
			while (true)
			{
				if (idx + 1 < res.focusButtons.size())
				{
					idx++;
				}
				else
				{
					idx = 0;
				}
				if (res.focusButtons[idx].lock()->isEnabled() == true ||
					idx == res.focusIdx)
				{
					break;
				}
			}
			auto focus = res.focusButtons[idx].lock();
			if (focus->isEnabled() == true && idx != res.focusIdx)
			{
				res.focusIdx = idx;
			}
		}
		break;
	}
}

void ResourceManager::processCompositeInputEvents(Game& game) const
{
	for (const auto& res : reverse(resources))
	{
		if (res.anyKeyAction != nullptr)
		{
			return;
		}
		for (const auto& ca : res.compositeInputActions)
		{
			if (ca.first.isActive() == true)
			{
				game.Events().addBack(ca.second);
				return;
			}
		}
	}
}
