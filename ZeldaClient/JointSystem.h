#pragma once
#include "Singleton.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class JointSystem final : public Singleton<JointSystem>
	{
	public:
		~JointSystem() override;

	public:
		void Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target);
	};
}

/*
 * 1.	플레이어가 울트라핸드로 오브젝트 하나를 선택
 *
 * 2.	오브젝트와 부착할 수 있는 오브젝트를 탐색
 *
 * 3.	일정 거리에 다다르면 Attatch가 가능.
 *
 * 4.	Attatch를 하면 두 오브젝트는 일정한 속력으로 서로에게 가까워짐
 *
 * 5.	두 물체가 충돌을 감지하면 충돌한 위치를 Anchor 위치로 하고 FixedJoint를 연결함.
 *
 * 6.	Anchor의 좌표는 월드로 나오기 때문에 로컬로 계산해서 조인트를 만들 필요가 있음.
*/