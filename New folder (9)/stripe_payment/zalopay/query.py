import hmac
import hashlib
import json
import urllib.parse
import urllib.request
from .config import ZALOPAY_CONFIG

def query_order(app_trans_id: str) -> dict:
    """
    Gửi yêu cầu truy vấn trạng thái đơn hàng đến ZaloPay sandbox

    Args:
        app_trans_id (str): Mã đơn hàng đã tạo từ ZaloPay

    Returns:
        dict: Kết quả phản hồi từ ZaloPay
    """
    config = ZALOPAY_CONFIG

    raw_data = f"{config['app_id']}|{app_trans_id}|{config['key1']}"
    mac = hmac.new(config['key1'].encode(), raw_data.encode(), hashlib.sha256).hexdigest()

    params = {
        "app_id": config["app_id"],
        "app_trans_id": app_trans_id,
        "mac": mac
    }

    try:
        data_encoded = urllib.parse.urlencode(params).encode()
        with urllib.request.urlopen(config["endpoint"]["query"], data=data_encoded) as response:
            result = json.loads(response.read())
            return result
    except Exception as e:
        return {"error": str(e)}