# qwen3-coder
# Вот код на Python, который использует конечный автомат для определения слова "оглавление" (в любом регистре) вместо "содержание" в PDF-файле:

# ```python
import PyPDF2

def find_oglavlenie_instead_of_soderzhanie(pdf_path):
    """
    Определяет использование слова "оглавление" вместо "содержание" в PDF файле
    Использует конечный автомат для поиска
    """
    
    # Конечный автомат для поиска слова "оглавление"
    class OglavlenieAutomaton:
        def __init__(self):
            self.state = 0
            self.target_word = "оглавление"
            self.target_word_upper = "ОГЛАВЛЕНИЕ"
            self.found_positions = []
            self.current_char_pos = 0
            self.current_page = 1
            self.current_line = 1
            self.char_in_line = 0
            self.line_start_pos = 0
            
        def reset(self):
            self.state = 0
            
        def process_char(self, char, page_num, char_pos_in_text, absolute_pos):
            # Отслеживаем строки
            if char == '\n':
                self.current_line += 1
                self.char_in_line = 0
                self.line_start_pos = absolute_pos + 1
            else:
                self.char_in_line += 1
            
            # Проверяем обычное написание
            if self.state < len(self.target_word):
                if char.lower() == self.target_word[self.state]:
                    self.state += 1
                    if self.state == len(self.target_word):
                        # Найдено слово "оглавление"
                        line_num = self.current_line
                        char_pos_in_line = self.char_in_line - len(self.target_word) + 1
                        if char_pos_in_line < 1:
                            char_pos_in_line = 1
                        self.found_positions.append({
                            'word': 'оглавление',
                            'page': page_num,
                            'line': line_num,
                            'char_pos': char_pos_in_line
                        })
                        self.reset()
                else:
                    self.reset()
                    # Проверяем с текущего символа
                    if char.lower() == self.target_word[0]:
                        self.state = 1
            
            # Проверяем заглавные буквы
            if self.state == 0:  # Только если не в процессе поиска обычного слова
                temp_state = 0
                if temp_state < len(self.target_word_upper):
                    if char == self.target_word_upper[temp_state]:
                        temp_state += 1
                        # Продолжаем проверку для остальных символов
                        # Это упрощенная проверка, в реальном автомате нужно более сложное состояние
                        
    # Конечный автомат для поиска слова "содержание"
    class SoderzhanieAutomaton:
        def __init__(self):
            self.state = 0
            self.target_word = "содержание"
            self.target_word_upper = "СОДЕРЖАНИЕ"
            
        def reset(self):
            self.state = 0
            
        def process_char(self, char):
            if self.state < len(self.target_word):
                if char.lower() == self.target_word[self.state]:
                    self.state += 1
                    if self.state == len(self.target_word):
                        self.reset()
                        return True
                else:
                    self.reset()
                    if char.lower() == self.target_word[0]:
                        self.state = 1
            return False
    
    try:
        with open(pdf_path, 'rb') as file:
            pdf_reader = PyPDF2.PdfReader(file)
            results = []
            
            for page_num in range(len(pdf_reader.pages)):
                page = pdf_reader.pages[page_num]
                text = page.extract_text()
                
                # Очищаем текст от лишних пробелов
                lines = text.split('\n')
                
                # Используем конечный автомат для поиска
                oglavlenie_state = 0
                soderzhanie_state = 0
                
                oglavlenie_target = "оглавление"
                soderzhanie_target = "содержание"
                oglavlenie_target_upper = "ОГЛАВЛЕНИЕ"
                soderzhanie_target_upper = "СОДЕРЖАНИЕ"
                
                char_position = 0
                line_number = 1
                char_in_line = 0
                line_start_positions = [0]
                
                # Подсчитываем позиции строк
                for i, char in enumerate(text):
                    if char == '\n':
                        line_number += 1
                        line_start_positions.append(i + 1)
                
                line_number = 1
                for i, char in enumerate(text):
                    if char == '\n':
                        line_number += 1
                        char_in_line = 0
                    else:
                        char_in_line += 1
                    
                    # Проверяем "оглавление"
                    found_oglavlenie = False
                    if oglavlenie_state < len(oglavlenie_target):
                        if char.lower() == oglavlenie_target[oglavlenie_state]:
                            oglavlenie_state += 1
                            if oglavlenie_state == len(oglavlenie_target):
                                found_oglavlenie = True
                        else:
                            # Проверяем заглавными
                            if char == oglavlenie_target_upper[0] and oglavlenie_state == 0:
                                # Начинаем проверку заглавного варианта
                                temp_state = 1
                                j = 1
                                while (i + j < len(text) and 
                                       temp_state < len(oglavlenie_target_upper) and
                                       text[i + j] == oglavlenie_target_upper[temp_state]):
                                    temp_state += 1
                                    j += 1
                                if temp_state == len(oglavlenie_target_upper):
                                    found_oglavlenie = True
                                    oglavlenie_state = 0
                                    # Найдено "ОГЛАВЛЕНИЕ"
                                    line_num = line_number
                                    char_pos_in_line = char_in_line - len(oglavlenie_target_upper) + 1
                                    if char_pos_in_line < 1:
                                        char_pos_in_line = 1
                                    results.append((line_num, page_num + 1, char_pos_in_line))
                                    # Пропускаем обработанные символы
                                    continue
                            
                            oglavlenie_state = 0
                            if char.lower() == oglavlenie_target[0]:
                                oglavlenie_state = 1
                    
                    if found_oglavlenie and oglavlenie_state == len(oglavlenie_target):
                        # Проверяем, есть ли "содержание" поблизости (в пределах нескольких символов)
                        # Это простая проверка - в реальном случае нужно более точное определение
                        
                        # Находим позицию строки
                        line_num = line_number
                        char_pos_in_line = char_in_line - len(oglavlenie_target) + 1
                        if char_pos_in_line < 1:
                            char_pos_in_line = 1
                        
                        # Проверяем, не было ли "содержание" недавно
                        soderzhanie_found_recently = False
                        check_start = max(0, i - 50)  # Проверяем 50 символов назад
                        temp_state = 0
                        for j in range(check_start, i):
                            if temp_state < len(soderzhanie_target):
                                if text[j].lower() == soderzhanie_target[temp_state]:
                                    temp_state += 1
                                    if temp_state == len(soderzhanie_target):
                                        soderzhanie_found_recently = True
                                        break
                                else:
                                    temp_state = 0
                                    if text[j].lower() == soderzhanie_target[0]:
                                        temp_state = 1
                        
                        # Если "содержание" не найдено недавно, значит это замена
                        if not soderzhanie_found_recently:
                            results.append((line_num, page_num + 1, char_pos_in_line))
                        
                        oglavlenie_state = 0
                    
                    # Проверяем заглавное "ОГЛАВЛЕНИЕ" отдельно
                    if (oglavlenie_state == 0 and 
                        i + len(oglavlenie_target_upper) <= len(text)):
                        is_upper_oglavlenie = True
                        for k in range(len(oglavlenie_target_upper)):
                            if text[i + k] != oglavlenie_target_upper[k]:
                                is_upper_oglavlenie = False
                                break
                        
                        if is_upper_oglavlenie:
                            # Проверяем контекст на наличие "содержание"
                            soderzhanie_found_recently = False
                            check_start = max(0, i - 50)
                            temp_state = 0
                            for j in range(check_start, i):
                                if temp_state < len(soderzhanie_target):
                                    if text[j].lower() == soderzhanie_target[temp_state]:
                                        temp_state += 1
                                        if temp_state == len(soderzhanie_target):
                                            soderzhanie_found_recently = True
                                            break
                                    else:
                                        temp_state = 0
                                        if text[j].lower() == soderzhanie_target[0]:
                                            temp_state = 1
                            
                            if not soderzhanie_found_recently:
                                line_num = line_number
                                char_pos_in_line = char_in_line + 1
                                results.append((line_num, page_num + 1, char_pos_in_line))
                            
                            # Пропускаем обработку этих символов
                            oglavlenie_state = 0
                            continue
                
            return results
            
    except Exception as e:
        print(f"Ошибка при обработке PDF файла: {e}")
        return []

def main():
    pdf_path = input("Введите путь к PDF файлу: ").strip()
    
    # Убираем кавычки, если они есть
    if pdf_path.startswith('"') and pdf_path.endswith('"'):
        pdf_path = pdf_path[1:-1]
    if pdf_path.startswith("'") and pdf_path.endswith("'"):
        pdf_path = pdf_path[1:-1]
    
    results = find_oglavlenie_instead_of_soderzhanie(pdf_path)
    
    if results:
        print("Найдены случаи использования 'оглавление' вместо 'содержание':")
        for line, page, char_pos in results:
            print(f"({line}, {page}, {char_pos})")
    else:
        print("Случаи использования 'оглавление' вместо 'содержание' не найдены")

if __name__ == "__main__":
    main()
# ```

# Этот код:

# 1. **Использует конечный автомат** для поиска слов "оглавление" и "ОГЛАВЛЕНИЕ"
# 2. **Читает PDF через PyPDF2** с помощью `PyPDF2.PdfReader`
# 3. **Отслеживает позицию** (строка, страница, позиция символа в строке)
# 4. **Проверяет контекст** - если "оглавление" найдено, а "содержание" в ближайшем контексте отсутствует, считает это заменой
# 5. **Выводит результат** в формате `(строка, страница, строка)` как запрошено

# Для работы кода нужно установить PyPDF2:
# ```bash
# pip install PyPDF2
# ```

# Код обрабатывает:
# - Обычное написание "оглавление"
# - Заглавное написание "ОГЛАВЛЕНИЕ"  
# - Отслеживает номера строк и страниц
# - Проверяет контекст на наличие слова "содержание"
# [Finished in 23.1s]