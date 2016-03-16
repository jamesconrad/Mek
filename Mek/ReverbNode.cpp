#include "ReverbNode.h"
ReverbNode::ReverbNode(){
	freverb = NULL;
}
ReverbNode::ReverbNode(FMOD::System* _fsystem,FMOD_REVERB_PROPERTIES _props, float _x, float _y, float _z, float _min, float _max){
	//
	//fsystem->createReverb(&freverb);
	//FMOD::Reverb *reverb;
	//this->props = props;
	//FMOD_REVERB_PROPERTIES reverbProps = props;
	//freverb->setProperties(&props);
	//pos = { _x, _y, _z };
	//reverb->set3DAttributes(&pos, _min, _max);
	//reverb->setActive(true);
	fsystem = _fsystem;
	fsystem->createReverb(&freverb);
	freverb->setProperties(&_props);
	this->pos.x = _x;
	this->pos.y = _y;
	this->pos.z = _z;
	freverb->set3DAttributes(&pos, _min, _max);
	freverb->setActive(true);
}
ReverbNode::~ReverbNode(){
	if (freverb)freverb->release();
}