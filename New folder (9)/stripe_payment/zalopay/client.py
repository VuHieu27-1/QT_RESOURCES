# zalopay/client.py

import time
import requests
from .config import ZALOPAY_CONFIG
from .utils import generate_mac

def generate_app_trans_id() -> str:
    """
    Tạo app_trans_id theo định dạng yyMMdd_xxxxxxxx (theo chuẩn của ZaloPay)
    """
    timestamp = int(time.time())
    date_prefix = time.strftime('%y%m%d')
    return f"{date_prefix}_{timestamp}"

def create_order(amount: int, app_user: str = "terminal_user") -> dict:
    """
    Tạo đơn hàng thanh toán trên hệ thống ZaloPay Sandbox

    Args:
        amount (int): Số tiền thanh toán (VNĐ)
        app_user (str): Mã người dùng (tùy chọn)

    Returns:
        dict: Phản hồi từ API của ZaloPay, bao gồm order_url và app_trans_id
    """
    config = ZALOPAY_CONFIG
    app_trans_id = generate_app_trans_id()
    app_time = int(time.time() * 1000)  # thời gian tính bằng milliseconds

    order_data = {
        "app_id": config["app_id"],
        "app_trans_id": app_trans_id,
        "app_user": app_user,
        "app_time": app_time,
        "amount": amount,
        "item": "[]",
        "embed_data": "{}",
        "description": f"Thanh toan don hang #{app_trans_id}",
        "pay_channel": "zalopayapp"
    }

    # Chuỗi raw data để ký: app_id|app_trans_id|app_user|amount|app_time|embed_data|item
    raw_data = "|".join([
        str(order_data["app_id"]),
        order_data["app_trans_id"],
        order_data["app_user"],
        str(order_data["amount"]),
        str(order_data["app_time"]),
        order_data["embed_data"],
        order_data["item"]
    ])

    order_data["mac"] = generate_mac(raw_data, config["key1"])

    try:
        response = requests.post(config["endpoint"]["create_order"], data=order_data)
        response.raise_for_status()
        result = response.json()
        result["app_trans_id"] = app_trans_id  # Gắn lại để sử dụng ở bước tiếp theo
        return result
    except requests.RequestException as e:
        return {"error": str(e)}
