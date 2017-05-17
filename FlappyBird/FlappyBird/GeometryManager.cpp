#include "stdafx.h"
#include "GeometryManager.h"


GeometryManager::GeometryManager()
{
}


GeometryManager::~GeometryManager()
{
}

std::shared_ptr<Geometry> GeometryManager::getGeometry(GEOMETRY type, LPDIRECT3DDEVICE9 device)
{
	auto findedIter = mGeometryCreated.find(type);
	if (findedIter != mGeometryCreated.end())
	{
		return findedIter->second;
	}

	std::shared_ptr<Geometry> geo = std::make_shared<Geometry>();
	geo->init(device);
	mGeometryCreated.insert(std::make_pair(type, geo));
	return geo;
}

void GeometryManager::clean()
{
	for (auto curGeo : mGeometryCreated)
	{
		curGeo.second->clean();
	}
	mGeometryCreated.clear();
}
