import urllib.parse
import hmac
import hashlib

class VNPay:
    def __init__(self):
        self.request_data = {}
        self.response_data = {}

    def get_payment_url(self, base_url, secret_key):
        # Sắp xếp theo thứ tự chữ cái
        sorted_items = sorted(self.request_data.items())

        # Hash data: không encode
        hash_data_list = []
        query_list = []

        for k, v in sorted_items:
            hash_data_list.append(f"{k}={v}")
            query_list.append(f"{k}={urllib.parse.quote_plus(str(v))}")

        hash_data = '&'.join(hash_data_list)
        query_string = '&'.join(query_list)

        secure_hash = hmac.new(secret_key.encode(), hash_data.encode(), hashlib.sha512).hexdigest()

        print("---- DỮ LIỆU DEBUG CHUẨN ----")
        print("HASH DATA:", hash_data)
        print("QUERY STRING:", query_string)
        print("SECURE HASH:", secure_hash)

        return f"{base_url}?{query_string}&vnp_SecureHash={secure_hash}"

    def validate_response(self, secret_key):
        data = self.response_data.copy()
        secure_hash = data.pop("vnp_SecureHash", None)
        data.pop("vnp_SecureHashType", None)

        sorted_keys = sorted(data.keys())
        hash_data = '&'.join(f"{k}={data[k]}" for k in sorted_keys)

        gen_hash = hmac.new(secret_key.encode(), hash_data.encode(), hashlib.sha512).hexdigest()
        return gen_hash == secure_hash
