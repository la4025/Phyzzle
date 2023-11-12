#pragma once
#include "./include/ITester.h"
#include "./include/RBTree.h"
#include <vector>

namespace flt
{
	namespace test
	{
		class TesterRBTree : public ITester
		{
			using Node = flt::RBTree<int, int>::Node;
			using Color = flt::RBTree<int, int>::Color;
			Node& nil = flt::RBTree<int, int>::s_nil;

		public:
			TesterRBTree();
			TesterRBTree(unsigned int seed);
			virtual ~TesterRBTree() {}

			virtual bool Test() override;

		private:
			void Preorder(Node* pNode, std::vector<int>* outVector);
			void Inorder(Node* pNode, std::vector<int>* outVector);
			void Postorder(Node* pNode, std::vector<int>* outVector);

			int _inputDataCount;
			RBTree<int, int> _tree;
			std::vector<int> _inputData;
		};
	}
}
