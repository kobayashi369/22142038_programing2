#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <string>
#include <map>


using namespace std;

// 技クラス
class Waza {
public:
    string name;
    int baseDamage;

    Waza() : name(""), baseDamage(0) {} // デフォルトコンストラクター
    Waza(string n, int d) : name(n), baseDamage(d) {} // パラメータ付きコンストラクター
};

// ポケモンクラス
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
            cout << waza.name << "！" << endl;
            cout << target->name << "に" << damage << "のダメージ！　HP: " << target->HP << endl;
        } else {
            cout << "無効な技の番号です。1～4で選択してください。" << endl;
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
        "1:かえんほうしゃ\n2:体当たり\n3:ずつき\n4:ひっかく",
        "1:十万ボルト\n2:体当たり\n3:アイアンテール\n4:ひっかく",
        "1:はっぱカッター\n2:体当たり\n3:かいりき\n4:はたく",
        "1:みずでっぽう\n2:体当たり\n3:かいりき\n4:ひっかく"
    };

    for (int i = 0; i < 4; ++i) {
        ofstream outfile(filenames[i]);
        if (!outfile) {
            cerr << "ファイルを開けませんでした: " << filenames[i] << endl;
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
        cerr << "ファイルを開けませんでした: " << filename << endl;
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

    // ダメージの設定
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

    //ポケモンのパラメータ、技を紐づけ
    Pokemon hitokage("ヒトカゲ", 60, 20, 25, waza1);
    Pokemon pikachu("ピカチュウ", 55, 25, 25, waza2);
    Pokemon fushigidane("フシギダネ", 60, 15, 35, waza3);
    Pokemon zenigame("ゼニガメ", 65, 25, 20, waza4);

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

    cout << "相手ポケモン: " << opponent->name << endl;
    cout << "自分のポケモン: " << selectedPokemon->name << endl;

    createWazaFiles();

    // 自分のポケモンの技を表示
    displayWazaOptions(selectedPokemon);

    while (selectedPokemon->HP > 0 && opponent->HP > 0) {
        int wazaNumber;
        cout << "\n使用する技の番号を選んでください (1～4): ";
        cin >> wazaNumber;

        if (cin.fail() || wazaNumber < 1 || wazaNumber > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "無効な技の番号です。1～4で選択してください。" << endl;
            continue;
        }

        cout << endl;
        selectedPokemon->useWaza(wazaNumber, opponent);

        if (opponent->HP <= 0) {
            cout << endl;
            cout << opponent->name << "は倒れた！" << endl;
            cout << selectedPokemon->name << "の勝利" << endl;
            break;
        }

        // 相手のポケモンが技を選択
        uniform_int_distribution<> wazaDist(1, 4);
        int opponentWazaNumber = wazaDist(ram);
        if (opponent->wazaMap.find(opponentWazaNumber) != opponent->wazaMap.end()) {
            cout << endl;
            opponent->useWaza(opponentWazaNumber, selectedPokemon);

            if (selectedPokemon->HP <= 0) {
                cout << endl;
                cout << selectedPokemon->name << "は倒れた！" << endl;
                cout << opponent->name << "に負けた…" << endl;
                break;
            }
        }
    }

    return 0;
}