#pragma once
class CBox
{
public:
	int right()
	{
		return left+width;
	}

	int bottom()
	{
		return top+height;
	}

	CRect ConvertToRect()
	{
		return CRect(left, top, left+width, top+height);
	}

	int left, top, width, height;
};
