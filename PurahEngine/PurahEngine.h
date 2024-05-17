#pragma once
/// 외부에는 이부분만 보여지게 하기 위하도록 설계
#include <Windows.h>
#include "PurahEngineAPI.h"

#include "Serializable.h"

// Object Base
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "AudioListener.h"
#include "AudioSource.h"

// Manager
#include "SceneManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "GamePadManager.h"
#include "TimeController.h"
#include "UnifiedInputManager.h"

// Physics
#include "EasingFunc.h"

#include "ZnRaycastInfo.h"

#include "PhysicsSystem.h"
#include "Physics.h"

#include "RigidBody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "ConvexCollider.h"

#include "FixedJoint.h"
#include "HingeJoint.h"
#include "BallJoint.h"
#include "DistanceJoint.h"
#include "SpringJoint.h"
#include "SlideJoint.h"

// Graphics
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"
#include "Animator.h"

// UI
#include "UI.h"
#include "Image.h"
#include "Button.h"
#include "ButtonGroup.h"

namespace PurahEngine
{
	extern "C" PURAHENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
	extern "C" PURAHENGINE_API void Run(_In_ int nCmdShow);
	extern "C" PURAHENGINE_API void Finalize();
};
