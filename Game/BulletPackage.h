#pragma once
enum BulletType { Gun, Cannon };
class BulletPackage {
public:
	BulletPackage(BulletType bulletType, int iAmount);
	BulletType m_bulletType;
	int m_iAmount;
};