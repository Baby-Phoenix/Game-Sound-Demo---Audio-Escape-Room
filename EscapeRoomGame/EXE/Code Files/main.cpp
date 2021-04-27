#include <glfw3.h>
#include <iostream>

#include "Input.h"
#include "AudioEngine.h"

void UpdateInput()
{
	/*if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{

	}*/
}

int main() {
	bool isEnd = false;
	bool isGameDone = false;
	

	//Setup FMOD
	AudioEngine& engine = AudioEngine::Instance();
	engine.Init();
	//Load Bank
	engine.LoadBank("Master");
	//Load Bus
	engine.LoadBus("Voice Over", "{90757eb7-c982-4e3d-b26f-1242732b93f4}");
	AudioBus& musicBus = engine.GetBus("Voice Over");
	musicBus.SetVolume(0.5);
	engine.LoadBus("Equip", "{aa195e69-5138-46cc-a43c-6c1a9d8c007a}");

	//Create events
	AudioEvent& IntroEvent = engine.CreateAudioEvent("Intro", "{94e910fa-d4c4-4877-b384-19d41ca75e03}");
	bool isIntroEventDone = false;
	AudioEvent& ScenarioDescEvent = engine.CreateAudioEvent("Scenario Description", "{3ed3bac1-db4e-4fcc-bef8-6e7e6cdb44f4}");
	bool isScenarioDescEventDone = false;
	AudioEvent& LookForwardEvent = engine.CreateAudioEvent("Look Forward", "{04f2fd2c-3e95-4a76-97da-9ea8bb2f7a0f}");
	//bool isLookForwardEventDone = false;
	AudioEvent& LookUpEvent = engine.CreateAudioEvent("Look Up", "{0b1ac40f-1e84-4d9e-b84c-0e8afd1b503b}");
	//bool isLookUpEventDone = false;
	AudioEvent& LookDownEvent = engine.CreateAudioEvent("Look Down", "{187cc751-5ffe-469f-92e0-6767962f2b2e}");
	//bool isLookDownEventDone = false;
	AudioEvent& LookLeftEvent = engine.CreateAudioEvent("Look Left", "{b21badca-9fee-42f1-83ad-84cdd1bb7fcc}");
	//bool isLookLeftEventDone = false;
	AudioEvent& LookLeftEvent2 = engine.CreateAudioEvent("Look Left2", "{9662b2ff-0155-4dc8-a4d8-81686cc7d0ec}");
	AudioEvent& LookRightEvent = engine.CreateAudioEvent("Look Right", "{370dbd38-5532-4b97-a97d-b660f774dc80}");
	//bool isLookRightEventDone = false;
	AudioEvent& InspectRockEvent = engine.CreateAudioEvent("Inspect Rock", "{a6e0d04f-47e6-4006-89c7-42f4c6278aad}");
	//bool isInspectRockEventDone = false;
	AudioEvent& GrappleEvent = engine.CreateAudioEvent("Use Grapple", "{ba095fd3-bad9-4eb2-bd00-c20560054a33}");
	//bool isGrappleEventDone = false;
	AudioEvent& BranchEvent = engine.CreateAudioEvent("Use Branch", "{534c9742-faa8-4683-ab5f-8ca413d124fe}");
	bool isBranchEventDone = false;
	AudioEvent& SawEvent = engine.CreateAudioEvent("Use Saw", "{f2f3cac6-d0f4-436b-8abc-47d5c45cb307}");
	//bool isSawEventDone = false;
	AudioEvent& VineEvent = engine.CreateAudioEvent("Use Vines", "{fbe7aa82-88cb-40c8-9d09-0cb4a77b3b4a}");
	//bool isVineEventDone = false;

	AudioEvent& EquipSawEvent = engine.CreateAudioEvent("Equip Saw", "{c4f9d6ff-4458-4626-abe3-1ef5184e52d7}");
	AudioEvent& EquipVinesEvent = engine.CreateAudioEvent("Equip Vines", "{210a3a39-27f8-40ec-a635-e1d21a70764b}");
	AudioEvent& EquipGrappleEvent = engine.CreateAudioEvent("Equip Grapple", "{52d5eb6b-513e-445b-bb93-db6a0b28cb6a}");
	AudioEvent& EquipBranchEvent = engine.CreateAudioEvent("Equip Branch", "{14e82cac-7c47-44ef-b8f1-50cb3ffae5d8}");
	
	AudioEvent& GameOverEvent = engine.CreateAudioEvent("Thanks for Playing", "{9fdf9f43-e627-4732-a1ca-8299818478de}");

	bool isSawAvail = false;
	bool isVineAvail = false;
	bool isGrappleAvail = false;
	bool isBranchAvail = false;
	bool isRockInspected = false;
	bool isLookLeft1Done = false;

	enum class Direction { NONE, UP, DOWN, LEFT, RIGHT, FORWARD };
	Direction currentDir = Direction::NONE;

	enum class Equipped { NONE, SAW, VINE, GRAPPLE, BRANCH };
	Equipped currentEquip = Equipped::BRANCH;

	while (!isGameDone)
	{
		if (!isIntroEventDone)
		{
			IntroEvent.Play();
			isIntroEventDone = true;
		}
		else if (isIntroEventDone && !IntroEvent.isPlaying() && Input::GetKeyDown(Key::In::Enter))
		{
			ScenarioDescEvent.Play();	
			isScenarioDescEventDone = true;
		}
		else if (isScenarioDescEventDone && !ScenarioDescEvent.isPlaying())
		{
			//Look left (find Pocket saw for inventory slot 1)
			if (Input::GetKeyDown(Key::In::LeftArrow) && !LookRightEvent.isPlaying() && !LookUpEvent.isPlaying() && !LookDownEvent.isPlaying() && !LookForwardEvent.isPlaying() &&!SawEvent.isPlaying() && !VineEvent.isPlaying() && !GrappleEvent.isPlaying() && !BranchEvent.isPlaying())
			{
				if (!isLookLeft1Done)
				{
					LookLeftEvent.Play();
					currentDir = Direction::LEFT;
					isSawAvail = true;
					isLookLeft1Done = true;
				}
				else if(!LookLeftEvent.isPlaying())
				{
					LookLeftEvent2.Play();
					currentDir = Direction::LEFT;
				}

			}
			//Look right (Can use branch to escape and WIN)
			else if (Input::GetKeyDown(Key::In::RightArrow) && !LookLeftEvent2.isPlaying() && !LookLeftEvent.isPlaying() && !LookUpEvent.isPlaying() && !LookDownEvent.isPlaying() && !LookForwardEvent.isPlaying() && !SawEvent.isPlaying() && !VineEvent.isPlaying() && !GrappleEvent.isPlaying() && !BranchEvent.isPlaying())
			{
				LookRightEvent.Play();
				currentDir = Direction::RIGHT;
			}
			//Look up (Can use grapple from slot 3 to add branch to slot 4)
			else if (Input::GetKeyDown(Key::In::UpArrow) && !LookLeftEvent2.isPlaying() && !LookRightEvent.isPlaying() && !LookLeftEvent.isPlaying() && !LookDownEvent.isPlaying() && !LookForwardEvent.isPlaying() && !SawEvent.isPlaying() && !VineEvent.isPlaying() && !GrappleEvent.isPlaying() && !BranchEvent.isPlaying())
			{
				LookUpEvent.Play();
				currentDir = Direction::UP;
			}
			//Look down (Can inspect rock and use vines from slot 2 to create grapple for slot 3)
			else if (Input::GetKeyDown(Key::In::DownArrow) && !LookLeftEvent2.isPlaying() && !LookRightEvent.isPlaying() && !LookUpEvent.isPlaying() && !LookLeftEvent.isPlaying() && !LookForwardEvent.isPlaying() && !SawEvent.isPlaying() && !VineEvent.isPlaying() && !GrappleEvent.isPlaying() && !BranchEvent.isPlaying())
			{
				LookDownEvent.Play();
				currentDir = Direction::DOWN;
			}
			//Look forward (Can use saw from slot 1. Add vines to slot 2)
			else if (Input::GetKeyDown(Key::In::Space) && !LookLeftEvent2.isPlaying() && !LookRightEvent.isPlaying() && !LookUpEvent.isPlaying() && !LookDownEvent.isPlaying() && !LookLeftEvent.isPlaying() && !SawEvent.isPlaying() && !VineEvent.isPlaying() && !GrappleEvent.isPlaying() && !BranchEvent.isPlaying())
			{
				LookForwardEvent.Play();
				currentDir = Direction::FORWARD;
			}

			if (!LookLeftEvent.isPlaying() && !LookRightEvent.isPlaying() && !LookUpEvent.isPlaying() && !LookDownEvent.isPlaying() && !LookForwardEvent.isPlaying())
			{
				//Saw
				if (Input::GetKeyDown(Key::In::One) || Input::GetKeyDown(Key::In::NumPad1))
				{
					if (isSawAvail && !EquipVinesEvent.isPlaying() && !EquipGrappleEvent.isPlaying() && !EquipBranchEvent.isPlaying())
					{
						currentEquip = Equipped::SAW;
						EquipSawEvent.Play();
					}
				}
				//Vine
				else if (Input::GetKeyDown(Key::In::Two) || Input::GetKeyDown(Key::In::NumPad2))
				{
					if (isVineAvail && !EquipSawEvent.isPlaying() && !EquipGrappleEvent.isPlaying() && !EquipBranchEvent.isPlaying())
					{
						currentEquip = Equipped::VINE;
						EquipVinesEvent.Play();
					}
				}
				//Grapple
				else if (Input::GetKeyDown(Key::In::Three) || Input::GetKeyDown(Key::In::NumPad3))
				{
					if (isGrappleAvail && !EquipVinesEvent.isPlaying() && !EquipSawEvent.isPlaying() && !EquipBranchEvent.isPlaying())
					{
						currentEquip = Equipped::GRAPPLE;
						EquipGrappleEvent.Play();
					}
				}
				//Branch
				else if (Input::GetKeyDown(Key::In::Four) || Input::GetKeyDown(Key::In::NumPad4))
				{
					if (isBranchAvail && !EquipVinesEvent.isPlaying() && !EquipGrappleEvent.isPlaying() && !EquipSawEvent.isPlaying())
					{
						currentEquip = Equipped::BRANCH;
						EquipBranchEvent.Play();
					}
				}
			}

			//Inspection (rock)
			if (Input::GetKeyDown(Key::In::I) && !VineEvent.isPlaying())
			{
				if (currentDir == Direction::DOWN && !LookDownEvent.isPlaying())
				{
					InspectRockEvent.Play();
					isRockInspected = true;
				}
			}
			//Use tool
			if (Input::GetKeyDown(Key::In::U))
			{
				//Use Saw to get vine
				if (currentDir == Direction::FORWARD && currentEquip == Equipped::SAW && !LookForwardEvent.isPlaying())
				{
					SawEvent.Play();
					isVineAvail = true;
				}
				//Use vine to get grapple
				else if (currentDir == Direction::DOWN && currentEquip == Equipped::VINE && isRockInspected && !LookDownEvent.isPlaying() &&!InspectRockEvent.isPlaying())
				{
					VineEvent.Play();
					isGrappleAvail = true;
				}
				else if (currentDir == Direction::UP && currentEquip == Equipped::GRAPPLE && !LookUpEvent.isPlaying())
				{
					GrappleEvent.Play();
					isBranchAvail = true;
				}
				else if (currentDir == Direction::RIGHT && currentEquip == Equipped::BRANCH && !LookRightEvent.isPlaying())
				{
					BranchEvent.Play();
					isBranchEventDone = true;
					engine.Update();
				}

			}
		}
		
		engine.Update();	

		
		
		Input::ResetKeys();
	}

	return 0;
}