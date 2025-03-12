#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>



class NameRecord {
public:
    std::string name;
    std::string gender;
    int count;

    NameRecord(std::string n, std::string g, int c)
        : name(n), gender(g), count(c) {}

    void print() const {
        std::cout << "Name: " << name << ", Sex: " << gender << ", Choose: " << count << '\n';
    }

    bool operator<(const NameRecord& other) const {
        return name < other.name;
    }
};

class NameStatistics {
public:
    std::vector<NameRecord> records;

    void addRecord(const NameRecord& record) {
        records.push_back(record);
    }

    NameRecord mostPopular() {
        return *std::max_element(records.begin(), records.end(),
            [](const NameRecord& a, const NameRecord& b) {
                return a.count < b.count;
            });
    }

    NameRecord mostPopularByGender(const std::string& gender) {
        auto it = std::max_element(records.begin(), records.end(),
            [&gender](const NameRecord& a, const NameRecord& b) {
                if (a.gender != gender) return true;
                if (b.gender != gender) return false;
                return a.count < b.count;
            });
        return (it != records.end()) ? *it : NameRecord(" ", " ", 0);
    }

    int countUnusedNames() {
        return std::count_if(records.begin(), records.end(), [](const NameRecord& r) {
            return r.count == 0;
            });
    }

    double medianCount() {
        std::vector<int> counts;
        for (const auto& r : records) counts.push_back(r.count);
        std::sort(counts.begin(), counts.end());
        size_t n = counts.size();
        return n % 2 == 0 ? (counts[n / 2 - 1] + counts[n / 2]) / 2.0 : counts[n / 2];
    }

    double averageCount() {
        if (records.empty()) return 0;
        int total = std::accumulate(records.begin(), records.end(), 0,
            [](int sum, const NameRecord& r) {
                return sum + r.count;
            });
        return static_cast<double>(total) / records.size();
    }

    int countRareNames(int threshold) {
        return std::count_if(records.begin(), records.end(),
            [threshold](const NameRecord& r) {
                return r.count < threshold;
            });
    }

    int countCommonNames(int threshold) {
        return std::count_if(records.begin(), records.end(),
            [threshold](const NameRecord& r) {
                return r.count > threshold;
            });
    }
};

void loadDataFromFile(const std::string& filename, NameStatistics& stats) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File opening error: " << filename << '\n';
        return;
    }

    std::string name, gender;
    int count;
    while (file >> name >> gender >> count) {
        stats.addRecord(NameRecord(name, gender, count));
    }

    file.close();
}

int main() {


    NameStatistics stats;

    loadDataFromFile("russian_names.csv", stats);

    auto popular = stats.mostPopular();
    std::cout << "Most popular name : ";
    popular.print();

    auto popularFemale = stats.mostPopularByGender("female");
    std::cout << "most popular female name: ";
    popularFemale.print();

    auto popularMale = stats.mostPopularByGender("male");
    std::cout << "most popular male name: ";
    popularMale.print();

    int unusedCount = stats.countUnusedNames();
    std::cout << " Number of unused names: " << unusedCount << '\n';

    double median = stats.medianCount();
    std::cout << " median number: " << median << '\n';

    double average = stats.averageCount();
    std::cout << "Average number: " << average << '\n';

    int rareCount = stats.countRareNames(5);
    std::cout << "Number of rare names: " << rareCount << '\n';

    int commonCount = stats.countCommonNames(50);
    std::cout << "Number of common names: " << commonCount << '\n';

    return 0;
}