#pragma once

#include <string>
#include <vector>
#include <map>

#include <Eigen\Dense>

namespace FBXLoader
{
	struct Float2
	{
		union
		{
			struct
			{
				float x, y;
			};

			struct
			{
				float u, v;
			};
		};
	};

	struct Float3
	{
		union
		{
			struct
			{
				float x, y, z;
			};
		};
	};

	struct Float4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};

			struct
			{
				float r, g, b, a;
			};
		};
	};

	using Color = Float4;

	struct Vertex
	{
		Float3 position;
		Float3 normal;
		Float2 textureCoordinate;
		unsigned int boneIndices[4];
		float weight[4];
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int materialIndex;
	};

	struct Material
	{
		bool useDiffuseMap;

		Color baseColor;
		std::wstring diffuseMap;
	};

	struct Bone
	{
		Bone* parent;
		std::vector<Bone*> children;

		std::wstring name;

		std::vector<unsigned int> meshes;

		Eigen::Matrix4f transformMatrix;
		Eigen::Matrix4f offsetMatrix;
	};

	struct Model
	{
		Bone* root;

		std::vector<Bone*> boneList;
		std::vector<Mesh*> meshList;
		std::vector<Material*> materialList;

		std::vector<Animation*> animationList;
	};

	struct Animation
	{
		std::wstring name;

		double duration; // Æ½´ç ½Ã°£
		double tickPerSecond; // ½Ã°£´ç Æ½

		std::map<Bone*, std::vector<AnimationKey>> animationKey;
		std::vector<Eigen::Matrix4f> animationMatrix;
	};

	struct AnimationKey
	{
		double time;
		Eigen::Matrix4f value;

		bool operator<(const AnimationKey& right)
		{
			return time < right.time;
		}

		bool operator<=(const AnimationKey& right)
		{
			return time <= right.time;
		}

		bool operator>(const AnimationKey& right)
		{
			return time > right.time;
		}

		bool operator>=(const AnimationKey& right)
		{
			return time >= right.time;
		}

		bool operator==(const AnimationKey& right)
		{
			return time == right.time;
		}

		bool operator!=(const AnimationKey& right)
		{
			return time != right.time;
		}
	};
}