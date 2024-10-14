#include <SDKDDKVer.h>
#include <Windows.h>
#include <D3D11.h>
#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

class script_global
{
public:
	explicit script_global(std::size_t index);

	script_global at(std::ptrdiff_t index);
	script_global at(std::ptrdiff_t index, std::size_t size);

	template <typename T>
	std::enable_if_t<std::is_pointer_v<T>, T> as()
	{
		return static_cast<T>(get());
	}

	template <typename T>
	std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(get());
	}
private:
	void* get();
	std::size_t m_index;
};