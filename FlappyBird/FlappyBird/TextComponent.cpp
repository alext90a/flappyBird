#include "stdafx.h"
#include "TextComponent.h"


TextComponent::TextComponent()
{
	mColor = D3DCOLOR_RGBA(255, 255, 255, 255);
}


TextComponent::~TextComponent()
{
}

void TextComponent::init(LPD3DXFONT* font)
{

	mFont = font;


}

void TextComponent::draw()
{
	(*mFont)->DrawTextA(NULL,
		mText.c_str(),
		strlen(mText.c_str()),
		&mTextRect,
		DT_LEFT | DT_TOP,
		mColor);
}

void TextComponent::setPos(int screenLeftX, int screenTopY, int screenRightX, int screenBottomY)
{
	SetRect(&mTextRect, screenLeftX, screenTopY, screenRightX, screenBottomY);
}

void TextComponent::setText(const std::string& text)
{
	mText = text;
}

void TextComponent::setColor(const D3DCOLOR& color)
{
	mColor = color;
}

const std::string& TextComponent::getText()const
{
	return mText;
}
