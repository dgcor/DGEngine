#pragma once
#ifdef USE_SFML_MOVIE_STUB
#include "Actions/Action.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "UIObject.h"

class Movie2 : public UIObject
{
private:
	sf::RectangleShape movie;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };
	std::shared_ptr<Action> actionComplete;

public:
	Movie2(const std::string& file_) : Movie2(file_.c_str()) {}
	Movie2(const char* file_) { movie.setFillColor(sf::Color::Black); }

	bool load() { return true; }
	void play() {}
	void pause() {}

	void setVolume(float volume) {}

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16);
	virtual void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action);

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return movie.getPosition(); }
	virtual const sf::Vector2f& Position() const { return movie.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { movie.setPosition(position); }
	virtual sf::Vector2f Size() const { return movie.getSize(); }
	virtual void Size(const sf::Vector2f& size) { movie.setSize(size); }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }
};
#endif	// USE_SFML_MOVIE_STUB
