#include "textPainter.h"

sf::Font* TextPainter::m_font = nullptr;
TextPainter::TextPainter(const string& fontPath)
	: m_origin(Origin::topLeft)
{
	if (!m_font)
	{
		m_font = new sf::Font;
		if (!m_font->loadFromFile(fontPath))
		{
			PRINT_ERROR("Can't load font: " + fontPath)
		}
	}

	m_text.setFont(*m_font);
	setSize(20);
	setText("");
}
TextPainter::~TextPainter()
{

}

void TextPainter::setText(const string& text)
{
	m_text.setString(text);
	updateOrigin();
}
const string &TextPainter::getText() const
{
	return m_text.getString();
}

void TextPainter::setColor(const sf::Color& color)
{
	m_text.setFillColor(color);
	m_text.setOutlineColor(color);
}
const sf::Color& TextPainter::getColor() const
{
	return m_text.getFillColor();
}

void TextPainter::setSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}
unsigned int TextPainter::getSize() const
{
	return m_text.getCharacterSize();
}

void TextPainter::setOrigin(Origin origin)
{
	m_origin = origin;
}
TextPainter::Origin TextPainter::getOrigin() const
{
	return m_origin;
}

void TextPainter::updateOrigin()
{
	sf::FloatRect rect = m_text.getGlobalBounds();
	sf::Vector2f originPos = m_text.getOrigin();
	switch (m_origin)
	{
		case Origin::topLeft:
		{
			originPos.x = 0;
			originPos.y = 0;
			break;
		}
		case Origin::topRight:
		{
			originPos.x = rect.width;
			originPos.y = 0;
			break;
		}
		case Origin::bottomLeft:
		{
			originPos.x = 0;
			originPos.y = rect.height;
			break;
		}
		case Origin::bottomRight:
		{
			originPos.x = rect.width;
			originPos.y = rect.height;
			break;
		}
		case Origin::center:
		{
			originPos.x = rect.width / 2.f;
			originPos.y = rect.height / 2.f;
			break;
		}
	}
	m_text.setOrigin(originPos);
}

void TextPainter::draw(sf::RenderWindow* window,
					   const sf::Vector2f& offset)
{
	m_text.setPosition(m_pos + offset);
	window->draw(m_text);
}
void TextPainter::drawDebug(sf::RenderWindow* window,
							const sf::Vector2f& offset)
{
	sf::FloatRect rect = m_text.getGlobalBounds();
	sf::RectangleShape bounds(sf::Vector2f(rect.width,rect.height));
	bounds.setFillColor(sf::Color(0, 0, 0, 0));
	bounds.setOutlineColor(m_text.getFillColor());
	bounds.setPosition(m_pos + offset - m_text.getOrigin());
	window->draw(bounds);
}