#include <stdio.h>
#include "headerHotel.h"


int main()
{

    //opening program hotel

        printf("============================================================\n");
        printf("        SELAMAT DATANG DI SISTEM MANAJEMEN THE ZENITH HOTEL  \n");
        printf("============================================================\n");
        printf("  Nikmati kemudahan dalam mengelola dan memesan kamar hotel.\n");
        printf("  Sistem ini dirancang untuk membantu:\n");
        printf("    - Tamu dalam memesan kamar secara cepat dan mudah\n");
        printf("    - Admin dalam mengelola kamar, tamu, dan data hotel\n");
        printf("\n");
        printf("  Silakan lanjutkan untuk memilih mode login Anda.\n");
        printf("============================================================\n\n");
    

    HotelData hotel;
    initHotel(&hotel);

    // insert data kamar awal ke hach table
    printf("Memuat data kamar awal ke dalam sistem...\n");
    insertKamarToHash(&hotel, 1, "Standard", 300000, "TV, AC");
    insertKamarToHash(&hotel, 2, "Deluxe", 450000, "TV, AC, Air Panas");
    insertKamarToHash(&hotel, 3, "Suite", 600000, "TV, AC, Air Panas");
    insertKamarToHash(&hotel, 4, "Standard", 300000, "TV, AC");
    insertKamarToHash(&hotel, 5, "VIP", 800000, "Full Service");

    // tampilkan sekali jika proses insert berhasil
    printf("data awal kamar berhasil dimasukkan ke hash table\n");

    menuLogin(&hotel);

    // Akhiri program: bebaskan semua memori dinamis
    hapusSemuaDataHotel(&hotel);


    
    return 0;
}
