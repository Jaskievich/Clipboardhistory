#include "stdafx.h"
#include "CClipBuffer.h"
#include <locale.h>

// Подключаем библиотеку WinRT
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <iostream>
#include <algorithm>

using namespace winrt;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation::Collections;

CClipBuffer::CClipBuffer() 
{
	std::setlocale(LC_ALL, "Russian");
}

std::vector<std::wstring> CClipBuffer::GetBuffer()
{
	std::vector<std::wstring> result;
    auto historyResult = Clipboard::GetHistoryItemsAsync().get();

    if (historyResult.Status() == ClipboardHistoryItemsResultStatus::Success) {
        IVectorView<ClipboardHistoryItem> historyItems = historyResult.Items();
    //    std::cout << "Найдено элементов в истории: " << historyItems.Size() << std::endl;

        for (const auto& item : historyItems) {
            auto dataPackageView = item.Content();
            // Проверяем наличие текста
            if (dataPackageView.Contains(StandardDataFormats::Text())) {
                auto text = dataPackageView.GetTextAsync().get();
             //   std::wcout << L"Текст: " << text.c_str() << std::endl;
                result.push_back(text.c_str());
            }
            // Проверяем наличие изображения (Bitmap)
            else if (dataPackageView.Contains(StandardDataFormats::Bitmap())) {
             //   std::cout << "Изображение" << std::endl;
                // Для получения изображения используйте dataPackageView.GetBitmapAsync()
            }
            // Другие форматы (Html, StorageItems и т.д.) могут не сохраняться в истории [citation:5]
        }
    }
    else {
        std::cerr << "Не удалось получить историю буфера обмена." << std::endl;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

