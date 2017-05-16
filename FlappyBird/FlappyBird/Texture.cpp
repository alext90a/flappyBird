#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

HRESULT Texture::create(LPDIRECT3DDEVICE9 device, std::string filePath)
{
	g_pd3dDevice = device;
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, filePath.c_str(), &g_pTexture)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, filePath.c_str(), &g_pTexture)))
		{
			MessageBox(NULL, "Could not find banana.bmp", "Textures.exe", MB_OK);
			return E_FAIL;
		}
	}
	return S_OK;
}

void Texture::draw()
{
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

void Texture::clean()
{
	if (g_pTexture != NULL)
		g_pTexture->Release();
}
