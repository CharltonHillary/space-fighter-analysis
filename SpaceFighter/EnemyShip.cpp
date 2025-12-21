
#include "EnemyShip.h"
#include "Level.h"



EnemyShip::EnemyShip()
{
	SetMaxHitPoints(1);
	SetCollisionRadius(20);
}


void EnemyShip::Update(const GameTime& gameTime)
{
	if (m_delaySeconds > 0)
	{
		m_delaySeconds -= gameTime.GetElapsedTime();

		if (m_delaySeconds <= 0)
		{
			GameObject::Activate();
		}
	}

	if (IsActive())
	{
		m_activationSeconds += gameTime.GetElapsedTime();
		if (m_activationSeconds > 2 && !IsOnScreen()) Deactivate();
	}

	Ship::Update(gameTime);
}


void EnemyShip::Initialize(const Vector2 position, const double delaySeconds)
{
	SetPosition(position);
	m_delaySeconds = delaySeconds;

	Ship::Initialize();
}


void EnemyShip::Hit(const float damage)
{
	Ship::Hit(damage);

	/*HC. Set condition and search for inactive enemy ships (will be used for spawn ships).
	Set positions of inactive ships and distance them. Activate 2 spawn.*/
	if (!IsActive() && GetCollisionRadius() > 15)
	{

		Level* pLevel = GetCurrentLevel();
		if (pLevel)
		{
			int spawnedCount = 0;

			for (GameObject* pObj : pLevel->GetObjects())
			{
				
				if (pObj != this && !pObj->IsActive() && pObj->HasMask(CollisionType::Enemy | CollisionType::Ship))
				{
					
					pObj->SetPosition(this->GetPosition());

					float spaceBetween = (spawnedCount == 0) ? -25.0f : 25.0f;
					pObj->TranslatePosition(spaceBetween, 0);
					pObj->SetCollisionRadius(10);
					pObj->Activate();
					spawnedCount++;
					if (spawnedCount >= 2) break;
				}
			}
		}
	}
}
