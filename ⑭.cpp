#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <string>
#include <map>


using namespace std;

// �Z�N���X
class Waza {
public:
    string name;
    int baseDamage;

    Waza() : name(""), baseDamage(0) {} // �f�t�H���g�R���X�g���N�^�[
    Waza(string n, int d) : name(n), baseDamage(d) {} // �p�����[�^�t���R���X�g���N�^�[
};

// �|�P�����N���X
class Pokemon {
public:
    string name;
    int HP;
    int attack;
    int defense;
    map<int, Waza> wazaMap;

    Pokemon(string n, int hp, int atk, int def, map<int, Waza> wz) 
        : name(n), HP(hp), attack(atk), defense(def), wazaMap(wz) {}

    void useWaza(int wazaNumber, Pokemon* target) {
        if (wazaMap.find(wazaNumber) != wazaMap.end()) {
            Waza waza = wazaMap[wazaNumber];
            int damage = waza.baseDamage + attack - target->defense;
            if (damage < 0) damage = 0; 
            target->HP -= damage;
            if (target->HP < 0) target->HP = 0; 
            cout << waza.name << "�I" << endl;
            cout << target->name << "��" << damage << "�̃_���[�W�I�@HP: " << target->HP << endl;
        } else {
            cout << "�����ȋZ�̔ԍ��ł��B1�`4�őI�����Ă��������B" << endl;
        }
    }
};

void displayWazaOptions(Pokemon* pokemon) {
    for (auto it = pokemon->wazaMap.begin(); it != pokemon->wazaMap.end(); ++it) {
        cout << it->first << ": " << it->second.name << endl;
    }
    cout << endl;
}

void createWazaFiles() {
    const char* filenames[] = {
        "waza1.txt",
        "waza2.txt",
        "waza3.txt",
        "waza4.txt"
    };

    const char* contents[] = {
        "1:������ق�����\n2:�̓�����\n3:����\n4:�Ђ�����",
        "1:�\���{���g\n2:�̓�����\n3:�A�C�A���e�[��\n4:�Ђ�����",
        "1:�͂��σJ�b�^�[\n2:�̓�����\n3:�����肫\n4:�͂���",
        "1:�݂��ł��ۂ�\n2:�̓�����\n3:�����肫\n4:�Ђ�����"
    };

    for (int i = 0; i < 4; ++i) {
        ofstream outfile(filenames[i]);
        if (!outfile) {
            cerr << "�t�@�C�����J���܂���ł���: " << filenames[i] << endl;
            return;
        }
        outfile << contents[i];
        outfile.close();
    }
}

map<int, Waza> loadWazaFromFile(const char* filename) {
    map<int, Waza> wazaMap;
    ifstream infile(filename);
    if (!infile) {
        cerr << "�t�@�C�����J���܂���ł���: " << filename << endl;
        return wazaMap;
    }

    string line;
    while (getline(infile, line)) {
        int number;
        string name;
        size_t pos1 = line.find(':');
        size_t pos2 = line.find('\n', pos1 + 1);

        if (pos1 != string::npos) {
            number = stoi(line.substr(0, pos1));
            name = line.substr(pos1 + 1);
            name.erase(remove(name.begin(), name.end(), '\n'), name.end()); 
            wazaMap[number] = Waza(name, 0); 
        }
    }
    infile.close();

    // �_���[�W�̐ݒ�
    if (filename == string("waza1.txt")) {
        wazaMap[1].baseDamage = 40; wazaMap[2].baseDamage = 40; wazaMap[3].baseDamage = 35; wazaMap[4].baseDamage = 25;
    } else if (filename == string("waza2.txt")) {
        wazaMap[1].baseDamage = 45; wazaMap[2].baseDamage = 40; wazaMap[3].baseDamage = 45; wazaMap[4].baseDamage = 25;
    } else if (filename == string("waza3.txt")) {
        wazaMap[1].baseDamage = 35; wazaMap[2].baseDamage = 40; wazaMap[3].baseDamage = 45; wazaMap[4].baseDamage = 25;
    } else if (filename == string("waza4.txt")) {
        wazaMap[1].baseDamage = 30; wazaMap[2].baseDamage = 40; wazaMap[3].baseDamage = 45; wazaMap[4].baseDamage = 25;
    }

    return wazaMap;
}

int main() {
    map<int, Waza> waza1 = loadWazaFromFile("waza1.txt");
    map<int, Waza> waza2 = loadWazaFromFile("waza2.txt");
    map<int, Waza> waza3 = loadWazaFromFile("waza3.txt");
    map<int, Waza> waza4 = loadWazaFromFile("waza4.txt");

    //�|�P�����̃p�����[�^�A�Z��R�Â�
    Pokemon hitokage("�q�g�J�Q", 60, 20, 25, waza1);
    Pokemon pikachu("�s�J�`���E", 55, 25, 25, waza2);
    Pokemon fushigidane("�t�V�M�_�l", 60, 15, 35, waza3);
    Pokemon zenigame("�[�j�K��", 65, 25, 20, waza4);

    vector<Pokemon*> pokemons = { &hitokage, &pikachu, &fushigidane, &zenigame };

    random_device rd;
    mt19937 ram(rd());
    uniform_int_distribution<> pokeDist(0, 3);
    int index = pokeDist(ram);

    Pokemon* selectedPokemon = pokemons[index];
    Pokemon* opponent = nullptr;

    for (auto pokemon : pokemons) {
        if (pokemon != selectedPokemon) {
            opponent = pokemon;
            break;
        }
    }

    cout << "����|�P����: " << opponent->name << endl;
    cout << "�����̃|�P����: " << selectedPokemon->name << endl;

    createWazaFiles();

    // �����̃|�P�����̋Z��\��
    displayWazaOptions(selectedPokemon);

    while (selectedPokemon->HP > 0 && opponent->HP > 0) {
        int wazaNumber;
        cout << "\n�g�p����Z�̔ԍ���I��ł������� (1�`4): ";
        cin >> wazaNumber;

        if (cin.fail() || wazaNumber < 1 || wazaNumber > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�����ȋZ�̔ԍ��ł��B1�`4�őI�����Ă��������B" << endl;
            continue;
        }

        cout << endl;
        selectedPokemon->useWaza(wazaNumber, opponent);

        if (opponent->HP <= 0) {
            cout << endl;
            cout << opponent->name << "�͓|�ꂽ�I" << endl;
            cout << selectedPokemon->name << "�̏���" << endl;
            break;
        }

        // ����̃|�P�������Z��I��
        uniform_int_distribution<> wazaDist(1, 4);
        int opponentWazaNumber = wazaDist(ram);
        if (opponent->wazaMap.find(opponentWazaNumber) != opponent->wazaMap.end()) {
            cout << endl;
            opponent->useWaza(opponentWazaNumber, selectedPokemon);

            if (selectedPokemon->HP <= 0) {
                cout << endl;
                cout << selectedPokemon->name << "�͓|�ꂽ�I" << endl;
                cout << opponent->name << "�ɕ������c" << endl;
                break;
            }
        }
    }

    return 0;
}