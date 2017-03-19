#pragma once

#include "CelCache.h"
#include "GameProperties.h"
#include <memory>
#include <string>

class Player;

class PlayerClass
{
private:
	std::array<std::shared_ptr<CelTextureCacheVector>, PlayerPaletteSize> celTextures;
	std::array<size_t, (size_t)PlayerStatus::Size> statusCelIndexes;

	std::string name;
	std::string type;
	std::string description;

	std::vector<LevelObjProperty> defaults;

public:
	PlayerClass() : statusCelIndexes() {}

	CelTextureCacheVector* getCelTexture(size_t paletteIdx) const
	{
		if (paletteIdx < PlayerPaletteSize)
		{
			return celTextures[paletteIdx].get();
		}
		return nullptr;
	}

	void setCelTexture(size_t paletteIdx,
		const std::shared_ptr<CelTextureCacheVector>& celTexture)
	{
		if (paletteIdx < PlayerPaletteSize)
		{
			celTextures[paletteIdx] = celTexture;
		}
	}

	void clearStatusCelIndexes() { statusCelIndexes.fill(0); }

	size_t getStatusCelIndex(PlayerStatus status)
	{
		if (status < PlayerStatus::Size)
		{
			return statusCelIndexes[(size_t)status];
		}
		return 0;
	}

	void setStatusCelIndex(PlayerStatus status, size_t idx)
	{
		if (status < PlayerStatus::Size)
		{
			statusCelIndexes[(size_t)status] = idx;
		}
	}

	const std::array<std::shared_ptr<CelTextureCacheVector>, PlayerPaletteSize>& CelTextures() const
	{
		return celTextures;
	}

	const std::vector<LevelObjProperty> Defaults() const { return defaults; }
	void setDefault(const char* prop, int16_t val);

	const std::string& Name() const { return name; }
	const std::string& Type() const { return type; }
	const std::string& Description() const { return description; }

	void Name(const std::string& name_) { name = name_; }
	void Type(const std::string& type_) { type = type_; }
	void Description(const std::string& description_) { description = description_; }
};
