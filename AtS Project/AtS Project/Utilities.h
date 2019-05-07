#pragma once

class Utilities
{
public:
	static Utilities& get()
	{
		static Utilities sUtilities;
		return sUtilities;
	}

	template<class Interface>
	inline void SafeRelease(Interface **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != NULL)
		{
			(*ppInterfaceToRelease)->Release();
			(*ppInterfaceToRelease) = NULL;
		}

	}

	void GetFileName(std::string path, std::string& filename, bool getPath = false)
	{
		filename = path.substr(path.find_last_of("\\") + 1);

		if (!getPath) { filename = filename.substr(0, filename.find_last_of('.')); }
	}

	static unsigned int PairingFunction(unsigned int i, unsigned int j)
	{
		return (0.5 * ((i + j) * (i + j + 1))) + j;
	}

private:
	Utilities() {}
};