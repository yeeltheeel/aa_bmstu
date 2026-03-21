# devstral-2
# Вот код на Python, который определяет, используется ли в PDF-файле слово "оглавление" вместо "содержание" (с учетом регистра), используя конечный автомат и библиотеку PyPDF2:

# ```python
import PyPDF2
from typing import List, Tuple, Optional

class ContentChecker:
    def __init__(self):
        # Конечный автомат для поиска "оглавление" или "содержание"
        self.states = {
            'start': {'о': 'о', 'с': 'с'},
            'о': {'г': 'ог'},
            'ог': {'л': 'огл'},
            'огл': {'а': 'огла'},
            'огла': {'в': 'оглав'},
            'оглав': {'л': 'оглавл'},
            'оглавл': {'е': 'оглавле'},
            'оглавле': {'н': 'оглавлен'},
            'оглавлен': {'и': 'оглавлени'},
            'оглавлени': {'е': 'found_оглавление'},
            'с': {'о': 'со'},
            'со': {'д': 'сод'},
            'сод': {'е': 'соде'},
            'соде': {'р': 'содер'},
            'содер': {'ж': 'содерж'},
            'содерж': {'а': 'содержа'},
            'содержа': {'н': 'содержан'},
            'содержан': {'и': 'содержани'},
            'содержани': {'е': 'found_содержание'}
        }
        self.current_state = 'start'
        self.found_words = []

    def reset(self):
        self.current_state = 'start'

    def process_char(self, char: str, page_num: int, line_num: int) -> Optional[Tuple[str, int, int]]:
        char = char.lower()
        if char in self.states.get(self.current_state, {}):
            self.current_state = self.states[self.current_state][char]
            if self.current_state.startswith('found_'):
                word = self.current_state.split('_')[1]
                self.reset()
                return (word, page_num, line_num)
        else:
            self.reset()
        return None

def check_pdf_content(pdf_path: str) -> List[Tuple[str, int, int]]:
    checker = ContentChecker()
    results = []

    with open(pdf_path, 'rb') as file:
        reader = PyPDF2.PdfReader(file)

        for page_num in range(len(reader.pages)):
            page = reader.pages[page_num]
            text = page.extract_text()

            if text:
                lines = text.split('\n')
                for line_num, line in enumerate(lines, 1):
                    for char in line:
                        result = checker.process_char(char, page_num + 1, line_num)
                        if result:
                            results.append(result)

    return results

def main():
    pdf_path = input("Введите путь к PDF-файлу: ")
    results = check_pdf_content(pdf_path)

    if not results:
        print("В файле не найдено ни 'оглавление', ни 'содержание'.")
        return

    # Проверяем, есть ли "оглавление" и нет ли "содержание"
    has_оглавление = any(word == 'оглавление' for word, _, _ in results)
    has_содержание = any(word == 'содержание' for word, _, _ in results)

    if has_оглавление and not has_содержание:
        print("В файле используется 'оглавление' вместо 'содержание':")
        for word, page, line in results:
            if word == 'оглавление':
                print(f"({word}, {page}, {line})")
    elif has_содержание and not has_оглавление:
        print("В файле используется 'содержание' (не 'оглавление').")
    else:
        print("В файле найдены оба варианта или ни одного.")

if __name__ == "__main__":
    main()
# ```

# ### Как работает код:
# 1. **Конечный автомат** реализован в классе `ContentChecker`, который ищет последовательности символов для слов "оглавление" и "содержание".
# 2. **Чтение PDF** осуществляется с помощью `PyPDF2`, который извлекает текст со страниц.
# 3. **Поиск** происходит посимвольно, с учетом текущего состояния автомата.
# 4. **Результаты** выводятся в формате `(строка, страница, строка)`, если найдено "оглавление" без "содержание".

# ### Пример вывода:
# ```
# Введите путь к PDF-файлу: example.pdf
# В файле используется 'оглавление' вместо 'содержание':
# (оглавление, 3, 5)
# (оглавление, 7, 2)
# ```

# Код учитывает регистр (преобразует все символы в нижний регистр) и работает без использования регулярных выражений.
# [Finished in 31.2s]