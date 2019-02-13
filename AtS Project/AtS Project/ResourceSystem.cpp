#include "stdafx.h"
#include "ResourceSystem.h"
#include "WindowSystem.h"
#include <string>

ResourceSystem::ResourceSystem() { }

ResourceSystem::~ResourceSystem()
{
	for (auto& itr : m_bitmaps)
	{
		Utilities::get().SafeRelease(&itr.second);
	}

	m_bitmaps.clear();

	m_bitmapsCount.clear();
}

HRESULT ResourceSystem::Init()
{
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	return hr;
}

void ResourceSystem::Update()
{
	if (m_bitmaps.empty()) { return; }

	for (auto& itr : m_bitmapsCount)
	{
		if (itr.second <= 0)
		{
			Utilities::get().SafeRelease(&m_bitmaps.find(itr.first)->second);
			m_bitmaps.erase(itr.first);

			m_bitmapsCount.erase(itr.first);
		}
	}
}

ID2D1Bitmap** ResourceSystem::GetBitmap(std::string id, std::string path)
{
	auto itr = m_bitmaps.find(id);
	if (itr != m_bitmaps.end())
	{
		m_bitmapsCount[id]++;
		return &itr->second;
	}

	HRESULT hr = LoadBitmapFromFile(path);

	if (SUCCEEDED(hr))
	{
		std::string filename;
		Utilities::get().GetFileName(path, filename);

		m_bitmapsCount[filename]++;
		return &m_bitmaps[filename];
	}

	OutputDebugString(L"\n");
	std::string str1 = "Unable to load bitmap at path: " + path;
	OutputDebugStringA(str1.c_str());
	OutputDebugString(L"\n");
	return nullptr;
}

void ResourceSystem::RegisterBitmap(std::string id, ID2D1Bitmap* bitmap)
{
	if (m_bitmaps.find(id) != m_bitmaps.end() || m_bitmapsCount.find(id) != m_bitmapsCount.end()) { return; }

	m_bitmaps.emplace(id, bitmap);
	m_bitmapsCount.emplace(id, 0);
}

HRESULT ResourceSystem::LoadBitmapFromFile(std::string uri) 
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	
	ID2D1Bitmap* pBitmap = NULL;
	LPWSTR wfilename = NULL;

	uri += '\0';
	int wideCharBufferSize = MultiByteToWideChar(
		CP_UTF8,
		MB_PRECOMPOSED,
		uri.c_str(),
		uri.length(),
		wfilename,
		0
	);

	if (wideCharBufferSize != 0)
	{
		wfilename = new WCHAR[wideCharBufferSize];
		MultiByteToWideChar(
			CP_UTF8,
			MB_PRECOMPOSED,
			uri.c_str(),
			uri.length(),
			wfilename,
			wideCharBufferSize
		);
		
		hr = m_pIWICFactory->CreateDecoderFromFilename(
			wfilename,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		delete[] wfilename;
		wfilename = NULL;
	}

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
	}


	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		ID2D1HwndRenderTarget* prenderTarget = WindowSystem::get().pRenderTarget();

		hr = prenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);

		std::string filename;
		Utilities::get().GetFileName(uri, filename);

		RegisterBitmap(filename, pBitmap);
	}



	Utilities::get().SafeRelease(&pDecoder);
	Utilities::get().SafeRelease(&pSource);
	Utilities::get().SafeRelease(&pStream);
	Utilities::get().SafeRelease(&pConverter);
	Utilities::get().SafeRelease(&pScaler);

	return hr;
}


ID2D1Bitmap* ResourceSystem::LoadBitmapFromFileTest(std::string uri)
{
	OutputDebugString(L"\n");
	std::string str1 = "Loading bitmap";
	OutputDebugStringA(str1.c_str());
	OutputDebugString(L"\n");

	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	ID2D1Bitmap* pBitmap = NULL;
	LPWSTR wfilename = NULL;

	uri += '\0';
	int wideCharBufferSize = MultiByteToWideChar(
		CP_UTF8,
		MB_PRECOMPOSED,
		uri.c_str(),
		uri.length(),
		wfilename,
		0
	);

	if (wideCharBufferSize != 0)
	{
		wfilename = new WCHAR[wideCharBufferSize];
		MultiByteToWideChar(
			CP_UTF8,
			MB_PRECOMPOSED,
			uri.c_str(),
			uri.length(),
			wfilename,
			wideCharBufferSize
		);

		OutputDebugString(L"\n");
		std::wstring str2 = L"Loading from" + std::wstring(wfilename);
		OutputDebugString(str2.c_str());
		OutputDebugString(L"\n");

		hr = m_pIWICFactory->CreateDecoderFromFilename(
			wfilename,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		delete[] wfilename;
		wfilename = NULL;
	}

	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"\n");
		std::wstring str2 = L"Decoder created!";
		OutputDebugString(str2.c_str());
		OutputDebugString(L"\n");

		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"\n");
		std::wstring str2 = L"Retrived frame successfully!";
		OutputDebugString(str2.c_str());
		OutputDebugString(L"\n");

		hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
	}


	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"\n");
		std::wstring str2 = L"Converter created!";
		OutputDebugString(str2.c_str());
		OutputDebugString(L"\n");

		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"\n");
		std::wstring str3 = L"Converter successfully initialized!";
		OutputDebugString(str3.c_str());
		OutputDebugString(L"\n");

		ID2D1HwndRenderTarget* prenderTarget = WindowSystem::get().pRenderTarget();

		hr = prenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);

		return pBitmap;
	}



	Utilities::get().SafeRelease(&pDecoder);
	Utilities::get().SafeRelease(&pSource);
	Utilities::get().SafeRelease(&pStream);
	Utilities::get().SafeRelease(&pConverter);
	Utilities::get().SafeRelease(&pScaler);
}