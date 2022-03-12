#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "drawable.h"




using std::string;
using std::vector;
class Display
{
	public:
	Display();
	Display(sf::Vector2u size, const string &title);
	~Display();

	void addDrawable(Drawable* obj);

	sf::Vector2i getRelaticeMousePos() const;

	

	void frameRateTarget(float fps);
	bool isOpen();
	void loop();
	void processEvents();
	bool needsFrameUpdate();
	void draw();

	

	protected:
	virtual void init(sf::Vector2u size, const string& title);
	

	private:
	sf::RenderWindow* m_window;
	sf::Vector2u m_windowSize;
	string m_windowTitle;

	bool m_exit;
	vector<Drawable*> m_drawableObjList;

	double m_targetFrameTimeMs;
	std::chrono::time_point<std::chrono::high_resolution_clock> *m_frameIntervalTime;
};
