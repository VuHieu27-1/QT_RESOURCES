# evcharge/views.py
import json
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from .vnpay import vnpay
from django.conf import settings
from datetime import datetime

@csrf_exempt
def create_payment(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        mode = data['mode']
        amount = int(data['amount'])

        order_id = datetime.now().strftime('%Y%m%d%H%M%S')
        vnp = vnpay()
        vnp.requestData = {
            'vnp_Version': '2.1.0',
            'vnp_Command': 'pay',
            'vnp_TmnCode': settings.VNPAY_TMN_CODE,
            'vnp_Amount': amount * 100,
            'vnp_CurrCode': 'VND',
            'vnp_TxnRef': order_id,
            'vnp_OrderInfo': f"Sac xe - {mode} - {amount} VND",
            'vnp_OrderType': 'billpayment',
            'vnp_Locale': 'vn',
            'vnp_CreateDate': datetime.now().strftime('%Y%m%d%H%M%S'),
            'vnp_IpAddr': '127.0.0.1',
            'vnp_ReturnUrl': settings.VNPAY_RETURN_URL
        }

        url = vnp.get_payment_url(settings.VNPAY_PAYMENT_URL, settings.VNPAY_HASH_SECRET_KEY)
        return JsonResponse({'payment_url': url})
    return JsonResponse({'error': 'Method not allowed'}, status=405)
