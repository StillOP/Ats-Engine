#pragma once
#include <map>
#include <vector>
#include <d2d1.h>
#include <wincodec.h>

#include "Utilities.h"

class ResourceSystem
{
public:
	~ResourceSystem();

	HRESULT Init();
	void Update();

	ID2D1Bitmap** GetBitmap(std::string id, std::string path = "");

	void RegisterBitmap(std::string id, ID2D1Bitmap* bitmap);

	static ResourceSystem& get()
	{
		static ResourceSystem resSys;
		return resSys;
	}
	
	ID2D1Bitmap* LoadBitmapFromFileTest(std::string uri);

private:
	ResourceSystem();

	HRESULT LoadBitmapFromFile(std::string uri);

	std::map<std::string, ID2D1Bitmap*> m_bitmaps;
	std::map<std::string, UINT> m_bitmapsCount;

	IWICImagingFactory* m_pIWICFactory;
};