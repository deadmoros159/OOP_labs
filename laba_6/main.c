#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

class NPC {
protected:
    string name;
    int x, y;
    
public:
    NPC(const string& name, int x, int y) : name(name), x(x), y(y) {}
    virtual ~NPC() = default;
    
    virtual string getType() const = 0;
    virtual void accept(class NPCVisitor& visitor) = 0;
    
    string getName() const { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
    virtual int getPower() const = 0;
    
    double distanceTo(const NPC& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
};

class NPCVisitor {
public:
    virtual ~NPCVisitor() = default;
    virtual void visit(class Dragon& dragon) = 0;
    virtual void visit(class Toad& toad) = 0;
    virtual void visit(class Knight& knight) = 0;
};

class Dragon : public NPC {
public:
    Dragon(const string& name, int x, int y) : NPC(name, x, y) {}
    
    string getType() const override { return "Dragon"; }
    void accept(NPCVisitor& visitor) override { visitor.visit(*this); }
    int getPower() const override { return 9; }
};

class Toad : public NPC {
public:
    Toad(const string& name, int x, int y) : NPC(name, x, y) {}
    
    string getType() const override { return "Toad"; }
    void accept(NPCVisitor& visitor) override { visitor.visit(*this); }
    int getPower() const override { return 10; }
};

class Knight : public NPC {
public:
    Knight(const string& name, int x, int y) : NPC(name, x, y) {}
    
    string getType() const override { return "Knight"; }
    void accept(NPCVisitor& visitor) override { visitor.visit(*this); }
    int getPower() const override { return 8; }
};

class NPCFactory {
public:
    virtual ~NPCFactory() {};
    virtual unique_ptr<NPC> createNPC(const string& data) = 0;
};

class ConcreteNPCFactory : public NPCFactory {
public:
    unique_ptr<NPC> createNPC(const string& data) override {
        istringstream iss(data);
        string type, name;
        int x, y;
        
        if (!(iss >> type >> name >> x >> y)) {
            return nullptr;
        }
        
        if (x < 0 || x > 500 || y < 0 || y > 500) {
            cerr << "Вне диапазона: " << x << ", " << y << endl;
            return nullptr;
        }
        
        if (type == "Dragon") {
            return make_unique<Dragon>(name, x, y);
        } else if (type == "Toad") {
            return make_unique<Toad>(name, x, y);
        } else if (type == "Knight") {
            return make_unique<Knight>(name, x, y);
        }
        
        return nullptr;
    }
};

class BattleObserver {
public:
    virtual ~BattleObserver() {};
    virtual void update(const string& event) = 0;
};

class ConsoleObserver : public BattleObserver {
public:
    void update(const string& event) override {
        cout << "[БОЙ] " << event << endl;
    }
};

class Observer_log : public BattleObserver {
    ofstream logFile;
    
public:
    Observer_log(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            cerr << "Не удалось открыть файл: " << filename << endl;
        }
    }
    
    ~Observer_log() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void update(const string& event) override {
        if (logFile.is_open()) {
            logFile << "[БОЙ] " << event << endl;
        }
    }
};

class BattleSubj {
    vector<BattleObserver*> observers;
    
public:
    void attach(BattleObserver* observer) {
        observers.push_back(observer);
    }
    
    void detach(BattleObserver* observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    
    void notify(const string& event) {
        for (auto observer : observers) {
            observer->update(event);
        }
    }
};

class BattleVisitor : public NPCVisitor {
private:
    NPC* currentAttacker;
    BattleSubj& subject;
    bool attackerWins;
    
public:
    BattleVisitor(NPC* attacker, BattleSubj& subject) 
        : currentAttacker(attacker), subject(subject), attackerWins(false) {}
    
    bool didAttackerWin() const { return attackerWins; }
    
    void visit(Dragon& dragon) override {
        Toad* toad = dynamic_cast<Toad*>(currentAttacker);
        Knight* knight = dynamic_cast<Knight*>(currentAttacker);
        
        if (toad) {
            attackerWins = true;
            subject.notify("Жаба '" + toad->getName() + "' съела дракона '" + dragon.getName() + "'");
        } else if (knight) {
            attackerWins = true;
            subject.notify("Рыцарь '" + knight->getName() + "' убил дракона '" + dragon.getName() + "'");
        } else {
            Dragon* dragonAttacker = dynamic_cast<Dragon*>(currentAttacker);
            if (dragonAttacker && dragonAttacker->getPower() > dragon.getPower()) {
                attackerWins = true;
                subject.notify("Дракон '" + dragonAttacker->getName() + "' убил дракона '" + dragon.getName() + "'");
            } else {
                attackerWins = false;
                subject.notify("Дракон '" + dragon.getName() + "' убил дракона '" + dragonAttacker->getName() + "'");
            }
        }
    }
    
    void visit(Toad& toad) override {
        attackerWins = true;
        
        Toad* toadAttacker = dynamic_cast<Toad*>(currentAttacker);
        if (toadAttacker) {
            subject.notify("Жаба '" + toadAttacker->getName() + "' съела жабу '" + toad.getName() + "'");
        } else {
            Dragon* dragon = dynamic_cast<Dragon*>(currentAttacker);
            Knight* knight = dynamic_cast<Knight*>(currentAttacker);
            
            if (dragon) {
                subject.notify("Жаба '" + toad.getName() + "' съела дракона '" + dragon->getName() + "'");
            } else if (knight) {
                subject.notify("Жаба '" + toad.getName() + "' съела рыцаря '" + knight->getName() + "'");
            }
        }
    }
    
    void visit(Knight& knight) override {
        Toad* toad = dynamic_cast<Toad*>(currentAttacker);
        Dragon* dragon = dynamic_cast<Dragon*>(currentAttacker);
        
        if (toad) {
            attackerWins = true;
            subject.notify("Жаба '" + toad->getName() + "' съела рыцаря '" + knight.getName() + "'");
        } else if (dragon) {
            attackerWins = true;
            subject.notify("Дракон '" + dragon->getName() + "' убил рыцаря '" + knight.getName() + "'");
        } else {
            Knight* knightAttacker = dynamic_cast<Knight*>(currentAttacker);
            if (knightAttacker && knightAttacker->getPower() > knight.getPower()) {
                attackerWins = true;
                subject.notify("Рыцарь '" + knightAttacker->getName() + "' убил рыцаря '" + knight.getName() + "'");
            } else {
                attackerWins = false;
                subject.notify("Рыцарь '" + knight.getName() + "' убил рыцаря '" + knightAttacker->getName() + "'");
            }
        }
    }
};

class DungeonEditor {
private:
    vector<unique_ptr<NPC>> npcs;
    unique_ptr<NPCFactory> factory;
    BattleSubj battleSubject;
    ConsoleObserver consoleObserver;
    Observer_log fileObserver;
    
public:
    DungeonEditor() 
        : factory(make_unique<ConcreteNPCFactory>()),
          fileObserver("log.txt") {
        battleSubject.attach(&consoleObserver);
        battleSubject.attach(&fileObserver);
    }
    
    void addNPC(unique_ptr<NPC> npc) {
        if (npc->getX() < 0 || npc->getX() > 500 || 
            npc->getY() < 0 || npc->getY() > 500) {
            cerr << "Координаты вне диапазона 0-500" << endl;
            return;
        }
        
        for (const auto& existing : npcs) {
            if (existing->getName() == npc->getName()) {
                cerr << "NPC с именем '" << npc->getName() << "' уже существует" << endl;
                return;
            }
        }
        
        npcs.push_back(move(npc));
        cout << "NPC добавлен!" << endl;
    }
    
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл: " << filename << endl;
            return;
        }
        
        for (const auto& npc : npcs) {
            file << npc->getType() << " " 
                 << npc->getName() << " " 
                 << npc->getX() << " " 
                 << npc->getY() << "\n";
        }
        
        file.close();
        cout << "Сохранено " << npcs.size() << " NPC в файл" << endl;
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл" << endl;
            return;
        }
        
        npcs.clear();
        string line;
        int count = 0;
        
        while (getline(file, line)) {
            auto npc = factory->createNPC(line);
            if (npc) {
                npcs.push_back(move(npc));
                count++;
            }
        }
        
        file.close();
        cout << "Загружено " << count << " NPC из файла" << endl;
    }
    
    void printAllNPCs() const {
        if (npcs.empty()) {
            cout << "В подземелье нет NPC." << endl;
            return;
        }
        
        cout << "\nСПИСОК NPC В ПОДЗЕМЕЛЬЕ" << endl;
        cout << "Всего: " << npcs.size() << " NPC\n" << endl;
        
        for (const auto& npc : npcs) {
            cout << "Тип: " << npc->getType() 
                      << ", Имя: " << npc->getName()
                      << ", Координаты: (" << npc->getX() 
                      << ", " << npc->getY() << ")" << endl;
        }
        cout << "Готово!\n" << endl;
    }
    
    void startBattleMode(double maxDistance) {
        cout << "\nБоевой режим" << endl;
        cout << "Дальность боя: " << maxDistance << endl;
        cout << "Участников: " << npcs.size() << endl;
        
        if (npcs.size() < 2) {
            cout << "Для боя нужно как минимум 2 NPC!" << endl;
            return;
        }
        
        bool battleOccurred = true;
        int round = 1;
        
        while (battleOccurred && npcs.size() > 1) {
            battleOccurred = false;
            cout << "\nРаунд " << round << endl;
            
            for (size_t i = 0; i < npcs.size() && npcs.size() > 1; ++i) {
                for (size_t j = 0; j < npcs.size(); ++j) {
                    if (i == j) continue;
                    
                    double dist = npcs[i]->distanceTo(*npcs[j]);
                    if (dist <= maxDistance) {
                        BattleVisitor visitor(npcs[i].get(), battleSubject);
                        npcs[j]->accept(visitor);
                        
                        if (visitor.didAttackerWin()) {
                            cout << "УДАЛЕН: " << npcs[j]->getType() 
                                      << " '" << npcs[j]->getName() << "'" << endl;
                            npcs.erase(npcs.begin() + j);
                            
                            if (j < i) i--;
                            j--;
                        } else {
                            cout << "УДАЛЕН: " << npcs[i]->getType() 
                                      << " '" << npcs[i]->getName() << "'" << endl;
                            npcs.erase(npcs.begin() + i);
                            i--;
                            break;
                        }
                        
                        battleOccurred = true;
                    }
                }
            }
            
            round++;
        }
        
        cout << "\nКонец боевого режима" << endl;
        cout << "В живых: " << npcs.size() << " NPC" << endl;
        
        if (!npcs.empty()) {
            cout << "Победитель(и):" << endl;
            for (const auto& npc : npcs) {
                cout << "  - " << npc->getType() << " '" << npc->getName() << "'" << endl;
            }
        }
    }
    
    void createSampleNPCs() {
        addNPC(make_unique<Dragon>("Smaug", 100, 200));
        addNPC(make_unique<Toad>("GiantToad", 150, 180));
        addNPC(make_unique<Knight>("Arthur", 120, 210));
        addNPC(make_unique<Dragon>("FireBreath", 300, 400));
        addNPC(make_unique<Toad>("PoisonFrog", 250, 350));
        addNPC(make_unique<Knight>("Lancelot", 280, 380));
    }
};

int main() {
    DungeonEditor editor;
    
    cout << "==========================================" << endl;
    editor.createSampleNPCs();
    
    editor.printAllNPCs();
    
    editor.saveToFile("dungeon.txt");
    
    cout << "\nЗапуск боевого режима" << endl;
    editor.startBattleMode(150.0);
    
    editor.printAllNPCs();
    
    return 0;
}
