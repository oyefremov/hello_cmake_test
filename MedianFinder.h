#include <algorithm>
#include <vector>
#include <array>

class MedianFinder {
public:
    MedianFinder() {
        
    }
    
    void addNum(int num) {
        if (num >= 0 && num < histogram.size()) {
            histogram[num]++;
            histogram_size++;
        }else{
            if (num < 0) {
                negative_nums.push_back(num);
            }else if (num >= histogram.size()) {
                large_nums.push_back(num);
            }
        }
    }
    
    double findMedian() {
        if (std::abs((int)negative_nums.size() - (int)large_nums.size()) <= histogram_size) {
            // median within the histogram
            return 0;
        }else if (negative_nums.size() > large_nums.size() + histogram_size) {
            // median in negative_nums
            auto other_size = large_nums.size() + histogram_size;
            assert(other_size < negative_nums.size());
            auto size_to_sort = negative_nums.size() - other_size;
            std::partial_sort(negative_nums.rbegin(), negative_nums.rbegin() + size_to_sort, negative_nums.rend());
            auto p = negative_nums.begin() + other_size;
            if (size_to_sort % 2 == 0) {
                return (*(p+(size_to_sort/2)) + *(p+(size_to_sort/2 - 1))) / 2.0;
            }else{
                return *(p+(size_to_sort/2));
            }
        }else {
            // median in large_nums
            auto other_size = negative_nums.size() + histogram_size;
            assert(large_nums.size() > negative_nums.size() + histogram_size);
            return 0;
        }
        // sort(nums.begin(), nums.end());
        // if (nums.size() % 2 == 0) {
        //     return (nums[nums.size() / 2 - 1] + nums[nums.size() / 2]) / 2.0;
        // } else {
        //     return nums[nums.size() / 2];
        // }
    }

    vector<int> negative_nums, large_nums;
    array<int, 101> histogram;
    int histogram_size = 0;
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */