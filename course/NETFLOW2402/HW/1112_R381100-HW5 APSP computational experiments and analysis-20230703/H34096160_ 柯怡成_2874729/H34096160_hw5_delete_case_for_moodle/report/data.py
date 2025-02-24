import os
import pandas as pd

# 設定輸入和輸出的目錄和檔案類型
directory = 'D:/01大學/課程/三下/1_678_網路最佳化管理/作業/H34096160_hw5/output'

# 創建一個空的資料表
data = pd.DataFrame(columns=['Algorithm', 'Number of Scans', 'Running Time'])

# 讀取目錄中的所有檔案
for filename in os.listdir(directory):
    if filename.endswith(".txt"):
        filepath = os.path.join(directory, filename)
        
        # 從檔名中獲取演算法名稱
        algorithm = filename.split("_")[-1].split(".")[0]
        
        # 讀取檔案內容
        with open(filepath, 'r') as file:
            lines = file.readlines()
            
            # 從每行中提取所需資料
            number_of_scans = lines[0].split(":")[1].strip()
            running_time = lines[1].split(":")[1].strip()
            
            # 將資料加入資料表
            data = data.append({'Algorithm': algorithm, 'Number of Scans': number_of_scans, 'Running Time': running_time}, ignore_index=True)

# 儲存資料為 Excel 和 CSV 檔案
data.to_excel('output.xlsx', index=False)
data.to_csv('output.csv', index=False)