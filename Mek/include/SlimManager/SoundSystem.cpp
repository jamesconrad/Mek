#include "SoundSystem.h"
void SoundSystem::Init(){
	int i = 0;
	
	m_systemVel = { 0.0, 0.0, 0.0 };
	std::cout << i << m_systemVel.x;
	m_systemPos = { 0.0, 0.0, 0.0 };
	m_systemForward = { 0.0, 0.0, 1.0 };
	m_systemUp = { 0.0, 1.0, 0.0 };
	m_name = NULL;

	m_result = FMOD::System_Create(&m_SystemPtr);
	if (m_result != FMOD_OK){
		std::cout << "Failed to create system! Error: "; ERRCHECK(m_result); std::cout << std::endl;
	}

	m_result = m_SystemPtr->getNumDrivers(&m_numberDrivers);
	if (m_result != FMOD_OK){
		std::cout << "Failed to get number of drivers! Error: "; ERRCHECK(m_result); std::cout << std::endl;
	}

	m_result = m_SystemPtr->getVersion(&m_version);
	if (m_result != FMOD_OK){
		std::cout << "Failed to get system version! Error: "; ERRCHECK(m_result); std::cout << std::endl;
	}

	if (m_numberDrivers == 0){
		m_result = m_SystemPtr->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (m_result != FMOD_OK){
			std::cout << "Failed to set system output to no sound! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}
	}
	else{
		//retrieves current system's speakermode;
		m_result = m_SystemPtr->getDriverCaps(0, &m_caps, 0, &m_speakermode);
		if (m_result != FMOD_OK){
			std::cout << "Failed to get speakermode! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}
		//sets speakermode to current system speakermode
		m_result = m_SystemPtr->setSpeakerMode(m_speakermode);
		if (m_result != FMOD_OK){
			std::cout << "Failed to set speakermode! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}

		if (m_caps & FMOD_CAPS_HARDWARE_EMULATED){
			m_result = m_SystemPtr->setDSPBufferSize(1024, 10);
			if (m_result != FMOD_OK){
				std::cout << "Failed to set DSP Buffer Size! Error: "; ERRCHECK(m_result); std::cout << std::endl;
			}
		}

		m_result = m_SystemPtr->getDriverInfo(0, m_name, 256, 0);
		if (m_result != FMOD_OK){
			std::cout << "Failed to get Driver Info! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}
	}

	m_result = m_SystemPtr->init(1000, FMOD_INIT_NORMAL, 0);
	//if speakermode is not supported by the system switch it to stereo
	if (m_result == FMOD_ERR_OUTPUT_CREATEBUFFER){
		m_result = m_SystemPtr->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (m_result != FMOD_OK){
			std::cout << "Failed to set Speakermode to Stereo! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}

		m_result = m_SystemPtr->init(100, FMOD_INIT_NORMAL, 0);
		if (m_result != FMOD_OK){
			std::cout << "Failed to Initialize System Stereo! Error: "; ERRCHECK(m_result); std::cout << std::endl;
		}
	}

	m_result = m_SystemPtr->set3DSettings(1, 1, 1);
	if (m_result != FMOD_OK){
		std::cout << "Failed to set 3D Settings! Error: "; ERRCHECK(m_result); std::cout << std::endl;
	}
}
void SoundSystem::GetChannelsPlaying(){
	system("cls");
	int channels, cpu;
	float dsp, stream, geometry, update, total;
	m_SystemPtr->getChannelsPlaying(&channels);
	m_SystemPtr->getCPUUsage(&dsp, &stream, &geometry, &update, &total);
	std::cout << "Channels: " << channels << " DSP: " << dsp << " Stream: " << stream << " Update: " << update << " Total: " << total << std::endl << std::flush;
}
SoundSystem::~SoundSystem(){
	delete m_name;
	m_result = m_SystemPtr->close();
	if (m_result != FMOD_OK){
		std::cout << "Failed to Close System: "; ERRCHECK(m_result); std::cout << std::endl;
	}
	m_result = m_SystemPtr->release();
	if (m_result != FMOD_OK){
		std::cout << "Failed to Release System: "; ERRCHECK(m_result); std::cout << std::endl;
	}
	delete m_SystemPtr;
}
void SoundSystem::Update(){
	m_result = m_SystemPtr->set3DListenerAttributes(0, &m_systemPos, &m_systemVel, &m_systemForward, &m_systemUp); //up=
	if (m_result != FMOD_OK){
		std::cout << "Failed to set 3D Listerner Attributes in System: "; ERRCHECK(m_result); std::cout << std::endl;
	}
	m_SystemPtr->update();
}

