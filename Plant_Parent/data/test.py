data = []
with open('data.txt') as f:
    for i in f:
        i_strip = i.rstrip("\n")
        data.append(list(i_strip.split(',')))
    i = 0
    for d in data:
        print(d)
        d = [d[i][0], float(d[i][1])]
        i += 1
        print(d)

print(float(data[0][1]))
print(data)
