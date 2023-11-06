#pragma once
#include <iostream>
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"

#include "ITester.h"

namespace flt
{
	namespace test
	{
		struct TestData
		{
			TestData()
			{
				std::cout << "TestData Created" << std::endl;
			}

			~TestData()
			{
				std::cout << "TestData Destroyed" << std::endl;
			}
		};

		struct BuilderTest
		{
			void Release();

			TestData* pTestData;
		};

		template struct Resource<BuilderTest>;

		struct BuilderTestBuilder : public IBuilder<BuilderTest>
		{
			BuilderTestBuilder(const std::wstring filePath) : IBuilder<BuilderTest>(filePath) {}

			virtual BuilderTest* build() const override;
		};

		class BuilderTester : public ITester
		{
		public:
			virtual bool Test() override
			{
				using namespace flt::test;

				BuilderTestBuilder builder(L"test");
				Resource<BuilderTest> test1(_resourceMgr, builder);
				Resource<BuilderTest> test2(_resourceMgr, builder);
				Resource<BuilderTest> test3(_resourceMgr, builder);

				test1.Release();
				test2 = std::move(test1);
				test3 = std::move(test2);

				return true;
			}

			ResourceMgr _resourceMgr;
		};
	}
}
