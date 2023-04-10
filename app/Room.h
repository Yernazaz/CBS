#include <string>
#include <vector>

using namespace std;

class Room {
public:
    Room(string code, string name, string location, int capacity, int examCapacity,
        string specification, vector<string> equipment);

    // Getters
    string getCode() const;
    string getName() const;
    string getLocation() const;
    int getCapacity() const;
    int getExamCapacity() const;
    string getSpecification() const;
    vector<string> getEquipment() const;

    // Setters
    void setCode(const string& code);
    void setName(const string& name);
    void setLocation(const string& location);
    void setCapacity(int capacity);
    void setExamCapacity(int examCapacity);
    void setSpecification(const string& specification);
    void setEquipment(const vector<string>& equipment);

private:
    string code;
    string name;
    string location;
    int capacity;
    int examCapacity;
    string specification;
    vector<string> equipment;
};
