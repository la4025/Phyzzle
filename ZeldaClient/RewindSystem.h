#pragma once
#include "Singleton.h"

namespace Pyhzzle
{
	class RewindSystem final : public Singleton<RewindSystem>
	{
	public:
		~RewindSystem() override;

	public:

	};
}


/*
 * 1.	벽과 바닥같은 지형지물을 제외하고 움직이는 물체는 전부 리와인드가 가능할 것. (사전에 정해져 있을 듯)
 *
 * 2.	포지션, 로테이션이 일정 이상 변화한 오브젝트들은 자신의 운동량을 저장함.
 *
 * 3.	잠을 잔다면 오브젝트의 운동량 저장을 그만 둠.
 *
 * 4.	리와인드를 사용한다면 저장된 운동량과 시간을 가지고 사이 값을 보간함.
 *
 * 5.	리와인드를 마치면 운동량을 저장한 컨테이너를 초기화하고 다시 저장한다.
 *
 * 6.	리와인드를 중간에 그만 둘 경우에도 5) 판정으로 컨테이너를 초기화한다.
 */