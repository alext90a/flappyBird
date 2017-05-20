#include "stdafx.h"
#include "HighscoreLine.h"


HighscoreLine::HighscoreLine()
{
}


HighscoreLine::~HighscoreLine()
{
}

void HighscoreLine::setTextComponent(TextComponent* nameText, TextComponent* scoreText)
{
	mNameTextComponent = nameText;
	mScoreTextComponent = scoreText;
}

void HighscoreLine::setValue(const std::string& name, int value)
{
	mScore = value;
	mNameTextComponent->setText(name);
	mScoreTextComponent->setText(std::to_string(value));
}

int HighscoreLine::getValue()const
{
	return mScore;
}

void HighscoreLine::setColor(const D3DCOLOR& color)
{
	mNameTextComponent->setColor(color);
	mScoreTextComponent->setColor(color);
}

const std::string& HighscoreLine::getName()const
{
	return mNameTextComponent->getText();
}
