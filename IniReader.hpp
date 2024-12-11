#pragma once
#include <string>
#include <type_traits>
#include "minIni.h"

class IniReader
{
private:
	std::string m_szFileName;

public:
	IniReader() = default;
	~IniReader() = default;

	IniReader(const std::string& filename)
	{
		m_szFileName = filename;
	}

	void SetFile(const std::string& filename)
	{
		m_szFileName = filename;
	}

	int ReadInteger(const std::string& section, const std::string& key, int default_value = 0)
	{
		return ini_getl(section.c_str(), key.c_str(), default_value, m_szFileName.c_str());
	}

	float ReadFloat(const std::string& section, const std::string& key, float default_value = 0.0f)
	{
		return ini_getf(section.c_str(), key.c_str(), default_value, m_szFileName.c_str());
	}

	bool ReadBoolean(const std::string& section, const std::string& key, bool default_value = false)
	{
		return ini_getbool(section.c_str(), key.c_str(), default_value, m_szFileName.c_str());
	}

	std::string ReadString(const std::string& section, const std::string& key, const std::string& default_value = "")
	{
		char buffer[INI_BUFFERSIZE];
		memset(buffer, 0, INI_BUFFERSIZE);
		ini_gets(section.c_str(), key.c_str(), default_value.c_str(), buffer, INI_BUFFERSIZE, m_szFileName.c_str());
		return std::string(buffer);
	}

	int WriteInteger(const std::string& section, const std::string& key, int value)
	{
		return ini_putl(section.c_str(), key.c_str(), value, m_szFileName.c_str());
	}

	int WriteFloat(const std::string& section, const std::string& key, float value)
	{
		return ini_putf(section.c_str(), key.c_str(), value, m_szFileName.c_str());
	}

	int WriteBoolean(const std::string& section, const std::string& key, bool value)
	{
		return ini_putbool(section.c_str(), key.c_str(), value, m_szFileName.c_str());
	}

	int WriteString(const std::string& section, const std::string& key, const std::string& value)
	{
		return ini_puts(section.c_str(), key.c_str(), value.c_str(), m_szFileName.c_str());
	}

	bool Exists(const std::string& section, const std::string& key)
	{
		return ini_haskey(section.c_str(), key.c_str(), m_szFileName.c_str()) == 1 ? true : false;
	}

	template <typename T>
	int Read(const std::string& section, const std::string& key, T& value)
	{
		if (std::is_same<T, int>::value)
		{
			value = ReadInteger(section, key);
			return 1;
		}
		else if (std::is_same<T, float>::value)
		{
			value = ReadFloat(section, key);
			return 1;
		}
		else if (std::is_same<T, bool>::value)
		{
			value = ReadBoolean(section, key);
			return 1;
		}
		else if (std::is_same<T, std::string>::value)
		{
			value = ReadString(section, key);
			return 1;
		}
		else
		{
			return 0;
		}
	}

	template <typename T>
	int Write(const std::string& section, const std::string& key, const T& value)
	{
		if (std::is_same<T, int>::value)
		{
			return WriteInteger(section, key, value);
		}
		else if (std::is_same<T, float>::value)
		{
			return WriteFloat(section, key, value);
		}
		else if (std::is_same<T, bool>::value)
		{
			return WriteBoolean(section, key, value);
		}
		else if (std::is_same<T, std::string>::value)
		{
			return WriteString(section, key, value);
		}
		else
		{
			return 0;
		}
	}

	template <typename T>
	int operator()(const std::string& section, const std::string& key, const T& value)
	{
		return Write<T>(section, key, value);
	}
};