#pragma once

#include "EventManager.h"
#include "FadeInOut.h"
#include "LoadingScreen.h"
#include <memory>
#include "Menu.h"
#include "Parser/ParseVariable.h"
#include "Queryable.h"
#include "ResourceManager.h"
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Variable.h"
#include <vector>

class Game : public sf::NonCopyable, public Queryable
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
	unsigned framerate{ 0 };
	bool smoothScreen{ false };
	bool stretchToFit{ false };
	bool keepAR{ true };
	bool enableInput{ true };
	bool pauseOnFocusLoss{ false };
	bool paused{ false };

	sf::Vector2i mousePositioni;
	sf::Vector2f mousePositionf;

	sf::Event::MouseButtonEvent mousePressEvt;
	sf::Event::MouseButtonEvent mouseReleaseEvt;
	sf::Event::MouseMoveEvent mouseMoveEvt;
	sf::Event::MouseWheelScrollEvent mouseScrollEvt;
	sf::Event::KeyEvent keyPressEvt;
	sf::Event::TextEvent textEnteredEvt;
	sf::Event::TouchEvent touchBeganEvt;
	sf::Event::TouchEvent touchMovedEvt;
	sf::Event::TouchEvent touchEndedEvt;
	bool mousePressed{ false };
	bool mouseReleased{ false };
	bool mouseMoved{ false };
	bool mouseScrolled{ false };
	bool keyPressed{ false };
	bool textEntered{ false };
	bool touchBegan{ false };
	bool touchMoved{ false };
	bool touchEnded{ false };

	unsigned musicVolume{ 100 };
	unsigned soundVolume{ 100 };

	sf::Time elapsedTime;

	std::string path;
	std::string title;
	std::string version;

	ResourceManager resourceManager;
	EventManager eventManager;

	std::map<std::string, Variable> variables;

	std::unique_ptr<LoadingScreen> loadingScreen;
	std::unique_ptr<FadeInOut> fadeInOut;

	void processEvents();
	void onClosed();
	void onResized(const sf::Event::SizeEvent& evt);
	void onLostFocus() noexcept;
	void onGainedFocus() noexcept;

	void onTextEntered(const sf::Event::TextEvent& evt) noexcept;
	void onKeyPressed(const sf::Event::KeyEvent& evt);
	void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& evt);
	void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
	void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);
	void onMouseMoved(const sf::Event::MouseMoveEvent& evt);
	void onTouchBegan(const sf::Event::TouchEvent& evt);
	void onTouchMoved(const sf::Event::TouchEvent& evt);
	void onTouchEnded(const sf::Event::TouchEvent& evt);

	void updateMouse(const sf::Vector2i mousePos);
	void updateEvents();
	void drawCursor();
	void drawUI();
	void update();
	void drawAndUpdate();
	void drawFadeEffect();
	void drawWindow();

	void updateSize();
	void updateWindowTex();

	std::map<std::string, Variable>::const_iterator findVariable(const std::string& key) const;

public:
	Game() noexcept : refSize(640, 480), minSize(640, 480), size(640, 480) {}
	~Game();

	void init();

	unsigned getOpenGLDepthBits() const { return window.getSettings().depthBits; }
	unsigned getOpenGLStencilBits() const { return window.getSettings().stencilBits; }
	unsigned getOpenGLAntialiasingLevel() const { return window.getSettings().antialiasingLevel; }
	unsigned getOpenGLMajorVersion() const { return window.getSettings().majorVersion; }
	unsigned getOpenGLMinorVersion() const { return window.getSettings().minorVersion; }
	bool getOpenGLSRgbCapable() const { return window.getSettings().sRgbCapable; }

	const sf::Vector2u& OldWindowSize() const noexcept { return oldSize; }
	const sf::Vector2u& WindowSize() const noexcept { return size; }
	const sf::Vector2u& WindowTexSize() const noexcept { return windowTexSize; }
	sf::Vector2f WindowTexSizef() const
	{
		return sf::Vector2f((float)windowTexSize.x, (float)windowTexSize.y);
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
	const sf::Vector2u& RefSize() const noexcept { return refSize; }
	void RefSize(const sf::Vector2u& size_)
	{
		if (window.isOpen() == false &&
			size_.x > minSize.x &&
			size_.y > minSize.y)
		{
			refSize = size_;
		}
	}
	const sf::Vector2u& MinSize() const noexcept { return minSize; }
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
	unsigned Framerate() const noexcept { return framerate; }
	bool SmoothScreen() const noexcept { return smoothScreen; }
	bool StretchToFit() const noexcept { return stretchToFit; }
	bool KeepAR() const noexcept { return keepAR; }

	const sf::Vector2i& MousePositioni() const noexcept { return mousePositioni; }
	const sf::Vector2f& MousePositionf() const noexcept { return mousePositionf; }

	const sf::Event::MouseButtonEvent& MousePress() const noexcept { return mousePressEvt; }
	const sf::Event::MouseButtonEvent& MouseRelease() const noexcept { return mouseReleaseEvt; }
	const sf::Event::MouseMoveEvent& MouseMove() const noexcept { return mouseMoveEvt; }
	const sf::Event::MouseWheelScrollEvent& MouseScroll() const noexcept { return mouseScrollEvt; }
	const sf::Event::KeyEvent& KeyPress() const noexcept { return keyPressEvt; }
	const sf::Event::TextEvent& TextEntered() const noexcept { return textEnteredEvt; }
	const sf::Event::TouchEvent& TouchBegan() const noexcept { return touchBeganEvt; }
	const sf::Event::TouchEvent& TouchMoved() const noexcept { return touchMovedEvt; }
	const sf::Event::TouchEvent& TouchEnded() const noexcept { return touchEndedEvt; }

	bool wasMousePressed() const noexcept { return mousePressed; }
	bool wasMouseReleased() const noexcept { return mouseReleased; }
	bool wasMouseMoved() const noexcept { return mouseMoved; }
	bool wasMouseScrolled() const noexcept { return mouseScrolled; }
	bool wasKeyPressed() const noexcept { return keyPressed; }
	bool wasTextEntered() const noexcept { return textEntered; }
	bool hasTouchBegan() const noexcept { return touchBegan; }
	bool hasTouchMoved() const noexcept { return touchMoved; }
	bool hasTouchEnded() const noexcept { return touchEnded; }

	void clearMousePressed() noexcept { mousePressed = false; }
	void clearMouseReleased() noexcept { mouseReleased = false; }
	void clearMouseMoved() noexcept { mouseMoved = false; }
	void clearMouseScrolled() noexcept { mouseScrolled = false; }
	void clearKeyPressed() noexcept { keyPressed = false; }
	void clearTextEntered() noexcept { textEntered = false; }
	void clearTouchBegan() noexcept { touchBegan = false; }
	void clearTouchMoved() noexcept { touchMoved = false; }
	void clearTouchEnded() noexcept { touchEnded = false; }

	void clearInputEvents() noexcept
	{
		mousePressed = false;
		mouseReleased = false;
		mouseMoved = false;
		mouseScrolled = false;
		keyPressed = false;
		textEntered = false;
		touchBegan = false;
		touchMoved = false;
		touchEnded = false;
	}

	void MinWidth(unsigned width_) noexcept { size.x = width_; }
	void MinHeight(unsigned height_) noexcept { size.y = height_; }
	void Framerate(unsigned framerate_)
	{
		if (framerate_ > 0)
		{
			framerate = std::clamp(framerate_, 30u, 60u);
		}
		else
		{
			framerate = 0;
		}
		if (window.isOpen() == true)
		{
			window.setFramerateLimit(framerate);
		}
	}
	void SmoothScreen(bool smooth_);
	void StretchToFit(bool stretchToFit_);
	void KeepAR(bool keepAR_);
	void PauseOnFocusLoss(bool pause_) noexcept { pauseOnFocusLoss = pause_; }

	unsigned MusicVolume() const noexcept { return musicVolume; }
	void MusicVolume(unsigned volume) noexcept
	{
		musicVolume = std::min(volume, 100u);
	}
	unsigned SoundVolume() const noexcept { return soundVolume; }
	void SoundVolume(unsigned volume) noexcept
	{
		soundVolume = std::min(volume, 100u);
	}

	void addPlayingSound(const sf::SoundBuffer& obj);
	void addPlayingSound(const sf::SoundBuffer* obj);

	const sf::Time& getElapsedTime() const noexcept { return elapsedTime; }

	const std::string& getPath() const noexcept { return path; }
	const std::string& getTitle() const noexcept { return title; }
	const std::string& getVersion() const noexcept { return version; }

	bool isInputEnabled() const noexcept { return enableInput; }
	void EnableInput(bool enable) noexcept { enableInput = enable; }

	sf::RenderWindow& Window() noexcept { return window; }
	const sf::RenderWindow& Window() const noexcept { return window; }

	const sf::RenderTexture& WindowTex() const noexcept { return windowTex; }

	ResourceManager& Resources() noexcept { return resourceManager; }
	const ResourceManager& Resources() const noexcept { return resourceManager; }
	EventManager& Events() noexcept { return eventManager; }

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

	LoadingScreen* getLoadingScreen() noexcept { return loadingScreen.get(); }
	void setLoadingScreen(std::unique_ptr<LoadingScreen> loadingScreen_) noexcept
	{
		loadingScreen = std::move(loadingScreen_);
	}

	void draw();
	bool drawLoadingScreen();

	FadeInOut* getFadeInOut() const noexcept { return fadeInOut.get(); }
	void setFadeInOut(std::unique_ptr<FadeInOut> fadeInOut_) noexcept
	{
		fadeInOut = std::move(fadeInOut_);
	}

	void updateMouse();
	void updateCursorPosition();

	void play();

	const std::map<std::string, Variable>& getVariables() const noexcept { return variables; }

	bool getVariableNoPercentage(const std::string& key, Variable& var) const;
	bool getVariable(const std::string& key, Variable& var) const;

	template <class T, class U>
	U getVarOrProp(const Variable& var, U defVal = U())
	{
		if (std::holds_alternative<T>(var) == true)
		{
			return (U)std::get<T>(var);
		}
		else if (std::holds_alternative<std::string>(var) == true)
		{
			Variable var2;
			if (getVarOrProp(std::get<std::string>(var), var2) == true)
			{
				if (std::holds_alternative<T>(var2) == true)
				{
					return (U)std::get<T>(var2);
				}
			}
		}
		return defVal;
	}

	bool getVarOrProp(const std::string& key, Variable& var) const;
	Variable getVarOrProp(const Variable& var) const;
	bool getVarOrPropBool(const std::string& key) const;
	bool getVarOrPropBool(const Variable& var) const;
	double getVarOrPropDouble(const std::string& key) const;
	double getVarOrPropDouble(const Variable& var) const;
	int64_t getVarOrPropLong(const std::string& key) const;
	int64_t getVarOrPropLong(const Variable& var) const;
	std::string getVarOrPropString(const std::string& key) const;
	std::string getVarOrPropString(const Variable& var) const;

	void clearVariable(const std::string& key);

	void setVariable(const std::string& key, const Variable& value);

	void saveVariables(const std::string& filePath, const std::vector<std::string>& varNames) const;

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	void setProperty(const std::string& prop, const Variable& val);

	virtual const Queryable* getQueryable(const std::string& prop) const;
};
