class BinarySearch(Jan_week4) {
    public int search(int[] nums, int k) {
        if (nums == null || nums.length == 0) return -1;
        int b = 0, e = nums.length - 1;
        while (b <= e) {
            int mid = b + (e - b) / 2; 
            if (nums[mid] == k) {
                return mid;
            }
            if (nums[b] <= nums[mid]) {
                if (k >= nums[b] && k < nums[mid]) {
                    e = mid - 1; 
                } else {
                    b = mid + 1; 
                }
            } else {
                if (k > nums[mid] && k <= nums[e]) {
                    b = mid + 1; 
                } else {
                    e = mid - 1; 
                }
            }
        }
        return -1; 
    }
	
    public boolean searchInARotatedSortedArrayII(int[] nums, int k) {
        int b = 0, e = nums.length - 1;
        while(b<=e)
        {
            int mid = b + (e-b)/2;
            if (nums[mid] == k) {
                return true;
            }
            if (nums[b] == nums[mid] && nums[mid] == nums[e]) {
                b++;
                e--;
                continue; 
            }
            if (nums[b] <= nums[mid]) {
                if (k >= nums[b] && k < nums[mid]) {
                    e = mid - 1; 
                } else {
                    b = mid + 1; 
                }
            } else {
                if (k > nums[mid] && k <= nums[e]) {
                    b = mid + 1; 
                } else {
                    e = mid - 1; 
                }
            }
        }
        return false;
    }
}	
