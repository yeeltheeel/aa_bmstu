from string import ascii_letters, digits
from random import randint, choice

nodes = []
a = 3200
b = 5000
for i in range(randint(a, b)):
	#t = ''.join(choice(ascii_letters + digits) for _ in range(randint(1, 20)))
	t = 'n' + str(i)
	if t not in nodes:
		nodes.append(t)
print(len(nodes), 'nodes')

graph = dict()
for node in nodes:
	if node not in graph.keys():
		graph[node] = []
	a = randint(0, 15)
	for i in range(a):
		b = randint(0, len(nodes) - 1)
		if nodes[b] not in graph[node] and nodes[b] != node:
			graph[node].append(nodes[b])
			if nodes[b] not in graph.keys():
				graph[nodes[b]] = [node]
#print(graph)
type = True

filename = 'testgraph.gv'
file = open(filename, 'w')
file.close()

file = open(filename, 'a')
if type:
	print('graph G {\n', end='')
	file.write('graph G {\n')
else:
	print('diagraph G {\n', end='')
	file.write('diagraph G {\n')
for node in graph.keys():
	print('\t' + node + ' -- { ', end='')
	file.write('\t' + node + ' -- { ')
	# for i in range (len(graph[node]) - 1):
	# 	print(graph[node][i] + ' ', end='')
	# 	file.write(graph[node][i] + ' ')
	# print(graph[node][len(graph[node]) - 1] + ' };\n', end='')
	# file.write(graph[node][len(graph[node]) - 1] + ' };\n')
	for n in graph[node]:
		print(n + ' ', end='')
		file.write(n + ' ')
	print('};\n', end='')
	file.write('};\n')
print('}\n', end='')
file.write('}\n')
file.close()
