#include <iostream>
#include <cstdlib>
#include <string>
#include <conio.h>

using namespace std;

struct node {
	int id;
	string nama;
	string telepon;
	node* left;
	node* right;
	int height;
};

// Fungsi untuk mencari tinggi dari sebuah node
int height(node* N) {
	if (N == NULL){
		return 0;
	}
	return N->height;
}

// Fungsi untuk menginisialisasi sebuah node baru
node* newNode(int id, string nama, string telepon) {
	node* nodeBaru = new node();
	nodeBaru->id = id;
	nodeBaru->nama = nama;
	nodeBaru->telepon = telepon;
	nodeBaru->left = NULL;
	nodeBaru->right = NULL;
	nodeBaru->height = 1;
	return nodeBaru;
}

// Fungsi untuk mencari node dengan id terkecil di subtree yang diberikan
node* minValueNode(node* nodeAkar) {
	node* current = nodeAkar;

	while (current->left != NULL){
		current = current->left;
	}

	return current;
}
// Fungsi untuk mendapatkan balance factor dari sebuah node
int getBalance(node* N) {
	if (N == NULL){
		return 0;
		}
	return height(N->left) - height(N->right);
}
// Fungsi untuk memutar node ke kanan
node* rightRotate(node* y) {
	node* x = y->left;
	node* T2 = x->right;

	// Putar
	x->right = y;
	y->left = T2;

	// Update tinggi
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;
	
	// Kembalikan node yang telah diputar
	return x;
}

// Fungsi untuk memutar node ke kiri
node* leftRotate(node* x) {
	node* y = x->right;
	node* T2 = y->left;
	
	// Putar
	y->left = x;
	x->right = T2;
	
	// Update tinggi
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	
	// Kembalikan node yang telah diputar
	return y;
}

// Fungsi untuk insert node ke dalam AVL Tree
node* insertAVL(node* nodeAkar, int id, string nama, string telepon) {
	// Jika tidak ada node yang tersedia, buat node baru
	if (nodeAkar == NULL){
		return newNode(id, nama, telepon);
	}
	
	// Jika id yang ingin dimasukkan lebih kecil dari id dari node akar, masukkan ke subtree kiri
	if (id < nodeAkar->id){
		nodeAkar->left = insertAVL(nodeAkar->left, id, nama, telepon);
	}
	// Jika id yang ingin dimasukkan lebih besar dari id dari node akar, masukkan ke subtree kanan
	else if (id > nodeAkar->id){
		nodeAkar->right = insertAVL(nodeAkar->right, id, nama, telepon);
	}
	// Jika id yang ingin dimasukkan sama dengan id dari node akar, abaikan insert
	else{
		return nodeAkar;
	}

	// Update tinggi node akar
	nodeAkar->height = 1 + max(height(nodeAkar->left), height(nodeAkar->right));

	// Hitung balance factor dari node akar
	int balance = getBalance(nodeAkar);
	
	// Jika balance factor lebih besar dari 1, berarti terjadi imbalance
	// Kemudian cek apakah terjadi imbalance kiri-kiri atau kiri-kanan
	if (balance > 1 && id < nodeAkar->left->id){
		return rightRotate(nodeAkar);
	}
	if (balance < -1 && id > nodeAkar->right->id){
		return leftRotate(nodeAkar);
	}
	if (balance > 1 && id > nodeAkar->left->id) {
		nodeAkar->left = leftRotate(nodeAkar->left);
		return rightRotate(nodeAkar);
	}
	if (balance < -1 && id < nodeAkar->right->id) {
		nodeAkar->right = rightRotate(nodeAkar->right);
		return leftRotate(nodeAkar);
	}

	// Jika tidak terjadi imbalance, kembalikan node akar
	return nodeAkar;
}

// Fungsi untuk delete node dari AVL Tree
node* deleteNodeAVL(node* nodeAkar, int id) {
	// Jika tidak ada node yang tersedia, kembalikan NULL
	if (nodeAkar == NULL){
		cout << "Data tidak ditemukan.\n";
		return NULL;
	}
	
	// Jika id yang ingin dihapus lebih kecil dari id dari node akar, hapus dari subtree kiri
	if (id < nodeAkar->id){
		nodeAkar->left = deleteNodeAVL(nodeAkar->left, id);
	}
	// Jika id yang ingin dihapus lebih besar dari id dari node akar, hapus dari subtree kanan
	else if (id > nodeAkar->id){
		nodeAkar->right = deleteNodeAVL(nodeAkar->right, id);
	}
	// Jika id yang ingin dihapus sama dengan id dari node akar
	else {
		// Jika node akar hanya memiliki salah satu child atau tidak memiliki child sama sekali
		if ((nodeAkar->left == NULL) || (nodeAkar->right == NULL)) {
			node* temp = nodeAkar->left ? nodeAkar->left : nodeAkar->right;
			// Jika tidak memiliki child sama sekali, hapus node akar
			if (temp == NULL) {
				temp = nodeAkar;
				nodeAkar = NULL;
			}else // Jika memiliki salah satu child, copy child ke node akar
			{*nodeAkar = *temp;
			  free(temp);
			}
		} else {
			// Jika node akar memiliki 2 child, cari node dengan id terkecil di subtree kanan
			node* temp = minValueNode(nodeAkar->right);
				// Copy isi node dengan id terkecil ke node akar
			nodeAkar->id = temp->id;
			nodeAkar->nama = temp->nama;
			nodeAkar->telepon = temp->telepon;
				// Hapus node dengan id terkecil dari subtree kanan
			nodeAkar->right = deleteNodeAVL(nodeAkar->right, temp->id);
		}
	}
	// Jika tree tidak memiliki node, kembalikan NULL
	if (nodeAkar == NULL){
		return NULL;
	}

	// Update tinggi node akar
	nodeAkar->height = 1 + max(height(nodeAkar->left), height(nodeAkar->right));

	// Hitung balance factor dari node akar
	int balance = getBalance(nodeAkar);

	// Jika balance factor lebih besar dari 1, berarti terjadi imbalance
	// Kemudian cek apakah terjadi imbalance kiri-kiri atau kiri-kanan
	if (balance > 1 && getBalance(nodeAkar->left) >= 0){
		return rightRotate(nodeAkar);
	}
	if (balance > 1 && getBalance(nodeAkar->left) < 0) {
		nodeAkar->left = leftRotate(nodeAkar->left);
		return rightRotate(nodeAkar);
	}	
	if (balance < -1 && getBalance(nodeAkar->right) <= 0){
		return leftRotate(nodeAkar);
	}
	if (balance < -1 && getBalance(nodeAkar->right) > 0) {
		nodeAkar->right = rightRotate(nodeAkar->right);
		return leftRotate(nodeAkar);
	}

	// Jika tidak terjadi imbalance, kembalikan node akar
	return nodeAkar;
}

// Fungsi untuk insert node ke dalam BST
node* insertBST(node* nodeAkar, int id, string nama, string telepon) {
	// Jika tidak ada node yang tersedia, buat node baru
	if (nodeAkar == NULL){
		return newNode(id, nama, telepon);
	}

	// Jika id yang ingin dimasukkan lebih kecil dari id dari node akar, masukkan ke subtree kiri
	if (id < nodeAkar->id){
		nodeAkar->left = insertBST(nodeAkar->left, id, nama, telepon);
	}
	// Jika id yang ingin dimasukkan lebih besar dari id dari node akar, masukkan ke subtree kanan
	else if (id > nodeAkar->id){
		nodeAkar->right = insertBST(nodeAkar->right, id, nama, telepon);
	}
	// Jika id yang ingin dimasukkan sama dengan id dari node akar, abaikan insert
	else{
		return nodeAkar;
	}

	// Kembalikan node akar
	return nodeAkar;
}

// Fungsi untuk delete node dari BST
node* deleteNodeBST(node* nodeAkar, int id) {
	// Jika tidak ada node yang tersedia, kembalikan NULL
	if (nodeAkar == NULL){
		cout << "Data tidak ditemukan.\n";
		return NULL;
	}

	// Jika id yang ingin dihapus lebih kecil dari id dari node akar, hapus dari subtree kiri
	if (id < nodeAkar->id){
		nodeAkar->left = deleteNodeBST(nodeAkar->left, id);
	}
	// Jika id yang ingin dihapus lebih besar dari id dari node akar, hapus dari subtree kanan
	else if (id > nodeAkar->id){
		nodeAkar->right = deleteNodeBST(nodeAkar->right, id);
	}
	// Jika id yang ingin dihapus sama dengan id dari node akar
	else {
		// Jika node akar hanya memiliki salah satu child atau tidak memiliki child sama sekali
		if ((nodeAkar->left == NULL) || (nodeAkar->right == NULL)) {
			node* temp = nodeAkar->left ? nodeAkar->left : nodeAkar->right;
			// Jika tidak memiliki child sama sekali, hapus node akar
			if (temp == NULL) {
				temp = nodeAkar;
				nodeAkar = NULL;
			}else {// Jika memiliki salah satu child, copy child ke node akar
				*nodeAkar = *temp;
				free(temp);
			}
		}else {
			// Jika node akar memiliki 2 child, cari node dengan id terkecil di subtree kanan
			node* temp = minValueNode(nodeAkar->right);

			// Copy isi node dengan id terkecil ke node akar
			nodeAkar->id = temp->id;
			nodeAkar->nama = temp->nama;
			nodeAkar->telepon = temp->telepon;

			// Hapus node dengan id terkecil dari subtree kanan
			nodeAkar->right = deleteNodeBST(nodeAkar->right, temp->id);
		}
	}
	// Jika tree tidak memiliki node, kembalikan NULL
	if (nodeAkar == NULL){
		return nodeAkar;
	}
	// Kembalikan node akar
	return nodeAkar;
}

// Fungsi untuk mencari node dengan id yang diberikan
node* search(node* nodeAkar, int id) {
	// Jika tidak ada node yang tersedia atau id yang dicari tidak ditemukan, kembalikan NULL
	if (nodeAkar == NULL || nodeAkar->id == id){
		return nodeAkar;
	}

	// Jika id yang dicari lebih kecil dari id dari node akar, cari di subtree kiri
	if (id < nodeAkar->id){
		return search(nodeAkar->left, id);
	}

	// Jika id yang dicari lebih besar dari id dari node akar, cari di subtree kanan
	return search(nodeAkar->right, id);
}

// Fungsi untuk menampilkan isi dari AVL Tree
void printAVL(node* nodeAkar) {
	if (nodeAkar != NULL) {
		printAVL(nodeAkar->left);
		cout << "ID: " << nodeAkar->id << ", Nama: " << nodeAkar->nama << ", Telepon: " << nodeAkar->telepon << endl;
		printAVL(nodeAkar->right);
	}
}

// Fungsi untuk menampilkan isi dari BST
void printBST(node* nodeAkar) {
	if (nodeAkar != NULL) {
		printBST(nodeAkar->left);
		cout << "ID: " << nodeAkar->id << ", Nama: " << nodeAkar->nama << ", Telepon: " << nodeAkar->telepon << endl;
		printBST(nodeAkar->right);
	}
}

int main() {
	node* rootAVL = NULL;
	node* rootBST = NULL;
	int pilihan;
	int id;
	string nama;
	string telepon;
	//mengisi data awal BST
	rootBST = insertBST(rootBST, 2212, "Nina", "0812345"); rootBST = insertBST(rootBST, 2202, "Nani", "0842345"); rootBST = insertBST(rootBST, 2210, "Nasywa", "0816345");
	rootBST = insertBST(rootBST, 2208, "Nurul", "0815345"); rootBST = insertBST(rootBST, 2220, "Anton", "0812346");rootBST = insertBST(rootBST, 2206, "Faiz", "0812555");
	rootBST = insertBST(rootBST, 2230, "Devin", "0815344"); rootBST = insertBST(rootBST, 2201, "Lulu", "0812326");rootBST = insertBST(rootBST, 2225, "Hasta", "0812455");
	rootBST = insertBST(rootBST, 2215, "Goku", "0815145");
	//Mengisi data awal AVL
	rootAVL = insertAVL(rootAVL, 2212, "Nina", "0812345"); rootAVL = insertAVL(rootAVL, 2202, "Nani", "0842345"); rootAVL = insertAVL(rootAVL, 2210, "Nasywa", "0816345");
	rootAVL = insertAVL(rootAVL, 2208, "Nurul", "0815345"); rootAVL = insertAVL(rootAVL, 2220, "Anton", "0812346");rootAVL = insertAVL(rootAVL, 2206, "Faiz", "0812555");
	rootAVL = insertAVL(rootAVL, 2230, "Devin", "0815344"); rootAVL = insertAVL(rootAVL, 2201, "Lulu", "0812326");rootAVL = insertAVL(rootAVL, 2225, "Hasta", "0812455");
	rootAVL = insertAVL(rootAVL, 2215, "Goku", "0815145");
	
	do {
		system("cls");
		cout<<"========================================"<<endl;
		cout<<"              KELOMPOK 8 "<<endl;
		cout<<"========================================"<<endl;
		cout<<"           Anggota Kelompok :"<<endl;
		cout<<"1. Daffa Kuanta Rahadian  -  20081010200"<<endl;
		cout<<"2. Faiz Nur Budi          -  21081010113"<<endl;
		cout<<"3. Nasywa Andin Safitri   -  21081010265"<<endl;
		cout<<"========================================\n"<<endl;
		cout<<"========================================"<<endl;
		cout<<"           PROGRAM DATA KONSUMEN        "<<endl;
		cout<<"              PROVIDER XYZ              "<<endl;
		cout<<"========================================"<<endl;
		cout << "Menu:" << endl;
		cout << "1. Insert node ke dalam AVL Tree" << endl;
		cout << "2. Insert node ke dalam BST" << endl;
		cout << "3. Delete node dari AVL Tree" << endl;
		cout << "4. Delete node dari BST" << endl;
		cout << "5. Search node" << endl;
		cout << "6. Tampilkan isi AVL Tree" << endl;
		cout << "7. Tampilkan isi BST" << endl;
		cout << "0. Keluar" << endl;
		cout << "Pilihan: ";
		cin >> pilihan;
		switch (pilihan) {
			case 1:{
				system ("cls");
				cout << "Masukkan ID Konsumen: ";
				cin >> id;
				cout << "Masukkan Nama Konsumen: ";
				cin >> nama;
				cout << "Masukkan Nomor Telepon: ";
				cin >> telepon;	
				rootAVL = insertAVL(rootAVL, id, nama, telepon);
				cout << "\nData Telah Ditambahkan" << endl;
				getch();
				break;
			}
			case 2:{
				system ("cls");
				cout << "Masukkan ID Konsumen: ";
				cin >> id;
				cout << "Masukkan Nama Konsumen: ";
				cin >> nama;
				cout << "Masukkan Nomor Telepon: ";
				cin >> telepon;
				rootBST = insertBST(rootBST, id, nama, telepon);
				cout << "\nData Telah Ditambahkan" << endl;
				getch();
				break;
			}
			case 3:{
				system ("cls");
				cout << "Masukkan ID yang ingin dihapus: ";
				cin >> id;
				rootAVL = deleteNodeAVL(rootAVL,id);
				getch();
			}
			break;
			case 4:{
				system ("cls");
				cout << "Masukkan ID yang ingin dihapus: ";
				cin >> id;
				rootBST = deleteNodeBST(rootBST, id);
				getch();
				break;
			}
			case 5:{
				system ("cls");
				cout << "Masukkan ID yang ingin dicari: ";
				cin >> id;
				cout << "\nHasil Pencarian di AVL : "<< endl;
				node *foundAVL = search(rootAVL, id);
				if (foundAVL != NULL){
					cout << "id konsumen : " << foundAVL->id << endl;
					cout << "Nama konsumen : " << foundAVL->nama << endl;
					cout << "Telepon : " << foundAVL->telepon << endl;
				}
				else{
					cout << "ID tidak ditemukan" << endl;
				}

				cout << "\n\nHasil Pencarian di BST : "<< endl;	
				node *foundBST = search(rootBST, id);
				if (foundBST != NULL){
					cout << "id konsumen : " << foundBST->id << endl;
					cout << "Nama konsumen : " << foundBST->nama << endl;
					cout << "Telepon : " << foundBST->telepon << endl;
				}else{
					cout << "ID tidak ditemukan" << endl;
				}
				getch();
				break;
			}
			case 6:{
				system ("cls");
				cout << "Isi AVL Tree: " << endl;
				printAVL(rootAVL);
				getch();
				break;
			}
			case 7:{
				system ("cls");
				cout << "Isi BST: " << endl;
				printBST(rootBST);
				getch();
				break;
			}
		}
	} while (pilihan != 0);

	return 0;
}
