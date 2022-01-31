#include <algorithm>
#include <functional>
#include "hierarchy.h"

// Reorder the entries vector so that adjacent entries tend to be nearby.
// You may want to implement box.cpp first.
void Hierarchy::Reorder_Entries()
{
    if(!entries.size()) return;
    auto tmp = entries;
    printf("entries.size()=%zu\n", entries.size());
    std::function<void(int, int, int)> my_sort = [&](int l, int r, int dim) {
        if(l >= r-1) {
            return;
        }
        // double mn = std::numeric_limits<double>::max();
        // double mx = std::numeric_limits<double>::min();
        // for(int i = l; i < r; i++) {
        //     mn = std::min(mn, entries[i].box.lo[dim]);
        //     mx = std::max(mn, entries[i].box.hi[dim]);
        // }
        // double mid = (mn+mx)/2;
        // int m = l;
        // for(int i = l; i < r; i++) {
        //     if(entries[i].box.hi[dim] < mid) {
        //         m++;
        //     }
        // }
        // int pt1 = l, pt2 = m;
        // for(int i = l; i < r; i++) {
        //     if(entries[i].box.hi[dim] < mid) {
        //         tmp[pt1++] = entries[i];
        //     } else {
        //         tmp[pt2++] = entries[i];
        //     }
        // }
        // for(int i = l; i < r; i++) {
        //     entries[i] = tmp[i];
        // }
        std::sort(begin(entries)+l, begin(entries)+r, [&](Entry a, Entry b) {
            return a.box.hi[dim] < b.box.hi[dim];
        });
        int m = (l+r)/2;
        my_sort(l, m, (dim+1)%3);
        my_sort(m+1, r, (dim+1)%3);
    };
    my_sort(0, entries.size(), 0);
}

// Populate tree from entries.
void Hierarchy::Build_Tree()
{
    if(!entries.size()) return;
    int n = entries.size();
    tree.resize(2*n-1);
    for(int i = n-1; i < 2*n-1; i++) {
        tree[i] = entries[i-(n-1)].box;
    }
    for(int i = n-2; i >= 0; i--) {
        tree[i] = tree[i*2+1].Union(tree[i*2+2]);
    }
}

// Return a list of candidates (indices into the entries list) whose
// bounding boxes intersect the ray.
void Hierarchy::Intersection_Candidates(const Ray& ray, std::vector<int>& candidates) const
{
    int n = entries.size();
    std::vector<int> wanzj;
    if(tree[0].Intersection(ray)) {
        wanzj.push_back(0);
    }
    size_t pt = 0;
    while(pt < wanzj.size()) {
        int id = wanzj[pt];
        if(id >= n-1) {
            candidates.push_back(id-(n-1));
        } else {
            if(tree[id*2+1].Intersection(ray)) {
                wanzj.push_back(id*2+1);
            }  
            if(tree[id*2+2].Intersection(ray)){
                wanzj.push_back(id*2+2);
            }
        }
        pt++;
    }
}
