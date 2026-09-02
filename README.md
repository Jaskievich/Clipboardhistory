A Windows utility for managing clipboard history with persistent storage and quick access to frequently used text snippets.

📋 Overview
This application monitors your clipboard, maintains a history of copied text, and allows you to save frequently used items to a persistent list. You can quickly paste any saved item into any application using a global hotkey.

✨ Features
Clipboard History - Automatically tracks and displays text copied to clipboard

Persistent Storage - Save selected items to a local file for future sessions

Quick Access - Double-click or press Enter to paste selected text into the active window

Global Hotkey - Toggle the main window visibility with Ctrl+Shift+V (or Ctrl+F as fallback)

Two-List Interface:

Left List - Current clipboard history (dynamically updated)

Right List - Saved items for permanent reuse

Easy Management - Move items between lists, clear saved items

Automatic Saving - Items in the right list are automatically saved to disk

🚀 How It Works
Start the application - It runs in the background

Copy text anywhere - The left list updates automatically with clipboard contents

Save important text - Select an item in the left list and click "→" to move it to the right list

Use saved text - Select any item in the right list and press:

Enter - Paste the text into your active application

OK button - Same as Enter

Hide/Show window - Press Ctrl+Shift+V to toggle visibility

🎮 User Interface Controls
Control	Function
"Update" button	Refresh left list with current clipboard history
"→" button	Move selected item from left to right list
"Erase" button	Clear all items from the right list
Enter / OK	Paste selected right-list item into active window
Cancel / Close	Exit the application
Ctrl+Shift+V	Show/hide application window
📁 Data Storage
The right list is automatically saved to clipboard_history.dat in the application directory

File format is binary with length-prefixed strings

Items are preserved between application sessions

💡 Usage Tips
Keyboard friendly - Use arrow keys and Enter for quick navigation

Hotkey conflict - If Ctrl+Shift+V is taken, the app falls back to Ctrl+F

Window stays hidden - The main window hides automatically after pasting text

No clipboard interference - The app reads clipboard data without modifying it

-----------------------------------------------------------------------------------------------------------------------------------------------
Утилита для управления историей буфера обмена с возможностью сохранения текста для постоянного использования в работе.

📋 Обзор
Приложение отслеживает буфер обмена, ведёт историю скопированного текста и позволяет сохранять часто используемые фрагменты в постоянный список. Вы можете быстро вставить любой сохранённый элемент в любое приложение с помощью глобальной горячей клавиши.

✨ Возможности
История буфера обмена - Автоматически отслеживает и отображает скопированный текст

Постоянное хранение - Сохраняет выбранные элементы в локальный файл для использования в будущих сессиях

Быстрая вставка - Двойной клик или нажатие Enter для вставки текста в активное окно

Глобальная горячая клавиша - Показ/скрытие окна по Ctrl+Shift+V (или Ctrl+F как запасной вариант)

Два списка:

Левый список - Текущая история буфера обмена (динамически обновляется)

Правый список - Сохранённые элементы для постоянного использования

Удобное управление - Перемещение элементов между списками, очистка сохранённых элементов

Автосохранение - Элементы в правом списке автоматически сохраняются на диск

🚀 Принцип работы
Запустите приложение - Оно работает в фоновом режиме

Копируйте текст где угодно - Левый список автоматически обновляется

Сохраните важный текст - Выберите элемент в левом списке и нажмите «→», чтобы переместить его в правый список

Используйте сохранённый текст - Выберите любой элемент в правом списке и нажмите:

Enter - Вставка текста в активное приложение

Кнопка OK - То же самое, что Enter

Скрыть/показать окно - Нажмите Ctrl+Shift+V

🎮 Управление интерфейсом
Элемент	Функция
Кнопка "Update"	Обновить левый список текущей историей буфера
Кнопка "→"	Переместить выбранный элемент из левого списка в правый
Кнопка "Erase"	Очистить все элементы из правого списка
Enter / OK	Вставить выбранный элемент из правого списка в активное окно
Отмена / Закрыть	Выход из приложения
Ctrl+Shift+V	Показать/скрыть окно приложения
📁 Хранение данных
Правый список автоматически сохраняется в clipboard_history.dat в папке приложения

Формат файла - бинарный с строками с префиксом длины

Элементы сохраняются между сессиями приложения

💡 Советы по использованию
Удобство с клавиатуры - Используйте стрелки и Enter для быстрой навигации

Конфликт горячих клавиш - Если Ctrl+Shift+V занята, приложение использует Ctrl+F

Окно скрывается автоматически - После вставки текста окно прячется

Без вмешательства в буфер - Приложение только читает буфер обмена, не изменяя его

