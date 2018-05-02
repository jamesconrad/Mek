#include "PresetManager.h"
void PresetManager::Init(std::string _filePath, std::string _fileName){
	std::ifstream m_file;
	std::string name = _filePath + _fileName;//_filePath just the file path without filename, _filename = just fie name together = complete path
	m_file.open(((name).c_str())); //opens file for reading

	std::string output;
	std::string line;

	if (m_file.is_open())
	{
		//getline(file, line);
		while (m_file.good())
		{
			//system("CLS");
			getline(m_file, line);

			std::string str(line);//reads in a line from a file into a string
			std::string buf; // Have a buffer string
			std::stringstream ss(str); // Insert the string into a stream 

			std::vector<std::string> tokens; // Create vector to hold our words

			while (ss >> buf)
				tokens.push_back(buf);
			std::string  t_name = tokens[0];
			int          t_Instance = std::stoi(tokens[1], NULL);
			int          t_Environment = std::stoi(tokens[2], NULL);
			float        t_EnvDiffusion = (float)std::stod(tokens[3], NULL);
			int          t_Room = std::stoi(tokens[4], NULL);
			int          t_RoomHF = std::stoi(tokens[5], NULL);
			int          t_RoomLF = std::stoi(tokens[6], NULL);
			float        t_DecayTime = (float)std::stod(tokens[7], NULL);
			float        t_DecayHFRatio = (float)std::stod(tokens[8], NULL);
			float        t_DecayLFRatio = (float)std::stod(tokens[9], NULL);
			int          t_Reflections = std::stoi(tokens[10], NULL);
			float        t_ReflectionsD = (float)std::stod(tokens[11], NULL);
			int          t_Reverb = std::stod(tokens[12], NULL);
			float        t_ReverbDelay = (float)std::stod(tokens[13], NULL);
			float        t_ModulationTi = (float)std::stod(tokens[14], NULL);
			float        t_ModulationDe = (float)std::stod(tokens[15], NULL);
			float        t_HFReference = (float)std::stod(tokens[16], NULL);
			float        t_LFReference = (float)std::stod(tokens[17], NULL);
			float        t_Diffusion = (float)std::stod(tokens[18], NULL);
			float        t_Density = (float)std::stod(tokens[19], NULL);
			unsigned int t_Flags = std::stoi(tokens[20], NULL);

			Preset* preset = new Preset;
			preset->init(t_name, t_Instance, t_Environment, t_EnvDiffusion, t_Room, t_RoomHF, t_RoomLF, t_DecayTime,
				t_DecayHFRatio, t_DecayLFRatio, t_Reflections, t_ReflectionsD, t_Reverb, t_ReverbDelay,
				t_ModulationTi, t_ModulationDe, t_HFReference, t_LFReference, t_Diffusion, t_Density,
				t_Flags);
			m_presets.push_back(preset);

		}
	}
}
void PresetManager::PrintAllPresets(){
	for (int c = 0; c < m_presets.size(); c++){
		m_presets[c]->PrintPreset();
	}
}
Preset* PresetManager::FindPreset(std::string _presetName){
	for (int c = 0; c < m_presets.size(); c++){
		if (m_presets[c]->GetPresetName() == _presetName)
			return m_presets[c];
	}

	std::cout << "c:PM,f:FP Could not find preset " << _presetName << " WRONG/MISSING PRESETNAME/FILE" << std::endl;
}
void Preset::init(std::string _name,int Instance, int Environment, float EnvDiffusion, int Room, int RoomHF, int RoomLF, float DecayTime, float DecayHFRatio, float DecayLFRatio, int Reflections, float ReflectionsDelay, int Reverb, float ReverbDelay, float ModulationTime, float ModulationDepth, float HFReference, float LFReference, float Diffusion, float Density, unsigned int Flags){
	m_presetName = _name;
	m_props = { Instance, Environment, EnvDiffusion, Room, RoomHF, RoomLF, DecayTime, DecayHFRatio, DecayLFRatio, Reflections, ReflectionsDelay, Reverb, ReverbDelay, ModulationTime, ModulationDepth, HFReference, LFReference, Diffusion, Density, Flags };
}
void Preset::PrintPreset(){
	std::cout << m_presetName << " " << m_props.Instance << " " << m_props.Environment << " " << m_props.EnvDiffusion << " " << m_props.Room << " " << m_props.RoomHF << " " << m_props.RoomLF << " " << m_props.DecayTime << " " << m_props.DecayHFRatio << " " << m_props.DecayLFRatio << " " << m_props.Reflections << " " << m_props.ReflectionsDelay << " " << m_props.Reverb << " " << m_props.ReverbDelay << " " << m_props.ModulationTime << " " << m_props.ModulationDepth << " " << m_props.HFReference << " " << m_props.LFReference << " " << m_props.Diffusion << " " << m_props.Density << " " << m_props.Flags << std::endl;
}