#include <stdio.h>   // Library untuk fungsi input/output seperti printf() dan scanf()
#include <stdlib.h>  // Library untuk fungsi umum seperti rand(), exit(), dan system()
#include <time.h>    // Library untuk fungsi terkait waktu dan tanggal

// Struktur data untuk menyimpan informasi produk
struct Produk{
    char produk[50];  // Array karakter untuk menyimpan nama produk (maksimal 50 karakter)
    float harga;      // Variabel float untuk menyimpan harga produk
};

// Fungsi untuk menampilkan menu toko dan daftar produk
void displayMenu(struct Produk produkDanHarga[], int jumlahProduk){
    printf("Selamat Datang di Toko SKENSA \n");
    printf("Silahkan Pilih Barang yang Anda Inginkan :\n");
    printf("=====================================================\n");
    printf("| No. |        Nama Barang        |      Harga      |\n");
    printf("-----------------------------------------------------\n");
    // Perulangan untuk menampilkan daftar produk dan harga
    for(int i = 0; i < jumlahProduk; i++){
        printf("|  %-2d |    %-19s    | Rp.%-12.0f |\n", i + 1, produkDanHarga[i].produk, produkDanHarga[i].harga);
    }
    printf("=====================================================\n");
    printf("\n  99. Struk Pembayaran");
    printf("\n  55. Reset Pilihan");
    printf("\n  00. Keluar\n");
    printf("\n=====================================================");
}

// Fungsi untuk mengurutkan data pesanan dari jumlah pembelian terbanyak (menggunakan algoritma bubble sort)
void sortPesanan(struct Produk produkDanHarga[], int jumlahProduk, int jumlahBeli[]){
    for(int i = 0; i < jumlahProduk - 1; i++){
        for(int j = 0; j < jumlahProduk - i - 1; j++){
            // Membandingkan jumlah pembelian dan menukar posisi jika lebih kecil
            if(jumlahBeli[j] < jumlahBeli[j + 1]){
                // Menukar posisi jumlah pembelian
                int tempQty = jumlahBeli[j];
                jumlahBeli[j] = jumlahBeli[j + 1];
                jumlahBeli[j + 1] = tempQty;

                // Menukar posisi data produk
                struct Produk temp = produkDanHarga[j];
                produkDanHarga[j] = produkDanHarga[j + 1];
                produkDanHarga[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk membuat ID struk unik
int generateStrukID(){
    time_t current_time;
    time (&current_time);  // Mendapatkan waktu saat ini

    int randNum = rand()%10000;  // Membuat angka acak antara 0-9999
    int strukID = randNum + (int)current_time;  // Menggabungkan waktu dan angka acak untuk ID unik

    return strukID;
}


int main()
{
    // Array untuk menyimpan data produk dan harga
    struct Produk produkDanHarga[]={
        {"Buku Tulis", 5000},
        {"Pensil", 2000},
        {"Penghapus", 1000},
        {"Penggaris", 1000},
        {"Bujur Sangkar", 500},
    };

    // Inisialisasi variabel untuk menyimpan data transaksi
    int jumlahProduk = sizeof(produkDanHarga)/sizeof(produkDanHarga[0]),  // Menghitung jumlah produk dalam array
        jumlah,        // Variabel untuk menyimpan jumlah barang yang dibeli
        jumlahBeli[5] = {0},  // Array untuk menyimpan jumlah pembelian per produk
        pilihan,       // Variabel untuk menyimpan pilihan menu
        strukID = generateStrukID(),  // Membuat ID struk
        no_barang = 0;  // Penghitung nomor barang

    // Variabel untuk perhitungan harga dan diskon
    float hargaAkhir[5] = {0},    // Array untuk menyimpan harga akhir per produk
          totalHarga,            // Variabel untuk total harga
          diskon[5] = {0},       // Array untuk menyimpan diskon per produk
          totalHargaAkhir = 0,   // Total harga sebelum diskon
          totalDiskon = 0,       // Total diskon
          totalBayar,            // Total yang harus dibayar
          uangBayar,             // Jumlah uang yang dibayarkan
          kembalian;             // Jumlah kembalian

    // Mengambil data waktu dan tanggal saat ini
    time_t current_time;
    struct tm* local_time;
    current_time = time(NULL);
    local_time = localtime(&current_time);
    char time_str[50];

    // Format waktu dan tanggal
    strftime(time_str, sizeof(time_str), " %a %b %d %H:%M:%S %Y", local_time);

    // Membuat nama file untuk struk
    char filename[50];
    sprintf(filename, "struk %d.txt", strukID);

    // Membuka file untuk menyimpan struk
    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        printf("\nGagal membuat file\n");
        exit(1);  // Keluar program jika gagal membuat file
    }

    // Menampilkan menu toko
    displayMenu(produkDanHarga, jumlahProduk);

    // Loop utama program untuk interaksi pengguna
    while (1){
        printf("\n\nMasukkan pilihan yang anda inginkan: ");
        scanf("%d", &pilihan);

        // Kondisi untuk memilih dan menambahkan barang ke keranjang
        if(pilihan >= 1 && pilihan <= jumlahProduk){
            // Menampilkan produk yang dipilih dan meminta input jumlah
            printf("\n [%d] Pembelian barang %s", pilihan, produkDanHarga[pilihan-1].produk);
            printf("\n     Jumlah: ");
            scanf("%d", &jumlah);

            printf("\n=====================================================");

            // Menyimpan data pembelian ke dalam array
            jumlahBeli[pilihan-1] += jumlah;                                    // Menambah jumlah pembelian produk
            hargaAkhir[pilihan-1] += produkDanHarga[pilihan-1].harga * jumlah;  // Menghitung harga akhir produk

            continue;  // Kembali ke awal loop

        // Opsi untuk menampilkan struk dan proses pembayaran
        } else if (pilihan == 99){
            // Mengurutkan pesanan berdasarkan jumlah pembelian
            sortPesanan(produkDanHarga, jumlahProduk, jumlahBeli);

            // Menampilkan rekap pesanan
            printf("\nRekap Pesanan Barang\n");
            printf("==============================================================================\n");
            printf("| No. |  Jumlah  |    Nama Barang    |    Harga    | Total Harga |   Diskon  |\n");
            printf("------------------------------------------------------------------------------\n");

            // Perulangan untuk menampilkan detail setiap pembelian
            for(int i = 0; i < jumlahProduk; i++){
                // Menghitung total harga per produk
                totalHarga = produkDanHarga[i].harga * jumlahBeli[i];

                // Hanya menampilkan produk yang dibeli (jumlah > 0)
                if(jumlahBeli[i] > 0){

                    // Menghitung diskon berdasarkan jumlah pembelian
                    if(jumlahBeli[i] < 3){
                        diskon[i] = 0;  // Tidak ada diskon untuk pembelian kurang dari 3
                    } else if (jumlahBeli[i] >= 3 && jumlahBeli[i] < 5){
                        diskon[i] = totalHarga * 0.10;  // Diskon 10% untuk pembelian 3-4 item
                    } else if (jumlahBeli[i] >= 5){
                        diskon[i] = totalHarga * 0.15;  // Diskon 15% untuk pembelian 5 item atau lebih
                    }

                    // Menampilkan detail produk yang dibeli
                    printf("|  %-2d |     %-2d   |   %-13s   |  Rp.%-7.0f |  Rp.%-6.0f  | Rp.%-7.0f|\n", i + 1, jumlahBeli[i],
                           produkDanHarga[i].produk, produkDanHarga[i].harga, totalHarga, diskon[i], no_barang++);
                }

                // Menghitung total keseluruhan
                totalHargaAkhir += hargaAkhir[i];
                totalDiskon += diskon[i];
                totalBayar = totalHargaAkhir - totalDiskon;
            }

            // Menampilkan ringkasan pembayaran
            printf("==============================================================================\n");
            printf("\n     Total Harga  :  Rp.%.0f", totalHargaAkhir);
            printf("\n     Total Diskon :  Rp.%.0f", totalDiskon);
            printf("\n     Total Bayar  :  Rp.%.0f", totalBayar);
            printf("\n\n==============================================================================\n");

            // Loop untuk validasi pembayaran
            while(1){
                // Meminta input jumlah pembayaran
                printf("Masukkan uang bayar: Rp.");
                scanf("%f", &uangBayar);

                // Validasi jumlah pembayaran dan menghitung kembalian
                if(uangBayar >= totalBayar){
                    kembalian = uangBayar - totalBayar;
                    printf("\n Kembalian : Rp.%.0f\n", kembalian);
                    break;  // Keluar dari loop jika pembayaran valid
                } else if (uangBayar < totalBayar){
                    printf("\n Maaf, uang anda kurang.\n");
                    continue;  // Meminta input ulang jika pembayaran kurang
                }
            }

            // Menulis data struk ke file
            fprintf(fp, "======================================================================\n");
            fprintf(fp, "|                             TOKO SKENSA                            |\n");
            fprintf(fp, "|                 Jl. HOS Cokroainoto No. 84 Denpasar               |\n");
            fprintf(fp, "|                          Telp. 0816285791                          |\n");
            fprintf(fp, "|                                Bali                                |\n");
            fprintf(fp, "| ID Struk: %-55d|\n", strukID);
            fprintf(fp, "======================================================================\n");
            fprintf(fp, "|      Nama Barang      |    Harga    |   Total Harga   |   Diskon   |\n");
            fprintf(fp, "----------------------------------------------------------------------\n");

            // Menulis detail setiap produk yang dibeli ke file struk
            for(int i = 0; i < jumlahProduk; i++){
                if(jumlahBeli[i] > 0){
                    totalHarga = produkDanHarga[i].harga * jumlahBeli[i];
                    fprintf(fp, "| x%d %-18s | Rp.%-8.0f | Rp.%-12.0f | Rp.%-8.0f|\n", jumlahBeli[i],
                            produkDanHarga[i].produk, produkDanHarga[i].harga, totalHarga, diskon[i]);
                }
            }

            // Menulis ringkasan pembayaran ke file struk
            fprintf(fp, "======================================================================");
            fprintf(fp, "\n|     Total Harga  :  Rp.%-44.0f|", totalHargaAkhir);
            fprintf(fp, "\n|     Total Diskon :  Rp.%-44.0f|", totalDiskon);
            fprintf(fp, "\n|     Total Bayar  :  Rp.%-44.0f|", totalBayar);
            fprintf(fp, "\n|         Uang Bayar  :  Rp.%-41.0f|", uangBayar);
            fprintf(fp, "\n|         Kembalian   :  Rp.%-41.0f|", kembalian);
            fprintf(fp, "\n| Waktu Dan Hari: %-51s|\n", time_str);
            fprintf(fp, "======================================================================\n");
            fclose(fp);  // Menutup file

            // Menampilkan pesan terima kasih dan informasi penyimpanan struk
            printf("\n Terima Kasih telah berbelanja!");
            printf("\nStruk tersimpan di %s\n", filename);

            break;  // Keluar dari loop utama setelah transaksi selesai

        // Opsi untuk mereset pilihan
        } else if (pilihan == 55){
            // Perulangan untuk mereset semua data pembelian
            for(int i = 0; i < jumlahProduk; i++){
                pilihan = 0;
                jumlahBeli[i] = 0;
                hargaAkhir[i] = 0;
            }
            system("cls");  // Membersihkan layar konsol
            printf("\t     [!] Pilihan telah direset [!]\n\n");

            // Menampilkan menu kembali
            displayMenu(produkDanHarga, jumlahProduk);
            continue;  // Kembali ke awal loop

         // Opsi untuk keluar dari program
        } else if (pilihan == 00){
            printf("exiting...");
            exit(0);  // Mengakhiri program
        } else {
            // Menampilkan pesan error jika pilihan tidak valid
            printf("\nPilihan tidak valid!");
            continue;  // Kembali ke awal loop
        }
    }
    return 0;  // Mengakhiri program dengan status sukses
}
