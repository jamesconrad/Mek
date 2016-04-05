
#include <time.h>
#include <vector>
#include "Manager.h"
#include "ConsoleMagic.h"
static double mps = 0.5;
static double pps = 0.1;

class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};

int main(){
	ConsoleMagic cm; //Create an instance of the ConsoleMagic class
	cm.Init(100, 50);//Resize the console window to 100 by 50 characters
	cm.SetTitle("Reverb Points");
	glm::vec3 pos{ 0.486725,0, 5.88874 };
	glm::vec3 spos{ 5.0f, 0, 14.0f };
	int key = 0;

	Manager* manager = new Manager;
	manager->Init();
	manager->GetSoundManager()->FindAndPlay("Background", "two");
	manager->GetSoundManager()->FindSound("Background", "two")->SetSoundPosition(spos);
	manager->GetSoundManager()->FindAndPlay("Background", "three");
	manager->GetSoundManager()->FindSound("Background", "three")->SetSoundPosition(pos);
	bool f = false;
	do
	{
		manager->Update();
		//manager->GetSoundManager()->FindSound("Background", "two")->PrintSoundInformation();
		//manager->DisplaySystemNode();
		if (_kbhit())
		{
			key = _getch();
			if (key == 'o'){
				spos = glm::vec3{ 25.0f, 0, 14.0f };
				pos = spos;
			}
			if (key == 'p'){
				spos = glm::vec3{10.3, 0, 13.0};
			}
			if (key == 'e'){
			
			}
			//Use WASD to move listener and sound
			if (key == 'w') {pos.z += 0.2f; }
			if (key == 's') {pos.z -= 0.2f; }
			if (key == 'a') {pos.x -= 0.2f; }
			if (key == 'd') {pos.x += 0.2f; }
			manager->GetSoundManager()->FindSound("Background", "two")->SetSoundPosition(spos);
			manager->SetSystemPosition(pos);
		}
		cm.Clear(' ', 0, 0); //Clear the console with ' ' on a black background
		//PrintFMODVector(FMOD_VECTOR{ spos.x, spos.y, spos.z });
		//Draw the sound Node
		for (int c = 0; c < manager->GetRNodeManager()->GetNodes().size(); c++){
			float x = (manager->GetRNodeManager()->GetNodes()[c]->GetRNodePos().x);
			float z = (manager->GetRNodeManager()->GetNodes()[c]->GetRNodePos().z);
			cm.SetCharacter(unsigned short(x + 0.5f), 49 - unsigned short(z + 0.5f), manager->GetRNodeManager()->GetNodes()[c]->GetRNodeName()[0]);
			cm.SetForegroundColor(unsigned short(x + 0.5f), 49 - unsigned short(z + 0.5f), 14);
		}
		cm.SetCharacter(unsigned short(spos.x + 0.5f), 49 - unsigned short(spos.z + 0.5f), 'S');
		cm.SetForegroundColor(unsigned short(spos.x + 0.5f), 49 - unsigned short(spos.z + 0.5f), 14);
		//Draw the listener
		float x = (manager->GetSoundSystem()->GetSystemPosition().x);
		float z = (manager->GetSoundSystem()->GetSystemPosition().z);
		cm.SetCharacter(unsigned short(x + 0.5f), 49 - unsigned short(z + 0.5f), 'l');
		cm.SetForegroundColor(unsigned short(x + 0.5f), 49 - unsigned short(z + 0.5f), 15);
		
		cm.Update(); //Display the new scene
		
		//must call this once every loop through the game loop
		
	} while (key != 27);
	manager->GetRNodeManager()->SetDestructAll(true);
	return 0;
}