#pragma once

// WINDOWS SPECIFIC STUFF

// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
// The following #defines disable a bunch of unused windows stuff. If you 
// get weird errors when trying to do some windows stuff, try removing some
// (or all) of these defines (it will increase build time though).
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <Windows.h>
#include <wrl.h>
//

//DirectX11 Stuff
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

// STL Stuff
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>

#include <memory>
#include <comdef.h>
#include <functional>
#include <chrono>

//CUSTOM INCLUDES
#include "engine/essentials/error.h"
#include "engine/essentials/timer.h"
#include "engine/essentials/log.h"

//Events
#include "engine/events/event.h"
#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "engine/events/mouseevent.h"

//Layer
#include "engine/layer/layerstack.h"

//KeyCodes
#include "engine/input/keyboardkeycodes.h"
#include "engine/input/mousekeycodes.h"