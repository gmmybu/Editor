#pragma once

#include "OgreVector3.h"

class SceneDoc;

class CTerrainBrush
{
public:
	CTerrainBrush(SceneDoc *Owner);
	~CTerrainBrush();

	int GetSize();
	void SetSize(int Size);

	int GetPower() { return mPower; }
	void SetPower(int Power) { mPower = Power; }

	bool IsVisible();
	void SetVisible(bool Visible);

	void Load(CString Filename);
	float At(float X, float Y);
	float At(int X, int Y);

	void Roaming(Ogre::Vector3 Center);

private:
	void RebuildIndicator();

	SceneDoc *mOwner;
	CString mBrushTexture;

	int mSize, mPower;
	float *mBuff;
	Ogre::ManualObject *mIndicator;
};
