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

	std::shared_ptr<Geometry> geo(new Geometry());
	if (type == GEOMETRY::POLY_1X1)
	{
		geo->init(device, 1, 1);
	}
	else if(type == GEOMETRY::POLY_1X2)
	{
		geo->init(device, 1, 2);
	}
	else if (type == GEOMETRY::POLY_1X3)
	{
		geo->init(device, 1, 3);
	}
	else if (type == GEOMETRY::POLY_1X4)
	{
		geo->init(device, 1, 4);
	}
	else if (type == GEOMETRY::POLY_1X6)
	{
		geo->init(device, 1, 6);
	}
	else if (type == GEOMETRY::BUTTON)
	{
		geo->init(device, 3, 1);
	}
	
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
