# vnpay_ev/urls.py
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('evcharge.urls')),  # route vào app evcharge
]
