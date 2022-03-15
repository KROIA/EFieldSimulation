#pragma once


#include <string>
#include "drawable.h"

using std::string;

class TextPainter : public Drawable
{
	public:
	enum Origin
	{
		topLeft,
		topRight,
		bottomLeft,
		bottomRight,
		center
	};

	TextPainter(const string &fontPath = "C:\\Windows\\Fonts\\Arial.ttf");
	~TextPainter();

	void setText(const string& text);
	const string &getText() const;

	void setColor(const sf::Color& color);
	const sf::Color& getColor() const;

	void setSize(unsigned int size);
	unsigned int getSize() const;

	void setOrigin(Origin origin);
	Origin getOrigin() const;

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));
	virtual void drawDebug(sf::RenderWindow* window,
						   const sf::Vector2f& offset = sf::Vector2f(0, 0));
	protected:
	
	private:
	void updateOrigin();


	sf::Text m_text;
	static sf::Font *m_font;
	Origin m_origin;
};