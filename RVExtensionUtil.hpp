#pragma once
#include <cstdint>
#ifdef _WIN32
#include <windows.h> // GetModuleHandle, GetProcAddress
#else // Linux
#include <dlfcn.h> // dlopen, dlclose, dlsym
#endif

// bitset
enum DllExtensionFeatureFlags : uint64_t
{
	RVFeature_ContextArgumentsVoidPtr = 1 << 0,
	RVFeature_ContextStackTrace = 1 << 1,
	RVFeature_ContextNoDefaultCall = 1 << 2,
};

struct RVContext_StackTrace
{
	struct StackTraceLine
	{
		// Line number in file (before preprocessing if preprocessed with line numbers)
		uint32_t lineNumber;
		// File offset in bytes from the start of the file (after preprocessing)
		uint32_t fileOffset;
		// Filepath to the source file
		const char* sourceFile;
		// scopeName set on that level
		const char* scopeName;
		// Complete fileContent of the sourceFile (after preprocessing, can be combined with fileOffset to find exact location)
		const char* fileContent;
	};

	StackTraceLine* lines = nullptr;
	uint32_t lineCount = 0;
};


struct CExtensionControlInterface
{
	CExtensionControlInterface()
	{
		memset(this, 0, sizeof(CExtensionControlInterface));
	}

	uint32_t size = sizeof(CExtensionControlInterface);

	enum Flags : uint32_t
	{
		None = 0,
		DynamicTexture = 1 << 0, // If set we do not set render target, and instead pass a texture that can be mapped with D3D11_MAP_WRITE_DISCARD
		TextureFormatBGRA = 1 << 1 // If set the texture is BGRA, if not set its ARGB
	};
	Flags flags = Flags::None;

	// This is passed to every callback
	void* context;

	// [required]
	// Render into currently active target
	// target is a ID3D11RenderTargetView if Flags::DynamicTexture is not set
	// target is a ID3D11Texture2D if Flags::DynamicTexture is set
	void (*OnDraw)(void* context, float alpha, void* target);
	// [required]
	// Called after "Unload" display EH and "Destroy" control EH
	// This is the last call before the control is destroyed. Release all your resources
	void (*OnDestroy)(void* context, int code);

	// optional
	// Called before OnDraw, if the control (including backing render target) size has changed
	void (*OnSizeChanged)(void* context, unsigned int width, unsigned int height);

	// Interactions
	// Optional all of these

	// Return if you accept the focus
	bool (*OnSetFocus)(void* context, bool focus);

	void (*OnLButtonDown)(void* context, float x, float y);
	void (*OnLButtonUp)(void* context, float x, float y);
	void (*OnLButtonClick)(void* context, float x, float y);
	void (*OnLButtonDblClick)(void* context, float x, float y);
	void (*OnRButtonDown)(void* context, float x, float y);
	void (*OnRButtonUp)(void* context, float x, float y);
	void (*OnRButtonClick)(void* context, float x, float y);
	void (*OnMouseMove)(void* context, float x, float y);
	// Return if the input was handled, if not it will be passed to the parent controlsgroup/display
	bool (*OnMouseZChanged)(void* context, float dz);

	void (*OnMouseEnter)(void* context, float x, float y);
	void (*OnMouseExit)(void* context, float x, float y);

	// Return if the key was handled, if not it will be passed to the parent controlsgroup/display
	bool (*OnKeyDown)(void* context, int dikCode);
	bool (*OnKeyUp)(void* context, int dikCode);
	bool (*OnChar)(void* context, unsigned nChar, unsigned nRepCnt, unsigned nFlags);
};

#ifdef _D3D11_CONSTANTS // Only available with d3d11.h header
struct RVExtensionRenderInfo
{
	ID3D11Device*            d3dDevice;
	ID3D11DeviceContext*     d3dDeviceContext;
};

struct RVExtensionGraphicsLockGuard
{
	//! Call this when done with the lock, forgetting to call this will freeze the game.
	virtual void ReleaseLock() const = 0;
	virtual ~RVExtensionGraphicsLockGuard() = default;
};
#endif


#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport)
#define CALL_CONVENTION __stdcall
#else // Linux
#if __GNUC__ >= 4
#define DLLEXPORT __attribute__ ((visibility ("default")))
#else
#define DLLEXPORT __attribute__((dllexport))
#endif

#define CALL_CONVENTION
#endif

extern "C"
{
	typedef int (CALL_CONVENTION RVExtensionCallbackProc)(char const* name, char const* function, char const* data);
	typedef void CALL_CONVENTION RVExtensionRequestContextProc();

#ifdef _D3D11_CONSTANTS // Only available with d3d11.h header
	typedef RVExtensionGraphicsLockGuard* (CALL_CONVENTION RVExtensionGLockProc)();
	typedef void (CALL_CONVENTION RVExtensionGSetWHkProc)(bool (CALL_CONVENTION *newHook)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam));
#endif

	//--- Called by Engine on extension load 
	DLLEXPORT void CALL_CONVENTION RVExtensionVersion(char* output, unsigned int outputSize);
	//--- STRING callExtension STRING
	DLLEXPORT void CALL_CONVENTION RVExtension(char* output, unsigned int outputSize, const char* function);
	//--- STRING callExtension ARRAY
	DLLEXPORT int CALL_CONVENTION RVExtensionArgs(char* output, unsigned int outputSize, const char* function, const char** argv, unsigned int argc);
	//--- Extension Callback
	DLLEXPORT void CALL_CONVENTION RVExtensionRegisterCallback(RVExtensionCallbackProc* callbackProc);
	//--- Request creation of UI element
	DLLEXPORT bool CALL_CONVENTION RVExtensionRequestUI(const char* uiClass, CExtensionControlInterface* interfaceStruct);
}

inline const void* FindRVFunction(const char* name)
{
#ifdef _WIN32
	return reinterpret_cast<void*>(GetProcAddress(GetModuleHandle(nullptr), name));
#else // Linux
	auto armaHandle = dlopen(nullptr, RTLD_LAZY | RTLD_NOLOAD);
	auto result = dlsym(armaHandle, name);
	dlclose(armaHandle);
	return result;
#endif
}



