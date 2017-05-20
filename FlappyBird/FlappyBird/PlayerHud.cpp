#include "stdafx.h"
#include "PlayerHud.h"


PlayerHud::PlayerHud()
{
}


PlayerHud::~PlayerHud()
{
}

void PlayerHud::setMessageComponent(std::shared_ptr<TextComponent> messageComponent)
{
	mGameMessage = messageComponent;
}

void PlayerHud::setScoreComponent(std::shared_ptr<TextComponent> scoreComponent)
{
	mScoreText = scoreComponent;
}

void PlayerHud::setMessage(const std::string& message)
{
	mGameMessage->setText(message);
}

void PlayerHud::setScore(const std::string& score)
{
	mScoreText->setText(score);
}

void PlayerHud::show(bool visible)
{
	mGameObject->setEnabled(visible);
}