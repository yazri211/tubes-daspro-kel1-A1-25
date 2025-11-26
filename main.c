#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define GREEN   "\033[1;32m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"
#define MAGENTA "\033[35m"

#define FILE_NAME "catatan.txt"
#define DATE_LEN 12 
#define JENIS_LEN 10 
#define KET_MAX 200

typedef struct {
    int id;
    char tanggal[DATE_LEN];
    char jenis[JENIS_LEN];
    double jumlah;
    char keterangan[KET_MAX];
} Data;

void hapusNewline(char *s);
int buatId();
void pause();
void pauseClear();
void clearInput();
void header();
void headerTabel();
void isiTabel(Data d);
void printMenu();
void menu();
int validTanggal(const char *tgl);
void toLowerAll(char *s);
void tambahCatatan();
void lihatCatatan();
void hapusCatatan();
void editCatatan();
void cariCatatan();
void laporanBulanan();
void laporanTahunan();
void laporan();


int main() {

    menu();
    return 0;
}


void hapusNewline(char *s) {
    s[strcspn(s, "\n")] = 0;
}

int buatId() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        return 1;
    }

    char line[512];
    int maxId = 0, id;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|", &id) == 1) {
            if (id > maxId) maxId = id;
        }
    }

    fclose(f);
    return maxId + 1;
}

void pause() {
    printf(CYAN"|!| ");
    system("pause"RESET);
}

void pauseClear() {
    pause();
    system("cls");
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void header(){
printf(MAGENTA"===========================================\n");
printf("||   C A T A T A N   K E U A N G A N     ||\n");
printf("||     ----- Kelompok 1 A1 -----         ||\n");
printf("===========================================\n\n"RESET);

}

void printMenu(){
    printf(MAGENTA"===========================================\n");
    printf("||               M E N U                 ||\n");
    printf("===========================================\n");
    printf("|| 0. Keluar                             ||\n");
    printf("|| 1. Tambah Catatan                     ||\n");
    printf("|| 2. Lihat Catatan                      ||\n");
    printf("|| 3. Hapus Catatan                      ||\n");
    printf("|| 4. Edit Catatan                       ||\n");
    printf("|| 5. Cari Catatan                       ||\n");
    printf("|| 6. Lihat Laporan                      ||\n");
    printf("===========================================\n\n"RESET);
}

void menu() {
    int pilihan;
    int valid;

    while (1) {
        header();
        printMenu();
        printf("Pilih menu: ");
        valid = scanf("%d", &pilihan);

        if (valid != 1) {
            clearInput();
            printf(RED "Input tidak valid!\n" RESET);
            pauseClear();
            continue;;
        }

        switch (pilihan) {
            case 0:
                printf(GREEN"Terima kasih telah menggunakan program ini!\n"RESET);
                return;
            case 1:
                tambahCatatan();
                pauseClear();
                break;
            case 2:
                lihatCatatan();
                pauseClear();
                break;
            case 3:
                hapusCatatan();
                pauseClear();
                break;
            case 4:
                editCatatan();
                pauseClear();
                break;
            case 5:
                cariCatatan();
                break;
            case 6:
                laporan();
                break;
            default:
                printf(RED "|X| Pilihan tidak valid. Silakan coba lagi!\n" RESET);
                pauseClear(); 
                break;
        }

    }
}

void headerTabel() {
    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    printf(MAGENTA "| %-3s | %-10s | %-6s | %-12s | %-40s |\n" RESET,
           "ID", "Tanggal", "Jenis", "Jumlah", "Keterangan");
    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
}

void isiTabel(Data d) {
    printf(MAGENTA"| %-3d | %-10s | %-6s | %-12.2lf | %-40s |\n"RESET,
           d.id, d.tanggal, d.jenis, d.jumlah, d.keterangan);
}

// =================== 1. TAMBAH CATATAN ===================
int validTanggal(const char *tgl) {
    int d, m, y;

    if (sscanf(tgl, "%d-%d-%d", &d, &m, &y) != 3) {
        return 0;
    }

    if (y < 1900 || y > 2100) return 0;

    if (m < 1 || m > 12) return 0;

    int hariPerBulan[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    int kabisat = (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0));
    if (kabisat && m == 2) {
        if (d >= 1 && d <= 29) return 1;
        return 0;
    }

    if (d < 1 || d > hariPerBulan[m - 1]) return 0;

    return 1;
}

void toLowerAll(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

void tambahCatatan() {
    FILE *f = fopen(FILE_NAME, "a");
    if (!f) {
        printf(RED "Gagal membuka file untuk menulis!\n" RESET);
        return;
    }

    Data d;
    d.id = buatId();

    clearInput();

    // INPUT TANGGAL
    while (1) {
        printf("Masukkan tanggal (dd-mm-yyyy)\t: ");
        if (fgets(d.tanggal, sizeof(d.tanggal), stdin) == NULL) {
            printf(RED "Gagal membaca tanggal!\n" RESET);
            fclose(f);
            return;
        }

        size_t len = strlen(d.tanggal);
        if (len > 0 && d.tanggal[len - 1] != '\n') {
        clearInput();
        printf(RED "Input terlalu panjang. Masukkan format dd-mm-yyyy (contoh: 31-12-2023).\n" RESET);
        continue;
        }

        hapusNewline(d.tanggal);
        
        if (d.tanggal[0] == '\0') {
            printf(RED "Tanggal tidak boleh kosong. Coba lagi.\n" RESET);
            continue;
        }

        if (!validTanggal(d.tanggal)) {
            printf(RED "Format tanggal tidak valid! Harus dd-mm-yyyy dan tanggal harus benar.\n" RESET);
            continue;
        }

        break;
    }

    // INPUT JENIS
    char pilih[32];
    while (1) {
        printf("Pilih jenis (masuk/keluar)\t: ");
        if (fgets(pilih, sizeof(pilih), stdin) == NULL) {
            printf(RED "Gagal membaca input jenis! Coba lagi.\n" RESET);
            continue;
        }
        hapusNewline(pilih);
        toLowerAll(pilih);

        if (strcmp(pilih, "masuk") == 0) {
            strcpy(d.jenis, "masuk");
            break;
        } else if (strcmp(pilih, "keluar") == 0) {
            strcpy(d.jenis, "keluar");
            break;
        } else {
            printf(RED "Pilihan jenis tidak valid! input (masuk/keluar).\n" RESET);
        }
    }

    // INPUT JUMLAH
    while (1) {
    printf("Masukkan jumlah\t\t\t: ");
    if (scanf("%lf", &d.jumlah) == 1) {
        clearInput();   
        break;
    }

    printf(RED "Input jumlah tidak valid! Masukkan angka saja.\n" RESET);
    clearInput();
    }

    // INPUT KETERANGAN
    printf("Masukkan keterangan\t\t: ");
    if (fgets(d.keterangan, sizeof(d.keterangan), stdin) == NULL) {
        d.keterangan[0] = '\0';
    } else {
        hapusNewline(d.keterangan);
    }

    // TULIS KE FILE
    fprintf(f, "%d|%s|%s|%.2lf|%s\n",
            d.id, d.tanggal, d.jenis, d.jumlah, d.keterangan);

    fclose(f);
    printf(GREEN "Catatan berhasil ditambah\n" RESET);


    headerTabel();
    isiTabel(d);
    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
}
// ========================================================

// =================== 2. LIHAT CATATAN ===================
void lihatCatatan() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "Belum ada catatan (file tidak ditemukan).\n" RESET);
        return;
    }

    char line[512];
    int ada = 0;
    Data d;

    headerTabel();

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                   &d.id, d.tanggal, d.jenis, &d.jumlah, d.keterangan) == 5) {
            isiTabel(d);
            ada = 1;
        }
    }

    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    fclose(f);

    if (!ada) {
        printf(YELLOW "Tidak ada catatan.\n" RESET);
    }
}
// ========================================================

// =================== 3. HAPUS CATATAN ===================
void hapusCatatan() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "Belum ada catatan (file tidak ditemukan).\n" RESET);
        return;
    }

    char line[512];
    int ada = 0;
    Data d;

    headerTabel();

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                   &d.id, d.tanggal, d.jenis, &d.jumlah, d.keterangan) == 5) {
            isiTabel(d);
            ada = 1;
        }
    }

    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    fclose(f);

    if (!ada) {
        printf(YELLOW "Tidak ada catatan.\n" RESET);
    }

    int targetId;
    printf(CYAN "Masukkan ID yang ingin dihapus: " RESET);
    if (scanf("%d", &targetId) != 1) {
        printf(RED "Input ID tidak valid!\n" RESET);
        clearInput();
        pauseClear();
        return;
    }
    clearInput();

    char yakin;
    printf(YELLOW "Apakah Anda yakin ingin menghapus ID %d? (Y/N): " RESET, targetId);
    scanf(" %c", &yakin);
    clearInput();

    if (yakin != 'Y' && yakin != 'y') {
        printf(CYAN "Penghapusan dibatalkan.\n" RESET);
        pauseClear();
        return;
    }

    f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "File tidak ditemukan saat proses hapus.\n" RESET);
        pauseClear();
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf(RED "Gagal membuat file sementara.\n" RESET);
        fclose(f);
        pauseClear();
        return;
    }

    int found = 0;
    int id;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|", &id) == 1 && id == targetId) {
            found = 1;     
            continue;
        }
        fputs(line, temp);   
    }

    fclose(f);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found) {
        printf(GREEN "Catatan dengan ID %d berhasil dihapus.\n" RESET, targetId);
    } else {
        printf(YELLOW "ID %d tidak ditemukan.\n" RESET, targetId);
    }
}
// ====================================================

// ================== 4. EDIT CATATAN ==================
void editCatatan() {
    lihatCatatan();

    int targetId;
    printf(CYAN "Masukkan ID yang ingin diedit: " RESET);
    if (scanf("%d", &targetId) != 1) {
        printf(RED "Input ID tidak valid!\n" RESET);
        clearInput();
        return;
    }
    clearInput();

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "File tidak ditemukan.\n" RESET);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf(RED "Gagal membuat file sementara.\n" RESET);
        fclose(f);
        return;
    }

    char line[512];
    int found = 0;
    Data d;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                   &d.id, d.tanggal, d.jenis, &d.jumlah, d.keterangan) == 5) {

            if (d.id == targetId) {
                found = 1;
                printf(YELLOW "Data lama:\n" RESET);
                headerTabel();
                isiTabel(d);
                printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
                printf(GREEN "Masukkan data baru:\n" RESET);

                char pilihJenis[32];
                while (1) {
                    printf("Pilih jenis baru (masuk/keluar)\t: ");
                    if (fgets(pilihJenis, sizeof(pilihJenis), stdin) == NULL) {
                        continue;
                    }
                    hapusNewline(pilihJenis);

                    if (pilihJenis[0] == '\0') {
                        break;
                    }

                    toLowerAll(pilihJenis);

                    if (strcmp(pilihJenis, "masuk") == 0) {
                        strcpy(d.jenis, "masuk");
                        break;
                    } else if (strcmp(pilihJenis, "keluar") == 0) {
                        strcpy(d.jenis, "keluar");
                        break;
                    } else {
                        printf(RED "Pilihan jenis tidak valid! input (masuk/keluar).\n" RESET);
                    }
                }

                while (1) {
                    printf("Masukkan jumlah baru\t\t: ");
                    if (scanf("%lf", &d.jumlah) == 1) {
                        clearInput();
                        break;
                    }
                    printf(RED "Input jumlah tidak valid! Masukkan angka saja.\n" RESET);
                    clearInput();
                }

                printf("Masukkan keterangan baru\t: ");
                char tmpKet[256];
                if (fgets(tmpKet, sizeof(tmpKet), stdin) == NULL) {

                } else {
                    hapusNewline(tmpKet);
                    if (tmpKet[0] != '\0') {
                        strncpy(d.keterangan, tmpKet, sizeof(d.keterangan));
                        d.keterangan[sizeof(d.keterangan)-1] = '\0';
                    }
                }

                fprintf(temp, "%d|%s|%s|%.2lf|%s\n",
                        d.id, d.tanggal, d.jenis, d.jumlah, d.keterangan);

            } else {
                fputs(line, temp);
            }
        } else {
            fputs(line, temp);
        }
    }

    fclose(f);
    fclose(temp);

    if (found) {
        if (remove(FILE_NAME) != 0) {
            printf(RED "Gagal menghapus file lama.\n" RESET);
            remove("temp.txt");
            return;
        }
        if (rename("temp.txt", FILE_NAME) != 0) {
            printf(RED "Gagal mengganti file.\n" RESET);
            return;
        }
        printf(GREEN "Catatan dengan ID %d berhasil diupdate.\n" RESET, targetId);
        headerTabel();
        isiTabel(d);
        printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    } else {
        remove("temp.txt");
        printf(YELLOW "ID %d tidak ditemukan.\n" RESET, targetId);
    }
}
// =======================================================

// =================== 5. CARI CATATAN ===================
void cariCatatan() {
    char buf[64];

    printf("===== CARI CATATAN (dd-mm-yyyy / mm-yyyy / yyyy) =====\n");
    printf("Masukkan pola pencarian: ");

    clearInput();
    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
    hapusNewline(buf);
    if (buf[0] == '\0') return;

    int d, m, y;

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "File tidak ditemukan.\n" RESET);
        pauseClear();
        return;
    }

    char line[512];
    Data row;
    int found = 0;

    // CARI TANGGAL (dd-mm-yyyy)
    if (sscanf(buf, "%d-%d-%d", &d, &m, &y) == 3) {

        if (!validTanggal(buf)) {
            printf(RED "Format tanggal tidak valid!\n" RESET);
            fclose(f);
            pauseClear();
            return;
        }

        printf(CYAN "Hasil pencarian tanggal: %s\n" RESET, buf);

        headerTabel();
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                       &row.id, row.tanggal, row.jenis, &row.jumlah, row.keterangan) == 5) {

                int dd2, mm2, yy2;
                if (sscanf(row.tanggal, "%d-%d-%d", &dd2, &mm2, &yy2) == 3) {
                    if (dd2 == d && mm2 == m && yy2 == y) {
                        isiTabel(row);
                        found = 1;
                    }
                }
            }
        }

        printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
        if (!found) printf(YELLOW "Tidak ditemukan data tanggal %s\n" RESET, buf);
        fclose(f);
        pauseClear();
        return;
    }

    // CARI BULAN (mm-yyyy)
    if (sscanf(buf, "%d-%d", &m, &y) == 2) {
        if (m < 1 || m > 12 || y < 1) {
            printf(RED "Format mm-yyyy tidak valid!\n" RESET);
            fclose(f);
            pauseClear();
            return;
        }

        printf(CYAN "Hasil pencarian bulan: %02d-%04d\n" RESET, m, y);
        headerTabel();

        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                       &row.id, row.tanggal, row.jenis, &row.jumlah, row.keterangan) == 5) {

                int dd2, mm2, yy2;
                if (sscanf(row.tanggal, "%d-%d-%d", &dd2, &mm2, &yy2) == 3) {
                    if (mm2 == m && yy2 == y) {
                        isiTabel(row);
                        found = 1;
                    }
                }
            }
        }

        printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
        if (!found) printf(YELLOW "Tidak ada catatan pada %02d-%04d\n" RESET, m, y);
        fclose(f);
        pauseClear();
        return;
    }

    // CARI TAHUN (yyyy)
    if (sscanf(buf, "%d", &y) == 1) {
        if (y < 1 || y > 9999) {
            printf(RED "Format tahun tidak valid!\n" RESET);
            fclose(f);
            pauseClear();
            return;
        }

        printf(CYAN "Hasil pencarian tahun: %04d\n" RESET, y);
        headerTabel();

        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                       &row.id, row.tanggal, row.jenis, &row.jumlah, row.keterangan) == 5) {

                int dd2, mm2, yy2;
                if (sscanf(row.tanggal, "%d-%d-%d", &dd2, &mm2, &yy2) == 3) {
                    if (yy2 == y) {
                        isiTabel(row);
                        found = 1;
                    }
                }
            }
        }

        printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
        if (!found) printf(YELLOW "Tidak ada catatan pada tahun %04d\n" RESET, y);
        fclose(f);
        pauseClear();
        return;
    }

    printf(RED "Format tidak dikenali.\nGunakan dd-mm-yyyy / mm-yyyy / yyyy.\n" RESET);
    fclose(f);
}
// ==========================================================

// =================== 6. LAPORAN BULANAN ===================
void laporanBulanan() {
    int bulan, tahun;
    int valid;

    while (1) {
        printf("Masukkan bulan (1-12)\t\t: ");
        valid = scanf("%d", &bulan);
        if (valid != 1) {
            clearInput();
            printf(RED "Input bulan tidak valid!\n" RESET);
            continue;
        }
        clearInput();
        if (bulan < 1 || bulan > 12) {
            printf(RED "Bulan harus di antara 1 sampai 12.\n" RESET);
            continue;
        }
        break;
    }

    while (1) {
        printf("Masukkan tahun (contoh: 2025): ");
        valid = scanf("%d", &tahun);
        if (valid != 1) {
            clearInput();
            printf(RED "Input tahun tidak valid!\n" RESET);
            continue;
        }
        clearInput();
        if (tahun < 1900 || tahun > 9999) {
            printf(RED "Tahun tidak masuk akal. Coba lagi.\n" RESET);
            continue;
        }
        break;
    }

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "File tidak ditemukan.\n" RESET);
        return;
    }

    char line[512];
    Data d;
    double totalMasuk = 0.0, totalKeluar = 0.0;
    int any = 0;

    header();
    printf(CYAN "===== LAPORAN BULANAN: %02d-%04d =====\n" RESET, bulan, tahun);
    headerTabel();

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                   &d.id, d.tanggal, d.jenis, &d.jumlah, d.keterangan) == 5) {
            int dd, mm, yy;
            if (sscanf(d.tanggal, "%d-%d-%d", &dd, &mm, &yy) == 3) {
                if (mm == bulan && yy == tahun) {
                    isiTabel(d);
                    any = 1;
                    if (strcmp(d.jenis, "masuk") == 0) totalMasuk += d.jumlah;
                    else totalKeluar += d.jumlah;
                }
            }
        }
    }

    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    if (!any) {
        printf(YELLOW "Tidak ada transaksi pada %02d-%04d\n" RESET, bulan, tahun);
    }
    printf("Total Masuk   : %.2lf\n", totalMasuk);
    printf("Total Keluar  : %.2lf\n", totalKeluar);
    printf("Saldo Period  : %.2lf\n", totalMasuk - totalKeluar);

    fclose(f);
    pauseClear();

}

void laporanTahunan() {
    int tahun;
    int valid;

    while (1) {
        printf("Masukkan tahun (contoh: 2025): ");
        valid = scanf("%d", &tahun);
        if (valid != 1) {
            clearInput();
            printf(RED "Input tahun tidak valid!\n" RESET);
            continue;
        }
        clearInput();
        if (tahun < 0 || tahun > 9999) {
            printf(RED "Tahun tidak masuk akal. Coba lagi.\n" RESET);
            continue;
        }
        break;
    }

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        printf(RED "File tidak ditemukan.\n" RESET);
        return;
    }

    char line[512];
    Data d;
    double totalMasuk = 0.0, totalKeluar = 0.0;
    int any = 0;

    printf(CYAN "===== LAPORAN TAHUNAN: %04d =====\n" RESET, tahun);
    headerTabel();

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%10[^|]|%9[^|]|%lf|%199[^\n]",
                   &d.id, d.tanggal, d.jenis, &d.jumlah, d.keterangan) == 5) {
            int dd, mm, yy;
            if (sscanf(d.tanggal, "%d-%d-%d", &dd, &mm, &yy) == 3) {
                if (yy == tahun) {
                    isiTabel(d);
                    any = 1;
                    if (strcmp(d.jenis, "masuk") == 0) totalMasuk += d.jumlah;
                    else totalKeluar += d.jumlah;
                }
            }
        }
    }

    printf(MAGENTA "---------------------------------------------------------------------------------------------\n" RESET);
    if (!any) {
        printf(YELLOW "Tidak ada transaksi pada tahun %04d\n" RESET, tahun);
    }
    printf("Total Masuk   : %.2lf\n", totalMasuk);
    printf("Total Keluar  : %.2lf\n", totalKeluar);
    printf("Saldo Period  : %.2lf\n", totalMasuk - totalKeluar);

    fclose(f);
    pauseClear();
}

void laporan() {
    int pilihan;
    int valid;

    while (1) {
        printf("======== LAPORAN ========\n");
        printf("|| 0. Kembali          ||\n");
        printf("|| 1. Laporan Bulanan  ||\n");
        printf("|| 2. Laporan Tahunan  ||\n");
        printf("========================\n");
        printf("Pilih: ");
        valid = scanf("%d", &pilihan);
        if (valid != 1) {
            clearInput();
            printf(RED "Input tidak valid!\n" RESET);
            pauseClear();
            continue;
        }
        clearInput();

        if (pilihan == 0) return;
        else if (pilihan == 1) {
            laporanBulanan();   
            return;             
        }
        else if (pilihan == 2) {
            laporanTahunan();   
            return;
        }         
        else {
            printf(RED "Pilihan tidak valid!\n" RESET);
            pauseClear();
        }
    }
}
//==========================================================
