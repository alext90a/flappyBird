#pragma once
class TextComponent : public Component
{
public:
	TextComponent();
	~TextComponent();
	void init(LPD3DXFONT* font);
	void draw() override;
	void setText(const std::string& text);
	void setPos(int screenLeftX, int screenTopY, int screenRightX, int screenBottomY);
	void setColor(const D3DCOLOR& color);

protected:
	LPD3DXFONT*	mFont;
	RECT mTextRect;
	std::string mText;
	D3DCOLOR mColor;
};

