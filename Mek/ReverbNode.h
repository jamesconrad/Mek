#include "SoundSystem.h"

class ReverbNode{
public:
	FMOD::Reverb *freverb;
	FMOD_REVERB_PROPERTIES props;
	FMOD_VECTOR reverbPos;
	FMOD_VECTOR pos;
	FMOD::System* fsystem;
	ReverbNode();
	ReverbNode(FMOD::System* _fsystem,FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max);
	~ReverbNode();

};