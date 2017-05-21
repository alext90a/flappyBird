#pragma once
class BackgroundController : public Component
{
public:
	BackgroundController();
	~BackgroundController();
	void insertBackgroundObject(std::shared_ptr<GameObject> backgroundObj);
	void setPlayer(std::shared_ptr<PlayerBird> player);
	void update()override;
protected:
	std::shared_ptr<GameObject> mBackgroundObject;
	std::shared_ptr<PlayerBird> mPlayer;
};

