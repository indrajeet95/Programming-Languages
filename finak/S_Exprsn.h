#include <string>
#include <map>
using namespace std;

enum sExpType { intATOM = 0, symATOM, notATOM };

class sExp {
public:
    static sExp* sATOM(string s);
    static void isATOM();
    static void delsATOM();
    sExp(sExpType);
    ~sExp();

private:
    static map<string,sExp*> collections;
    sExpType type;
    int val;
    std::string name;
    sExp * left;
    sExp * right;

public:
    sExpType getType() const {
        return type;
    }

    void setType(sExpType t) {
        type = t;
    }

    int getVal() const {
        return val;
    }

    void setVal(int v) {
        val = v;
    }

    const string &getName() const {
        return name;
    }

    void setName(const string &n) {
        name = n;
    }

    sExp *getLeft() const {
        return left;
    }

    void setLeft(sExp *l) {
        left = l;
    }

    sExp *getRight() const {
        return right;
    }

    void setRight(sExp *r) {
        right = r;
    }
};
