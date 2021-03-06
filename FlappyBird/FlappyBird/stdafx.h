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
#include <unordered_map>
#include <set>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <list>
#include <unordered_set>
#include <functional>
#include <Mmsystem.h>
#include <windowsx.h>
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
#include "Renderable.h"
#include "Button.h"
#include "TextComponent.h"
#include "HighscoreLine.h"
#include "HighScoreDialog.h"
#include "PlayerBird.h"
#include "BackgroundController.h"
#include "GroundObjectController.h"
#include "PlayerHud.h"
#include "Camera.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Game.h"
#include "GameEngine.h"

// TODO: reference additional headers your program requires here
