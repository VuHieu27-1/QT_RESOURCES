from django.urls import path
from .views import create_payment, payment_return 

urlpatterns = [
    path('create_payment/', create_payment, name='create_payment'),
    path('payment_return/', payment_return, name='payment_return'),
]
