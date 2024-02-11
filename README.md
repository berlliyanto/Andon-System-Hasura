# Modular Andon System
Improvisasi dari versi andon sebelumnya menggunakan sistem komunikasi master dan slave.

## A. Pendahuluan
#### Master
Master berfungsi sebagai otak atau jembatan penghubung antara slave 1 dan slave 2, master juga yang menangani dalam request dan response dari server. Master menerima input dari RFID dan menggunakan module Ethernet supaya memiliki koneksi internet.

#### Slave
Slave berfungsi sebagai kontroller yang menerima input dari sensor dan aktuator. Slave 1 menerima input dari TFT, Proximity, dan relay, serta menampilkan output pada TFT. Sedangkan Slave 2 memberi output pada Panel DMD RGB untuk menampilkan status dan calling.

## B. Daftar Komponen

### 1. Master (ESP)
    - ESP 32
    - RFID
    - Ethernet ENC28J60
    - Step Down 5V DC to 3.3 V DC
    - Push Button

### 2. Slave 1 (ESP)
    - ESP 32
    - TFT LCD 3.5 Inch
    - Sensor Proximity Capacitive
    - Step UP 5V DC to 24V DC
    - Relay 24V

### 3. Slave 2 (ESP)
    - ESP 32
    - Panel DMD RGB (HUB 75)

## C. Arsitektur Sistem

Berikut merupakan arsitektur sistem yang telah dibuat.

![system](/images/system_architecture.PNG)

Pada arsitektur sistem diatas terdapat 1 Master unit dan 2 buah slave. Master dan slave akan melakukan komunikasi secara serial melalui pin Rx dan Tx pada masing - masing alat. Data yang dikirim melalui serial adalah berbentuk JSON, sebagai contoh saat pengguna melakukan aksi panggilan terhadap orang maintenance maka slave 1 akan mengirim JSON {"state": "call", "type": "Maintenance"} ke master. Lalu master akan meyimpan dulu data JSON yang diterima dari slave 1 dan akan mengirimkan kembali ke kedua slave jika master berhasil menerima response 200 dari server. Selanjutnya karena kedua slave sudah menerima kembali data JSON dari master maka akan dilakukan pembaharuan state atau tampilan pada setiap slave. Kondisi tadi berlaku hanya untuk operasi call, serve, end dan add mp, khusus untuk operasi change yaitu perubahan status maka JSON yang sebelumnya disimpan akan ditimpa oleh JSON yang berasal dari server.

## D. Persyaratan
- Download [Arduino IDE](https://www.arduino.cc/en/software).
- Download [ESP32 Board Manager](https://github.com/iotechbugs/esp32-arduino) atau bisa download melalui board manager.
- Komponen yang sudah disebutkan sebelumnya.
- Download library yang dibutuhkan.

## E. Library
- ArduinoJson
- SPI
- EthernetENC
- MFRC522
- Adafruit_GFX
- MCUFRIEND_kbv
- TouchScreen
- qrcode
- ESP32-HUB75-MatrixPanel-I2S-DMA

## F. Catatan Penting

### 1. Program / Sintaks
- Dalam komunikasi serial pada JSOn selalu sertakan key "state" dan "type" sebagai identitas dari operasi yang akan dilakukan
    - Perhatikan key pertama yang diterima dari reponse body
    - char* body = strstr(bodyStart, "{\"data\":{\"work_center_id\":");