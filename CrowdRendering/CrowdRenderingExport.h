#ifdef CROWDRENDERING_EXPORTS
#define CROWD_RENDERING_DLL_EXPORT __declspec(dllexport)
#else
#define CROWD_RENDERING_DLL_EXPORT __declspec(dllimport)
#endif