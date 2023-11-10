#pragma once
#include "./include/ITester.h"
#include "./include/RBTree.h"

namespace flt
{
	namespace test
	{
		class TesterRBTree : public ITester
		{
		public:
			TesterRBTree();
			~TesterRBTree() {}

			virtual bool Test() override;

		private:
			RBTree<int, int> _tree;
		};
	}
}
