import hmac
import hashlib

def generate_mac(raw_data: str, key: str) -> str:
    """
    Tạo chữ ký HMAC SHA256 để xác thực với ZaloPay API

    Args:
        raw_data (str): chuỗi dữ liệu cần ký
        key (str): key1 từ cấu hình

    Returns:
        str: chuỗi mã hóa HMAC SHA256
    """
    return hmac.new(key.encode(), raw_data.encode(), hashlib.sha256).hexdigest()