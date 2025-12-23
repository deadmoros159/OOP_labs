#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// Включаем наши классы (предполагаем, что они в одном файле или заголовке)
#include "main.c++"  // Или раскомментируй весь код здесь

// ============================================
// ТЕСТ 1: СОЗДАНИЕ NPC (Базовые классы)
// ============================================

TEST(NPCTest, CreateAndCheckNPC) {
    // Проверяем создание всех типов NPC
    Dragon dragon("Smaug", 100, 200);
    Toad toad("GiantToad", 150, 180);
    Knight knight("Arthur", 120, 210);
    
    EXPECT_EQ(dragon.getType(), "Dragon");
    EXPECT_EQ(toad.getType(), "Toad");
    EXPECT_EQ(knight.getType(), "Knight");
    
    EXPECT_EQ(dragon.getName(), "Smaug");
    EXPECT_EQ(dragon.getX(), 100);
    EXPECT_EQ(dragon.getY(), 200);
}

// ============================================
// ТЕСТ 2: FACTORY - СОЗДАНИЕ ИЗ СТРОКИ
// ============================================

TEST(FactoryTest, CreateNPCFromString) {
    ConcreteNPCFactory factory;
    
    // Тест корректных данных
    auto dragon = factory.createNPC("Dragon FireDragon 250 300");
    ASSERT_NE(dragon, nullptr);
    EXPECT_EQ(dragon->getType(), "Dragon");
    EXPECT_EQ(dragon->getName(), "FireDragon");
    EXPECT_EQ(dragon->getX(), 250);
    EXPECT_EQ(dragon->getY(), 300);
    
    // Тест ошибок
    auto invalid1 = factory.createNPC("Dragon NoCoords");
    EXPECT_EQ(invalid1, nullptr);
    
    auto invalid2 = factory.createNPC("Wizard Gandalf 100 200");
    EXPECT_EQ(invalid2, nullptr);
    
    auto invalid3 = factory.createNPC("Dragon BadCoord 600 700");
    EXPECT_EQ(invalid3, nullptr);
}

// ============================================
// ТЕСТ 3: ЛОГИКА БОЯ - ЖАБА ЕСТ ВСЕХ
// ============================================

TEST(BattleTest, ToadEatsEveryone) {
    BattleSubj subject;
    Toad toad("HungryToad", 100, 100);
    Dragon dragon("FireDragon", 100, 100);
    Knight knight("BraveKnight", 100, 100);
    
    // 1. Жаба атакует Дракона → Жаба побеждает
    BattleVisitor visitor1(&toad, subject);
    dragon.accept(visitor1);
    EXPECT_TRUE(visitor1.didAttackerWin());
    
    // 2. Дракон атакует Жабу → Жаба побеждает (атакующий дракон проигрывает)
    BattleVisitor visitor2(&dragon, subject);
    toad.accept(visitor2);
    EXPECT_FALSE(visitor2.didAttackerWin());
    
    // 3. Жаба атакует Рыцаря → Жаба побеждает
    BattleVisitor visitor3(&toad, subject);
    knight.accept(visitor3);
    EXPECT_TRUE(visitor3.didAttackerWin());
}

// ============================================
// ТЕСТ 4: ЛОГИКА БОЯ - РЫЦАРЬ vs ДРАКОН
// ============================================

TEST(BattleTest, KnightVsDragonRules) {
    BattleSubj subject;
    Knight knight("DragonSlayer", 100, 100);
    Dragon dragon("AncientDragon", 100, 100);
    
    // Рыцарь атакует Дракона → Рыцарь побеждает
    BattleVisitor visitor1(&knight, subject);
    dragon.accept(visitor1);
    EXPECT_TRUE(visitor1.didAttackerWin());
    
    // Дракон атакует Рыцаря → Дракон побеждает
    BattleVisitor visitor2(&dragon, subject);
    knight.accept(visitor2);
    EXPECT_TRUE(visitor2.didAttackerWin());
}

// ============================================
// ТЕСТ 5: DUNGEON EDITOR - ДОБАВЛЕНИЕ NPC
// ============================================

TEST(DungeonTest, AddAndValidateNPC) {
    DungeonEditor editor;
    
    // Корректное добавление
    auto dragon = make_unique<Dragon>("TestDragon", 250, 300);
    editor.addNPC(move(dragon));
    
    // Неверные координаты - не должно добавить
    auto bad1 = make_unique<Dragon>("Bad1", 600, 300);
    editor.addNPC(move(bad1));
    
    auto bad2 = make_unique<Dragon>("Bad2", 250, -50);
    editor.addNPC(move(bad2));
    
    // Дублирование имени - не должно добавить
    auto duplicate = make_unique<Dragon>("TestDragon", 300, 400);
    editor.addNPC(move(duplicate));
    
    // Проверяем через сохранение
    editor.saveToFile("test_dungeon.txt");
    
    // Читаем файл и проверяем
    ifstream file("test_dungeon.txt");
    ASSERT_TRUE(file.is_open());
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (line.find("TestDragon") != string::npos) count++;
        if (line.find("Bad") != string::npos) count++;  // Не должно быть
    }
    
    file.close();
    remove("test_dungeon.txt");
    
    // Должен быть только один NPC TestDragon
    EXPECT_EQ(count, 1);
}

// ============================================
// ТЕСТ 6: СОХРАНЕНИЕ И ЗАГРУЗКА
// ============================================

TEST(DungeonTest, SaveAndLoad) {
    DungeonEditor editor1;
    
    // Создаем и сохраняем
    editor1.addNPC(make_unique<Dragon>("SaveDragon", 100, 200));
    editor1.addNPC(make_unique<Toad>("SaveToad", 150, 250));
    editor1.addNPC(make_unique<Knight>("SaveKnight", 200, 300));
    
    editor1.saveToFile("save_test.txt");
    
    // Загружаем в новый редактор
    DungeonEditor editor2;
    editor2.loadFromFile("save_test.txt");
    
    // Сохраняем загруженное и сравниваем файлы
    editor2.saveToFile("load_test.txt");
    
    // Сравниваем содержимое файлов
    ifstream file1("save_test.txt");
    ifstream file2("load_test.txt");
    
    string content1, content2, line;
    while (getline(file1, line)) content1 += line + "\n";
    while (getline(file2, line)) content2 += line + "\n";
    
    file1.close();
    file2.close();
    
    // Файлы должны быть идентичны
    EXPECT_EQ(content1, content2);
    
    // Очистка
    remove("save_test.txt");
    remove("load_test.txt");
}

// ============================================
// ТЕСТ 7: БОЕВОЙ РЕЖИМ (интеграционный)
// ============================================

TEST(DungeonTest, BattleModeIntegration) {
    DungeonEditor editor;
    
    // Создаем NPC для боя
    editor.addNPC(make_unique<Toad>("BattleToad", 100, 100));
    editor.addNPC(make_unique<Dragon>("BattleDragon", 105, 105));  // Близко
    editor.addNPC(make_unique<Knight>("FarKnight", 400, 400));     // Далеко
    
    // Запускаем бой с дальностью 50
    editor.startBattleMode(50.0);
    
    // Сохраняем результаты
    editor.saveToFile("battle_result.txt");
    
    // Проверяем файл результатов
    ifstream file("battle_result.txt");
    ASSERT_TRUE(file.is_open());
    
    vector<string> surviving_npcs;
    string line;
    while (getline(file, line)) {
        if (line.find("BattleToad") != string::npos) surviving_npcs.push_back("Toad");
        if (line.find("BattleDragon") != string::npos) surviving_npcs.push_back("Dragon");
        if (line.find("FarKnight") != string::npos) surviving_npcs.push_back("Knight");
    }
    
    file.close();
    remove("battle_result.txt");
    
    // Анализируем результаты
    // Жаба должна была победить дракона (они были близко)
    // Рыцарь далеко - не должен был участвовать в бою
    
    bool toad_survived = false;
    bool dragon_survived = false;
    bool knight_survived = false;
    
    for (const auto& npc : surviving_npcs) {
        if (npc == "Toad") toad_survived = true;
        if (npc == "Dragon") dragon_survived = true;
        if (npc == "Knight") knight_survived = true;
    }
    
    // Проверяем логику:
    // 1. Жаба должна выжить (она ест всех)
    EXPECT_TRUE(toad_survived);
    
    // 2. Дракон должен быть убит (жабой)
    EXPECT_FALSE(dragon_survived);
    
    // 3. Рыцарь должен выжить (он был далеко)
    EXPECT_TRUE(knight_survived);
}

// ============================================
// ОСНОВНАЯ ФУНКЦИЯ
// ============================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}