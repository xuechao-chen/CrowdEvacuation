#ifdef _DEBUG
#define CROWD_SIMULATION_DLL_EXPORT
#else
#ifdef CROWDSIMULATION_EXPORTS
#define CROWD_SIMULATION_DLL_EXPORT __declspec(dllexport)
#else
#define CROWD_SIMULATION_DLL_EXPORT __declspec(dllimport)
#endif
#endif // _DEBUG