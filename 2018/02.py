with open(f"{__file__}.txt") as i:
    global input
    input = i.read();

input = input.strip('\n')
input = input.split('\n')

from collections import Counter

tw = 0
th = 0
for i in input:
    c = Counter(i);
    if 2 in c.values():
        tw += 1;
    if 3 in c.values():
        th += 1;

print(tw * th)

import itertools
for i in itertools.product(input, input):
    if i[0] == i[1]: continue;

    diff = 0
    c = 0

    for h in range(len(i[0])):
        if i[0][h] != i[1][h]:
            diff += 1
            c = h

    if diff == 1:
        d = list(i[0])

        del d[c]
        print(''.join(d))
        break

