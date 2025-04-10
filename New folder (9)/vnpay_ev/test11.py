import hmac
import hashlib

secret_key = 'O4LQ6ASQQKMGUCOSIQGOPWCF7DZF05CG'
hash_data = 'vnp_Amount=5000000&vnp_Command=pay&vnp_CreateDate=20250406231147&vnp_CurrCode=VND&vnp_ExpireDate=20250406232647&vnp_IpAddr=171.225.184.76&vnp_Locale=vn&vnp_OrderInfo=sac_fast_50000_VND&vnp_OrderType=billpayment&vnp_ReturnUrl=https://1395-171-225-184-76.ngrok-free.app/payment_return/&vnp_TmnCode=1O1EZ0CY&vnp_TxnRef=20250406231147&vnp_Version=2.1.0'

hash_result = hmac.new(secret_key.encode(), hash_data.encode(), hashlib.sha512).hexdigest()
print("✅ Secure Hash:", hash_result)
