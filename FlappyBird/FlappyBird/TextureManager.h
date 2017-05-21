#pragma once
#include "stdafx.h"
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	void init(LPDIRECT3DDEVICE9 device);
	std::shared_ptr<Texture> getTexture(const std::string& texturePath);
	void clean();
protected:
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
	std::map <std::string, std::shared_ptr<Texture>> mUsedTexture;
};

