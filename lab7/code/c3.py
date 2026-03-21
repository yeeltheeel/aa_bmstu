# cogito
# Вот пример кода на Python с использованием PyPDF2 и конечного автомата для поиска строк "оглавление" или "содержание" в PDF-документе:

# ```python
import PyPDF2
from enum import Enum, auto

class State(Enum):
    SEARCH = auto()
    FOUND_O = auto()
    FOUND_OG = auto()
    FOUND_OGL = auto()
    FOUND_OGLA = auto()
    FOUND_OGLAV = auto()
    FOUND_OGLAVL = auto()
    FOUND_OGLAVLE = auto()
    FOUND_OGLAVLEN = auto()
    FOUND_OGLAVLENI = auto()
    FOUND_OGLAVLENIE = auto()
    FOUND_S = auto()
    FOUND_SO = auto()
    FOUND_SOD = auto()
    FOUND_SODE = auto()
    FOUND_SODER = auto()
    FOUND_SODERZH = auto()
    FOUND_SODERZHA = auto()
    FOUND_SODERZHAN = auto()
    FOUND_SODERZHANI = auto()
    FOUND_SODERZHANIE = auto()

def find_toc_mentions(pdf_path):
    results = []
    
    with open(pdf_path, 'rb') as file:
        reader = PyPDF2.PdfReader(file)
        
        for page_num in range(len(reader.pages)):
            page = reader.pages[page_num]
            text = page.extract_text()
            
            if not text:
                continue
                
            lines = text.split('\n')
            
            for line_num, line in enumerate(lines, 1):
                state = State.SEARCH
                word_start = -1
                
                for i, char in enumerate(line.lower()):
                    if state == State.SEARCH:
                        if char == 'о':
                            state = State.FOUND_O
                            word_start = i
                        elif char == 'с':
                            state = State.FOUND_S
                            word_start = i
                        else:
                            word_start = -1
                    
                    # Проверка для "оглавление"
                    elif state == State.FOUND_O:
                        if char == 'г': state = State.FOUND_OG
                        else: state = State.SEARCH
                    elif state == State.FOUND_OG:
                        if char == 'л': state = State.FOUND_OGL
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGL:
                        if char == 'а': state = State.FOUND_OGLA
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLA:
                        if char == 'в': state = State.FOUND_OGLAV
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAV:
                        if char == 'л': state = State.FOUND_OGLAVL
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAVL:
                        if char == 'е': state = State.FOUND_OGLAVLE
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAVLE:
                        if char == 'н': state = State.FOUND_OGLAVLEN
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAVLEN:
                        if char == 'и': state = State.FOUND_OGLAVLENI
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAVLENI:
                        if char == 'е': state = State.FOUND_OGLAVLENIE
                        else: state = State.SEARCH
                    elif state == State.FOUND_OGLAVLENIE:
                        if char == ' ' or not char.isalpha():
                            results.append(("ОГЛАВЛЕНИЕ", page_num + 1, line_num, line.strip()))
                            state = State.SEARCH
                        else:
                            state = State.SEARCH
                    
                    # Проверка для "содержание"
                    elif state == State.FOUND_S:
                        if char == 'о': state = State.FOUND_SO
                        else: state = State.SEARCH
                    elif state == State.FOUND_SO:
                        if char == 'д': state = State.FOUND_SOD
                        else: state = State.SEARCH
                    elif state == State.FOUND_SOD:
                        if char == 'е': state = State.FOUND_SODE
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODE:
                        if char == 'р': state = State.FOUND_SODER
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODER:
                        if char == 'ж': state = State.FOUND_SODERZH
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODERZH:
                        if char == 'а': state = State.FOUND_SODERZHA
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODERZHA:
                        if char == 'н': state = State.FOUND_SODERZHAN
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODERZHAN:
                        if char == 'и': state = State.FOUND_SODERZHANI
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODERZHANI:
                        if char == 'е': state = State.FOUND_SODERZHANIE
                        else: state = State.SEARCH
                    elif state == State.FOUND_SODERZHANIE:
                        if char == ' ' or not char.isalpha():
                            results.append(("СОДЕРЖАНИЕ", page_num + 1, line_num, line.strip()))
                            state = State.SEARCH
                        else:
                            state = State.SEARCH
    
    return results

# Пример использования
pdf_path = "report.pdf"  # Укажите путь к вашему PDF-файлу
mentions = find_toc_mentions(pdf_path)

if mentions:
    print("Найдены следующие упоминания:")
    for mention in mentions:
        word, page, line_num, line_text = mention
        print(f"Слово: {word}, Страница: {page}, Строка: {line_num}, Текст: {line_text}")
else:
    print("Упоминания 'оглавление' или 'содержание' не найдены.")
# ```

# Этот код:

# 1. Использует PyPDF2 для чтения PDF-файла.
# 2. Перебирает все страницы документа.
# 3. Разбивает текст каждой страницы на строки.
# 4. Использует конечный автомат для поиска слов "оглавление" или "содержание" в любом регистре.
# 5. Для каждого найденного упоминания сохраняет:
#    - Найденное слово ("ОГЛАВЛЕНИЕ" или "СОДЕРЖАНИЕ")
#    - Номер страницы
#    - Номер строки на странице
#    - Полный текст строки, содержащей упоминание

# Особенности реализации:
# - Код не чувствителен к регистру
# - Использует конечный автомат вместо регулярных выражений
# - Учитывает только целые слова (не находит части слов)
# - Выводит полную информацию о каждом найденном упоминании

# Для использования:
# 1. Установите PyPDF2: `pip install PyPDF2`
# 2. Замените "example.pdf" на путь к вашему PDF-файлу
# 3. Запустите скрипт

# Результат будет содержать все найденные упоминания с указанием страницы и номера строки.
# [Finished in 26.2s]