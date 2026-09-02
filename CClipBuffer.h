#pragma once
#include <vector>
#include <string>
class CClipBuffer
{
public:

	CClipBuffer(const CClipBuffer&) = delete;
	CClipBuffer& operator=(const CClipBuffer&) = delete;
    // Глобальная точка доступа к экземпляру (Meyers Singleton)
    static CClipBuffer& GetInstance()
    {
        static CClipBuffer instance; // Создается при первом вызове (thread-safe в C++11)
        return instance;
    }

	std::vector<std::wstring> GetBuffer();

private:

    CClipBuffer();

    ~CClipBuffer() = default;

};

