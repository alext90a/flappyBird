#pragma once

enum class GEOMETRY
{
	POLY_1X1,
	POLY_1X2,
	POLY_1X3,
	POLY_1X4,
	POLY_1X6,
	POLY_10X1,
	BUTTON
};

class GeometryManager
{
public:
	
	GeometryManager();
	~GeometryManager();

	std::shared_ptr<Geometry> getGeometry(GEOMETRY type, LPDIRECT3DDEVICE9 device);
	void clean();

protected:
	std::map<GEOMETRY, std::shared_ptr<Geometry>> mGeometryCreated;
};

