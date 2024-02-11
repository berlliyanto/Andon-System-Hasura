# Modular Andon System
Improvisasi dari versi andon sebelumnya menggunakan sistem komunikasi master dan slave.

## A. Peran
#### Master
Master berfungsi sebagai otak atau jembatan penghubung antara slave 1 dan slave 2, master juga yang menangani dalam request dan response dari server. Master menerima input dari RFID dan menggunakan module Ethernet supaya memiliki koneksi internet.

#### Slave
Slave berfungsi sebagai kontroller yang menerima input dari sensor dan aktuator. Slave 1 menerima input dari TFT, Proximity, dan relay, serta menampilkan output pada TFT. Sedangkan Slave 2 memberi output pada Panel DMD RGB untuk menampilkan status dan calling.

## B. Daftar Komponen

### 1. Master (ESP)
    a. ESP 32
    b. RFID
    c. Ethernet ENC28J60
    d. Step Down 5V DC to 3.3 V DC
    e. Push Button

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

![system](/images/system_architecture.PNG)