class BinarySearch(Jan_week3) {
    public int lowerBound(int[] nums, int x) {
        int n = nums.length;
        int b = 0, e = n-1; 
        int ans=n;
        while (b <= e) {
            int m = b + (e - b) / 2; 
            if (nums[m] >= x) {
                ans = m;      
                e = m - 1;    
            } else {
                b = m + 1;   
            }
        }
        return ans;
    }

    public int upperBound(int[] nums, int x) {
        int n = nums.length;
        int b = 0, e = n - 1; 
        int ans = n; 

        while (b <= e) {
            int m = b + (e - b) / 2; 

            if (nums[m] > x) {
                ans = m;      
                e = m - 1;    
            } else {
                b = m + 1;    
            }
        }
        return ans;
    }
    public int countOccurrences(int[] arr, int target) {
        int n=arr.length, pos=-1;
        int count = 0;
        
        for(int i = 0; i<n; i++)
        {
            if(arr[i]==target)
            {
                pos = i;
                break;
            }
        }
        if(pos==-1)
            return 0;
        for (int i = pos; i < n && arr[i] == target; i++) {
            count++;
        }
        return count;
    }
}