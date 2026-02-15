# Dynamic-Tensor-TinyML-Optimization
Kısıtlı bellek kaynaklarına sahip mikrodenetleyiciler (TinyML) için C dilinde geliştirilmiş, dinamik veri yapılı ve Quantization destekli tensör kütüphanesi.

Bu proje, kısıtlı bellek kaynaklarına (RAM) sahip mikrodenetleyiciler (Arduino, ESP32 vb.) üzerinde yapay zeka modellerini verimli bir şekilde çalıştırmak için tasarlanmış dinamik bir tensör kütüphanesidir.

## 🚀 Projenin Amacı
Geleneksel 32-bit Float dizileri, düşük güçlü cihazların belleğini hızla tüketir. Bu kütüphane, `union` yapısı kullanarak aynı bellek alanında ihtiyaca göre farklı hassasiyetlerde (Float32, Int16, Int8) veri saklanmasına olanak tanır.

## 🛠️ Teknik Özellikler
* **Dinamik Veri Yapısı:** `typedef union` ve `struct` kullanılarak bellek verimliliği sağlandı.
* **Quantization (Nicemleme):** Float32 verileri Int8 formatına sıkıştırarak bellek kullanımında %75 tasarruf sağlar.
* **Matris Yönetimi:** 2D matris (rows/cols) desteği ile katman tabanlı hesaplamalara uygundur.
* **Bellek Yönetimi:** `malloc` ve `free` fonksiyonları ile dinamik bellek kontrolü sağlanmaktadır.

## 📈 Quantization Sonuçları
Sistem, float verileri int8'e dönüştürdüğünde sağlanan tasarrufu otomatik olarak hesaplar:
- **Eski Boyut (Float32):** 4 Byte / eleman
- **Yeni Boyut (Int8):** 1 Byte / eleman
- **Sağlanan Tasarruf:** %75

## 💻 Kurulum ve Çalıştırma
Projeyi Xcode veya herhangi bir C derleyicisi (gcc) ile derleyebilirsiniz:
```bash
gcc main.c -o tensor_project
./tensor_project
