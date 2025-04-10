from django.urls import path
from . import views

urlpatterns = [
    path('payment_return/', views.payment_return, name='payment_return'),
    path('create_payment/', views.create_payment, name='create_payment'),
]
