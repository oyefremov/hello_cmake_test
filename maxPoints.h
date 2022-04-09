#include <array>
#include <vector>
#include <map>
#include <numeric>

class Solution {
    
    using key = array<int, 4>;

    static
    key hash(int x1, int y1, int x2, int y2){
        if (y1 == y2){
            return {0, y1, 1, 0};
        }
        if (x1 == x2){
            return {x1, 0, 0, 1};
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        int k = gcd(dx, dy);
        if (dx < 0) k = -k;
        dx /= k;
        dy /= k;
        k = x1/dx;
        x1 -= dx * k;
        y1 -= dy * k;
        return {x1, y1, dx, dy};
    }

    static
    int points_count(int s) {
        for (int r = 1; r < 1000000; ++r) {
            if (r * (r - 1) / 2 == s)
                return r;
        }
        return 0;
    }

    
public:
    int maxPoints(vector<vector<int>>& points) {
        for (size_t i=1; i<points.size(); ++i){
            auto& a = points[i];
            for (size_t j=0; j<i; ++j){
                auto& b = points[j];
                counts[hash(a[0],a[1],b[0],b[1])]++;
            }
        }
        int max = 0;
        for (auto& [key,count] : counts)
            if (count > max) max = count;
        return points_count(max);
    }
    
    map<key, int> counts; 
};