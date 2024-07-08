
class Creature {

protected:

    std::string m_name;
    char m_symbol;
    int m_health, m_damage, m_gold;

public:

    Creature(std::string name, char symbol, int health, int damage, int gold)
        : m_name {name}
        , m_symbol {symbol}
        , m_health {health}
        , m_damage {damage}
        , m_gold {gold} {}

    const std::string& getName() const {return m_name;}
    char getSymbol() const {return m_symbol;}
    int getHealth() const {return m_health;}
    int getDamage() const {return m_damage;}
    int getGold() const {return m_gold;}

    void setName(std::string name) {m_name = name;}
    void setSymbol(char symbol) {m_symbol = symbol;}
    void setHealth(int health) {m_health = health;}
    void setDamage(int damage) {m_damage = damage;}
    void setGold(int gold) {m_gold = gold;}

    void reduceHealth(int health) {m_health -= health;}
    bool isDead() {return m_health <= 0;}
    void addGold(int gold) {m_gold += gold;}

};

class Player : public Creature {

private:

    int m_playersLevel {1};

public:

    Player(std::string& name)
        : Creature {name, '@', 10, 1, 0} {

        cout << "Welcome, " << getName() << ".\n";
        cout << "You have " << getHealth() <<
        " and are carrying " << getGold() << " gold.\n";

    }

    void levelUp() {++m_playersLevel; ++m_damage;}
    int getPlayersLevel() {return m_playersLevel;}
    bool hasWon() {return m_playersLevel >= 20;}

    void drinkPotion(const std::string& potionName, int potionSize) {

        if (potionName == "Health" && potionSize == 1) {
            m_health += 2;
        } else if (potionName == "Health" && potionSize == 2) {
            m_health += 2;
        } else if (potionName == "Health" && potionSize == 3) {
            m_health += 5;
        }
        if (potionName == "Strength" && potionSize == 1) {
            m_damage += 1;
        } else if (potionName == "Strength" && potionSize == 2) {
            m_damage += 1;
        } else if (potionName == "Strength" && potionSize == 3) {
            m_damage += 1;
        }
        if (potionName == "Poison" && potionSize == 1) {
            m_health -= 1;
        } else if (potionName == "Poison" && potionSize == 2) {
            m_health -= 1;
        } else if (potionName == "Poison" && potionSize == 3) {
            m_health -= 1;
        }

    }

};

class Monster : public Creature {

public:

    enum Type {
        dragon,
        orc,
        slime,
        max_types,
    };

    Monster(Type type)
        : Creature {monsterData[type]}
        , m_type {type} {} // default constructor

    static Monster getRandomMonster() {

        int randomMonster {Random::get(0, max_types-1)};

        return Monster{static_cast<Type>(randomMonster)};

    }

private:

    Monster::Type m_type;

    static inline Creature monsterData[] {
        Creature {"dragon", 'D', 10, 4, 100},
        Creature {"orc", 'o', 4, 2, 25},
        Creature {"slime", 's', 1, 1, 10}
    };

};

class Potion {

public:

    enum Type {

        Health,
        Strength,
        Poison,
        max_types,

    };

    Potion(Type type, int size)
        : m_potionType {type}
        , m_potionSize {size} {}

    std::string getPotionName() const {

        switch (m_potionType) {
            case Health:
                return "Health";
            case Strength:
                return "Strength";
            case Poison:
                return "Poison";
            default:
                return "Unknown Potion";
        }

    }

    std::string getPotionSize() const {

        switch (m_potionSize) {
            case 1:
                return "Small";
            case 2:
                return "Medium";
            case 3:
                return "Large";
            default:
                return "Unknown Size";
        }

    }

    static Potion getRandomPotion() {

        Type randomPotion = static_cast<Type>(Random::get(0, max_types-1));

        int randomSize = Random::get(1,3);

        return Potion(randomPotion,randomSize);

    }

    void drink(Player& player) const {

        std::string potionName = getPotionName();
        player.drinkPotion(potionName, m_potionSize);

    }

private:

    Type m_potionType;
    int m_potionSize;

};

void attackPlayer(Player& player, Monster& monster);

void attackMonster(Player& player, Monster& monster) {

    int damage = player.getDamage();

    monster.reduceHealth(damage);
    cout << "You hit the " << monster.getName() << " for " <<
    damage << " damage.\n";
    //cout << monster.getName() << " new health level is: " <<
    //monster.getHealth() << '\n';
    attackPlayer(player, monster);

}

void attackPlayer(Player& player, Monster& monster) {

    int damage = monster.getDamage();

    player.reduceHealth(damage);
    cout << "The " << monster.getName() << " hit you for " <<
    damage << " damage.\n";
    //cout << player.getName() << " new health level is: " <<
    //player.getHealth() << '\n';

}

void fightMonster(Player& player) {

    while (!player.isDead()) {

        Monster monster {Monster::getRandomMonster()};

        cout << "You have encountered a " <<
        monster.getName() << "(" << monster.getSymbol()
        << ")\n";

        while (!monster.isDead() && !player.isDead()) {

            char input;
            int escape {Random::get(1,2)};

            cout << "(R)un or (F)ight: ";
            cin >> input;

            if (!std::isalpha(input) || (input != 'r' || input != 'R' ||
                input != 'f' || input != 'F')) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.clear();
            }
            if (input == 'r' || input == 'R') {
                //int escape {Random::get(1,2)};
                if (escape == 1) { // if escape is 1 you fled successfully
                    cout << "You successfully fled.\n";
                    break;
                } else if (escape == 2) {
                    cout << "You failed the flee.\n";
                    attackPlayer(player, monster);
                }
            } else if (input == 'f' || input == 'F') {
                attackMonster(player, monster);
            }

        } // end of inner while loop

        if (monster.isDead()) {
            cout << "You killed the " << monster.getName() << '\n';
            player.levelUp();
            cout << "You are now level " << player.getPlayersLevel() << '\n';
            cout << "You found " << monster.getGold() << " gold.\n";
            player.addGold(monster.getGold());

            int foundPotion {Random::get(0,1)};
            bool end = false;


            if (foundPotion == 0 || foundPotion == 1 || foundPotion == 9) {
                cout << "You found a mythical Potion! ";

                while (!end) {

                    cout << "Do you want to drink it? [y/n]: ";
                    char input;
                    cin >> input;

                    if (!std::isalpha(input) || (input != 'y' || input != 'Y' ||
                    input != 'n' || input != 'N')) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.clear();
                    }
                    if (input == 'y' || input == 'Y') {
                        Potion potion = Potion::getRandomPotion();
                        cout << "You drank a " << potion.getPotionSize() <<
                        " potion of " << potion.getPotionName() << '\n';
                        potion.drink(player);
                        end = true;
                    } else if (input == 'n' || input == 'N'){
                        cout << "Potion has disappeared.\n";
                        end = true;
                    }
                }
            }
        }

    } // end of outer while loop

    if (player.isDead()) {
        cout << "You died at level " << player.getPlayersLevel() <<
        " with " << player.getGold() << " gold.\n";
        cout << "Too bad you can't take it with you!\n";
    }

}

int main() {

    cout << "Enter your name: ";
    std::string name;
    cin >> name;

    Player p(name);

    fightMonster(p);

}
