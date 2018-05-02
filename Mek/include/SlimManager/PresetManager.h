#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H
#include <fstream>
#include <ostream>
#include <sstream>
#include "SoundSystem.h"
class Preset{
public:
	Preset(){}
	void init(std::string _name, int Instance, int Environment, float EnvDiffusion, int Room, int RoomHF, int RoomLF, float DecayTime, float DecayHFRatio, float DecayLFRatio, int Reflections, float ReflectionsDelay, int Reverb, float ReverbDelay, float ModulationTime, float ModulationDepth, float HFReference, float LFReference, float Diffusion, float Density, unsigned int Flags);
	inline std::string GetPresetName(){ return m_presetName; }
	inline FMOD_REVERB_PROPERTIES GetReverbProperties(){ return m_props; }
	inline Preset* GetPreset(){ return this; }
	//dipslay functions
	void PrintPreset();
private:
	std::string m_presetName;
	FMOD_REVERB_PROPERTIES m_props;
};

class PresetManager{
public:
	//init functions
	PresetManager(){}
	void Init(std::string _filePath, std::string _fileName);
	//control functions
	Preset* FindPreset(std::string _presetName);
	//display
	void PrintAllPresets();

	inline std::vector<Preset*> GetPresets(){ return m_presets; }
	inline PresetManager* GetPresetManager(){ return this; }
	
private:
	std::vector<Preset*> m_presets;
};


#endif          