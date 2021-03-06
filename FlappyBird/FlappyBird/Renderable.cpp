#include "stdafx.h"


Renderable::Renderable()
{
}


Renderable::~Renderable()
{
	mGeometry.reset();
	mTexture.reset();
}

HRESULT Renderable::init(std::shared_ptr<Geometry> geometry, std::shared_ptr<Texture> texture)
{
	
	mTexture = texture;
	mGeometry = geometry;
	return S_OK;
}

void Renderable::draw()
{
	mTexture->draw();
	mGeometry->draw();
}






