#include "stdafx.h"
#include "TextComponent.h"


TextComponent::TextComponent()
{
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
		D3DCOLOR_RGBA(255, 1, 1, 255));
}

void TextComponent::setPos(int screenLeftX, int screenTopY, int screenRightX, int screenBottomY)
{
	SetRect(&mTextRect, screenLeftX, screenTopY, screenRightX, screenBottomY);
}

void TextComponent::setText(std::string text)
{
	mText = text;
}
