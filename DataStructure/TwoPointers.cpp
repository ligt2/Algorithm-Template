#include<bits/stdc++.h>
using namespace std;
using ll = long long;

/*=============================================================
  双指针模板

  应用场景:
  1. 滑动窗口（最长/最短子串/子数组满足条件）
  2. 尺取法（两数之和/三数之和 去重枚举）
  3. 快慢指针（链表环检测/找中点）
  4. 对撞指针（有序数组查找/容器最大水）
  5. 归并/去重（合并有序数组）

  核心思想: 利用单调性，避免暴力双层循环 O(n²) → O(n)
=============================================================*/

// =====================================================================
// 1. 滑动窗口 - 最长子数组（和 ≤ k）
// =====================================================================
// 返回最长长度，窗口维护 [l, r) 左闭右开
int maxSubarraySum(vector<int>& a, int k) {
    int n = a.size(), l = 0, ans = 0;
    ll sum = 0;
    for (int r = 0; r < n; r++) {
        sum += a[r];                    // 扩右边界
        while (sum > k) sum -= a[l++];  // 收缩左边界
        ans = max(ans, r - l + 1);
    }
    return ans;
}

// =====================================================================
// 2. 滑动窗口 - 最短子数组（和 ≥ k）【最小覆盖子串同理】
// =====================================================================
int minSubarraySum(vector<int>& a, int k) {
    int n = a.size(), l = 0, ans = n + 1;
    ll sum = 0;
    for (int r = 0; r < n; r++) {
        sum += a[r];
        while (sum >= k) {
            ans = min(ans, r - l + 1);
            sum -= a[l++];
        }
    }
    return ans > n ? -1 : ans;
}

// =====================================================================
// 3. 滑动窗口 - 最长不重复子串（哈希表维护）
// =====================================================================
int lengthOfLongestSubstring(string s) {
    int n = s.size(), l = 0, ans = 0;
    unordered_map<char, int> mp;
    for (int r = 0; r < n; r++) {
        mp[s[r]]++;
        while (mp[s[r]] > 1) mp[s[l++]]--;  // 出现重复，左指针右移
        ans = max(ans, r - l + 1);
    }
    return ans;
}

// =====================================================================
// 4. 对撞指针 - 有序数组两数之和（返回下标 1-based）
// =====================================================================
pair<int,int> twoSum(vector<int>& a, int target) {
    int l = 0, r = a.size() - 1;
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == target) return {l + 1, r + 1};
        else if (sum < target) l++;
        else r--;
    }
    return {-1, -1};  // 无解
}

// =====================================================================
// 5. 对撞指针 - 容器盛最多的水（LC11）
// =====================================================================
int maxArea(vector<int>& h) {
    int l = 0, r = h.size() - 1, ans = 0;
    while (l < r) {
        ans = max(ans, min(h[l], h[r]) * (r - l));
        if (h[l] < h[r]) l++;  // 移动短板
        else r--;
    }
    return ans;
}

// =====================================================================
// 6. 尺取法 - 三数之和去重（返回所有不重复三元组）
// =====================================================================
vector<vector<int>> threeSum(vector<int>& a) {
    sort(a.begin(), a.end());
    int n = a.size();
    vector<vector<int>> res;
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && a[i] == a[i-1]) continue;  // 跳过重复 i
        int l = i + 1, r = n - 1, target = -a[i];
        while (l < r) {
            int sum = a[l] + a[r];
            if (sum == target) {
                res.push_back({a[i], a[l], a[r]});
                while (l < r && a[l] == a[l+1]) l++;  // 跳过重复 l
                while (l < r && a[r] == a[r-1]) r--;  // 跳过重复 r
                l++; r--;
            } else if (sum < target) l++;
            else r--;
        }
    }
    return res;
}

// =====================================================================
// 7. 快慢指针 - 链表找环（Floyd判圈算法）
// =====================================================================
struct ListNode { int val; ListNode* next; };
bool hasCycle(ListNode* head) {
    if (!head) return false;
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;  // 相遇即有环
    }
    return false;
}

// =====================================================================
//                            测试
// =====================================================================
int main() {
    // --- 1. 最长子数组和 ≤ k ---
    {
        vector<int> a = {1, 2, 3, 4, 5};
        assert(maxSubarraySum(a, 9) == 3);  // [2,3,4]
        cout << "[SlideWin Max] OK\n";
    }
    // --- 2. 最短子数组和 ≥ k ---
    {
        vector<int> a = {2, 3, 1, 2, 4, 3};
        assert(minSubarraySum(a, 7) == 2);  // [4,3]
        cout << "[SlideWin Min] OK\n";
    }
    // --- 3. 最长不重复子串 ---
    {
        assert(lengthOfLongestSubstring("abcabcbb") == 3);  // "abc"
        cout << "[Longest Unique] OK\n";
    }
    // --- 4. 两数之和 ---
    {
        vector<int> a = {2, 7, 11, 15};
        auto [i, j] = twoSum(a, 9);
        assert(i == 1 && j == 2);  // 1-based
        cout << "[TwoSum] OK\n";
    }
    // --- 5. 盛最多的水 ---
    {
        vector<int> h = {1, 8, 6, 2, 5, 4, 8, 3, 7};
        assert(maxArea(h) == 49);  // h[1]=8, h[8]=7, 距离7
        cout << "[MaxArea] OK\n";
    }
    // --- 6. 三数之和 ---
    {
        vector<int> a = {-1, 0, 1, 2, -1, -4};
        auto res = threeSum(a);
        assert(res.size() == 2);  // {-1,-1,2}, {-1,0,1}
        cout << "[ThreeSum] OK: " << res.size() << " solutions\n";
    }
    // --- 7. 链表环检测 ---
    {
        ListNode n1{1}, n2{2}, n3{3};
        n1.next = &n2; n2.next = &n3; n3.next = &n2;  // 有环
        assert(hasCycle(&n1));
        cout << "[Cycle Detection] OK\n";
    }
    
    cout << "\n=== All TwoPointers Templates Passed ===\n";
}