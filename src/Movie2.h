#pragma once

#include "Actions/Action.h"
#include "UIObject.h"
#ifndef DGENGINE_MOVIE_STUB
#include "sfeMovie/Movie.hpp"
#else
#include <SFML/Graphics/RectangleShape.hpp>
#endif
#include "SFML/PhysFSStream.h"

class Movie : public UIObject
{
private:
#ifndef DGENGINE_MOVIE_STUB
	std::unique_ptr<sf::PhysFSStream> file;
	sfe::Movie movie;
	sf::Vector2f size;
#else
	sf::RectangleShape movie;
#endif
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };
	std::shared_ptr<Action> actionComplete;

public:
#ifndef DGENGINE_MOVIE_STUB
	Movie(const std::string_view file_) : file(std::make_unique<sf::PhysFSStream>(file_.data())) {}

	bool load();
	void play() { movie.play(); }
	void pause() { movie.pause(); }
	void setVolume(float volume) { movie.setVolume(volume); }
#else
	Movie(const std::string_view file_) { movie.setFillColor(sf::Color::Black); }

	bool load();
	void play() noexcept {}
	void pause() noexcept {}
	void setVolume(float volume) noexcept {}
#endif

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const noexcept override { return anchor; }
	void setAnchor(const Anchor anchor_) noexcept override { anchor = anchor_; }
	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const override { return movie.getPosition(); }
	const sf::Vector2f& Position() const override { return movie.getPosition(); }
	void Position(const sf::Vector2f& position) override { movie.setPosition(position); }
#ifndef DGENGINE_MOVIE_STUB
	sf::Vector2f Size() const noexcept override { return size; }
	void Size(const sf::Vector2f& size_) override
	{
		size = size_;
		movie.fit(sf::FloatRect(movie.getPosition(), size_));
	}
#else
	sf::Vector2f Size() const override { return movie.getSize(); }
	void Size(const sf::Vector2f& size) override { movie.setSize(size); }
#endif

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	void draw(const Game& game, sf::RenderTarget& target) const override;
	void update(Game& game) override;
	bool getProperty(const std::string_view prop, Variable& var) const override;
};
