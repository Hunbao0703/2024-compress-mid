# 2024 compress mid
2024資料壓縮 期中

1. languae: python, IDE: vs code
 
 2. library: json, heapq, os, bitarray
    
    補充: 
    
        heapq (用於排序nodes, 若使用lamba則會太過遲鈍, 導致速度過慢, 簡單測試後, 大概差接近5倍的時間)

        os (用於查看壓縮比)

        bitarray (將霍夫曼編碼出來的二進制轉換為byte儲存, 可將檔案縮小, 否則若直接儲存二進制, 則一個數字佔據一個byte, 檔案大小不減反增)
    
 3. 西遊記 (0.447)

    黃金甲 (4.025)
    
    Lyrics_GangNamStyle (0.90718)
    
    PeterPan (0.5628)



how to run?

    1. run compress.py

    2. 壓縮檔案 1, 解壓縮檔案 2

    3. 壓縮檔案須包含文件檔格式 (如 西遊記.txt)

    4. 解壓縮同上 (如 西遊記.txt.huf)
