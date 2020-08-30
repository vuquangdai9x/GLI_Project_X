#include "MapBorder.h"
#include "SceneManager2D.h"
#include "WorldManager.h"
#include"UserData.h"

MapBorder::MapBorder(int id, Player* player, float damage) : SpriteLoopTexture(id)
{
	m_player = player;
	m_damage = damage;
}
MapBorder::~MapBorder() {
}
void MapBorder::Update(float deltaTime) {
	SpriteLoopTexture::Update(deltaTime);
	Vector2 position(m_position.x, m_player->GetPosition().y);
	SetPosition(Vector3(position.x, position.y, m_position.z));
	m_obstacleBody->body->SetTransform(b2Vec2(position.x, position.y), 0);
}
void MapBorder::createCollider() {
	m_obstacleBody = Singleton<WorldManager>::GetInstance()->createRectagle(MAP_BORDER, m_position.x, m_position.y, 0.95 * m_originSize.x * this->GetScale().x, m_originSize.y * this->GetScale().y);
	UserData* user = (UserData*)m_obstacleBody->body->GetUserData();
	user->m_damage = m_damage;
}