# comment

width = 100
height = 20000

# comment

start = " "

for i in range(width-1):
    start += " "

# comment

board = [None] * height
board[0] = start + "X "

##
##
for i in range(1, height):
    board[i] = start + "  "

print("NEW BOARD")
for i in range(height-1):
    for j in range(1, width+1):
        left = board[i][j-1]
        right = board[i][j+1]
        current = board[i][j]
        board[i+1] = list(board[i+1])
        if current == "X":
            aliveNeighbors = 0
            if left == "X":
                aliveNeighbors += 1
            if right == "X":
                aliveNeighbors += 1
            if aliveNeighbors <= 1:
                board[(i+1)][j] = "X"
            else:
                board[(i+1)][j] = " "
        else:
            aliveNeighbors = 0
            if left == "X":
                aliveNeighbors += 1
            if right == "X":
                aliveNeighbors += 1
            if aliveNeighbors > 0:
                board[(i+1)][j] = "X"
            else:
                board[(i+1)][j] = " "
        board[i+1] = ''.join(board[i+1])
    print("#" + board[i] + "#")