#include "stdafx.h"
#include "SoundManager.h"


void SoundManager::Update(unsigned char key, bool bIsPressed)
{
	
}

void SoundManager::Click()
{
	m_soloud.play(m_player[0]);
}

void SoundManager::Player(int state)
{
	if (state == P_GETITEM) {
		m_soloud.play(m_player[0]);
	}
	else if (state == P_CHANGE) {
		m_soloud.play(m_player[1]);
	}
	else if (state == P_INJUIRED) {
		m_soloud.play(m_player[2]);
	}
	else if (state == P_DIED) {
		m_soloud.play(m_player[3]);
	}
}

void SoundManager::Fire(int index)
{
	m_soloud.stop(iHandleFire);
	//m_soloud.setSamplerate(iHandleFire, 0.75f);
	if (index == 0 | index == 3) {
		iHandleFire = m_soloud.play(m_bullet[0]);
	}
	else if (index == 1 | index == 2) {
		iHandleFire = m_soloud.play(m_bullet[2]);
	}
	else if (index == 4) {
		iHandleFire = m_soloud.play(m_bullet[1]);
	}
	else if (index == 5) {
		iHandleFire = m_soloud.play(m_bullet[3]);
	} 
}

void SoundManager::OnOffSound()
{
	SetOnOff *= -1;
	if (SetOnOff == 1) {
		m_soloud.setVolume(iHandleBackGround, 0.0f);
	}
	else if (SetOnOff == -1) {
		m_soloud.setVolume(iHandleBackGround, 2.0f);
	}
}

void SoundManager::Enemy(int state)
{
	if (state == E_INJUIRED) {
		m_soloud.play(m_enemy[0]);
	}
	else if (state == E_DIED) {
		m_soloud.play(m_enemy[1]);
	}
	else if (state == E_BUG) {
		iHandleEnemy = m_soloud.play(m_enemy[2]);
	}
}


void SoundManager::Init()
{
	m_soloud.init();

	m_backGround.load("../Soloud/lib_audio/background.wav");
	m_backGround.setLooping(1);
	iHandleBackGround=m_soloud.playBackground(m_backGround);
	m_soloud.setVolume(iHandleBackGround, 2.0f);

	m_bullet[0].load("../Soloud/lib_audio/pistol.wav");
	m_bullet[1].load("../Soloud/lib_audio/canon.wav");
	m_bullet[2].load("../Soloud/lib_audio/shotgun.wav");
	m_bullet[3].load("../Soloud/lib_audio/supercanon.wav");

	m_player[0].load("../Soloud/lib_audio/item.wav");
	m_player[1].load("../Soloud/lib_audio/doi_sung.wav");
	m_player[2].load("../Soloud/lib_audio/trung_dan.wav");
	m_player[3].load("../Soloud/lib_audio/endgame.wav");

	m_enemy[0].load("../Soloud/lib_audio/e_injuied.wav");
	m_enemy[1].load("../Soloud/lib_audio/kill.wav");
	m_enemy[2].load("../Soloud/lib_audio/bug.wav");

}
