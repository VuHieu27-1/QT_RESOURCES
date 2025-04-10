# charging/controller.py

import csv
import os
from datetime import datetime

# Đơn giá sạc (VNĐ/phút)
CHARGING_RATES = {
    "slow": 500,
    "fast": 1000
}

# Công suất tiêu thụ (kW)
POWER_KW = {
    "slow": 1.0,
    "fast": 2.0
}

LOG_FILE = "charging_log.csv"

def calculate_duration(amount: int, mode: str) -> int:
    """
    Tính thời gian sạc dựa trên số tiền và chế độ sạc
    """
    rate = CHARGING_RATES.get(mode)
    if not rate:
        raise ValueError("Chế độ sạc không hợp lệ.")
    return amount // rate

def calculate_kwh(duration_minutes: int, mode: str) -> float:
    """
    Tính điện năng tiêu thụ theo thời gian và công suất
    """
    power = POWER_KW.get(mode)
    if not power:
        raise ValueError("Chế độ sạc không hợp lệ.")
    return (duration_minutes / 60.0) * power

def log_transaction(app_trans_id: str, amount: int, mode: str, duration: int, kwh: float):
    """
    Ghi log giao dịch vào file CSV
    """
    header = ["timestamp", "app_trans_id", "amount", "mode", "duration_minutes", "kwh"]
    row = [datetime.now().isoformat(), app_trans_id, amount, mode, duration, round(kwh, 2)]

    file_exists = os.path.exists(LOG_FILE)
    with open(LOG_FILE, mode='a', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        if not file_exists:
            writer.writerow(header)
        writer.writerow(row)

    print(f"📝 Giao dịch đã được ghi log vào {LOG_FILE}")