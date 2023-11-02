#pragma once

struct ResourceID
{
	union
	{
		struct
		{
			unsigned long d1;
			unsigned short d2;
			unsigned short d3;
			unsigned char d4[4];
		};

		struct
		{
			unsigned long long ull1;
			unsigned long long ull2;
		};

		unsigned long long ull[2];

		unsigned int ui[4];

		unsigned short us[8];

		unsigned char uc[16];
	};

	bool operator==(const ResourceID& right) const
	{
		return ull[0] == right.ull[0] && ull[1] == right.ull[1];
	}

	bool operator!=(const ResourceID& right) const
	{
		return (ull[0] != right.ull[0]) || (ull[1] != right.ull[1]);
	}
};

const ResourceID ID_NULL = { 0llu, 0ull };