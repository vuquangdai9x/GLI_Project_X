#include "DecorateObject.h"
#include "UserData.h"
#include "EffectManager.h"
#include "SceneManager2D.h"
#include "WorldManager.h"

DecorateObject::DecorateObject(int id): Sprite(id)
{
	m_iDamagedEffectId = -1;
	m_isDestroyed = false;
	m_physicsBody = NULL;
	m_colliderRatio = Vector2(1, 1);
}


DecorateObject::~DecorateObject()
{
}

DecorateObject * DecorateObject::CreateClone(int id)
{
	DecorateObject* newObj = new DecorateObject(id);
	newObj->InitInfo(m_pivot, m_isDeleteOnDestroyed, m_iDamagedEffectId, m_colliderRatio);
	newObj->Init(*this);
	newObj->CreatePhysicsBody();
	return newObj;
}

void DecorateObject::CreatePhysicsBody()
{
	printf("%f %f \n",m_originSize.x * this->GetScale().x * m_colliderRatio.x, m_originSize.y * this->GetScale().y * m_colliderRatio.y);
	m_physicsBody = Singleton<WorldManager>::GetInstance()->createRectagle(
		DECORATE_OBJ,
		m_position.x, m_position.y,
		m_originSize.x * this->GetScale().x * m_colliderRatio.x,
		m_originSize.y * this->GetScale().y * m_colliderRatio.y
	);
	UserData* user = (UserData*)this->m_physicsBody->body->GetUserData();
	m_physicsBody->body->SetAwake(false);
	user->m_damage = 0;
}

void DecorateObject::InitInfo(Vector2 pivot, bool isDeleteOnDestroyed, int iDamagedEffectId, Vector2 colliderRatio)
{
	m_pivot = pivot;
	m_colliderRatio = colliderRatio;
	m_isDeleteOnDestroyed = isDeleteOnDestroyed;
	m_iDamagedEffectId = iDamagedEffectId;
}

void DecorateObject::Update(float deltaTime)
{
	if (!m_isDestroyed) {
		UserData* user = (UserData*)m_physicsBody->body->GetUserData();
		if (user->IsCollison > 0) {
			Singleton<EffectManager>::GetInstance()->CreateParticlesSystem(m_position, m_iDamagedEffectId);
			if (m_isDeleteOnDestroyed) {
				Singleton<SceneManager2D>::GetInstance()->RemoveObject(this);
				delete this;
			}
			else {
				float prevFrameHeight = m_animationController.m_frameHeight;
				float prevFrameWidth = m_animationController.m_frameWidth;
				m_animationController.RunAnimState(0, -1);
				SetPosition(Vector3(
					m_position.x + 2 * m_pivot.x * (prevFrameWidth - m_animationController.m_frameWidth) * m_scale.x,
					m_position.y + 2 * m_pivot.y * (prevFrameHeight - m_animationController.m_frameHeight) * m_scale.y,
					m_position.z
				));
			}
			m_physicsBody->setActive(false);
		}
	}
}

void DecorateObject::SetPosition(Vector3 position)
{
	Vector3 realPosition(
		position.x - m_pivot.x * m_animationController.m_frameWidth * m_scale.x,
		position.y - m_pivot.y * m_animationController.m_frameHeight * m_scale.y,
		position.z
	);
	Sprite::SetPosition(realPosition);
	if (m_physicsBody != NULL) {
		m_physicsBody->body->SetTransform(b2Vec2(realPosition.x, realPosition.y), m_rotation);
	}
}
