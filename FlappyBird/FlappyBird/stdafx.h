// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <list>
#include <unordered_set>
#include <functional>
#include <Mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include "Component.h"
#include "Geometry.h"
#include "GeometryManager.h"
#include "GameConstants.h"
#include "BoundingBox.h"
#include "Texture.h"
#include "Sprite.h"

#include "Bird.h"

#include "TextureManager.h"
#include "GameObject.h"
#include "Game.h"

// TODO: reference additional headers your program requires here
