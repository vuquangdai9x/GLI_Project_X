#pragma once
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"
#include "Singleton.h"

class SoundManager
{
private:
	SoLoud::Soloud m_soloud;
	SoLoud::WavStream m_backGround;
	SoLoud::Wav m_bullet[4]; //Pistol, Canon, Shotgun, Supercannon;
	SoLoud::Wav m_player[4]; //GetItem, ChangeWeapon, Injured, Died;
	SoLoud::Wav m_enemy[2]; //Injured, Died; 

	int iHandleBackGround;
	int iHandleFire;
public:	
	SoundManager() {}
	~SoundManager() {
		m_soloud.deinit();
	}
	void Update(unsigned char key, bool bIsPressed);
	void Click();
	void ChangeWeapon();
	void Fire(int index);
	void Collistion();
	void Init();
};

