#include <iostream>
#include <vector>
using namespace std;

#define displayVector(v) do { \
    std::cout << "["; \
    for (size_t i = 0; i < (v).size(); i++) { \
        std::cout << (v)[i]; \
        if (i < (v).size() - 1) { \
            std::cout << ", "; \
        } \
    } \
    std::cout << "]" << std::endl; \
} while (false)

int findFreeSlot(vector<int> & locations){
    if(locations[0] != 0) {   
        locations.insert(locations.begin(), 0);
        return 0;
    }
    for(int i = 0; i < locations.size() - 1; i++){
        if((locations[i+1] - locations[i]) >= 2) {
            locations.insert(locations.begin() + i + 1, i + 1);
            return i + 1;
        }
    }
    int newSize = locations.size();
    locations.push_back(newSize);
    return newSize;
}

int main(){
    vector<int> locations = {1, 5, 6};
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    cout << findFreeSlot(locations) << endl;
    displayVector(locations);
    return 0;
}
