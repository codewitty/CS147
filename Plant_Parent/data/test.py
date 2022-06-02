data = []
with open('data.txt') as f:
    for i in f:
        i_strip = i.rstrip("\n")
        print(i)
        print(type(i))
        d = list(i_strip.split(','))
        print(f'First element of d: {d[1]}')
        d[1] = float(d[1])
        data.append(d)
        print(f'Data:{data}')
    i = 0
print(f'Data Final:{data}')
