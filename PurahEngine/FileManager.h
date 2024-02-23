#pragma once
#include "PurahEngineAPI.h"

namespace PurahEngine
{
	class PURAHENGINE_API FileManager
	{
	public:

	private:
		// 파일을 읽고 있는중인가?
		bool isRunning;


	private:
		FileManager();
		~FileManager();
		FileManager(const FileManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		FileManager& operator=(const FileManager& ref) = delete;

	public:
		static FileManager& GetInstance();
	};
}

