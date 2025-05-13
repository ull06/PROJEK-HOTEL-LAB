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
}
