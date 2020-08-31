#include "MapBorder.h"
#include "SceneManager2D.h"
#include "WorldManager.h"
#include"UserData.h"

MapBorder::MapBorder(int id, Player* player, float damage) : SpriteLoopTexture(id)
{
	m_player = player;
	m_damage = damage;
	m_physicsBody = NULL;
}
MapBorder::~MapBorder() {
}
void MapBorder::Update(float deltaTime) {
	SpriteLoopTexture::Update(deltaTime);
	Vector2 position(m_position.x, m_player->GetPosition().y);
	SetPosition(Vector3(position.x, position.y, m_position.z));
	if (m_physicsBody != NULL)
		m_physicsBody->body->SetTransform(b2Vec2(position.x, position.y), 0);
}
void MapBorder::createCollider() {
	m_physicsBody = Singleton<WorldManager>::GetInstance()->createRectagle(MAP_BORDER, m_position.x, m_position.y, 0.85 * m_originSize.x * this->GetScale().x, m_originSize.y * this->GetScale().y);
	UserData* user = (UserData*)m_physicsBody->body->GetUserData();
	user->m_damage = m_damage;
}