with open(f"{__file__}.txt") as i:
    global input
    input = i.read();

input = input.strip('\n')
input = input.split('\n')

print(sum([(-1 if i[0] == '-' else 1) * int(i[1:]) for i in input]));

visited = set()
visited.add(0)
sum = 0
while True:
    for i in input:
        sum += (-1 if i[0] == '-' else 1) * int(i[1:]);

        if sum in visited:
            print(sum)
            exit(0)

        visited.add(sum)

