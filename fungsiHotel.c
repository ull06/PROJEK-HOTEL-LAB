#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  "headerHotel.h"

#define MAX_KAMAR 200
#define HASH_SIZE 211

// hash function
int hashFunc(int noKamar)
{
    return noKamar % HASH_SIZE;
}

// fungsi hash
void initHash(HashKamar *hash)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hash->table[i] = NULL;
    }
}

// inisialisasi queue riwayat check Out
void initRiwayatCheckOut(RiwayatCheckOutQueue *q)
{
    q->front = q->rear = NULL;
}

// inisialisasi checkIn list
void initCheckList(CheckInList *q)
{
    q->front = q->rear = NULL;
}

// fungsi inisialiasi waiting list
void initWaitingList(WaitingList *q)
{
    q->front = q->rear = NULL;
}

// fungsi inisialisasi stack pembatalan
void initStackPembatalan(StackPembatalan *s)
{
    s->top = NULL;
    s->size = 0;
}

// fungsi inisialisasi data hotel
void initHotel(HotelData *hotel)
{
    initHash(&hotel->hashKamar);                    // hash table kamar
    initCheckList(&hotel->antreanCheckIn);          // antrean check -in
    initCheckList(&hotel->tamuAktif);               // tamu aktif
    initWaitingList(&hotel->antreanWaitingList);    // waiting list
    initRiwayatCheckOut(&hotel->riwayatCheckOut);   // riwayat check-out
    initStackPembatalan(&hotel->riwayatPembatalan); // riwayat pembatalan
}

// fungsi untuk menambahkan tamu ke dalam stack
void pushPembatalan(StackPembatalan *ps, Tamu *tamu, const char *alasan)
{
    PembatalanPtr baru = malloc(sizeof(Pembatalan));
    if (baru == NULL)
    {
        printf("error: gagal mengalokasikan memori!\n");
        return;
    }
    baru->tamu = tamu;
    strncpy(baru->alasan, alasan, sizeof(baru->alasan) - 1);
    baru->alasan[sizeof(baru->alasan) - 1] = '\0';
    baru->next = ps->top;
    ps->top = baru;
    ps->size++;
}

// fungsi undo pembatalan
void undoPembatalan(StackPembatalan *ps)
{
    if (ps == NULL || ps->top == NULL)
    {
        printf("Tidak ada pembatalan untuk di-undo!\n");
        return;
    }
    PembatalanPtr undo = ps->top;
    ps->top = ps->top->next;
    free(undo);
    ps->size--;
}

// fungsi untuk menghitung kapasitas kamar
int hitungTotalKamar(const HashKamar *hash)
{
    int total = 0;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hash->table[i];
        while (current != NULL)
        {
            total++;
            current = current->next;
        }
    }
    return total;
}

// menu untuk informasi kamar hotel
void daftarTamuAktif(HotelData *hotel)
{
    printf("\n========================= Daftar Tamu AKtif =============================\n");

    if (hotel == NULL || hotel->tamuAktif.front == NULL)
    {
        printf("Tidak ada tamu yang sedang menginap\n");
        return;
    }

    int jumlahTamuAktif = 1;
    CheckInPtr current = hotel->tamuAktif.front;
    while (current != NULL)
    {
        jumlahTamuAktif++;
        current = current->next;
    }

    int no = 1;
    printf("------------------------------------------------------------------\n");
    printf("| %-4s | %-50s | %-10s | %-15s |\n", "no", "Nama Tamu", "No. Kamar", "Status");
    printf("------------------------------------------------------------------\n");

    current = hotel->tamuAktif.front;

    while (current != NULL)
    {
        printf("| %-4d | %-50s | %-10d | %-15s |\n",
               no,
               current->tamu->namaTamu,
               current->tamu->noKamar,
               "Checked-In");
        current = current->next;
        no++;
    }
    printf("------------------------------------------------------------------\n");
}

void tampilkanAntreanCheckIn(HotelData *hotel)
{

    printf("\n=================== Antrean Tamu Check-In ====================\n");

    if (hotel == NULL || hotel->antreanCheckIn.front == NULL)
    {
        printf("Tidak ada antrean check-in.\n");
        return;
    }

    // Hitung jumlah tamu dalam antrean
    int jumlahTamu = 0;
    CheckInPtr current = hotel->antreanCheckIn.front;
    while (current != NULL)
    {
        if (current->tamu->statusPemesanan != -1)
            jumlahTamu++;
        current = current->next;
    }

    // Tampilkan jumlah tamu
    printf("Jumlah tamu dalam antrean check-in: %d\n", jumlahTamu);

    printf("--------------------------------------------------------------\n");
    printf("| %-4s | %-20s | %-10s | %-15s |\n", "No", "Nama Tamu", "No. Kamar", "Status");
    printf("--------------------------------------------------------------\n");

    current = hotel->antreanCheckIn.front;
    int no = 1;
    while (current != NULL)
    {

        printf("| %-4d | %-20s | %-10d | %-15s |\n",
               no,
               current->tamu->namaTamu,
               current->tamu->noKamar,
               "Booking");
        current = current->next;
        no++;
    }

    printf("--------------------------------------------------------------\n");
}

void tampilkanWaitingList(HotelData *hotel)
{
    printf("\n========== Antrean Waiting List ===========\n");

    if (hotel == NULL || hotel->antreanWaitingList.front == NULL)
    {
        printf("Tidak ada antrean waiting list.\n");
        return;
    }

    printf("-------------------------------------\n");
    printf("| %-20s | %-10s |\n", "Nama Tamu", "No. Kamar");
    printf("-------------------------------------\n");

    WaitingListPtr current = hotel->antreanWaitingList.front;
    while (current != NULL)
    {
        printf("| %-20s | %-10d |\n",
               current->tamu->namaTamu,
               current->tamu->noKamar);
        current = current->next;
    }

    printf("-------------------------------------\n");
}

void tampilkanRiwayatPembatalan(HotelData *hotel)
{
    printf("\n=================== Riwayat Pembatalan ====================\n");

    if (hotel == NULL || hotel->riwayatPembatalan.top == NULL)
    {
        printf("Tidak ada riwayat pembatalan.\n");
        return;
    }

    printf("---------------------------------------------------------\n");
    printf("| %-20s | %-30s |\n", "Nama Tamu", "Alasan Pembatalan");
    printf("---------------------------------------------------------\n");

    PembatalanPtr current = hotel->riwayatPembatalan.top;
    while (current != NULL)
    {
        printf("| %-20s | %-30s |\n",
               current->tamu->namaTamu,
               current->alasan);
        current = current->next;
    }

    printf("---------------------------------------------------------\n");
}

int inputMenuAman(int batasBawah, int batasAtas)
{
    char buffer[100];
    int pilihan;
    while (1)
    {
        printf("Pilih menu (%d-%d): ", batasBawah, batasAtas);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            // cek jika buffer hanya newline (user tekan enter saja)
            if (buffer[0] == '\n')
            {
                printf("Input kosong! Silakan masukkan angka.\n");
                continue;
            }
            // validasi angka
            if (sscanf(buffer, "%d", &pilihan) == 1)
            {
                if (pilihan >= batasBawah && pilihan <= batasAtas)
                {
                    return pilihan;
                }
                else
                {
                    printf("Pilihan di luar rentang! Masukkan angka antara %d dan %d.\n", batasBawah, batasAtas);
                }
            }
            else
            {
                printf("Input tidak valid! Masukkan angka saja.\n");
            }
        }
        else
        {
            printf("Terjadi kesalahan saat membaca input.\n");
        }
    }
}
void tampilkanRiwayatCheckOut(HotelData *hotel)
{
    if (hotel == NULL || hotel->riwayatCheckOut.front == NULL)
    {
        printf("\nBelum ada tamu yang checkout.\n");
        return;
    }

    printf("\n=== Riwayat Check-Out Tamu ===\n");
    printf("+-----+----------------------+\n");
    printf("| No  | Nama Tamu            |\n");
    printf("+-----+----------------------+\n");

    CheckOutPtr current = hotel->riwayatCheckOut.front;
    int nomor = 1;
    while (current != NULL)
    {
        printf("| %-3d | %-20s |\n", nomor++, current->tamu->namaTamu);
        current = current->next;
    }

    printf("+-----+----------------------+\n");
}

void informasiHotel(HotelData *hotel)
{
    int pilihan;

    do
    {
        printf("\n ==== Informasi Hotel ===\n");
        printf("1. Melihat daftar tamu aktif\n");
        printf("2. Melihat daftar antrean check in\n");
        printf("3. Melihat daftar antrean waiting list\n");
        printf("4. Melihat riwayat check-out\n");
        printf("5. Melihat Riwayat pembatalan kamar oleh tamu\n");
        printf("6. Kembali\n");
        pilihan = inputMenuAman(1, 6);

        switch (pilihan)
        {
        case 1:
            daftarTamuAktif(hotel);
            break;
        case 2:
            tampilkanAntreanCheckIn(hotel);
            break;
        case 3:
            tampilkanWaitingList(hotel);
            break;
        case 4:
            tampilkanRiwayatCheckOut(hotel);
            break;
        case 5:
            tampilkanRiwayatPembatalan(hotel);
            break;
        case 6:
            printf("Kembali ke menu utama...\n");
            return;
        default:
            printf("Pilihan tidak valid!\n");
            break;
        }

    } while (pilihan != 6);
}

// fungsi memasukkan tamu ke antrean check in
void tambahTamuKeAntreanCheckIn(CheckInList *queue, TamuPtr tamu)
{
    CheckInPtr newNode = (CheckInPtr)malloc(sizeof(AntreanCheckIn));
    if (newNode == NULL)
    {
        printf("Gagal alokasi memori untuk antrean check-in.\n");
        return;
    }

    newNode->tamu = tamu;
    newNode->next = NULL;

    if (queue->rear == NULL)
    {
        // Antrean kosong
        queue->front = queue->rear = newNode;
    }
    else
    {
        // Tambah ke belakang antrean (FIFO)
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}
/ fungsi untuk proses check in
void checkInTamu(HotelData *hotel)
{
    if (hotel->antreanCheckIn.front == NULL)
    {
        printf("\nTidak ada tamu dalam antrean check-in.\n");
        return;
    }

    // Ambil tamu dari antrean check-in (dequeue)
    CheckInPtr checkInNode = hotel->antreanCheckIn.front;
    TamuPtr tamu = checkInNode->tamu;
    hotel->antreanCheckIn.front = checkInNode->next;
    if (hotel->antreanCheckIn.front == NULL)
        hotel->antreanCheckIn.rear = NULL;

    // Tandai tamu sudah check-in
    tamu->statusPemesanan = 1;

    // Tandai kamar sebagai terisi
    int hashIndex = hashFunc(tamu->noKamar);
    KamarNodePtrt kamarNode = hotel->hashKamar.table[hashIndex];
    while (kamarNode != NULL)
    {
        if (kamarNode->data.noKamar == tamu->noKamar)
        {
            kamarNode->data.status = 1; // terisi
            break;
        }
        kamarNode = kamarNode->next;
    }

    // Masukkan tamu ke antrean tamuAktif (enqueue)
    CheckInPtr aktifBaru = malloc(sizeof(AntreanCheckIn));
    if (!aktifBaru)
    {
        printf("Gagal alokasi memori untuk tamu aktif.\n");
        return;
    }
    aktifBaru->tamu = tamu;
    aktifBaru->next = NULL;
    if (hotel->tamuAktif.rear == NULL)
    {
        hotel->tamuAktif.front = hotel->tamuAktif.rear = aktifBaru;
    }
    else
    {
        hotel->tamuAktif.rear->next = aktifBaru;
        hotel->tamuAktif.rear = aktifBaru;
    }

    // Menampilkan tabel check-in
    printf("\n+----+----------------------+-------------+---------------------+\n");
    printf("| No | Nama Tamu            | Nomor Kamar | Status Kamar        |\n");
    printf("+----+----------------------+-------------+---------------------+\n");

    // Menampilkan daftar tamu yang sudah check-in
    CheckInPtr tamuAktifNode = hotel->tamuAktif.front;
    int nomorUrut = 1; // mulai dari nomor urut 2
    while (tamuAktifNode != NULL)
    {
        printf("| %-2d | %-20s | %-10d | %-19s |\n", nomorUrut, tamuAktifNode->tamu->namaTamu, tamuAktifNode->tamu->noKamar, tamuAktifNode->tamu->statusPemesanan == 1 ? "Terisi" : "Kosong");
        tamuAktifNode = tamuAktifNode->next;
        nomorUrut++;
    }

    printf("+----+----------------------+-------------+---------------------+\n");

    printf("\nTamu '%s' telah berhasil check-in ke kamar %d.\n", tamu->namaTamu, tamu->noKamar);
    free(checkInNode); // hapus node antrean check-in
}

