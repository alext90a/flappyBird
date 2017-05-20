#pragma once
class PlayerHud : public Component
{
public:
	PlayerHud();
	~PlayerHud();

	void setScoreComponent(std::shared_ptr<TextComponent> component);
	void setMessageComponent(std::shared_ptr<TextComponent> component);
	
	void setScore(const std::string& text);
	void setMessage(const std::string& text);
	void show(bool visible);

protected:
	std::shared_ptr<TextComponent> mScoreText = nullptr;
	std::shared_ptr<TextComponent> mGameMessage = nullptr;
};

