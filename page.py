class Page:
    num = 0
    last_used = 0
    cnt = 0
    table_num = -1

    def __init__(self, num):
        self.num = num

    def __str__(self):
        return f"P(num={self.num},lu={self.last_used},cnt={self.cnt})"

