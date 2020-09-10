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
		
		if (user1->m_type == PLAYER) {
			user2->m_receiveDamage = 0;
			b2Vec2 pos1 = b1->GetPosition();
			b2Vec2 pos2 = b2->GetPosition();
			b2Vec2 rs = pos1 - pos2;
			rs.Normalize();
			rs *= 50;
			
			b1->ApplyLinearImpulseToCenter(rs, true);
		}
		else if (user2->m_type == PLAYER) { 
			user1->m_receiveDamage = 0;
			b2Vec2 pos1 = b1->GetPosition();
			b2Vec2 pos2 = b2->GetPosition();
			b2Vec2 rs = pos2 - pos1;
			rs.Normalize();
			rs *= 50;
			b2->ApplyLinearImpulseToCenter(rs, true);
		}
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
