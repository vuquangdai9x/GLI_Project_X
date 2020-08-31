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
	void Player(int state);
	void Fire(int index);
	void Init();

	const static int P_GETITEM = 0;
	const static int P_CHANGE = 1;
	const static int P_INJUIRED = 2;
	const static int P_DIED = 3;

	const static int E_INJUIRED = 0;
	const static int E_DIED = 1;
};

