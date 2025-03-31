from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings
from urllib.parse import urlencode
import hmac
import hashlib

@csrf_exempt
def vnpay_ipn(request):
    input_data = request.GET
    if not input_data:
        return JsonResponse({'RspCode': '99', 'Message': 'Invalid request'})

    vnp_data = input_data.dict()

    # Lưu lại các giá trị cần thiết
    vnp_secure_hash = vnp_data.pop('vnp_SecureHash', None)
    sorted_data = sorted(vnp_data.items())
    hash_data = '&'.join([f"{k}={v}" for k, v in sorted_data])

    # Tạo hash mới từ secret key
    computed_hash = hmac.new(
        bytes("092LVCTP288T25T103HGYL6ZKXBOSVQL", 'utf-8'),
        bytes(hash_data, 'utf-8'),
        hashlib.sha512
    ).hexdigest()

    print("🔍 HASH DATA:", hash_data)
    print("✅ HASH TỪ VNPay:", vnp_secure_hash)
    print("✅ HASH CỦA MÌNH:", computed_hash)

    if vnp_secure_hash != computed_hash:
        return JsonResponse({'RspCode': '97', 'Message': 'Invalid Signature'})

    order_id = vnp_data.get('vnp_TxnRef')
    amount = int(vnp_data.get('vnp_Amount')) / 100
    response_code = vnp_data.get('vnp_ResponseCode')
    transaction_no = vnp_data.get('vnp_TransactionNo')

    # Giả lập kiểm tra đơn hàng
    first_time = True  # bạn nên kiểm tra từ DB
    correct_amount = True  # kiểm tra giá trị đơn hàng

    if not correct_amount:
        return JsonResponse({'RspCode': '04', 'Message': 'Invalid amount'})

    if first_time:
        if response_code == '00':
            print("✅ Thanh toán thành công cho đơn hàng", order_id)
        else:
            print("❌ Giao dịch không thành công. Mã:", response_code)

        return JsonResponse({'RspCode': '00', 'Message': 'Confirm Success'})
    else:
        return JsonResponse({'RspCode': '02', 'Message': 'Order Already Updated'})
