#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>

#include <windows.h>

template <typename T>
bool SetRegistryValue(const std::wstring& sub_key, const std::wstring& val_name, T value)
{
	HKEY hKey;
	LONG openRes = RegOpenKeyExW(HKEY_CURRENT_USER, sub_key.c_str(), 0, KEY_SET_VALUE, &hKey);
	bool result = false;

	if (openRes == ERROR_SUCCESS)
	{
		LONG setRes;
		if constexpr (std::is_same_v<T, DWORD>)
			setRes = RegSetValueExW(
				hKey, val_name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
		else
			setRes = RegSetValueExW(
				hKey, val_name.c_str(), 0, REG_SZ,
				reinterpret_cast<const BYTE*>(value.c_str()), (value.size() + 1) * sizeof(wchar_t));

		if (setRes == ERROR_SUCCESS)
		{
			std::wcout << L"Successfully set " << val_name << L" to " << value << L" in " << sub_key << std::endl;
			result = true;
		}
		else
			std::wcerr << L"Failed to set value. Error: " << setRes << std::endl;

		RegCloseKey(hKey);
	}
	else
		std::wcerr << L"Failed to open registry key. Error: " << openRes << std::endl;

	return result;
}

std::wstring ToWstring(const std::string& str)
{
	const int str_sz = static_cast<int>(str.size());
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str_sz, nullptr, 0);

	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str_sz, &wstr[0], size_needed);

	return wstr;
}

int main(int argc, char* argv[])
{
	const std::wstring prx_enable_value = L"ProxyEnable";
	const std::wstring prx_server_value = L"ProxyServer";

	if (argc == 1)
	{
		const std::wstring process_name = std::filesystem::path(argv[0]).filename().wstring();

		std::wcerr << std::endl << L"Usage: " << process_name <<
			L" <enable (1) / disable (0)> [proto=IP:port]" << std::endl <<
			L"Example: " << process_name << " 1 socks=127.0.0.1:9050" << std::endl;

		return 1;
	}

	const std::wstring sub_key = L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
	const std::string enabled_str = argv[1];

	try
	{
		if (!static_cast<DWORD>(std::stoi(enabled_str)))
			return SetRegistryValue(sub_key, prx_enable_value, 0ul) ? 0 : 1;
	}
	catch (...)
	{
		std::wcerr << "Invalid argument 'enabled' provided. Must be either 0 or 1." << std::endl;
	}

	std::wstring addr;
	{
		const std::string addr_arg = argv[2];
		addr = ToWstring(addr_arg);
	}

	bool is_ok = SetRegistryValue(sub_key, prx_enable_value, 1ul)
		&& SetRegistryValue(sub_key, prx_server_value, addr);

	return is_ok ? 0 : 1;
}
