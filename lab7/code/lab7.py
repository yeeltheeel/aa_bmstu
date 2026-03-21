from ollama import chat
from ollama import ChatResponse

# ollama cloud api key: 0fd3046588784b47ac3e4fed62f9789b.Nbr7jr7KDCguWbq80em2YpxW

# qwen3-coder: ollama pull qwen3-coder:480b-cloud
# devstral-2: ollama pull devstral-2:123b-cloud
# cogito-2.1: ollama pull cogito-2.1:671b-cloud
# gemma3: ollama pull gemma3:4b-cloud

my_prompt = '''
				Write code in Python3 which takes user pdf file and searches if substring1 = <<оглавление>> is used 
				instead of substring2 = <<содержание>> (can be uppercase) using custom regular expression.
				Use PyPDF2 library for reading file, no regex, implement finite automaton logic. 
				Output result as list of tuples (substring 1 or 2, page, line in page).
				'''

ru_prompt = '''
				Напиши код на python, который определяет в указанном пользователем pdf файле, используется ли там оглавление вместо 
				содержание (могут быть заглавными). Для определения использовать конечный автомат, не regex. Для чтения использовать PyPDF2.
				Если используется оглавление используется вместо содержание, то вывод в формате (строка, страница, строка).
			'''

# response: ChatResponse = chat(model='qwen3-coder:480b-cloud', messages=[{'role': 'user', 'content': ru_prompt, }, ])
# print(response['message']['content'])

# response: ChatResponse = chat(model='devstral-2:123b-cloud', messages=[{'role': 'user', 'content': ru_prompt, }, ])
# print(response['message']['content'])

# response: ChatResponse = chat(model='cogito-2.1:671b-cloud', messages=[{'role': 'user', 'content': ru_prompt, }, ])
# print(response['message']['content'])

response: ChatResponse = chat(model='gemma3:4b-cloud', messages=[{'role': 'user', 'content': ru_prompt, }, ])
print(response['message']['content'])