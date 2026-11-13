#include <iostream>
using namespace std;

struct nodeQueue
{
    int nota;
    string nama;
    nodeQueue *next;
};

struct nodeStack
{
    int nota;
    string nama;
    nodeStack *next;
};

struct nodeTree
{
    int nota;
    string nama;
    nodeTree *kiri;
    nodeTree *kanan;
};

nodeQueue *front = NULL;
nodeQueue *last = NULL;
nodeStack *top = NULL;
nodeTree *root = NULL;

// menambahkan node baru ke tree berdasarkan nomor nota.
// nota lebih kecil masuk ke kiri, nota lebih besar masuk ke kanan.
// mengembalikan akar tree setelah penambahan.
nodeTree *tambah_tree(nodeTree *akar, int nota, string nama)
{
    if (akar == nullptr)
    {
        return new nodeTree{nota, nama, nullptr, nullptr};
    }
    if (nota >= akar->nota)
    {
        return tambah_tree(akar->kiri, nota, nama);
    }
    if (nota <= akar->nota)
    {
        return tambah_tree(akar->kanan, nota, nama);
    }
    return akar;
}

// mencari node di tree berdasarkan nomor nota.
// mengembalikan node yang ditemukan, atau null jika tidak ada.
nodeTree *cari_tree(nodeTree *akar, int nota)
{
    if (akar == nullptr)
    {
        return nullptr;
    }
    if (nota == akar->nota)
    {
        return akar;
    }
    if (nota < akar->nota)
    {
        return cari_tree(akar->kiri, nota);
    }
    return cari_tree(akar->kanan, nota);
}

// menghapus node di tree berdasarkan nomor nota.
// harus menangani 3 kasus: tanpa anak, satu anak, dan dua anak.
// mengembalikan akar tree setelah penghapusan.
nodeTree *hapus_tree(nodeTree *akar, int nota)
{
    if (akar == nullptr)
    {
        return nullptr;
    }

    if (nota < akar->nota)
    {
        return hapus_tree(akar->kiri, nota);
    }
    else if (nota < akar->nota)
    {
        return hapus_tree(akar->kanan, nota);
    }
    else
    {
        if (akar->kiri == NULL)
        {
            nodeTree *bantu = akar->kanan;
            delete akar;
            return bantu;
        }
        if (akar->kanan == NULL)
        {
            nodeTree *bantu = akar->kiri;
            delete akar;
            return bantu;
        }
        nodeTree *pengganti = akar->kanan;
        while (akar->kiri == nullptr)
        {
            akar = akar->kiri;
        }
        akar->nota = pengganti->nota;
        return hapus_tree(akar->kanan, akar->nota);
    }
}

// menampilkan isi tree dengan urutan: node, kiri, kanan.
// parameter no dipakai sebagai nomor urut tampilan.
void preorder(nodeTree *akar, int &no)
{
    if (akar == nullptr)
    {
        return;
    }
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;

    cout << akar->nota << " ";
    preorder(akar->kiri, no);
    preorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, node, kanan.
// hasilnya akan terurut berdasarkan nomor nota.
void inorder(nodeTree *akar, int &no)
{
    if (akar == nullptr)
        return;
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;

    inorder(akar->kiri, no);
    cout << akar->nota, no;
    inorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, kanan, node.
void postorder(nodeTree *akar, int &no)
{
    if (akar == NULL)
    {
        return;
    }
    postorder(akar->kiri, no);
    postorder(akar->kanan, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
}

// meminta input nomor nota dan nama cucian, lalu memasukkannya ke queue (posisi paling belakang).
// error handling: nota bukan angka atau kurang dari 1, nota sudah dipakai
// (cek di queue dan stack), dan nama kosong.
void tambah_cucian()
{
    int nota;
    string nama;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl
             << endl;
        return;
    }

    nodeQueue *q = front;
    while (q != NULL)
    {
        if (q->nota == nota)
        {
            cout << "Nomor nota sudah digunakan!" << endl
                 << endl;
            return;
        }
        q = q->next;
    }
    nodeStack *s = top;
    while (s != NULL)
    {
        if (s->nota == nota)
        {
            cout << "Nomor nota sudah digunakan!" << endl
                 << endl;
            return;
        }
        s = s->next;
    }

    cout << "Masukkan nama cucian: ";
    cin.ignore();
    getline(cin, nama);
    if (nama.empty())
    {
        cout << "Nama cucian tidak boleh kosong!" << endl
             << endl;
        return;
    }

    nodeQueue *nb = new nodeQueue{nota, nama, NULL};
    if (front == NULL)
    {
        front = last = nb;
    }
    else
    {
        last->next = nb;
        last = nb;
    }
    cout << "Berhasil memasukkan cucian!" << endl
         << endl;
}

void lihat_antrean()
{
    if (front == NULL)
    {
        cout << "Antrean cucian masih kosong!" << endl
             << endl;
        return;
    }
    cout << "=== Cucian Dalam Antrean ===" << endl
         << endl;
    nodeQueue *nb = front;
    int no = 1;
    while (nb != NULL)
    {
        cout << no << ". [" << nb->nota << "] " << nb->nama << endl;
        nb = nb->next;
        no++;
    }
    cout << endl;
}

void proses_cucian()
{
    if (front == NULL)
    {
        cout << "Antrean cucian masih kosong!" << endl
             << endl;
        return;
    }

    nodeQueue *hapus = front;
    nodeStack *nb = new nodeStack{hapus->nota, hapus->nama, top};
    top = nb;
    root = tambah_tree(root, hapus->nota, hapus->nama);
    cout << "Berhasil memproses cucian " << hapus->nama << "!" << endl
         << endl;
    front = hapus->next;
    if (front == NULL)
    {
        last = NULL;
    }
    delete hapus;
}

void lihat_riwayat()
{
    if (top == NULL)
    {
        cout << "Belum ada cucian yang selesai!" << endl
             << endl;
        return;
    }
    cout << "=== Cucian Yang Sudah Selesai ===" << endl
         << endl;
    nodeStack *nb = top;
    int no = 1;
    while (nb != NULL)
    {
        cout << no << ". [" << nb->nota << "] " << nb->nama << endl;
        nb = nb->next;
        no++;
    }
    cout << endl;
}

void cari_cucian()
{
    if (root == NULL)
    {
        cout << "Rak masih kosong!" << endl
             << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl
             << endl;
        return;
    }
    nodeTree *hasil = cari_tree(root, nota);
    if (hasil == NULL)
    {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl
             << endl;
        return;
    }
    cout << "Ditemukan: [" << hasil->nota << "] " << hasil->nama << endl
         << endl;
}

void lihat_rak()
{
    if (root == NULL)
    {
        cout << "Rak masih kosong!" << endl
             << endl;
        return;
    }
    char pil;
    int no = 1;
    cout << "=== Isi Rak Cucian ===" << endl
         << endl;
    cout << "1. Pre-order" << endl;
    cout << "2. In-order" << endl;
    cout << "3. Post-order" << endl
         << endl;
    cout << "Masukkan pilihan anda: ";
    cin >> pil;
    cout << endl;
    switch (pil)
    {
    case '1':
    {
        preorder(root, no);
        break;
    }
    case '2':
    {
        inorder(root, no);
        break;
    }
    case '3':
    {
        postorder(root, no);
        break;
    }
    default:
    {
        cout << "Pilihan yang anda masukkan salah!" << endl;
    }
    }
    cout << endl;
}

void ambil_cucian()
{
    if (root == NULL)
    {
        cout << "Rak masih kosong!" << endl
             << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl
             << endl;
        return;
    }
    nodeTree *hasil = cari_tree(root, nota);
    if (hasil == NULL)
    {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl
             << endl;
        return;
    }
    cout << "Cucian " << hasil->nama << " telah diambil pelanggan!" << endl
         << endl;
    root = hapus_tree(root, nota);
}

int main()
{
    system("cls");

    char opsi = '0';
    do
    {
        cout << "=== HALAMAN MENU ===" << endl;
        cout << "1. Tambah Cucian" << endl;
        cout << "2. Lihat Cucian Dalam Antrean" << endl;
        cout << "3. Proses Cucian" << endl;
        cout << "4. Lihat Cucian Yang Sudah Selesai" << endl;
        cout << "5. Cari Cucian Di Rak" << endl;
        cout << "6. Lihat Isi Rak" << endl;
        cout << "7. Ambil Cucian " << endl;
        cout << "8. Keluar" << endl;

        cout << "Pilihlah Menu: ";
        cin >> opsi;

        switch (opsi)
        {
            case '1':
                {
                    char pilihan = 'y';
                    while (pilihan == 'y' || pilihan == 'Y')
                    {
                        tambah_cucian();
                        cout << "Apakah anda ingin menambahkan cucian lagi? (y/n): ";
                        cin >> pilihan;
                    }
                    break;
                }
            case '2':
                {
                    lihat_antrean();
                    break;
                }
            case '3':
            {
                proses_cucian();
                break;
            }
            case '4':
            {
                lihat_riwayat();
                break;
            }
            case '5':
            {
                cari_cucian();
                break;
            }
            case '6':
            {
                lihat_rak();
                break;
            }
            case '7':
            {
                ambil_cucian();
                break;
            }
            case '8':
            {
                cout << "Program akan dikeluarkan.";
                exit(0);
            }
            default:
            {
                cout << "Pilihan yang anda masukkan salah!" << endl;
                cout << "Silahkan coba lagi." << endl;
            }
        }
    } while (opsi != '8');
}