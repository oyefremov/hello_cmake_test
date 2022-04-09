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
            size_t left = negative_nums.size();
            size_t right = large_nums.size();
            auto lp = histogram.begin();
            auto rp = histogram.end() - 1;
            auto last_lp = lp, last_rp = rp;
            while (std::distance(lp, rp) > 1) {
                if (left + *lp < right + *rp) {
                    left += *lp;
                    lp++;
                    if (*lp) last_lp = lp;
                }else{
                    right += *rp;
                    rp--;
                    if (*rp) last_rp = rp;
                }

            }
            if (left + *lp < right + *rp){
                return distance(histogram.begin(), last_rp);
            }else if(left + *lp == right + *rp){
                return (distance(histogram.begin(), last_lp) + distance(histogram.begin(), last_rp)) / 2.0;
            }else {
                return distance(histogram.begin(), last_lp);
            }
        }else if (negative_nums.size() > large_nums.size() + histogram_size) {
            // median in negative_nums
            auto other_size = large_nums.size() + histogram_size;
            assert(other_size < negative_nums.size());
            auto size_to_sort = negative_nums.size() - other_size;
            auto index = other_size + size_to_sort / 2;
            std::nth_element(negative_nums.begin(), negative_nums.begin() + index, negative_nums.end());
            if (size_to_sort % 2 == 0) {
                auto prev = std::max_element(negative_nums.begin(), negative_nums.begin() + index);
                return (negative_nums[index] + *prev) / 2.0;
            }else{
                return negative_nums[index];
            }
        }else {
            // median in large_nums
            auto other_size = negative_nums.size() + histogram_size;
            assert(large_nums.size() > negative_nums.size() + histogram_size);
            auto size_to_sort = large_nums.size() - other_size;
            auto index = size_to_sort / 2;
            std::nth_element(large_nums.begin(), large_nums.begin() + index, large_nums.end());
            if (size_to_sort % 2 == 0) {
                auto prev = std::max_element(large_nums.begin(), large_nums.begin() + index);
                return (large_nums[index] + *prev) / 2.0;
            }
            else {
                return large_nums[index];
            }
        }
        // sort(nums.begin(), nums.end());
        // if (nums.size() % 2 == 0) {
        //     return (nums[nums.size() / 2 - 1] + nums[nums.size() / 2]) / 2.0;
        // } else {
        //     return nums[nums.size() / 2];
        // }
    }

    vector<int> negative_nums, large_nums;
    array<int, 101> histogram = {};
    int histogram_size = 0;
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */