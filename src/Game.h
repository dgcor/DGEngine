#pragma once

#include "EventManager.h"
#include "Game/Level.h"
#include "LoadingScreen.h"
#include <memory>
#include "Menu.h"
#include "Parser/ParseVariable.h"
#include "Rectangle.h"
#include "ResourceManager.h"
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Variable.h"
#include <vector>

class Game : public sf::NonCopyable
{
private:
	sf::RenderWindow window;
	sf::RenderTexture windowTex;
	sf::Sprite windowSprite;

	sf::Vector2u refSize;
	sf::Vector2u minSize;
	sf::Vector2u oldSize;
	sf::Vector2u size;
	sf::Vector2u windowTexSize;
	unsigned framerate = 60;
	bool smoothScreen = false;
	bool stretchToFit = false;
	bool keepAR = true;
	bool disableInput = false;
	bool pauseOnFocusLoss = false;
	bool paused = false;

	sf::Vector2f mousePosition;
	sf::Mouse::Button mouseButton;
	sf::Event::MouseWheelScrollEvent mouseWheel;
	sf::Clock mouseClickClock;
	bool mouseClicked = false;
	bool mouseDoubleClicked = false;
	bool mouseMoved = false;
	bool mousePressed = false;
	bool mouseReleased = false;
	bool mouseScrolled = false;
	char keyboardChar = false;
	sf::Event::KeyEvent keyPressed;

	unsigned musicVolume = 100;
	unsigned soundVolume = 100;

	sf::Time elapsedTime;

	std::string path;
	std::string title;
	std::string version;

	ResourceManager resourceManager;
	EventManager eventManager;

	std::map<std::string, Variable> variables;

	std::unique_ptr<LoadingScreen> loadingScreen;
	std::unique_ptr<Rectangle> fadeInOut;

	void processEvents();
	void onClosed();
	void onResized(const sf::Event::SizeEvent& evt);
	void onLostFocus();
	void onGainedFocus();
	void onTextEntered(const sf::Event::TextEvent& evt);
	void onKeyPressed(const sf::Event::KeyEvent& evt);
	void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt);
	void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
	void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);
	void onMouseMoved(const sf::Event::MouseMoveEvent& evt);
#ifdef __ANDROID__
	void onTouchBegan(const sf::Event::TouchEvent& evt);
	void onTouchMoved(const sf::Event::TouchEvent& evt);
	void onTouchEnded(const sf::Event::TouchEvent& evt);
#endif

	void updateMouse(const sf::Vector2i mousePos);
	void checkKeyPress();
	void updateEvents();
	void drawCursor();
	void drawAndUpdate();
	void drawFadeEffect();
	void drawWindow();

	void updateSize();
	void updateWindowTex();

	std::map<std::string, Variable>::const_iterator findVariable(const std::string& key) const;

public:
	Game() : refSize(640, 480), minSize(640, 480), size(640, 480) {}
	~Game();

	void init();

	const sf::Vector2u& OldWindowSize() const { return oldSize; }
	const sf::Vector2u& WindowSize() const { return size; }
	const sf::Vector2u& WindowTexSize() const { return windowTexSize; }
	sf::Vector2f WindowTexSizef() const
	{
		return sf::Vector2f((float)windowTexSize.x, (float)windowTexSize.x);
	}
	void WindowSize(const sf::Vector2u& size_)
	{
		if (size_.x >= minSize.x && size_.y >= minSize.y)
		{
			if (window.isOpen() == true)
			{
				window.setSize(size_);
			}
			else
			{
				size = size_;
			}
		}
	}
	const sf::Vector2u& RefSize() const { return refSize; }
	void RefSize(const sf::Vector2u& size_)
	{
		if (window.isOpen() == false &&
			size_.x > minSize.x &&
			size_.y > minSize.y)
		{
			refSize = size_;
		}
	}
	const sf::Vector2u& MinSize() const { return minSize; }
	void MinSize(const sf::Vector2u& size_)
	{
		if (window.isOpen() == true)
		{
			return;
		}
		minSize = size_;
		if (size.x < minSize.x && size.y < minSize.y)
		{
			size = minSize;
		}
	}
	unsigned Framerate() const { return framerate; }
	bool SmoothScreen() const { return smoothScreen; }
	bool StretchToFit() const { return stretchToFit; }
	bool KeepAR() const { return keepAR; }

	const sf::Vector2f& MousePosition() const { return mousePosition; }
	sf::Mouse::Button getMouseButton() { return mouseButton; }
	const sf::Event::MouseWheelScrollEvent& getMouseWheelScroll() const { return mouseWheel; }
	void clearMouseClicked() { mouseClicked = false; }
	void clearMouseDoubleClicked() { mouseDoubleClicked = false; }
	void clearMouseScrolled() { mouseScrolled = false; }
	bool wasMouseClicked() const { return mouseClicked; }
	bool wasMouseDoubleClicked() const { return mouseDoubleClicked; }
	bool wasMouseMoved() const { return mouseMoved; }
	bool isMousePressed() const { return mousePressed; }
	bool wasMouseReleased() const { return mouseReleased; }
	bool wasMouseScrolled() const { return mouseScrolled; }
	char getKeyboardChar() const { return keyboardChar; }

	void MinWidth(unsigned width_) { size.x = width_; }
	void MinHeight(unsigned height_) { size.y = height_; }
	void Framerate(unsigned framerate_) { framerate = std::min(std::max(framerate_, 30u), 60u); }
	void MousePosition(const sf::Vector2f& mouse_) { mousePosition = mouse_; }
	void SmoothScreen(bool smooth_);
	void StretchToFit(bool stretchToFit_);
	void KeepAR(bool keepAR_);
	void PauseOnFocusLoss(bool pause_) { pauseOnFocusLoss = pause_; }

	unsigned MusicVolume() const { return musicVolume; }
	void MusicVolume(unsigned volume)
	{
		musicVolume <= 100 ? musicVolume = volume : musicVolume = 100;
	}
	unsigned SoundVolume() const { return soundVolume; }
	void SoundVolume(unsigned volume)
	{
		soundVolume <= 100 ? soundVolume = volume : soundVolume = 100;
	}

	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	const sf::Time& getElapsedTime() { return elapsedTime; }

	const std::string& getPath() const { return path; }
	const std::string& getTitle() const { return title; }
	const std::string& getVersion() const { return version; }

	void enableInput(bool enable) { disableInput = !enable; }

	sf::RenderWindow& Window() { return window; }
	const sf::RenderWindow& Window() const { return window; }

	const sf::RenderTexture& WindowTex() const { return windowTex; }

	ResourceManager& Resources() { return resourceManager; }
	const ResourceManager& Resources() const { return resourceManager; }
	EventManager& Events() { return eventManager; }

	void setPath(const std::string& path_) { path = path_; }
	void setTitle(const std::string& title_)
	{
		title = title_;
		if (window.isOpen() == true)
		{
			window.setTitle(title);
		}
	}
	void setVersion(const std::string& version_) { version = version_; }

	LoadingScreen* getLoadingScreen() { return loadingScreen.get(); }
	void setLoadingScreen(std::unique_ptr<LoadingScreen> loadingScreen_) { loadingScreen = std::move(loadingScreen_); }

	Rectangle* getFadeInOut() { return fadeInOut.get(); }
	void setFadeInOut(std::unique_ptr<Rectangle> fadeInOut_) { fadeInOut = std::move(fadeInOut_); }

	void updateMouse();

	void play();

	const std::map<std::string, Variable>& getVariables() { return variables; }

	template <class T, class U>
	U getVariable(const Variable& var, U defVal = U())
	{
		if (var.is<T>() == true)
		{
			return (U)var.get<T>();
		}
		else if (var.is<std::string>() == true)
		{
			auto var2 = getVariable(var.get<std::string>());
			if (var2.is<T>() == true)
			{
				return (U)var2.get<T>();
			}
		}
		return defVal;
	}

	Variable getVariable(const std::string& key) const;
	bool getVariableBool(const std::string& key) const;
	double getVariableDouble(const std::string& key) const;
	int64_t getVariableLong(const std::string& key) const;
	std::string getVariableString(const std::string& key) const;

	void clearVariable(const std::string& key);

	void setVariable(const std::string& key, const Variable& value);

	void saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const;

	bool drawLoadingScreen();

	Variable getProperty(const std::string& prop) const;
	void setProperty(const std::string& prop, const Variable& val);
};
