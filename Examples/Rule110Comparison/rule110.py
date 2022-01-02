# comment

width = 150
height = 20000

# comment

last = ""

start = " "
for i in range(width-1):
    start += " "

# comment
blank = start + " "

last = start + "X "

##
##
first = blank

print("NEW BOARD")
for i in range(height-1):
    for j in range(1, width-1):
        left = last[j-1]
        right = last[j+1]
        current = last[j]
        first = list(first)
        if current == "X":
            aliveNeighbors = 0
            if left == "X":
                aliveNeighbors += 1
            if right == "X":
                aliveNeighbors += 1
            if aliveNeighbors <= 1:
                first[j] = "X"
            else:
                first[j] = " "
        else:
            aliveNeighbors = 0
            if left == "X":
                aliveNeighbors += 1
            if right == "X":
                aliveNeighbors += 1
            if aliveNeighbors > 0:
                first[j] = "X"
            else:
                first[j] = " "
        first = ''.join(first)
    print("#" + last + "#")
    last = first
    first = blank