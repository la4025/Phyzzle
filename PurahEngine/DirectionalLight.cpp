#include "DirectionalLight.h"

namespace PurahEngine
{
	void DirectionalLight::OnDataLoadComplete()
	{
		CreateDirectionalLight();
	}

	DirectionalLight::~DirectionalLight()
	{
		ReleaseLight();
	}


}