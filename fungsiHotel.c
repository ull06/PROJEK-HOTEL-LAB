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
// fungsi untuk proses check in
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
// fungsi untuk mengecek waiting list saat kamar baru kosong (check out)
void cekWaitingListSetelahCheckOut(HotelData *hotel, int noKamar)
{
    WaitingListPtr current = hotel->antreanWaitingList.front;
    WaitingListPtr prev = NULL;

    while (current != NULL)
    {
        if (current->tamu->noKamar == noKamar)
        {
            // Tamu ini sedang menunggu kamar yang baru kosong
            TamuPtr tamuSiapMasuk = current->tamu;

            // Hapus dari antrean waiting list
            if (prev == NULL)
            {
                hotel->antreanWaitingList.front = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            if (current == hotel->antreanWaitingList.rear)
            {
                hotel->antreanWaitingList.rear = prev;
            }

            free(current);

            printf("\nKamar %d kini kosong. Tamu '%s' dari waiting list diproses.\n",
                   noKamar, tamuSiapMasuk->namaTamu);

            // Masukkan ke antrean check-in
            tambahTamuKeAntreanCheckIn(&hotel->antreanCheckIn, tamuSiapMasuk);

            // Update status kamar
            int idx = hashFunc(noKamar);
            KamarNodePtrt node = hotel->hashKamar.table[idx];
            while (node != NULL)
            {
                if (node->data.noKamar == noKamar)
                {
                    node->data.status = 1;
                    break;
                }
                node = node->next;
            }

            printf("%s' telah dipindahkan ke antrean check-in untuk kamar %d.\n",
                   tamuSiapMasuk->namaTamu, noKamar);
            return; // hanya ambil satu tamu dari waiting list
        }

        prev = current;
        current = current->next;
    }
}

// fungsi untuk proses check out
void checkOutTamu(HotelData *hotel)
{
    if (hotel->tamuAktif.front == NULL)
    {
        printf("\nTidak ada tamu yang sedang menginap.\n");
        return;
    }

    // Ambil tamu dari antrean tamuAktif (dequeue)
    CheckInPtr tamuNode = hotel->tamuAktif.front;
    TamuPtr tamu = tamuNode->tamu;
    hotel->tamuAktif.front = tamuNode->next;
    if (hotel->tamuAktif.front == NULL)
        hotel->tamuAktif.rear = NULL;

    // Tandai tamu telah check-out
    tamu->statusPemesanan = 2;

    // Tandai kamar sebagai kosong
    int hashIndex = hashFunc(tamu->noKamar);
    KamarNodePtrt kamarNode = hotel->hashKamar.table[hashIndex];
    int ditemukan = 0;
    while (kamarNode != NULL)
    {
        if (kamarNode->data.noKamar == tamu->noKamar)
        {
            kamarNode->data.status = 0; // Kamar jadi kosong
            ditemukan = 1;
            break;
        }
        kamarNode = kamarNode->next;
    }
    if (!ditemukan)
    {
        printf("Peringatan: Kamar nomor %d tidak ditemukan di sistem\n", tamu->noKamar);
    }

    // cek apakah ada tamu di waiting list untuk kamar ini
    cekWaitingListSetelahCheckOut(hotel, tamu->noKamar);

    // Masukkan tamu ke riwayat check-out
    CheckOutPtr riwayatBaru = malloc(sizeof(RiwayatCheckOut));
    if (!riwayatBaru)
    {
        printf("Gagal alokasi memori untuk riwayat check-out.\n");
        return;
    }
    riwayatBaru->tamu = tamu;
    riwayatBaru->next = NULL;

    // masukkan ke dalam queue riwayatCheckOut
    if (hotel->riwayatCheckOut.rear == NULL)
    {
        hotel->riwayatCheckOut.front = hotel->riwayatCheckOut.rear = riwayatBaru;
    }
    else
    {
        hotel->riwayatCheckOut.rear->next = riwayatBaru;
        hotel->riwayatCheckOut.rear = riwayatBaru;
    }

    // Menampilkan pesan berhasil check-out
    printf("\nTamu '%s' dengan kamar %d telah berhasil check-out.\n", tamu->namaTamu, tamu->noKamar);

    free(tamuNode); // Hapus node dari antrean tamuAktif
}

// fungsi memasukkan data kamar awal ke hash table
void insertKamarToHash(HotelData *hotel, int noKamar, char *tipeKamar, int harga, char *fasilitas)
{
    int hashIndex = hashFunc(noKamar);

    // Cek apakah kamar sudah ada di hash table
    KamarNodePtrt current = hotel->hashKamar.table[hashIndex];
    while (current != NULL)
    {
        if (current->data.noKamar == noKamar)
        {
            printf("Kamar dengan nomor %d sudah ada!\n", noKamar);
            return;
        }
        current = current->next;
    }

    // Buat node kamar baru
    KamarNodePtrt newNode = malloc(sizeof(KamarNode));
    if (!newNode)
    {
        printf("Gagal alokasi memori!\n");
        return;
    }

    // Set data kamar
    newNode->data.noKamar = noKamar;
    newNode->data.harga = harga;
    strcpy(newNode->data.tipeKamar, tipeKamar);
    strcpy(newNode->data.fasilitas, fasilitas);
    newNode->data.status = 0; // otomatis "kosong"

    // Masukkan kamar baru ke dalam hash table
    newNode->next = hotel->hashKamar.table[hashIndex];
    hotel->hashKamar.table[hashIndex] = newNode;
}

// fungsi tambah kamar
void tambahKamar(HotelData *hotel)
{
    // cek kapasitas kamar
    if (hitungTotalKamar(&hotel->hashKamar) >= MAX_KAMAR)
    {
        printf("Maaf, kamar hotel sudah penuh!\n");
        return;
    }

    int nomor;
    printf("\n=== Tambah Kamar ===\n");
    printf("Nomor Kamar: ");
    scanf("%d", &nomor);
    while (getchar() != '\n')
        ;

    // Cek apakah kamar sudah ada di hash table
    int hashIndex = hashFunc(nomor);
    KamarNodePtrt current = hotel->hashKamar.table[hashIndex];
    while (current != NULL)
    {
        if (current->data.noKamar == nomor)
        {
            printf("Nomor kamar '%d' sudah terdaftar!\n", nomor);
            return;
        }
        current = current->next;
    }

    // jika nomor kamar belum ada, buat kamar baru
    Kamar kamarBaru;
    kamarBaru.noKamar = nomor;

    // getchar(); //bersihkan newline setelah scanf

    printf("Tipe kamar: ");
    fgets(kamarBaru.tipeKamar, sizeof(kamarBaru.tipeKamar), stdin);
    kamarBaru.tipeKamar[strcspn(kamarBaru.tipeKamar, "\n")] = '\0';

    printf("Harga: ");
    scanf("%d", &kamarBaru.harga);
    while (getchar() != '\n')
        ;

    printf("Fasilitas: ");
    fgets(kamarBaru.fasilitas, sizeof(kamarBaru.fasilitas), stdin);
    kamarBaru.fasilitas[strcspn(kamarBaru.fasilitas, "\n")] = '\0';

    kamarBaru.status = 0; // kosong

    // Memasukkan kamar ke dalam hash table
    KamarNodePtrt newNode = malloc(sizeof(KamarNode));
    if (newNode == NULL)
    {
        printf("Gagal mengalokasikan memori untuk kamar baru!\n");
        return;
    }
    newNode->data = kamarBaru;
    newNode->next = hotel->hashKamar.table[hashIndex];
    hotel->hashKamar.table[hashIndex] = newNode;

    printf("Kamar berhasil ditambahkan.\n");
}

void updateHarga(HotelData *hotel)
{

    int nomor, hargaBaru;
    printf("\n=== Update Harga Kamar ===\n");
    printf("Masukkan nomor kamar: ");
    scanf("%d", &nomor);
    printf("Masukkan harga baru: ");
    scanf("%d", &hargaBaru);
    getchar();

    // cek harga baru agar tidak negatif
    if (hargaBaru < 0)
    {
        printf("Harga tidak boleh negatif!\n");
        return;
    }

    // Menggunakan hash table untuk mencari kamar berdasarkan nomor kamar
    int hashIndex = hashFunc(nomor);                           // Menghitung index berdasarkan nomor kamar
    KamarNodePtrt current = hotel->hashKamar.table[hashIndex]; // Mengakses bucket hash sesuai index

    if (current == NULL)
    {
        // Jika bucket kosong, berarti kamar dengan nomor tersebut belum ada
        printf("Kamar dengan nomor %d tidak ditemukan.\n", nomor);
        return;
    }

    // Mencari kamar dengan nomor yang sesuai dalam linked list di dalam bucket
    while (current != NULL)
    {
        if (current->data.noKamar == nomor)
        {
            current->data.harga = hargaBaru; // Update harga
            printf("Harga kamar nomor %d berhasil diperbaharui.\n", nomor);
            return;
        }
        current = current->next; // Lanjutkan ke node berikutnya jika belum ditemukan
    }

    // Jika nomor kamar tidak ditemukan dalam linked list
    printf("Kamar dengan nomor %d tidak ditemukan.\n", nomor);
}

// fungsi untuk update fasilitas kamar
void updateFasilitas(HotelData *hotel)
{
    int nomor;
    char fasilitasBaru[100];
    printf("\n=== Update Fasilitas Kamar ===\n");
    printf("Masukkan nomor kamar: ");
    scanf("%d", &nomor);
    getchar();

    printf("Masukkan fasilitas baru: ");
    fgets(fasilitasBaru, sizeof(fasilitasBaru), stdin);
    fasilitasBaru[strcspn(fasilitasBaru, "\n")] = '\0'; // hapus newline

    // Menggunakan hash table untuk mencari kamar berdasarkan nomor kamar
    int hashIndex = hashFunc(nomor);
    KamarNodePtrt current = hotel->hashKamar.table[hashIndex];
    while (current != NULL)
    {
        if (current->data.noKamar == nomor)
        {
            strcpy(current->data.fasilitas, fasilitasBaru);
            printf("Fasilitas kamar nomor %d berhasil diperbaharui.\n", nomor);
            return;
        }
        current = current->next;
    }

    printf("Kamar dengan nomor %d tidak ditemukan.\n", nomor);
}

// fungsi untuk update tipe kamar
void updateTipeKamar(HotelData *hotel)
{
    int nomor;
    char tipeBaru[100]; // ini lebih aman daripada lngusng akses memori utama penyimpanan
    printf("\n=== Update Tipe Kamar ===\n");
    printf("Masukkan nomor kamar: ");
    scanf("%d", &nomor);
    getchar();

    printf("Masukkan tipe kamar baru: ");
    fgets(tipeBaru, sizeof(tipeBaru), stdin);
    tipeBaru[strcspn(tipeBaru, "\n")] = '\0';

    // Menggunakan hash table untuk mencari kamar berdasarkan nomor kamar
    int hashIndex = hashFunc(nomor);
    KamarNodePtrt current = hotel->hashKamar.table[hashIndex];
    while (current != NULL)
    {
        if (current->data.noKamar == nomor)
        {
            strcpy(current->data.tipeKamar, tipeBaru);
            printf("Tipe kamar nomor %d berhasil diperbaharui.\n", nomor);
            return;
        }
        current = current->next;
    }

    printf("Kamar dengan nomor %d tidak ditemukan.\n", nomor);
}

// fungsi untuk menampilkan daftar kamar
void tampilkanKamar(const HotelData *hotel)
{

    printf("\n========================= Daftar Kamar ========================================\n");

    // Hitung jumlah kamar menggunakan hash table
    int total = hitungTotalKamar(&hotel->hashKamar);
    printf("Jumlah Kamar: %d\n", total);

    printf("+------------+------------+------------+----------------------+-----------------+\n");
    printf("| %-10s | %-10s | %-10s | %-20s | %-15s |\n",
           "No.Kamar", "Tipe Kamar", "Harga", "Fasilitas", "Status");
    printf("+------------+------------+------------+----------------------+-----------------+\n");
    // cek apakah sudah ada kamar
    if (total == 0)
    {
        printf("| %-10d | %-10s | %-10d | %-20s | %-15s |\n", 0, "-", 0, "-", "-");
        printf("+-----------------------------------------------------------------------+\n");
        printf("Belum ada kamar yang ditambahkan.\n");
        return;
    }

    // Telusuri semua indeks di hash table
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            Kamar k = current->data;
            printf("| %-10d | %-10s | %-10d | %-20s | %-15s |\n",
                   k.noKamar, k.tipeKamar, k.harga, k.fasilitas,
                   (k.status == 0) ? "Kosong" : "Terisi");

            current = current->next;
        }
    }

    printf("+===============================================================================+\n");
}// fungsi kelola  kamar
void kelolaKamar(HotelData *hotel)
{

    int pilihan;
    do
    {
        printf("\n=== Melakukan Pengelolaan Kamar ===\n");
        printf("1. Tambah kamar\n");
        printf("2. Update harga kamar\n");
        printf("3. Update fasilitas kamar\n");
        printf("4. Update tipe kamar\n");
        printf("5. Tampilkan informasi kamar\n");
        printf("6. Kembali\n");
        pilihan = inputMenuAman(1, 6);

        switch (pilihan)
        {
        case 1:
            tambahKamar(hotel);
            break;
        case 2:
            updateHarga(hotel);
            break;
        case 3:
            updateFasilitas(hotel);
            break;
        case 4:
            updateTipeKamar(hotel);
            break;
        case 5:
            tampilkanKamar(hotel);
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

void cariTipeKamar(HotelData *hotel)
{
    char tipe[100];
    int jumlahTipe = 0;
    int ditemukan = 0;

    getchar(); // buang newline sebelum fgets
    printf("\n=== Cari Berdasarkan Tipe Kamar ===\n");
    printf("Masukkan tipe kamar: ");
    fgets(tipe, sizeof(tipe), stdin);
    tipe[strcspn(tipe, "\n")] = '\0';

    // Hitung jumlah kamar dengan tipe tersebut
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            if (strcmp(current->data.tipeKamar, tipe) == 0)
            {
                jumlahTipe++;
            }
            current = current->next;
        }
    }

    printf("\nJumlah kamar dengan tipe '%s': %d\n", tipe, jumlahTipe);
    printf("Hasil pencarian:\n");
    printf("+-------------+---------------+-----------+----------------------+----------+\n");
    printf("| No. Kamar   | Tipe Kamar    | Harga     | Fasilitas            | Status   |\n");
    printf("+-------------+---------------+-----------+----------------------+----------+\n");

    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            if (strcmp(current->data.tipeKamar, tipe) == 0)
            {
                printf("| %-11d | %-13s | Rp%-7d | %-20s | %-8s |\n",
                       current->data.noKamar, current->data.tipeKamar,
                       current->data.harga, current->data.fasilitas,
                       current->data.status ? "Terisi" : "Kosong");
                ditemukan = 1;
            }
            current = current->next;
        }
    }

    if (!ditemukan)
        printf("| Tidak ada kamar dengan tipe '%s'. |\n", tipe);

    printf("+--------------------------------------------------------------------------+\n");
}

void cariHargaKamar(HotelData *hotel)
{
    int harga;
    int ditemukan = 0;

    printf("\n=== Cari Berdasarkan Harga Kamar ===\n");
    printf("Masukkan harga kamar: ");

    // Validasi input: pastikan input angka yang valid
    while (scanf("%d", &harga) != 1 || harga < 0)
    {
        printf("Input tidak valid. Masukkan angka harga kamar yang benar: ");
        while (getchar() != '\n')
            ; // clear input buffer
    }

    printf("\nHasil pencarian harga kamar Rp%d:\n", harga);
    printf("+-------------+---------------+-----------+----------------------+----------+\n");
    printf("| No. Kamar   | Tipe Kamar    | Harga     | Fasilitas            | Status   |\n");
    printf("+-------------+---------------+-----------+----------------------+----------+\n");

    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            if (current->data.harga == harga)
            {
                printf("| %-11d | %-13s | Rp%-7d | %-20s | %-8s |\n",
                       current->data.noKamar, current->data.tipeKamar,
                       current->data.harga, current->data.fasilitas,
                       current->data.status ? "Terisi" : "Kosong");
                ditemukan = 1;
            }
            current = current->next;
        }
    }

    if (!ditemukan)
    {
        printf("| Tidak ada kamar dengan harga Rp%-45d|\n", harga);
    }

    printf("+--------------------------------------------------------------------------+\n");
}

void cariNoKamar(HotelData *hotel)
{
    if (hotel == NULL)
    {
        printf("Error: Data hotel tidak valid!\n");
        return;
    }
    int nomor;
    printf("\n=== Cari Berdasarkan Nomor Kamar ===\n");
    printf("Masukkan nomor kamar: ");
    scanf("%d", &nomor);

    int hashIdx = hashFunc(nomor);
    KamarNodePtrt kamar = hotel->hashKamar.table[hashIdx];

    while (kamar != NULL)
    {
        if (kamar->data.noKamar == nomor)
        {
            // Tampilkan info kamar dalam tabel
            printf("\n+-------------+---------------+-----------+----------------------+----------+\n");
            printf("| No. Kamar     | Tipe Kamar    | Harga     | Fasilitas            | Status   |\n");
            printf("+---------------+---------------+-----------+----------------------+----------+\n");
            printf("| %-11d | %-13s | Rp%-7d | %-20s | %-8s |\n",
                   kamar->data.noKamar,
                   kamar->data.tipeKamar,
                   kamar->data.harga,
                   kamar->data.fasilitas,
                   kamar->data.status ? "Terisi" : "Kosong");

            printf("+---------------+---------------+-------------+---------------------+----------+\n");

            return;
        }
        kamar = kamar->next;
    }

    printf("Kamar dengan nomor %d tidak ditemukan.\n", nomor);
}

void cariNamaTamu(const HotelData *hotel)
{
    if (hotel == NULL || hotel->tamuAktif.front == NULL)
    {
        printf("\nDaftar tamu aktif kosong!\n");
        return;
    }

    char namaCari[50];
    int ditemukan = 0;

    printf("\n=== Cari Tamu Aktif (Sudah Check-In) ===\n");
    printf("Masukkan nama tamu: ");
    getchar(); // Bersihkan newline sebelumnya
    fgets(namaCari, sizeof(namaCari), stdin);
    namaCari[strcspn(namaCari, "\n")] = '\0'; // Hapus newline

    // Header tabel
    printf("\n+----------------------+-------------+---------------------+\n");
    printf("| Nama Tamu              | No. Kamar   | Status Pemesanan    |\n");
    printf("+------------------------+-------------+---------------------+\n");

    // Iterasi melalui antrean tamu aktif (linked list)
    CheckInPtr current = hotel->tamuAktif.front;
    while (current != NULL)
    {
        if (current->tamu != NULL && strcmp(current->tamu->namaTamu, namaCari) == 0)
        {
            // Tentukan status pemesanan berdasarkan nilai status
            char status[20];
            switch (current->tamu->statusPemesanan)
            {
            case 0:
                strcpy(status, "Booking (Antrean)");
                break;
            case 1:
                strcpy(status, "Checked In");
                break;
            case 2:
                strcpy(status, "Checked Out");
                break;
            case 3:
                strcpy(status, "Waiting List");
                break;
            case -1:
                strcpy(status, "Pembatalan");
                break;
            default:
                strcpy(status, "Status Tidak Dikenal");
                break;
            }

            // Cetak data tamu
            printf("| %-20s | %-11d | %-19s |\n",
                   current->tamu->namaTamu,
                   current->tamu->noKamar,
                   status);
            ditemukan = 1;
        }
        current = current->next;
    }

    if (!ditemukan)
    {
        printf("| %-52s |\n", "Tamu tidak ditemukan!");
    }

    printf("+------------------------+-------------+---------------------+\n");
}

void pencarianData(HotelData *hotel)
{
    int pilihan;
    do
    {
        printf("\n=== Menu Pencarian ===\n");
        printf("Cari berdasarkan: \n");
        printf("1. No. Kamar\n");
        printf("2. Tipe Kamar\n");
        printf("3. Harga Kamar\n");
        printf("4. Nama Tamu \n");
        printf("5. Kembali\n");
        pilihan = inputMenuAman(1, 5);

        switch (pilihan)
        {
        case 1:
            cariNoKamar(hotel);
            break;
        case 2:
            cariTipeKamar(hotel);
            break;
        case 3:
            cariHargaKamar(hotel);
            break;
        case 4:
            cariNamaTamu(hotel);
            break;
        case 0:
            printf("Kembali ke menu utama...\n");
            return;
        default:
            printf("Pilihan tidak valid. Coba lagi!\n");
            break;
        }
    } while (pilihan != 0);
}

// MENU UTAMA UNTUK ADMIN
void menuAdmin(HotelData *hotel)
{
    int pilihan;
    do
    {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Melihat & Mengelola Data Kamar\n");
        printf("2. Informasi hotel\n");
        printf("3. Pencarian data\n");
        printf("4. Proses check-in\n");
        printf("5. Proses check-out\n");
        printf("6. Logout\n");
        pilihan = inputMenuAman(1, 6);

        switch (pilihan)
        {
        case 1:
            kelolaKamar(hotel);
            break;
        case 2:
            informasiHotel(hotel);
            break;
        case 3:
            pencarianData(hotel);
            break;
        case 4:
            checkInTamu(hotel);
            break;
        case 5:
            checkOutTamu(hotel);
            break;
        case 6:
            printf("Logout...\n");
            return;
        default:
            printf("Pilihan tidak valid!\n");
            break;
        }
    } while (pilihan != 6);
}

// fungsi fungsi untuk BAGIAN MENU TAMU

// fungsi untuk slain kamar dari hash ke array
int salinKamarKeArray(HotelData *hotel, Kamar array[])
{
    int index = 0;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            array[index++] = current->data;
            current = current->next;
        }
    }
    // Lprintf("[DEBUG SALIN] Total kamar disalin: %d\n", index);
    return index; // kembalikan total kamar yang berhasil disalin
}

// HeapSort untuk harga kamar
void swapKamar(Kamar *a, Kamar *b)
{
    Kamar temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Kamar array[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && array[left].harga > array[largest].harga)
        largest = left;

    if (right < n && array[right].harga > array[largest].harga)
        largest = right;

    if (largest != i)
    {
        swapKamar(&array[i], &array[largest]);
        heapify(array, n, largest);
    }
}

void heapSort(Kamar array[], int n)
{

    // bangun heap
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(array, n, i);
    }
    // ekstrak elemen satu per satu
    for (int i = n - 1; i > 0; i--)
    {
        swapKamar(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}

// fungsi untuk menampilkan kamar setelah proses sorting harga
void tampilkanKamarUrutHarga(HotelData *hotel)
{
    printf("\n========================= Daftar Kamar Berdasarkan Harga ========================\n");

    // Hitung jumlah kamar menggunakan hash table
    int total = hitungTotalKamar(&hotel->hashKamar);
    printf("Jumlah Kamar: %d\n", total);

    printf("---------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-20s | %-15s | \n",
           "No.Kamar", "Tipe Kamar", "Harga", "Fasilitas", "Status");
    printf("---------------------------------------------------------------------------------\n");

    // cek apakah sudah ada kamar
    if (total == 0)
    {
        printf("| %-10d | %-10s | %-10d | %-20s |\n", 0, "-", 0, "-");
        printf("--------------------------------------------------------------\n");
        printf("Belum ada data kamar yang dapat ditampilkan saat ini\n");
        return;
    }

    // Salin kamar dari hash ke array
    Kamar kamarArray[MAX_KAMAR];
    int jumlahKamar = salinKamarKeArray(hotel, kamarArray); // Misalnya, kita sudah punya fungsi untuk menyalin data kamar

    // Urutkan kamar berdasarkan harga menggunakan heap sort
    heapSort(kamarArray, jumlahKamar); // Mengurutkan berdasarkan harga kamar

    // Menampilkan kamar yang sudah diurutkan berdasarkan harga
    for (int i = 0; i < jumlahKamar; i++)
    {
        Kamar k = kamarArray[i];
        printf("| %-10d | %-10s | %-10d | %-20s | %-15s |\n",
               k.noKamar, k.tipeKamar, k.harga, k.fasilitas,
               (k.status == 0) ? "Kosong" : "Terisi");
    }

    printf("=================================================================================\n");
}

// menu tampilkan kamar dengan filter
void lihatKamar(HotelData *hotel)
{
    int pilihan;

    do
    {
        printf("\n=== Menu Lihat Kamar ===\n");
        printf("1. Tampilkan semua kamar tanpa filter harga\n");
        printf("2. Tampilkan kamar dengan filter harga (termurah - termahal)\n");
        printf("3. Kembali\n");
        pilihan = inputMenuAman(1, 3);

        switch (pilihan)
        {
        case 1:
            tampilkanKamar(hotel);
            break;
        case 2:
            tampilkanKamarUrutHarga(hotel);
            break;
        case 0:
            printf("kembali ke menu utama...\n");
            return;
        default:
            printf("Pilihan tidak valid\n");
            break;
        }
    } while (pilihan != 3);
}

// fungsi untuk mengecek ketersedian kamar berdasarkan no kamar
Kamar *cekKetersediaanKamarUntukBooking(HotelData *hotel, int nomor)
{
    int idx = hashFunc(nomor);
    KamarNodePtrt node = hotel->hashKamar.table[idx];

    while (node != NULL)
    {
        if (node->data.noKamar == nomor)
        {
            if (node->data.status == 0)
            {
                printf("\nKamar %d tersedia untuk dipesan.\n", nomor);
                return &node->data;
            }
            else
            {
                printf("\nMaaf, kamar %d sedang terisi.\n", nomor);
                return NULL;
            }
        }
        node = node->next;
    }

    printf("\nNomor kamar %d tidak ditemukan.\n", nomor);
    return NULL;
}

// fungsi menampilkan daftar kamar kosong untuk tamu
void tampilkanKamarKosong(HotelData *hotel)
{
    printf("\n=== Daftar Kamar Kosong ===\n");

    int jumlahKamarKosong = 0; // Variabel untuk menghitung kamar kosong

    // Menghitung jumlah kamar kosong
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            if (current->data.status == 0)
            {                        // Cek apakah kamar kosong
                jumlahKamarKosong++; // Tambah jumlah kamar kosong
            }
            current = current->next;
        }
    }

    // Menampilkan jumlah kamar kosong
    if (jumlahKamarKosong > 0)
    {
        printf("\nJumlah Kamar Kosong: %d\n", jumlahKamarKosong);
        printf("-------------+------------+------------+----------------------+\n");
        printf("| No.Kamar   | Tipe Kamar | Harga      | Fasilitas            |\n");
        printf("-------------+------------+------------+----------------------+\n");

        // Menampilkan kamar kosong
        for (int i = 0; i < HASH_SIZE; i++)
        {
            KamarNodePtrt current = hotel->hashKamar.table[i];
            while (current != NULL)
            {
                if (current->data.status == 0)
                { // Kamar kosong
                    Kamar k = current->data;
                    printf("| %-10d | %-10s | %-10d | %-20s |\n",
                           k.noKamar, k.tipeKamar, k.harga, k.fasilitas);
                }
                current = current->next;
            }
        }
    }
    else
    {
        printf("\nMaaf, saat ini tidak ada kamar kosong.\n");
    }

    printf("-------------+------------+------------+----------------------+\n");
}

void prosesPemesananLangsung(HotelData *hotel, int noKamar)
{
    char namaTamu[50];

    printf("Masukkan nama Anda untuk pemesanan: ");
    // getchar(); // Bersihkan newline
    fgets(namaTamu, sizeof(namaTamu), stdin);
    namaTamu[strcspn(namaTamu, "\n")] = '\0'; // Hapus newline

    // Alokasi memori untuk tamu
    TamuPtr tamuBaru = (TamuPtr)malloc(sizeof(Tamu));
    if (tamuBaru == NULL)
    {
        printf("Gagal mengalokasikan memori untuk tamu.\n");
        return;
    }

    strcpy(tamuBaru->namaTamu, namaTamu);
    tamuBaru->noKamar = noKamar;
    tamuBaru->statusPemesanan = 0; // Belum check-in

    // Masukkan ke antrean check-in
    tambahTamuKeAntreanCheckIn(&hotel->antreanCheckIn, tamuBaru);

    // Update status kamar jadi terisi
    int idx = hashFunc(noKamar);
    KamarNodePtrt node = hotel->hashKamar.table[idx];
    while (node != NULL)
    {
        if (node->data.noKamar == noKamar)
        {
            node->data.status = 1;
            node->data.tamu = tamuBaru;
            break;
        }
        node = node->next;
    }

    printf("\nPemesanan berhasil! '%s' telah masuk ke antrean check-in untuk kamar %d.\n",
           namaTamu, noKamar);
}

// fungsi untuk menambahkan tamu ke antrean waiting list
void tambahTamuKeWaitingList(WaitingList *list, TamuPtr tamu)
{
    WaitingListPtr nodeBaru = (WaitingListPtr)malloc(sizeof(AntreanWaitingList));
    if (nodeBaru == NULL)
    {
        printf("Gagal alokasi memori untuk antrean waiting list.\n");
        return;
    }

    nodeBaru->tamu = tamu;
    nodeBaru->next = NULL;

    if (list->rear == NULL)
    {
        list->front = list->rear = nodeBaru;
    }
    else
    {
        list->rear->next = nodeBaru;
        list->rear = nodeBaru;
    }

    printf("'%s' telah ditambahkan ke antrean waiting list untuk kamar %d.\n",
           tamu->namaTamu, tamu->noKamar);
}

// fungsi proses masuk waiting list (saat kamar penuh)
void prosesMasukWaitingList(HotelData *hotel, int noKamar)
{
    char namaTamu[100];

    printf("\nMasukkan nama Anda untuk waiting list kamar %d: ", noKamar);
    // getchar(); // bersihkan newline
    fgets(namaTamu, sizeof(namaTamu), stdin);
    namaTamu[strcspn(namaTamu, "\n")] = '\0';

    TamuPtr tamuBaru = (TamuPtr)malloc(sizeof(Tamu));
    if (!tamuBaru)
    {
        printf("Gagal alokasi memori.\n");
        return;
    }

    strcpy(tamuBaru->namaTamu, namaTamu);
    tamuBaru->noKamar = noKamar;
    tamuBaru->statusPemesanan = 3;

    tambahTamuKeWaitingList(&hotel->antreanWaitingList, tamuBaru);
}

// fungsi pemesanan kamar
void pemesananKamar(HotelData *hotel)
{
    int noKamar;

    printf("\n=== Pemesanan Kamar ===\n");
    printf("Masukkan nomor kamar yang ingin Anda pesan: ");
    scanf("%d", &noKamar);
    getchar();

    // cek apakah kamar tersedia
    Kamar *kamar = cekKetersediaanKamarUntukBooking(hotel, noKamar);

    if (kamar != NULL)
    {
        // jika tersedia, lanjut ke pemesanan langsung
        prosesPemesananLangsung(hotel, noKamar);
    }
    else
    {
        // jika kamar tidak tersedia, berikan tiga solusi
        int pilihan;
        printf("\nKamar %d tidak tersedia\n", noKamar);
        printf("Demi kenyamanan, kami menyediakan tiga solusi\n");
        printf("1. Tampilkan Kamar kosong\n");
        printf("2. Masuk ke antrean waiting list\n");
        printf("3. Batal (tidak jadi memesan)\n");
        printf("4. Kembali...\n");
        pilihan = inputMenuAman(1, 4);

        switch (pilihan)
        {
        case 1:
            tampilkanKamarKosong(hotel);
            break;

        case 2:
        {
            char konfirmasi;
            printf("\nApakah Anda ingin masuk ke antrean waiting list untuk kamar %d? (y/n): ", noKamar);
            // getchar(); // Bersihkan buffer newline
            scanf(" %c", &konfirmasi);
            getchar();

            if (konfirmasi == 'y' || konfirmasi == 'Y')
            {
                prosesMasukWaitingList(hotel, noKamar);
                printf("Anda telah masuk ke dalam antrean waiting list.\n");
            }
            else
            {
                printf("Tidak jadi masuk antrean waiting list.\n");
            }
            break;
        }
        case 3:
            printf("Tidak jadi melakukan pemesanan\n");
            printf("Terimakasi telah melihat kamar kami\n");
            break;
        case 4:
            printf("Kembali ke menu utama...\n");
            return;
        default:
            printf("Pilihan tidak valid\n");
            break;
        }
    }
}

void hapusDariAntreanCheckIn(HotelData *hotel, TamuPtr tamu)
{
    CheckInPtr *current = &hotel->antreanCheckIn.front;
    CheckInPtr prev = NULL;

    while (*current != NULL)
    {
        if ((*current)->tamu == tamu)
        {
            CheckInPtr temp = *current;
            if (prev == NULL)
            {
                hotel->antreanCheckIn.front = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            if (temp == hotel->antreanCheckIn.rear)
            {
                hotel->antreanCheckIn.rear = prev;
            }
            free(temp);
            return;
        }
        prev = *current;
        current = &(*current)->next;
    }
}

void batalkanPemesanan(HotelData *hotel)
{
    char nama[100];
    int noKamar;
    printf("\n=== Pembatalan Pemesanan ===\n");
    printf("Masukkan nomor kamar yang ingin dibatalkan: ");
    scanf("%d", &noKamar);
    getchar(); // bersihkan newline

    // Cari tamu berdasarkan nama tamu melalui hash table yang terhubung dengan kamar
    int idx = hashFunc(noKamar); // Hash berdasarkan nama tamu (atau noKamar jika kamu ingin berdasarkan kamar)
    KamarNodePtrt node = hotel->hashKamar.table[idx];

    // Cari tamu yang sesuai dalam hash table kamar
    while (node != NULL)
    {
        if (node->data.noKamar == noKamar)
        {
            TamuPtr tamu = node->data.tamu;

            tamu->statusPemesanan = -1; // update status pemesanan
            // Hapus tamu dari data kamar
            hapusDariAntreanCheckIn(hotel, tamu);
            node->data.tamu = NULL; // Menghapus data tamu dari kamar

            // Update status kamar jadi kosong
            node->data.status = 0; // Kamar sekarang tersedia

            // Minta alasan pembatalan
            char alasan[100];
            printf("Masukkan alasan pembatalan: ");
            fgets(alasan, sizeof(alasan), stdin);
            alasan[strcspn(alasan, "\n")] = '\0'; // Hapus newline jika ada

            // Buat node pembatalan dan push ke stack
            PembatalanPtr baru = (PembatalanPtr)malloc(sizeof(Pembatalan));
            if (!baru)
            {
                printf("Gagal alokasi memori untuk riwayat pembatalan.\n");
                return;
            }
            strcpy(baru->alasan, alasan);
            baru->tamu = tamu;
            baru->next = hotel->riwayatPembatalan.top;
            hotel->riwayatPembatalan.top = baru;
            hotel->riwayatPembatalan.size++;

            printf("Pemesanan atas nama '%s' sukses dibatalkan.\n", tamu->namaTamu);

            // Otomatis cek waiting list setelah pembatalan
            cekWaitingListSetelahCheckOut(hotel, tamu->noKamar);
            return;
        }
        node = node->next;
    }

    // Jika tamu tidak ditemukan di hash table
    printf("Nama '%s' tidak ditemukan di dalam pemesanan hotel.\n", nama);
    printf("Jika anda sudah check-in, pembatalan tidak bisa dilakukan. Terimakasih\n");
}

// fungsi untuk mengecek status pemesanan
void cekStatusTamu(HashKamar *hotel)
{
    int noKamar;
    printf("\n=== Cek Status Pemesanan ===\n");
    printf("Masukkan nomor kamar yang ingin dicek: ");
    scanf("%d", &noKamar);
    getchar();

    // Cari nomor kamar di hash table
    int hashIndex = hashFunc(noKamar);
    KamarNodePtrt kamarNode = hotel->table[hashIndex];

    if (kamarNode == NULL)
    {
        printf("Kamar dengan nomor %d tidak ditemukan.\n", noKamar);
        return;
    }

    Kamar *kamar = NULL;
    while (kamarNode != NULL)
    {
        if (kamarNode->data.noKamar == noKamar)
        {
            kamar = &kamarNode->data;
            break;
        }
        kamarNode = kamarNode->next;
    }

    if (kamar == NULL)
    {
        printf("Kamar dengan nomor %d tidak ditemukan.\n", noKamar);
        return;
    }

    // Cek status tamu terkait nomor kamar
    TamuPtr tamu = kamar->tamu;
    if (tamu != NULL)
    {
        printf("\nInformasi Status Pemesanan untuk Kamar %d:\n", noKamar);
        printf("+----------------------+---------------------+\n");
        printf("| Nama Tamu            | Status Pemesanan    |\n");
        printf("+----------------------+---------------------+\n");

        if (tamu->statusPemesanan == 0)
        {
            printf("| %-20s | %-19s |\n", tamu->namaTamu, "Booking");
        }
        else if (tamu->statusPemesanan == 1)
        {
            printf("| %-20s | %-19s |\n", tamu->namaTamu, "Checked In");
        }
        else if (tamu->statusPemesanan == 2)
        {
            printf("| %-20s | %-19s |\n", tamu->namaTamu, "Checked Out");
        }
        else if (tamu->statusPemesanan == 3)
        {
            printf("| %-20s | %-19s |\n", tamu->namaTamu, "Waiting List");
        }
        printf("+----------------------+---------------------+\n");
    }
    else
    {
        printf("\nKamar %d saat ini kosong atau tidak ada pemesanan aktif.\n", noKamar);
    }
}

// fungsi tampilan panduan tamu untuk melakukan pemesanan kamar
void tampilkanPanduan()
{
    printf("\n=== PANDUAN PEMESANAN KAMAR UNTUK TAMU ===\n");
    printf("1. Gunakan fitur 'Lihat Daftar Kamar' untuk melihat kamar yang tersedia.\n");
    printf("2. Jika menemukan kamar kosong, catat nomor kamarnya.\n");
    printf("3. Gunakan fitur 'Pesan Kamar' dan masukkan nomor kamar serta data diri Anda.\n");
    printf("4. Setelah berhasil dipesan, status kamar akan berubah menjadi 'Terisi'.\n");
    printf("5. Anda bisa melihat status pesanan Anda di fitur 'Lihat Status Pemesanan'.\n");
    printf("6. Jika kamar penuh, Anda akan masuk ke dalam daftar tunggu (waiting list).\n");
    printf("7. Jika ingin membatalkan, gunakan fitur 'Batalkan Pemesanan'.\n");
    printf("Silakan pilih menu sesuai kebutuhan Anda.\n");
    printf("============================================================\n");
}

// MENU TAMU
void menuTamu(HotelData *hotel)
{
    int pilihan;
    do
    {
        printf("\n=== Menu Tamu ===\n");
        printf("1. Lihat Panduan Pemesanan Kamar\n");
        printf("2. Lihat Kamar\n");
        printf("3. Pesan Kamar\n");
        printf("4. Cek Status Pemesanan\n");
        printf("5. Batalkan Pemesanan Kamar yang telah dilakukan\n");
        printf("6. Logout...\n");
        pilihan = inputMenuAman(1, 6);

        switch (pilihan)
        {
        case 1:
            tampilkanPanduan(); break;
        case 2:
            lihatKamar(hotel);
            break;
        case 3:
            pemesananKamar(hotel);
            break;
        case 4:
            cekStatusTamu(&hotel->hashKamar);
            break;
        case 5:
            batalkanPemesanan(hotel);
            break;
        case 6:
            printf("Keluar dari sistem\n");
            printf("Terimakasi telah berkunjung ke sistem hotel kami\n");
            return;
        default:
            printf("Pilihan Tidak valid!\n");
            break;
        }

    } while (pilihan != 6);
}

// kumpulan fungsi free memori agar tidak terjadi memory leak

// fungsi hapus semua kamar untuk hash table kamar
void hapusSemuaKamar(HotelData *hotel)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        KamarNodePtrt current = hotel->hashKamar.table[i];
        while (current != NULL)
        {
            KamarNodePtrt hapus = current;
            current = current->next;
            free(hapus);
        }
        hotel->hashKamar.table[i] = NULL;
    }
}

void hapusCheckInList(CheckInList *list)
{
    CheckInPtr current = list->front;
    while (current != NULL)
    {
        CheckInPtr hapus = current;
        current = current->next;
        free(hapus->tamu); // karena Tamu dialokasikan dengan malloc
        free(hapus);
    }
    list->front = list->rear = NULL;
}

void hapusWaitingList(WaitingList *list)
{
    WaitingListPtr current = list->front;
    while (current != NULL)
    {
        WaitingListPtr hapus = current;
        current = current->next;
        free(hapus->tamu); // free data tamu juga
        free(hapus);
    }
    list->front = list->rear = NULL;
}

void hapusStackPembatalan(StackPembatalan *stack)
{
    PembatalanPtr current = stack->top;
    while (current != NULL)
    {
        PembatalanPtr hapus = current;
        current = current->next;
        free(hapus->tamu); // free tamunya juga
        free(hapus);
    }
    stack->top = NULL;
    stack->size = 0;
}

// Fungsi untuk menghapus semua riwayat check-out
void hapusRiwayatCheckOut(RiwayatCheckOutQueue *queue)
{
    CheckOutPtr current = queue->front;
    while (current != NULL)
    {
        CheckOutPtr hapus = current;
        current = current->next;
        free(hapus->tamu); // free data tamu
        free(hapus);       // free node
    }
    queue->front = queue->rear = NULL; // Set queue menjadi kosong
}

// Fungsi untuk menghapus semua data hotel sekaligus
void hapusSemuaDataHotel(HotelData *hotel)
{
    hapusSemuaKamar(hotel);
    hapusCheckInList(&hotel->antreanCheckIn);
    hapusCheckInList(&hotel->tamuAktif);
    hapusWaitingList(&hotel->antreanWaitingList);
    hapusStackPembatalan(&hotel->riwayatPembatalan);
    hapusRiwayatCheckOut(&hotel->riwayatCheckOut); // Memanggil pembersihan riwayat check-out
}

void sambutanTamu(){
    printf("\n============================================================\n");
    printf("                  SELAMAT DATANG, TAMU YANG TERHORMAT        \n");
    printf("============================================================\n");
    printf("  Terima kasih telah memilih Hotel XYZ untuk menginap.\n");
    printf("  Anda dapat melakukan berbagai hal, seperti:\n");
    printf("    - Melakukan pemesanan kamar\n");
    printf("    - Melihat antrean check-in\n");
    printf("    - Masuk daftar tunggu jika kamar penuh\n");
    printf("    - Membatalkan pemesanan\n");
    printf("\n  Kami akan memastikan pengalaman menginap Anda menyenangkan.\n");
    printf("============================================================\n\n");
}

void sambutanAdmin()
{
    printf("\n============================================================\n");
    printf("               SELAMAT DATANG, ADMINISTRATOR HOTEL           \n");
    printf("============================================================\n");
    printf("  Anda memiliki akses penuh untuk mengelola sistem hotel.\n");
    printf("  Berikut yang dapat Anda lakukan:\n");
    printf("    - Menambahkan atau memperbarui data kamar\n");
    printf("    - Melihat dan mengelola antrean check-in & waiting list\n");
    printf("    - Melihat riwayat pembatalan dan check-out\n");
    printf("    - Memastikan operasional hotel berjalan lancar\n");
    printf("\n  Silakan pilih menu yang tersedia untuk mulai bekerja.\n");
    printf("============================================================\n\n");
}

void menuLogin(HotelData *hotel)
{
    int pilihan;
    do
    {
        printf("\n=== Selamat Datang di Sistem Hotel ===\n");
        printf("1. Login sebagai Admin\n");
        printf("2. Login sebagai Tamu\n");
        printf("3. Keluar\n");
        pilihan = inputMenuAman(1, 3);

        switch (pilihan)
        {
        case 1:
            sambutanAdmin();
            menuAdmin(hotel);
            break;
        case 2:
            sambutanTamu();
            menuTamu(hotel);
            break;
        case 3:
            printf("Terima kasih telah menggunakan sistem pemesanan Hotel The Zenith Hotel!\n");
            printf("Sampai jumpa di kunjungan berikutnya\n");
            printf("Semoga hari anda menyenangkan\n\n");
            break;
        default:
            printf("Pilihan tidak valid. Coba lagi.\n");
        }
    } while (pilihan != 3);
}


