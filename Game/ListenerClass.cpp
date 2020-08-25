#include "ListenerClass.h"
#include "ItemBody.h"
#include "Bullet.h"
#include "UserData.h"
void ListenerClass::BeginContact(b2Contact* contact)
{
	b2Fixture* f1 = contact->GetFixtureA();
	b2Fixture* f2 = contact->GetFixtureB();

	b2Body* b1 = f1->GetBody();
	b2Body* b2 = f2->GetBody();

	UserData* user1 = (UserData*)b1->GetUserData();
	UserData* user2 = (UserData*)b2->GetUserData();
	
	
	if (user1 && user2) {
		user1->IsCollison ++;
		user1->m_typeB = user2->m_type;
		user1->m_receiveDamage = user2->m_damage;
		user2->IsCollison ++;
		user2->m_typeB = user1->m_type;
		user2->m_receiveDamage = user1->m_damage;
	}
}

void ListenerClass::EndContact(b2Contact* contact)
{
	b2Fixture* f1 = contact->GetFixtureA();
	b2Fixture* f2 = contact->GetFixtureB();

	b2Body* b1 = f1->GetBody();
	b2Body* b2 = f2->GetBody();

	UserData* user1 = (UserData*)b1->GetUserData();
	UserData* user2 = (UserData*)b2->GetUserData();


	if (user1 && user2) {
		user1->IsCollison --;
		user1->m_typeB = user1->m_type;
		user1->m_receiveDamage = 0;
		user2->IsCollison --;
		user2->m_typeB = user2->m_type;
		user2->m_receiveDamage = 0;
	}
}
