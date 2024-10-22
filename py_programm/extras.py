def read_potential(filename, rows):
    matrix = []
    with open(filename, "r") as file:
        for i in range(rows):
            string = file.readline()
            row = list(map(float, string.split()))
            matrix.append(row)
    return matrix


def write_potential(filename, x, y, charges):
    with open(filename, "w", newline='') as file:
        print(f"{x} {y}\n", end='', file=file)
        print(f"{len(charges)}\n", end='', file=file)
        for i in range(len(charges)):
            print(f"{charges[i].x} {charges[i].y} {charges[i].value}\n", end='', file=file)


class MyEvent:
    def __init__(self, x, y):
        self.x = x
        self.y = y
