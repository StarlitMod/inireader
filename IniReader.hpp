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

	bool WriteInteger(const std::string& section, const std::string& key, int value)
	{
		return ini_putl(section.c_str(), key.c_str(), value, m_szFileName.c_str()) != 0 ? true : false;
	}

	bool WriteFloat(const std::string& section, const std::string& key, float value)
	{
		return ini_putf(section.c_str(), key.c_str(), value, m_szFileName.c_str()) != 0 ? true : false;
	}

	bool WriteBoolean(const std::string& section, const std::string& key, bool value)
	{
		return ini_putbool(section.c_str(), key.c_str(), value, m_szFileName.c_str()) != 0 ? true : false;
	}

	bool WriteString(const std::string& section, const std::string& key, const std::string& str)
	{
		return ini_puts(section.c_str(), key.c_str(), str.c_str(), m_szFileName.c_str()) != 0 ? true : false;
	}

	template <typename T>
	T Read(const std::string& section, const std::string& key, const T& default_value)
	{
		if constexpr (std::is_same_v<T, bool> && sizeof(T) == 1)
		{
			return ReadBoolean(section, key, default_value);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return ReadInteger(section, key, default_value);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return ReadFloat(section, key, default_value);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return ReadString(section, key, default_value);
		}
		else if constexpr (std::is_same_v<T, char*>)
		{
			return ReadString(section, key, std::string(default_value));
		}
		else
		{
			return default_value;
		}
	}

	template <typename T>
	bool Write(const std::string& section, const std::string& key, const T& value)
	{
		if constexpr (std::is_same_v<T, bool> && sizeof(T) == 1)
		{
			return WriteBoolean(section, key, value);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return WriteInteger(section, key, value);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return WriteFloat(section, key, value);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return WriteString(section, key, value);
		}
		else if constexpr (std::is_same_v<T, char*>)
		{
			return WriteString(section, key, std::string(value));
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	void operator()(const std::string& section, const std::string& key, const T& value)
	{
		Write<T>(section, key, value);
	}
};