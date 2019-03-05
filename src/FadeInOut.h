#pragma once

#include "Actions/Action.h"
#include <memory>
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Time.hpp"

// fade out - hide
// fade in - show
class FadeInOut
{
private:
	sf::Color color{ sf::Color::Transparent };
	bool isFadeOut{ false };
	bool enableInput{ false };
	uint8_t fadeOffset{ 0 };
	sf::Time frameTime;
	sf::Time currentTime;
	std::shared_ptr<Action> action;
	bool running{ false };
	bool updateEnableInput{ false };

	void UpdateFade() noexcept;
	bool HasFadeEnded() noexcept;

public:
	const sf::Color& getColor() const noexcept { return color; }

	void Reset(sf::Color color_, bool isFadeOut_, bool enableInput_, uint8_t fadeOffset_,
		const sf::Time& frameTime_, const std::shared_ptr<Action>& action_);

	virtual void update(Game& game) noexcept;
};
