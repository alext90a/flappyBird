#pragma once
class BackgroundController : public Component
{
public:
	BackgroundController();
	~BackgroundController();
	void insertBackgroundObject(std::shared_ptr<GameObject> backgroundObj);
	void setPlayer(std::shared_ptr<Bird> player);
	void setStartPos();
	void update()override;
protected:
	std::vector<std::shared_ptr<GameObject>> mBackgroundObjects;
	std::shared_ptr<Bird> mPlayer;
};

