import json
import heapq
import os

from bitarray import bitarray

class Node():
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.right = None
        self.left = None

    def __lt__(self, other): return self.freq < other.freq


def cal_freq(data):
    freq = {}
    for row in data:
        if row in freq: freq[row] += 1
        else: freq[row] = 1

    return freq



def build_tree(data):
    heap = [Node(char, f) for char, f in freq.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)

        merged = Node(None, left.freq + right.freq)
        merged.left = left
        merged.right = right

        heapq.heappush(heap, merged)

    return heap[0]


def build_code_map(tree, code_map={}, result=""):
    if not tree.char == None: 
        code_map[tree.char] = result
        return

    if tree.left: build_code_map(tree.left, code_map, result + "0")
    if tree.right: build_code_map(tree.right, code_map, result + "1")

    return code_map

def compress(data, code_map): return ''.join([code_map[row] for row in data])


def decompress(data, code_map):
    result = []
    reverse_code_map = {code: char for char, code in code_map.items()}

    tmp = []
    for row in data:
        tmp.append(row)
        if ''.join(tmp) in reverse_code_map:
            result.append(reverse_code_map[''.join(tmp)])
            tmp.clear()

    return ''.join(result)




def save(data, code_map, file_name):
    with open(f"./compress_file/{file_name}.huf", 'wb') as f:
        json_code_map = json.dumps(code_map).encode('utf8')
        json_length = len(json_code_map)

        f.write(json_length.to_bytes(4, 'big'))
        f.write(json_code_map)

        bitarray(data).tofile(f)

    print("完成\n壓縮比:" , os.path.getsize(f"./compress_file/{file_name}.huf") / os.path.getsize(f'./data/{file_name}'))


def load(file_name):
    with open(f"./compress_file/{file_name}", 'rb') as f:
        json_length = int.from_bytes(f.read(4), 'big')
        json_code_map = f.read(json_length).decode('utf8')

        code_map = json.loads(json_code_map)

        bits = bitarray()
        bits.fromfile(f)
        data = bits.to01()

        return data, code_map














if __name__ == "__main__":

    c = int(input("壓縮檔案請輸入 1\n解壓縮檔案請輸入 2\nenter: "))
    if c == 1:

        file_name = str(input("請輸入檔案名稱 (包含檔案格式): "))

        with open(f"./data/{file_name}", 'r', encoding="utf8") as f:
            data = f.read()


        freq = cal_freq(data)
        
        tree = build_tree(freq)

        code_map = build_code_map(tree)

        compress_data = compress(data, code_map)

        save(compress_data, code_map, file_name)


    elif c == 2:
        
        file_name = str(input("請輸入檔案名稱 (包含檔案格式 *包含.huf*): "))


        data, code_map = load(file_name)

        with open(f"./output/{file_name.replace('.txt.huf', '_decompress.txt')}", 'w+', encoding="utf8") as f:
            f.write(decompress(data, code_map))









