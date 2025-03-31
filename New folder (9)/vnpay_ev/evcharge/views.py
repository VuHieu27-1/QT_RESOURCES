from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.conf import settings
from django.shortcuts import render
from datetime import datetime
from .vnpay import VNPay
import json
import logging
import os
log_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'charging_log.txt')
logging.basicConfig(
    filename=log_file,
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)
@csrf_exempt
def create_payment(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        mode = data.get('mode', 'fast')  # fast / slow
        amount = int(data.get('amount', 0))  # tiền người dùng nhập

        order_id = datetime.now().strftime('%Y%m%d%H%M%S')

        vnp = VNPay()
        vnp.request_data = {
            'vnp_Version': '2.1.0',
            'vnp_Command': 'pay',
            'vnp_TmnCode': settings.VNPAY_TMN_CODE,
            'vnp_Amount': amount * 100,
            'vnp_CurrCode': 'VND',
            'vnp_TxnRef': order_id,
            'vnp_OrderInfo': f"Sạc xe chế độ {mode} - {amount} VND",
            'vnp_OrderType': 'billpayment',
            'vnp_Locale': 'vn',
            'vnp_CreateDate': datetime.now().strftime('%Y%m%d%H%M%S'),
            'vnp_IpAddr': request.META.get('REMOTE_ADDR'),
            'vnp_ReturnUrl': settings.VNPAY_RETURN_URL
        }

        payment_url = vnp.get_payment_url(settings.VNPAY_PAYMENT_URL, settings.VNPAY_HASH_SECRET_KEY)
        return JsonResponse({'payment_url': payment_url})

    return JsonResponse({'error': 'Method not allowed'}, status=405)

def payment_return(request):
    vnp = VNPay()
    vnp.response_data = request.GET.dict()

    if vnp.validate_response(settings.VNPAY_HASH_SECRET_KEY):
        if vnp.response_data.get("vnp_ResponseCode") == "00":
            logging.info(f"Giao dịch thành công | Mã: {vnp.response_data.get('vnp_TransactionNo')} | Số tiền: {int(vnp.response_data.get('vnp_Amount')) / 100} VND")
            result = "✅ Thanh toán thành công!"
        else:
            logging.error("❌ Giao dịch thất bại.")
            result = "❌ Thanh toán thất bại!"

        return render(request, "payment_return.html", {
            "result": result,
            "data": vnp.response_data
        })
    else:
        return render(request, "payment_return.html", {
            "result": "⚠️ Sai checksum! Giao dịch không hợp lệ!",
            "data": {}
        })

