from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings
from django.shortcuts import render
from datetime import datetime, timedelta
from datetime import datetime
from .vnpay import VNPay
import json
import logging
import os

# Thiết lập logging
log_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'charging_log.txt')
logging.basicConfig(
    filename=log_file,
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)
def get_client_ip(request):
    x_forwarded_for = request.META.get('HTTP_X_FORWARDED_FOR')
    if x_forwarded_for:
        return x_forwarded_for.split(',')[0]
    return request.META.get('REMOTE_ADDR', '127.0.0.1')
# View tạo thanh toán
@csrf_exempt
def create_payment(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    try:
        data = json.loads(request.body)
        mode = data.get('mode', 'fast')
        amount = int(data.get('amount', 0))
        if amount <= 0:
            return JsonResponse({'error': 'Số tiền không hợp lệ'}, status=400)
    except Exception as e:
        logging.error(f"Lỗi phân tích request JSON: {e}")
        return JsonResponse({'error': 'Dữ liệu gửi lên không hợp lệ'}, status=400)
    create_date = datetime.now()
    order_id = create_date.strftime('%Y%m%d%H%M%S')
    expire_date = create_date + timedelta(minutes=15)

    vnp = VNPay()
    vnp.request_data = {
        'vnp_Version': '2.1.0',
        'vnp_Command': 'pay',
        'vnp_TmnCode': settings.VNPAY_TMN_CODE,
        'vnp_Amount': amount * 100,
        'vnp_CurrCode': 'VND',
        'vnp_TxnRef': order_id,
        'vnp_OrderInfo': f'sac_{mode}_{amount}_VND',   
        'vnp_OrderType': 'topup',  # hoặc 'topup'
        'vnp_Locale': 'vn',
        'vnp_CreateDate': create_date.strftime('%Y%m%d%H%M%S'),
        'vnp_ExpireDate': expire_date.strftime('%Y%m%d%H%M%S'),
        'vnp_IpAddr': get_client_ip(request),  # fallback nếu không có
        'vnp_ReturnURL': settings.VNPAY_RETURN_URL
    }

    payment_url = vnp.get_payment_url(settings.VNPAY_PAYMENT_URL, settings.VNPAY_HASH_SECRET_KEY)
    logging.info(f"🔗 Tạo URL thanh toán: {payment_url}")
    return JsonResponse({'payment_url': payment_url})


# View nhận kết quả trả về
@csrf_exempt
def payment_return(request):
    print("⚡ ĐÃ NHẬN VNPAY CALLBACK:")
    for k, v in request.GET.items():
        print(f"{k}: {v}")
    vnp = VNPay()
    vnp.response_data = request.GET.dict()

    if vnp.validate_response(settings.VNPAY_HASH_SECRET_KEY):
        response_code = vnp.response_data.get("vnp_ResponseCode")
        amount = int(vnp.response_data.get('vnp_Amount', 0)) / 100
        txn_no = vnp.response_data.get("vnp_TransactionNo", "N/A")

        if response_code == "00":
            logging.info(f"✅ Giao dịch thành công | Mã giao dịch: {txn_no} | Số tiền: {amount} VND")
            result = "✅ Thanh toán thành công!"
        else:
            logging.warning(f"❌ Giao dịch thất bại | Mã giao dịch: {txn_no} | Mã phản hồi: {response_code}")
            result = "❌ Thanh toán thất bại!"
    else:
        logging.error("⚠️ Sai checksum từ VNPAY callback. Giao dịch không hợp lệ!")
        result = "⚠️ Sai checksum! Giao dịch không hợp lệ!"
        vnp.response_data = {}

    return render(request, "payment_return.html", {
        "result": result,
        "data": vnp.response_data
    })
