#pragma once
#ifdef USE_SFML_MOVIE_STUB
#include "MovieStub.h"
#else
#include "Actions/Action.h"
#include "PhysFSStream.h"
#include <SFML/Graphics.hpp>
#include <sfeMovie/Movie.hpp>
#include "UIObject.h"

class Movie2 : public UIObject
{
private:
	sfe::Movie movie;
	sf::Vector2f size;
	Anchor anchor{ Anchor::Top | Anchor::Left };
	bool visible{ true };
	std::unique_ptr<sf::PhysFSStream> file;
	std::shared_ptr<Action> actionComplete;

public:
	Movie2(const std::string& file_) : Movie2(file_.c_str()) {}
	Movie2(const char* file_) : file(std::make_unique<sf::PhysFSStream>(file_)) {}

	bool load()
	{
		if (file == nullptr || file->hasError() == true)
		{
			return false;
		}
		bool ret = movie.openFromStream(*file);
		if (ret == true)
		{
			size = movie.getSize();
		}
		return ret;
	}
	void play() { movie.play(); }
	void pause() { movie.pause(); }

	void setActionComplete(const std::shared_ptr<Action>& action_) { actionComplete = action_; }
	void setVolume(float volume) { movie.setVolume(volume); }

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return movie.getPosition(); }
	virtual const sf::Vector2f& Position() const { return movie.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { movie.setPosition(position); }
	virtual sf::Vector2f Size() const { return size; }
	virtual void Size(const sf::Vector2f& size_)
	{
		size = size_;
		movie.fit(sf::FloatRect(movie.getPosition(), size_));
	}

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual Variable getProperty(const std::string& prop) const;
};
#endif	// USE_SFML_MOVIE_STUB
