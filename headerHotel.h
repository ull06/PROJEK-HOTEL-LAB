#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_KAMAR 200
#define HASH_SIZE 211

typedef struct tamu *TamuPtr;
typedef struct tamu
{
    char namaTamu[50];
    int noKamar;
    int statusPemesanan;
    /* 0: booking (antrean chekc in), 1: check in (menginap), 2: check -out
    3: waiting list, -1: pemesanan dibatalkan*/
    TamuPtr next; //pointer ke linked list tamu
} Tamu;

typedef struct
{
    int noKamar;
    int harga;
    char tipeKamar[100];
    char fasilitas[100];
    int status; // 0 = kosong, 1 = terisi
    TamuPtr tamu; // pointer ke data tamu 
} Kamar;

typedef struct kamarNode *KamarNodePtrt;
typedef struct kamarNode{
    Kamar data;
    KamarNodePtrt next; // Pointer ke node berikutnya jika terjadi collision dalam hash table
} KamarNode;


typedef struct {
    KamarNodePtrt table[HASH_SIZE];
}HashKamar;




// queueu antrean check in
typedef struct antreanCheckIn *CheckInPtr;
typedef struct antreanCheckIn
{
    TamuPtr tamu;
    CheckInPtr next;
} AntreanCheckIn;

typedef struct
{
    CheckInPtr front, rear;
} CheckInList;

typedef struct riwayatCheckOut *CheckOutPtr;
typedef struct riwayatCheckOut
{
    TamuPtr tamu;
    CheckOutPtr next;

} RiwayatCheckOut;

typedef struct riwayatCheckOutQueue
{
    CheckOutPtr front, rear;
} RiwayatCheckOutQueue;

// queueu untuk antrean waiting list
typedef struct antreanWaitingList *WaitingListPtr;
typedef struct antreanWaitingList
{
    TamuPtr tamu;        // pointer ke sturct tamu
    WaitingListPtr next; // pointer ke waiting list berikutnya
} AntreanWaitingList;

typedef struct
{
    WaitingListPtr front, rear;
} WaitingList;

// untuk pembatalan
typedef struct pembatalan *PembatalanPtr;
typedef struct pembatalan
{
    char alasan[100]; //tempat nyimpan alasan
    TamuPtr tamu; //pointer ke data tamu
    PembatalanPtr next;
} Pembatalan;

typedef struct stackPembatalan
{
    PembatalanPtr top;
    unsigned size;
} StackPembatalan;


typedef struct hotelData
{
    HashKamar hashKamar;      // daftar semua kamar hotel
   // int totalKamar;    // jumlah kamar terdaftar
    CheckInList antreanCheckIn;        // tamu yang sudh booking tapi belum chek in
    CheckInList tamuAktif;             // taamu yang sudah chek in, sdh menginap
    WaitingList antreanWaitingList;    // antrean jika kamar yang dipesan penuh
    RiwayatCheckOutQueue riwayatCheckOut;
    StackPembatalan riwayatPembatalan; // riwayat pembatalan
} HotelData;

int hashFunc(int noKamar);
void initHash(HashKamar *hash);
void initRiwayatCheckOut(RiwayatCheckOutQueue *q);
void initCheckList(CheckInList *q);
void initWaitingList(WaitingList *q);
void initStackPembatalan (StackPembatalan *s);
void initHotel(HotelData *hotel);
void pushPembatalan(StackPembatalan *ps, Tamu *tamu, const char *alasan);
void undoPembatalan(StackPembatalan *ps);
int hitungTotalKamar(const HashKamar *hash);
void daftarTamuAktif(HotelData *hotel);
void tampilkanAntreanCheckIn(HotelData *hotel);
void tampilkanWaitingList(HotelData *hotel);
void tampilkanRiwayatPembatalan(HotelData *hotel);
void tampilkanRiwayatCheckOut(HotelData *hotel);
void informasiHotel(HotelData *hotel);
void tambahTamuKeAntreanCheckIn(CheckInList *queue, TamuPtr tamu);
void checkInTamu(HotelData *hotel);
void cekWaitingListSetelahCheckOut(HotelData *hotel, int noKamar);
void checkOutTamu(HotelData *hotel);
void insertKamarToHash(HotelData *hotel, int noKamar, char *tipeKamar, int harga, char *fasilitas);
void tambahKamar(HotelData *hotel);
void updateHarga(HotelData *hotel);
void updateFasilitas(HotelData *hotel);
void updateTipeKamar(HotelData *hotel);
void tampilkanKamar(const HotelData *hotel);
void kelolaKamar(HotelData *hotel);
void cariTipeKamar(HotelData *hotel);
void cariHargaKamar(HotelData *hotel);
void cariNoKamar(HotelData *hotel);
void cariNamaTamu(const HotelData *hotel);
void pencarianData(HotelData *hotel);
void menuAdmin(HotelData *hotel);
int salinKamarKeArray(HotelData *hotel, Kamar array[]);
void swapKamar(Kamar *a, Kamar *b);
void heapify(Kamar array[], int n, int i);
void heapSort(Kamar array[], int n);
void tampilkanKamarUrutHarga(HotelData *hotel);
void lihatKamar(HotelData *hotel);
Kamar *cekKetersediaanKamarUntukBooking(HotelData *hotel, int nomor);
void tampilkanKamarKosong(HotelData *hotel);
void prosesPemesananLangsung(HotelData *hotel, int noKamar);
void tambahTamuKeWaitingList(WaitingList *list, TamuPtr tamu);
void prosesMasukWaitingList(HotelData *hotel, int noKamar);
void pemesananKamar(HotelData *hotel);
void hapusDariAntreanCheckIn(HotelData *hotel, TamuPtr tamu); 
void batalkanPemesanan(HotelData *hotel);
void cekStatusTamu(HashKamar *hotel);
void tampilkanPanduan();
void menuTamu(HotelData *hotel);
void hapusSemuaKamar(HotelData *hotel);
void hapusCheckInList(CheckInList *list);
void hapusWaitingList(WaitingList *list);
void hapusStackPembatalan(StackPembatalan *stack);
void hapusRiwayatCheckOut(RiwayatCheckOutQueue *queue);
void hapusSemuaDataHotel(HotelData *hotel);
void sambutanTamu();
void sambutanAdmin();
void menuLogin(HotelData *hotel);

