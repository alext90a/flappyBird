#pragma once
#include "stdafx.h"



class Renderable : public IComponent
{
public:



	Renderable();
	~Renderable();

	HRESULT init(std::shared_ptr<Geometry> geometry, std::shared_ptr<Texture> texture);
	void draw()override;


protected:
	
	std::shared_ptr<Geometry> mGeometry = nullptr;
	std::shared_ptr<Texture> mTexture = nullptr;
	
};

