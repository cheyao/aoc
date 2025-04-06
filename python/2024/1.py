
with open("1.txt") as f:
    content = f.read().strip().split('\n');
    a = [int(i.split('   ')[0]) for i in content]
    b = [int(i.split('   ')[1]) for i in content]
    a.sort();
    b.sort();
    sum1 = sum(max(p) - min(p) for p in zip(a, b))
    print(sum1)

    sum2 = sum(b.count(i)* i for i in a)
    print(sum2)
    
