#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

namespace PurahEngine
{
	class Image;

	class PURAHENGINE_API Button : public Component
	{
	public:
		void Start() override;

		void Push();
		void Select();
		void Unselect();

	private:
		void PushEvent();

	private:
		bool selected;

		bool selectedWithStart;
		Image* targetImage;
		std::wstring selectedImage;
		std::wstring unselectedImage;
		std::wstring pushedImage;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}