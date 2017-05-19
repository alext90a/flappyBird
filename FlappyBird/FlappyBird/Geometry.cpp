#include "stdafx.h"
#include "Geometry.h"


Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

HRESULT Geometry::init(LPDIRECT3DDEVICE9 device, float width, float height)
{
	g_pd3dDevice = device;
	mTopLeft = D3DXVECTOR3(-width / 2.0f, height / 2.0f, 0.0f);
	mBottomRight = D3DXVECTOR3(width / 2.0f, -height / 2.0f, 0.0f);
	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 4, (void**)&pVertices, 0)))
		return E_FAIL;
	
	pVertices[0].position = D3DXVECTOR3(-1.0f * width/2.0f, -1.0f *height/2.0f, 0.0f);
	pVertices[0].color = 0xffffffff;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = height;

	pVertices[1].position = D3DXVECTOR3(-1.0f * width/2.0f, height/2.0f, 0.0f);
	pVertices[1].color = 0xffffffff;
	pVertices[1].tu = 0.0f;
	pVertices[1].tv = 0.0f;

	pVertices[2].position = D3DXVECTOR3(width/2.0f, -1.0f * height/2.0f, 0.0f);
	pVertices[2].color = 0xffffffff;
	pVertices[2].tu = width;
	pVertices[2].tv = height;

	pVertices[3].position = D3DXVECTOR3(width/2.0f, height/2.0f, 0.0f);
	pVertices[3].color = 0xffffffff;
	pVertices[3].tu = width;
	pVertices[3].tv = 0.0f;

	g_pVB->Unlock();

	VOID* pVoid;
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3

	};

	// create an index buffer interface called i_buffer
	if (FAILED(g_pd3dDevice->CreateIndexBuffer(6 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIB,
		NULL)))
	{
		return E_FAIL;
	}

	// lock i_buffer and load the indices into it
	g_pIB->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	g_pIB->Unlock();




	return S_OK;
}

D3DXVECTOR3 Geometry::getTopLeft()const
{
	return mTopLeft;
}

D3DXVECTOR3 Geometry::getBottomRight()const
{
	return mBottomRight;
}

void Geometry::draw()
{
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void Geometry::clean()
{
	if (g_pVB != NULL)
		g_pVB->Release();
	if (g_pIB != NULL)
		g_pIB->Release();
}
