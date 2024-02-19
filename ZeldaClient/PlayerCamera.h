#pragma once
#include "Component.h"

class PlayerCamera : public PurahEngine::Component
{
public:
				~PlayerCamera()	override = default;
	void		Awake()			override;
	void		Start()			override;
	void		FixedUpdate()	override;
	void		Update()		override;
	void		Initialize()	override;
};

