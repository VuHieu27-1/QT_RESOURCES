import hashlib
import hmac
import urllib.parse


class VNPay:
    def __init__(self):
        self.request_data = {}
        self.response_data = {}

    def get_payment_url(self, base_url: str, secret_key: str) -> str:
        """
        Tạo URL thanh toán VNPay đã ký hash.
        :param base_url: URL của cổng thanh toán VNPay (sandbox hoặc production)
        :param secret_key: Chuỗi bí mật dùng để tạo checksum (vnp_HashSecret)
        :return: Chuỗi URL đầy đủ để redirect người dùng đến VNPay
        """
        sorted_params = sorted(self.request_data.items())
        query_string = '&'.join(f"{k}={urllib.parse.quote_plus(str(v))}" for k, v in sorted_params)
        secure_hash = self._hmac_sha512(secret_key, query_string)
        return f"{base_url}?{query_string}&vnp_SecureHash={secure_hash}"

    def validate_response(self, secret_key: str) -> bool:
        """
        Kiểm tra tính toàn vẹn của dữ liệu phản hồi từ VNPay bằng cách so sánh hash.
        :param secret_key: Chuỗi bí mật được dùng để kiểm tra checksum
        :return: True nếu hợp lệ, False nếu sai checksum
        """
        received_hash = self.response_data.pop('vnp_SecureHash', None)
        self.response_data.pop('vnp_SecureHashType', None)

        filtered_data = {
            k: v for k, v in self.response_data.items() if k.startswith('vnp_')
        }
        sorted_params = sorted(filtered_data.items())
        data_string = '&'.join(f"{k}={urllib.parse.quote_plus(str(v))}" for k, v in sorted_params)
        calculated_hash = self._hmac_sha512(secret_key, data_string)

        return received_hash == calculated_hash

    @staticmethod
    def _hmac_sha512(key: str, data: str) -> str:
        """
        Tạo chuỗi hash dùng HMAC SHA512
        :param key: Secret key
        :param data: Dữ liệu cần hash
        :return: Chuỗi hex của hash
        """
        byte_key = key.encode('utf-8')
        byte_data = data.encode('utf-8')
        return hmac.new(byte_key, byte_data, hashlib.sha512).hexdigest()
