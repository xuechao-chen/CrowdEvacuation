#ifdef CROWDSIMULATION_EXPORTS
#define CROWD_SIMULATION_DLL_EXPORT __declspec(dllexport)
#else
#define CROWD_SIMULATION_DLL_EXPORT __declspec(dllimport)
#endif