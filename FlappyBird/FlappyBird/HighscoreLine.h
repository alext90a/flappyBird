#pragma once
class TextComponent;
class HighscoreLine : public Component
{
public:
	HighscoreLine();
	~HighscoreLine();
	void setTextComponent(TextComponent* nameText, TextComponent* scoreText);
	void setValue(const std::string& name, int score);
	int getValue()const;
	const std::string& getName()const;
	void setColor(const D3DCOLOR& color);


protected:
	TextComponent* mNameTextComponent;
	TextComponent* mScoreTextComponent;
	int mScore;
};

