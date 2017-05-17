#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

void TextureManager::init(LPDIRECT3DDEVICE9 device)
{
	g_pd3dDevice = device;
}

void TextureManager::clean()
{
	for (auto curTex : mUsedTexture)
	{
		curTex.second->clean();
	}
}

std::shared_ptr<Texture> TextureManager::createTexture(std::string filePath)
{
	auto findedIter = mUsedTexture.find(filePath);
	if (findedIter != mUsedTexture.end())
	{
		return findedIter->second;
	}

	std::shared_ptr<Texture> createdTexture = std::make_shared<Texture>();
	if (createdTexture->create(g_pd3dDevice, filePath) == S_OK)
	{
		mUsedTexture.insert(std::make_pair(filePath, createdTexture));
	}
	else
	{
		return nullptr;
	}
	return createdTexture;
}
