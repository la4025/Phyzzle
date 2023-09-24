#pragma once

namespace PurahEngine
{
	class Component
	{
	public:
		virtual void Awake();
		virtual void Start();

		/// Update
		// 기존 업데이트
		virtual void Update();
		// 물리 관련 업데이트
		virtual void FixedUpdate();
		// Upate 후에 한번 더 업데이트 (주로 카메라 관련에서 사용한다고 한다)
		virtual void LateUpdate();

		/// OnCollision
		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter();
		// 충돌체가 충돌을 유지할 때 호출
		virtual void OnCollisionStay();
		// 충돌체가 충돌을 벗어났을 때 호출
		virtual void OnCollisionExit();
		
		/// OnTrigger
		// 트리거가 충돌했을 때 호출
		virtual void OnTriggerEnter();
		// 트리거가 충돌율 유지할 때 호출
		virtual void OnTriggerStay();
		// 트리거가 충돌을 벗어났을 때 호출
		virtual void OnTriggerExit();
		
		/// OnMouse
		// 마우스가 오브젝트 위에 올라갔을 때 호출
		virtual void OnMouseEnter();
		// 마우스가 오브젝트 위에서 유지할 때 호출
		virtual void OnMouseStay();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		virtual void OnMouseExit();
	};
}

